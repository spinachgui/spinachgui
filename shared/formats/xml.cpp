
#include <shared/formats/xml.hpp>
#include <shared/formats/xsd/spinxml_schema.hpp>
#include <shared/nuclear_data.hpp>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <shared/basic_math.hpp>

using namespace SpinXML;
using namespace xml_schema;

using std::endl;
using std::cout;
using std::cerr;

#define LAB_FRAME 0
/*
kind::value GetXSDTypeFromSpinXMLType(SpinXML::Interaction::Type subtype) {
  switch(subtype) {
  case SpinXML::Interaction::HFC:
    return kind::hfc;
  case SpinXML::Interaction::G_TENSER:
    return kind::g_tenser;
  case SpinXML::Interaction::ZFS:
    return kind::zfs;
  case SpinXML::Interaction::EXCHANGE:
    return kind::exchange;
  case SpinXML::Interaction::SHIELDING:
    return kind::shielding;
  case SpinXML::Interaction::SCALAR:
    return kind::scalar;
  case SpinXML::Interaction::QUADRUPOLAR:
    return kind::quadrupolar;
  case SpinXML::Interaction::DIPOLAR:
    return kind::dipolar;
  case SpinXML::Interaction::CUSTOM_LINEAR:
  case SpinXML::Interaction::CUSTOM_BILINEAR:
  case SpinXML::Interaction::CUSTOM_QUADRATIC:
    return kind::custem;
  default:
    throw std::runtime_error("Unknown sub type when outputing to an XML file");
  }
}

SpinXML::Interaction::Type GetSpinXMLTypeFromXSDType(kind::value subtype) {
  switch(subtype) {
  case kind::hfc:
    return SpinXML::Interaction::HFC;
  case kind::g_tenser:
    return SpinXML::Interaction::G_TENSER;
  case kind::zfs:
    return SpinXML::Interaction::ZFS;
  case kind::exchange:
    return SpinXML::Interaction::EXCHANGE;
  case kind::shielding:
    return SpinXML::Interaction::SHIELDING;
  case kind::scalar:
    return SpinXML::Interaction::SCALAR;
  case kind::quadrupolar:
    return SpinXML::Interaction::QUADRUPOLAR;
  case kind::dipolar:
    return SpinXML::Interaction::DIPOLAR;
  case kind::custem:
    return SpinXML::Interaction::CUSTOM_LINEAR;
  default:
    throw std::runtime_error("Unknown sub type when when reading an XML file. Is the right schema loaded?");
  }
}

long GetReferenceFrameNumber(const orientation o) {
	if(o.euler_angles().present()) {
		return o.euler_angles().get().reference_frame();
	} else if(o.angle_axis().present()) {
		return o.angle_axis().get().axis().reference_frame();
	} else if(o.quaternion().present()) {
		return o.quaternion().get().reference_frame();
	} else if(o.dcm().present()) {
		return o.dcm().get().reference_frame();
	}
	//This should never happen, but it does, fail noisly. Also shuts up a compiler warning.
	throw std::logic_error("GetReferenceFrameNumber() was unable to determin the reference frame of an orientation");
}

void SetReferenceFrameNumber(orientation& o,long n) {
	if(o.euler_angles().present()) {
		o.euler_angles().get().reference_frame(n);
	} else if(o.angle_axis().present()) {
		o.angle_axis().get().axis().reference_frame(n);
	} else if(o.quaternion().present()) {
		o.quaternion().get().reference_frame(n);
	} else if(o.dcm().present()) {
		o.dcm().get().reference_frame(n);
	}
}


orientation ConvertOrientationToXML(const SpinXML::Orientation& o) {
  SpinXML::Orientation::Type type=o.GetType();
  orientation oout;
  switch(type) {
  case SpinXML::Orientation::EULER:{
    EulerAngles ea=o.GetAsEuler();
    euler_angles xsd_ea(ea.alpha,ea.beta,ea.gamma,LAB_FRAME);
    oout.euler_angles(xsd_ea);
    break;
  }
  case SpinXML::Orientation::ANGLE_AXIS: {
    Vector3d axis;
    AngleAxisd aa=o.GetAsAngleAxis();
    vector axis_prime(double(aa.axis().x()),
					  double(aa.axis().y()),
					  double(aa.axis().z()),LAB_FRAME);
    angle_axis xsd_aa(aa.angle(),axis_prime);
    oout.angle_axis(xsd_aa);
    break;
  }
  case SpinXML::Orientation::QUATERNION: {
    Quaterniond q=o.GetAsQuaternion();
    quaternion xsd_q(q.w(),q.x(),q.y(),q.z(),LAB_FRAME);
    oout.quaternion(xsd_q);
    break;
  }
  case SpinXML::Orientation::DCM: {
	  Matrix3d dcm = o.GetAsDCM();
	  matrix mat(dcm(0,0),dcm(0,1),dcm(0,2),
				 dcm(1,0),dcm(1,1),dcm(1,2),
				 dcm(2,0),dcm(2,1),dcm(2,2),LAB_FRAME);
	  oout.dcm(mat);
	  break;
  }
  default:
    throw std::runtime_error("Unknown orientation type encounted");
  }
  return oout;
}

SpinXML::Orientation ConvertXMLToOrientation(const orientation& o) {
	SpinXML::Orientation ret(Quaterniond(1,0,0,0));
	if(o.euler_angles().present()) {
		double alpha,beta,gamma;
		euler_angles ea = o.euler_angles().get();
		alpha=ea.alpha();
		beta=ea.beta();
		gamma=ea.gamma();
		ret = EulerAngles(alpha,beta,gamma);
	} else if(o.angle_axis().present()) {
		angle_axis aa = o.angle_axis().get();
		vector axis=aa.axis();
		double angle=aa.angle();
		ret = AngleAxisd(angle,Vector3d(axis.x(),axis.y(),axis.z()));
	} else if(o.quaternion().present()) {
		double re,i,j,k;
		quaternion q = o.quaternion().get();
		re=q.re();
		i=q.i();
		j=q.j();
		k=q.k();
		ret = Quaterniond(re,i,j,k);
	} else if(o.dcm().present()) {
		matrix xsd_dcm = o.dcm().get();
		ret = MakeMatrix3d(xsd_dcm.xx(),xsd_dcm.xy(),xsd_dcm.xz(),
						   xsd_dcm.yx(),xsd_dcm.yy(),xsd_dcm.yz(),
						   xsd_dcm.zx(),xsd_dcm.zy(),xsd_dcm.zz());

	} else {
		throw std::runtime_error("Unknown orientation form encounted. Is the XSD schema corrupt?");
	}
	return ret;
}

*/
void SpinXML::XMLLoader::LoadFile(SpinSystem* libss,const char* filename) const {
	/*
	//libss => "library spin system" as apposed to the xml spin system ss
	std::auto_ptr<spin_system> ss;
	try {
		xml_schema::properties p;
		p.no_namespace_schema_location(mSchemaLocation);
		ss=spin_system_(filename,0,p);
	} catch(const xml_schema::exception& e) {
		std::cerr << e << std::endl;
		std::ostringstream errStream;
		errStream << e << std::endl;
		throw std::runtime_error(errStream.str());
	}

	libss->Clear();

	//Load the reference frames.
	spin_system::reference_frame_sequence frames=ss->reference_frame();
	long frameCount=frames.size();

	//We can't assume they appear in order so we need to make two
	//passes. On the first pass we allocate memory and store the
	//pointers in a integer->frame map.
	std::map<long,Frame*> frameMap;

	for(long i=0;i<frameCount;i++) {
		vector r=frames[i].origin();
		if(frames[i].number() == 0) {
			throw std::runtime_error("Reference frame 0 is reserved as the lab frame");
		}
		Frame* frame = new Frame(Vector3d(r.x(),r.y(),r.z()),
								 ConvertXMLToOrientation(frames[i].orientation()),
								 UnitSystem::GetDefault());
		frameMap.insert(std::pair<long,Frame*>(frames[i].number(),frame));
	}
	Frame* labFrame = libss->GetLabFrame();
	for(long i=0;i<frameCount;i++) {
		long parent = frames[i].origin().reference_frame();
		long parent2 = GetReferenceFrameNumber(frames[i].orientation());

		if(parent != parent2) {
			throw std::runtime_error("Reference frame parents mismatch");
		}

		if(parent == 0) { //This is in the lab frame
			labFrame->AddChild(frameMap[frames[i].number()]);
			continue;
		}
		frameMap[parent]->AddChild(frameMap[frames[i].number()]);
	}


	//Load the spins
	spin_system::spin_sequence spins=ss->spin();
	long spinCount=spins.size();
	for(long i=0;i<spinCount;i++) {
		spin xsdSpin=spins[i];

		length x,y,z;
		vector coords=xsdSpin.coordinates().present() ? xsdSpin.coordinates().get() : vector(0,0,0,LAB_FRAME);
		x=coords.x();
		y=coords.y();
		z=coords.z();

		SpinXML::Spin* newSpin = new Spin(Vector3d(x,y,z),
										  xsdSpin.label().present() ? xsdSpin.label().get() : "",
										  getElementBySymbol(xsdSpin.isotope().c_str()),
										  getIsotopeBySymbol(xsdSpin.isotope().c_str()));
		libss->InsertSpin(newSpin);
	}

	//Load the interactions
	spin_system::interaction_sequence inters=ss->interaction();
	long interactionCount=inters.size();

	for(long i=0;i<interactionCount;i++) {
		SpinXML::Interaction* thisInter = NULL;
		interaction xsdInter=inters[i];

		SpinXML::Interaction::Type type=GetSpinXMLTypeFromXSDType(xsdInter.kind());

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
			switch(type) {
			case SpinXML::Interaction::G_TENSER:
			case SpinXML::Interaction::SHIELDING:
			case SpinXML::Interaction::CUSTOM_LINEAR:
				thisInter = new SpinXML::Interaction(0.0,type,libss->GetSpin(spinNumber));
				break;
			default:
				throw std::runtime_error("Linear interaction was of a non linear subtype");
			}
		} else if(spinNumber2==spinNumber2) {
			//Interaction must be quadratic
			switch(type) {
			case SpinXML::Interaction::QUADRUPOLAR:
			case SpinXML::Interaction::ZFS:
			case SpinXML::Interaction::CUSTOM_QUADRATIC:
				thisInter = new SpinXML::Interaction(0.0,type,libss->GetSpin(spinNumber));
				break;
			default:
				throw std::runtime_error("Quadrupolar interaction was of a non linear subtype");
			}
		} else {
			switch(type) {
			case SpinXML::Interaction::EXCHANGE:
			case SpinXML::Interaction::DIPOLAR:
			case SpinXML::Interaction::SCALAR:
			case SpinXML::Interaction::CUSTOM_BILINEAR:
				thisInter = new SpinXML::Interaction(0.0,type,libss->GetSpin(spinNumber),libss->GetSpin(spinNumber2));
				break;
			default:
				throw std::runtime_error("Quadrupolar interaction was of a non linear subtype");
			}
		}


		//Get the numerical value of the teser
		if(xsdInter.scalar().present()) {
			double scalar;
			scalar=xsdInter.scalar().get();
			thisInter->SetScalar(scalar);
		} else if(xsdInter.tensor().present()) {
			tensor xml_matrx = xsdInter.tensor().get();
			Matrix3d mat = MakeMatrix3d(xml_matrx.xx(),xml_matrx.xy(),xml_matrx.xz(),
										xml_matrx.yx(),xml_matrx.yy(),xml_matrx.yz(),
										xml_matrx.zx(),xml_matrx.zy(),xml_matrx.zz());
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
		//Save the interation
		libss->InsertInteraction(thisInter);
	}*/
}

