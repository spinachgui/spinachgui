
#include <shared/formats/xml.hpp>
#include <auto/spinxml_schema.hpp>
#include <shared/nuclear_data.hpp>
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
  case SpinXML::Interaction::ST_CUSTOM_LINEAR:
  case SpinXML::Interaction::ST_CUSTOM_BILINEAR:
  case SpinXML::Interaction::ST_CUSTOM_QUADRATIC:
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
    return SpinXML::Interaction::ST_CUSTOM_LINEAR;
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

SpinXML::Orientation ConvertXMLToOrientation(const orientation& o) {
  SpinXML::Orientation ret;
  if(o.euler_angles().present()) {
    double alpha,beta,gamma;
    euler_angles ea=o.euler_angles().get();
    alpha=ea.alpha();
    beta=ea.beta();
    gamma=ea.gamma();
    ret.SetEuler(alpha,beta,gamma);
  } else if(o.angle_axis().present()) {
    angle_axis aa=o.angle_axis().get();
    vector axis=aa.axis();
    double angle=aa.angle();
    ret.SetAngleAxis(angle,SpinXML::Vector3(axis.x(),axis.y(),axis.z()));
  } else if(o.quaternion().present()) {
    double re,i,j,k;
    quaternion q=o.quaternion().get();
    re=q.re();
    i=q.i();
    j=q.j();
    k=q.k();
    ret.SetQuaternion(re,i,j,k);
  } else if(o.eigensystem().present()) {
    eigensystem es=o.eigensystem().get();
    vector vx=es.x_axis();
    vector vy=es.y_axis();
    vector vz=es.z_axis();    
    ret.SetEigenSystem(SpinXML::Vector3(vx.x(),vx.y(),vx.z()),
		       SpinXML::Vector3(vy.x(),vy.y(),vy.z()),
    		       SpinXML::Vector3(vz.x(),vz.y(),vz.z()));
    
  } else {
    throw std::runtime_error("Unknown orientation form encounted. Is the XSD schema corrupt?");
  }
  return ret;
}


void SpinXML::XMLLoader::LoadFile(SpinSystem* libss,const char* filename) const {
  //libss => "library spin system" as apposed to the xml spin system ss
  std::auto_ptr<spin_system> ss;
  try {
    cout << "Trying" << mSchemaLocation << endl;

    xml_schema::properties p;
    //p.schema_location("http://www.w3.org/XML/1998/namespace","xml.xsd");
    p.no_namespace_schema_location(mSchemaLocation);

    ss=spin_system_(filename,0,p);
  } catch(const xml_schema::exception& e) {
    std::cerr << e << std::endl;
    std::ostringstream errStream;
    errStream << e << std::endl;
    throw std::runtime_error(errStream.str());
  }

  libss->Clear();

  spin_system::spin_sequence spins=ss->spin(); 
  long spinCount=spins.size();
  for(long i=0;i<spinCount;i++) {
    spin xsdSpin=spins[i];

    length x,y,z;
    vector coords=xsdSpin.coordinates();
    x=coords.x();
    y=coords.y();
    z=coords.z();

    SpinXML::Spin* newSpin;
    if(xsdSpin.label().present()) {
      newSpin=new SpinXML::Spin(Vector3l(x,y,z),xsdSpin.label().get());
    } else {
      newSpin=new SpinXML::Spin(Vector3l(x,y,z),"");
    }
    newSpin->SetElement(getElementBySymbol(xsdSpin.element().c_str()));

    libss->InsertSpin(newSpin);
  }

  spin_system::interaction_sequence inters=ss->interaction();
  long interactionCount=inters.size();

  for(long i=0;i<interactionCount;i++) {
    SpinXML::Interaction* thisInter=new SpinXML::Interaction;
    interaction xsdInter=inters[i];

    Interaction::SubType st=GetSpinXMLTypeFromXSDType(xsdInter.type());

    //TODO: Decide how to handle units
    long spinNumber=xsdInter.spin_1();
    long spinNumber2=-1;
    if(spinNumber <0 || spinNumber >= spinCount) {
      throw std::runtime_error("Spin index in interaction out of range");
    }

    if(xsdInter.spin_2().present()) {
      spinNumber2=xsdInter.spin_2().get();
      if(spinNumber2 < 0 || spinNumber2 > spinCount) {
	throw std::runtime_error("Spin index in interaction out of range");
      }
    }
    //Now check the subtype is valid given the form
    if(spinNumber2==-1) {
      //Interaction must be linear
      switch(st) {
      case SpinXML::Interaction::ST_G_TENSER:
      case SpinXML::Interaction::ST_SHIELDING:
      case SpinXML::Interaction::ST_CUSTOM_LINEAR:
	thisInter->SetSubType(st,libss->GetSpin(spinNumber));
	break;
      default:
	throw std::runtime_error("Linear interaction was of a non linear subtype");
      }
    } else if(spinNumber2==spinNumber2) {
      //Interaction must be quadratic
      switch(st) {
      case SpinXML::Interaction::ST_QUADRUPOLAR:
      case SpinXML::Interaction::ST_ZFS:
      case SpinXML::Interaction::ST_CUSTOM_QUADRATIC:
	thisInter->SetSubType(st,libss->GetSpin(spinNumber),libss->GetSpin(spinNumber));
	break;
      default:
	throw std::runtime_error("Quadrupolar interaction was of a non linear subtype");
      }
    } else {
      switch(st) {
      case SpinXML::Interaction::ST_EXCHANGE:
      case SpinXML::Interaction::ST_DIPOLAR:
      case SpinXML::Interaction::ST_SCALAR:
      case SpinXML::Interaction::ST_CUSTOM_BILINEAR:
	thisInter->SetSubType(st,libss->GetSpin(spinNumber),libss->GetSpin(spinNumber2));
	break;
      default:
	throw std::runtime_error("Quadrupolar interaction was of a non linear subtype");
      }
    }




    if(xsdInter.scalar().present()) {
      double scalar;
      scalar=xsdInter.scalar().get();
      thisInter->SetScalar(scalar);
    } else if(xsdInter.matrix().present()) {
      matrix::element_sequence eseq=xsdInter.matrix().get().element();
      SpinXML::Matrix3 mat(eseq[0],eseq[1],eseq[2],
			   eseq[3],eseq[4],eseq[5],
			   eseq[6],eseq[7],eseq[8]);
      thisInter->SetMatrix(mat);
    } else if(xsdInter.eigenvalues().present()) {
      SpinXML::Orientation o(ConvertXMLToOrientation(xsdInter.orientation().get()));
      eigenvalues eigv=xsdInter.eigenvalues().get();
      thisInter->SetEigenvalues(eigv.XX(),eigv.YY(),eigv.ZZ(),o);
    } else if(xsdInter.axiality_rhombicity().present()) {
      SpinXML::Orientation o(ConvertXMLToOrientation(xsdInter.orientation().get()));
      axiality_rhombicity ar=xsdInter.axiality_rhombicity().get();
      thisInter->SetAxRhom(ar.ax(),ar.rh(),ar.iso(),o);
    } else if(xsdInter.span_skew().present()) {
      SpinXML::Orientation o(ConvertXMLToOrientation(xsdInter.orientation().get()));
      span_skew spanskew=xsdInter.span_skew().get();
      thisInter->SetSpanSkew(spanskew.span(),spanskew.skew(),spanskew.iso(),o);
    } else {
      throw std::runtime_error("Interaction appeared to not be specified (is the xsd schema corrupt?)");
    }
  }
}

