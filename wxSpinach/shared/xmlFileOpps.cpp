

#include <shared/spinsys.hpp>
#include <shared/spinsys_spec.hpp>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

using std::endl;
using std::cout;
using std::cerr;



type::value GetXSDTypeFromSpinXMLType(SpinXML::Interaction::SubType subtype) {
  switch(subtype) {
  case SpinXML::Interaction::ST_HFC:
    return type::hfc;
  case SpinXML::Interaction::ST_G_TENSER:
    return type::g_tenser;
  case SpinXML::Interaction::ST_ZFS:
    return type::zfs;
  case SpinXML::Interaction::ST_EXCHANGE:
    return type::exchange;
  case SpinXML::Interaction::ST_SHIELDING:
    return type::shielding;
  case SpinXML::Interaction::ST_SCALAR:
    return type::scalar;
  case SpinXML::Interaction::ST_QUADRUPOLAR:
    return type::quadrupolar;
  case SpinXML::Interaction::ST_DIPOLAR:
    return type::dipolar;
  case SpinXML::Interaction::ST_CUSTOM:
    return type::custem;
  default:
    throw std::runtime_error("Unknown sub type when outputing to an XML file");
  }
}

SpinXML::Interaction::SubType GetSpinXMLTypeFromXSDType(type::value subtype) {
  switch(subtype) {
  case type::hfc:
    return SpinXML::Interaction::ST_HFC;
  case type::g_tenser:
    return SpinXML::Interaction::ST_G_TENSER;
  case type::zfs:
    return SpinXML::Interaction::ST_ZFS;
  case type::exchange:
    return SpinXML::Interaction::ST_EXCHANGE;
  case type::shielding:
    return SpinXML::Interaction::ST_SHIELDING;
  case type::scalar:
    return SpinXML::Interaction::ST_SCALAR;
  case type::quadrupolar:
    return SpinXML::Interaction::ST_QUADRUPOLAR;
  case type::dipolar:
    return SpinXML::Interaction::ST_DIPOLAR;
  case type::custem:
    return SpinXML::Interaction::ST_CUSTOM;
  default:
    throw std::runtime_error("Unknown sub type when when reading an XML file. Is the right schema loaded?");
  }
}


orientation ConvertOrientationToXML(const SpinXML::Orientation& o) {
  SpinXML::Orientation::Type type=o.GetType();
  orientation oout;
  switch(type) {
  case SpinXML::Orientation::EULER:{
    double alpha,beta,gamma;
    o.GetEuler(&alpha,&beta,&gamma);
    euler_angles ea(alpha,beta,gamma);
    oout.euler_angles(ea);
    break;
  }
  case SpinXML::Orientation::ANGLE_AXIS: {
    double angle;
    SpinXML::Vector3 axis;
    o.GetAngleAxis(&angle,&axis);
    vector axis_prime(axis.GetX(),axis.GetY(),axis.GetZ());
    angle_axis aa(angle,axis_prime);
    oout.angle_axis(aa);
    break;
  }
  case SpinXML::Orientation::QUATERNION: {
    double re,i,j,k;
    o.GetQuaternion(&re,&i,&j,&k);
    quaternion q(re,i,j,k);
    oout.quaternion(q);
    break;
  }
  case SpinXML::Orientation::EIGENSYSTEM: {
    SpinXML::Vector3 vx;
    SpinXML::Vector3 vy;
    SpinXML::Vector3 vz;
    o.GetEigenSystem(&vx,&vy,&vz);
    vector vx_prime(vx.GetX(),vx.GetY(),vx.GetZ());
    vector vy_prime(vy.GetX(),vy.GetY(),vy.GetZ());
    vector vz_prime(vz.GetX(),vz.GetY(),vz.GetZ());
    eigensystem es(vx_prime,vy_prime,vz_prime);
    oout.eigensystem(es);
    break;
  }
  default:
    throw std::runtime_error("Unknown orientation type encounted");
  }
  return oout;
}