void SpinXML::XMLLoader::SaveFile(const SpinSystem* libss,const char* filename) const {
    /*
	spin_system ss;

    spin_system::spin_sequence spins;
    for(long i=0;i<libss->GetSpinCount();i++) {
		SpinXML::Spin* thisSpin=libss->GetSpin(i);

		double x,y,z;
		thisSpin->GetCoordinates(&x,&y,&z);
		std::string label(thisSpin->GetLabel());


		spin outSpin(i,"");
		outSpin.coordinates(vector(x,y,z,LAB_FRAME));
		outSpin.label(label);
		outSpin.isotope(getElementSymbol(thisSpin->GetElement()));
		spins.push_back(outSpin);
    }
    ss.spin(spins);

    spin_system::interaction_sequence inters;
	std::vector<Interaction*> interactions = libss->GetAllInteractions();
	long InterCount = interactions.size();

    for(long i=0;i<InterCount;i++) {
		SpinXML::Interaction* thisInter=interactions[i];
		interaction1 inter(GetXSDTypeFromSpinXMLType(thisInter->GetType()),
						  "MHz",
						  libss->GetSpinNumber(thisInter->GetSpin1()));

		if(thisInter->GetSpin2() != NULL) {
			inter.spin_2(libss->GetSpinNumber(thisInter->GetSpin2()));
		}

		switch(thisInter->GetStorage()) {
		case SpinXML::Interaction::STORAGE_SCALAR: {
			double scalar;
			thisInter->GetScalar(&scalar);
			inter.scalar(scalar);
			break;
		}
		case SpinXML::Interaction::MATRIX: {
			Matrix3d mat;
			thisInter->GetMatrix(&mat);
			tensor xml_matrix(mat(0,0),mat(1,0),mat(2,0),
							  mat(0,1),mat(1,1),mat(2,1),
							  mat(0,2),mat(1,2),mat(2,2),LAB_FRAME);
			inter.tensor(xml_matrix);
			break;
		}
		case SpinXML::Interaction::EIGENVALUES: {
			SpinXML::Orientation o(Quaterniond(1,0,0,0));
			double xx,yy,zz;
			thisInter->GetEigenvalues(&xx,&yy,&zz,&o);
			eigenvalues eigv(xx,yy,zz);
			inter.orientation(ConvertOrientationToXML(o));
			inter.eigenvalues(eigv);
			break;
		}
		case SpinXML::Interaction::AXRHOM: {
			SpinXML::Orientation o(Quaterniond(1,0,0,0));
			double ax,rhom,iso;
			thisInter->GetAxRhom(&ax,&rhom,&iso,&o);
			axiality_rhombicity ar(iso,ax,rhom);
			inter.orientation(ConvertOrientationToXML(o));
			inter.axiality_rhombicity(ar);
			break;
		}
		case SpinXML::Interaction::SPANSKEW: {
			SpinXML::Orientation o(Quaterniond(1,0,0,0));
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
    ss.interaction(inters);

	//Hack to get a nested function
	struct walkFrameTree {
		static void Walk(Frame* frame,spin_system::reference_frame_sequence* XMLFrameSequence,long* counter,long parent) {
			//Write out this reference frame
			Vector3d T = frame->GetTranslation();
			orientation o = ConvertOrientationToXML(frame->GetOrientation());
			SetReferenceFrameNumber(o,parent);
			reference_frame xmlFrame = reference_frame(vector(T.x(),T.y(),T.z(),parent),o,++(*counter));
			xmlFrame.label("Reference Frame Labels in implimented yet.");
			XMLFrameSequence->push_back(xmlFrame);

			//Recuse down the reference frames
			std::vector<Frame*> children = frame->GetChildren();
			unsigned long length = children.size();
			for(unsigned long i = 0; i < length; i++) {
				Walk(children[i],XMLFrameSequence,counter,*counter);
			}
		}
	};
    spin_system::reference_frame_sequence XMLFrameSequence;
	std::vector<Frame*> frames = libss->GetLabFrame()->GetChildren();
	//Lab frame is implied
	unsigned long length = frames.size();
	long counter=0;
	for(unsigned long i; i<length; i++) {
		walkFrameTree::Walk(frames[i],&XMLFrameSequence,&counter,0);
	}
	ss.reference_frame(XMLFrameSequence);

	//Write to disk

    xml_schema::namespace_infomap map;
    map[""].name = "";
	map[""].schema = mSchemaLocation;

    std::ofstream fout(filename);
    if(!fout.is_open()) {
		std::cerr << "Could not open " << filename << std::endl;
		return;
	}

    try {
		spin_system_(fout, ss, map);
    } catch (const xml_schema::exception& e) {
		std::cerr << e << std::endl;
		std::ostringstream errStream;
		errStream << e << std::endl;
		throw std::runtime_error(errStream.str());
	}*/
}