void SpinXML::XMLLoader::SaveFile(const SpinSystem* libss,const char* filename) const {
    xml_schema::namespace_infomap map;
    map[""].name = "";
    map[""].schema = "../data/spinxml_schema.xsd";

    std::ofstream fout(filename);
    if(!fout.is_open()) {
	std::cerr << "Could not open " << filename << std::endl;
	return;
    }
    spin_system ss;

    spin_system::spin_sequence spins; 
    for(long i=0;i<libss->GetSpinCount();i++) {
	SpinXML::Spin* thisSpin=libss->GetSpin(i);

	double x,y,z;
	thisSpin->GetCoordinates(&x,&y,&z);
	std::string label(thisSpin->GetLabel());

	vector coords(x,y,z);
	spin outSpin(coords,i,0);
	outSpin.label(label);
	outSpin.element(getElementSymbol(thisSpin->GetElement()));
	spins.push_back(outSpin);
    }
    ss.spin(spins);

    spin_system::interaction_sequence inters;
    for(long i=0;i<libss->GetSpinCount();i++) {
	SpinXML::Spin* spin=libss->GetSpin(i);
	std::vector<SpinXML::Interaction*> thisInters=spin->GetInteractionsOnce();
	for(unsigned long j=0;j<thisInters.size();j++) {
	    SpinXML::Interaction* thisInter=thisInters[j];

	    interaction1 inter(GetXSDTypeFromSpinXMLType(thisInter->GetSubType()),
			       "MHz",
			       libss->GetSpinNumber(thisInter->GetSpin1()),
			       0);
      
	    if(thisInter->GetSpin2() != NULL) {
		inter.spin_2(libss->GetSpinNumber(thisInter->GetSpin2()));
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
		eseq.resize(9);  //Yes we need to do this. Codesynthsis
		//doesn't do it for us
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
		SpinXML::Orientation o;
		double xx,yy,zz;
		thisInter->GetEigenvalues(&xx,&yy,&zz,&o);
		eigenvalues eigv(xx,yy,zz);
		inter.orientation(ConvertOrientationToXML(o));
		inter.eigenvalues(eigv);
		break;
	    }
	    case SpinXML::Interaction::AXRHOM: {
		SpinXML::Orientation o;
		double ax,rhom,iso;
		thisInter->GetAxRhom(&ax,&rhom,&iso,&o);
		axiality_rhombicity ar(iso,ax,rhom);
		inter.orientation(ConvertOrientationToXML(o));
		inter.axiality_rhombicity(ar);
		break;
	    }
	    case SpinXML::Interaction::SPANSKEW: {
		SpinXML::Orientation o;
		double span,skew,iso;
		thisInter->GetSpanSkew(&span,&skew,&iso,&o);
		span_skew spsk(iso,span,skew);
		inter.orientation(ConvertOrientationToXML(o));
		inter.span_skew(spsk);
		break;
	    }
	    default:
		throw std::runtime_error("Interaction is of an unknown form");
		};
	    inters.push_back(inter);
	}
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