SpinXML::Orientation* ConvertXMLToOrientation(const orientation& o) {
  SpinXML::Orientation* ret=new SpinXML::Orientation;
  if(o.euler_angles().present()) {
    double alpha,beta,gamma;
    euler_angles ea=o.euler_angles().get();
    alpha=ea.alpha();
    beta=ea.beta();
    gamma=ea.gamma();
    ret->SetEuler(alpha,beta,gamma);
  } else if(o.angle_axis().present()) {
    angle_axis aa=o.angle_axis().get();
    vector axis=aa.axis();
    double angle=aa.angle();
    ret->SetAngleAxis(angle,new SpinXML::Vector3(axis.x(),axis.y(),axis.z()));
  } else if(o.quaternion().present()) {
    double re,i,j,k;
    quaternion q=o.quaternion().get();
    re=q.re();
    i=q.i();
    j=q.j();
    k=q.k();
    ret->SetQuaternion(re,i,j,k);
  } else if(o.eigensystem().present()) {
    eigensystem es=o.eigensystem().get();
    vector vx=es.x_axis();
    vector vy=es.y_axis();
    vector vz=es.z_axis();    
    ret->SetEigenSystem(new SpinXML::Vector3(vx.x(),vx.y(),vx.z()),
			new SpinXML::Vector3(vy.x(),vy.y(),vy.z()),
			new SpinXML::Vector3(vz.x(),vz.y(),vz.z()));
  } else {
    throw std::runtime_error("Unknown orientation form encounted. Is the XSD schema corrupt?");
  }
  return ret;
}


void SpinXML::SpinSystem::LoadFromXMLFile(const char* filename)  {
  std::auto_ptr<spin_system> ss;
  try {
     ss=spin_system_(filename);
  } catch(const xml_schema::exception& e) {
    std::cerr << e << std::endl;
    std::ostringstream errStream;
    errStream << e << std::endl;
    throw std::runtime_error(errStream.str());
  }

  Clear();

  spin_system::spin_sequence spins=ss->spin(); 
  long spinCount=spins.size();
  mSpins.resize(spinCount);
  for(long i=0;i<spinCount;i++) {
    spin xsdSpin=spins[i];

    double x,y,z;
    vector coords=xsdSpin.coordinates();
    x=coords.x();
    y=coords.y();
    z=coords.z();


    if(xsdSpin.label().present()) {
      mSpins[i]=new SpinXML::Spin(this,new Vector3(x,y,z),xsdSpin.label().get(),GetRootFrame());
    } else {
      mSpins[i]=new SpinXML::Spin(this,new Vector3(x,y,z),"",GetRootFrame());
    }
    

  }

  spin_system::interaction_sequence inters=ss->interaction();
  long interactionCount=inters.size();
  mInteractions.resize(interactionCount);
  for(long i=0;i<interactionCount;i++) {
    SpinXML::Interaction* thisInter=mInteractions[i]=new SpinXML::Interaction;
    interaction xsdInter=inters[i];

    thisInter->SetSubType(GetSpinXMLTypeFromXSDType(xsdInter.type()));
    //TODO: Decide how to handle units
    long spinNumber=xsdInter.spin_1();
    if(spinNumber <0 || spinNumber >= spinCount) {
      throw std::runtime_error("Spin index in interaction out of range");
    }
    thisInter->SetSpin1(mSpins[spinNumber]);

    if(xsdInter.spin_2().present()) {
      long spinNumber2=xsdInter.spin_2().get();
      if(spinNumber2 < 0 || spinNumber2 > spinCount) {
	throw std::runtime_error("Spin index in interaction out of range");
      }
      thisInter->SetSpin1(mSpins[spinNumber]);

    } else {
      thisInter->SetSpin2(NULL);      
    }


    if(xsdInter.scalar().present()) {
      double scalar;
      scalar=xsdInter.scalar().get();
      thisInter->SetScalar(scalar);
    } else if(xsdInter.matrix().present()) {
      matrix::element_sequence eseq=xsdInter.matrix().get().element();
      SpinXML::Matrix3* mat=new SpinXML::Matrix3(eseq[0],eseq[1],eseq[2],
						 eseq[3],eseq[4],eseq[5],
						 eseq[6],eseq[7],eseq[8]);
      thisInter->SetMatrix(mat);
    } else if(xsdInter.eigenvalues().present()) {
      SpinXML::Orientation* o=ConvertXMLToOrientation(xsdInter.orientation().get());
      double xx,yy,zz;
      eigenvalues eigv=xsdInter.eigenvalues().get();
      thisInter->SetEigenvalues(eigv.XX(),eigv.YY(),eigv.ZZ(),o);
    } else if(xsdInter.axiality_rhombicity().present()) {
      SpinXML::Orientation* o=ConvertXMLToOrientation(xsdInter.orientation().get());
      double ax,rhom,iso;
      axiality_rhombicity ar=xsdInter.axiality_rhombicity().get();
      thisInter->SetAxRhom(ar.ax(),ar.rh(),ar.iso(),o);
    } else if(xsdInter.span_skew().present()) {
      SpinXML::Orientation* o=ConvertXMLToOrientation(xsdInter.orientation().get());
      double span,skew,iso;
      span_skew spanskew=xsdInter.span_skew().get();
      thisInter->SetSpanSkew(spanskew.span(),spanskew.skew(),spanskew.iso(),o);
    } else {
      throw std::runtime_error("Interaction appeared to not be specified (is the xsd schema corrupt?)");
    }
  }
}

void SpinXML::SpinSystem::SaveToXMLFile(const char* filename) const {
  xml_schema::namespace_infomap map;
  map[""].name = "";
  map[""].schema = "../data/spinsys_spec.xsd";

  std::ofstream fout(filename);
  if(!fout.is_open()) {
    std::cerr << "Could not open " << filename << std::endl;
    return;
  }
  spin_system ss;

  spin_system::spin_sequence spins; 
  for(long i=0;i<GetSpinCount();i++) {
    SpinXML::Spin* thisSpin=GetSpin(i);

    double x,y,z;
    thisSpin->GetCoordinates(&x,&y,&z);
    std::string label(thisSpin->GetLabel());

    vector coords(x,y,z);
    spin outSpin(coords,i,"H1",0);
    outSpin.label(label);
    spins.push_back(outSpin);
  }
  ss.spin(spins);

  spin_system::interaction_sequence inters;
  for(long i=0;i<mInteractions.size();i++) {
    SpinXML::Interaction* thisInter=mInteractions[i];

    interaction1 inter(GetXSDTypeFromSpinXMLType(thisInter->GetSubType()),
		       "MHz",
		       GetSpinNumber(thisInter->GetSpin1()),
		       0);

    if(thisInter->GetSpin2() != NULL) {
      inter.spin_2(GetSpinNumber(thisInter->GetSpin2()));
    }

    switch(thisInter->GetType()) {
    case SpinXML::Interaction::SCALAR: {
      double scalar;
      thisInter->GetScalar(&scalar);
      inter.scalar(scalar);
      break;
    }
    case SpinXML::Interaction::MATRIX: {
      SpinXML::Matrix3 mat;
      thisInter->GetMatrix(&mat);
      matrix::element_sequence eseq;
      eseq[0]=mat(0,0);
      eseq[1]=mat(1,0);
      eseq[2]=mat(2,0);

      eseq[3]=mat(0,1);
      eseq[4]=mat(1,1);
      eseq[5]=mat(2,1);

      eseq[6]=mat(0,2);
      eseq[7]=mat(1,2);
      eseq[8]=mat(2,2);
      matrix xsdmat;
      xsdmat.element(eseq);
      inter.matrix(xsdmat);
      break;
    }
    case SpinXML::Interaction::EIGENVALUES: {
      SpinXML::Orientation* o;
      double xx,yy,zz;
      thisInter->GetEigenvalues(&xx,&yy,&zz,&o);
      eigenvalues eigv(xx,yy,zz);
      inter.orientation(ConvertOrientationToXML(*o));
      inter.eigenvalues(eigv);
      break;
    }
    case SpinXML::Interaction::AXRHOM: {
      SpinXML::Orientation* o;
      double ax,rhom,iso;
      thisInter->GetAxRhom(&ax,&rhom,&iso,&o);
      axiality_rhombicity ar(iso,ax,rhom);
      inter.orientation(ConvertOrientationToXML(*o));
      inter.axiality_rhombicity(ar);
      break;
    }
    case SpinXML::Interaction::SPANSKEW: {
      SpinXML::Orientation* o;
      double span,skew,iso;
      thisInter->GetSpanSkew(&span,&skew,&iso,&o);
      span_skew spsk(iso,span,skew);
      inter.orientation(ConvertOrientationToXML(*o));
      inter.span_skew(spsk);
      break;
    }
    default:
      throw std::runtime_error("Interaction is of an unknown form");
    };
    inters.push_back(inter);
  }
  ss.interaction(inters);

  try {
    spin_system_(fout, ss, map);
  } catch (const xml_schema::exception& e) {
    std::cerr << e << std::endl;
    std::ostringstream errStream;
    errStream << e << std::endl;
    throw std::runtime_error(errStream.str());
  }

}
