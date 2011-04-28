
#include <shared/formats/tinyxml/tinyxml.h>
#include <shared/spinsys.hpp>
#include <shared/interaction.hpp>
#include <shared/formats/xml.hpp>
#include <shared/foreach.hpp>
#include <shared/basic_math.hpp>
#include <map>

using namespace SpinXML;
using namespace std;
using namespace Eigen;

#define LAB_FRAME 0

//================================================================================//
//                                     Units                                      //
//================================================================================//
// For now, everything in Anstroms and MHz. If the units field is set
// to anything else it's wrong

string _MHz_ = "MHz";
string _Angstrom_ = "Angstroms";
string _unitless_ = "unitless";

//================================================================================//
//                                     INIT                                       //
//================================================================================//

map<Interaction::Type,string> gType2XMLKind;
map<string,Interaction::Type> gXMLKind2Type;

__XMLInit::__XMLInit() {
	gType2XMLKind[Interaction::HFC]              = "hfc";
	gType2XMLKind[Interaction::G_TENSER]	     = "g-tenser";
	gType2XMLKind[Interaction::ZFS]     	     = "zfs";
	gType2XMLKind[Interaction::EXCHANGE]	     = "exchange";
	gType2XMLKind[Interaction::SHIELDING]        = "shielding";
	gType2XMLKind[Interaction::SCALAR]           = "scalar";
	gType2XMLKind[Interaction::QUADRUPOLAR]      = "quadrupolar";
	gType2XMLKind[Interaction::DIPOLAR]          = "dipolar";
	gType2XMLKind[Interaction::CUSTOM_LINEAR]    = "custem";
	gType2XMLKind[Interaction::CUSTOM_BILINEAR]  = "custem";
	gType2XMLKind[Interaction::CUSTOM_QUADRATIC] = "custem";

	gXMLKind2Type["hfc"]         = Interaction::HFC;
	gXMLKind2Type["g-tenser"]    = Interaction::G_TENSER;
	gXMLKind2Type["zfs"]         = Interaction::ZFS; 
	gXMLKind2Type["exchange"]    = Interaction::EXCHANGE ;
	gXMLKind2Type["shielding"]   = Interaction::SHIELDING;
	gXMLKind2Type["scalar"]      = Interaction::SCALAR; 
	gXMLKind2Type["quadrupolar"] = Interaction::QUADRUPOLAR;
	gXMLKind2Type["dipolar"]     = Interaction::DIPOLAR;
	gXMLKind2Type["custem"]		 = Interaction::CUSTOM_LINEAR;
}


__XMLInit::~__XMLInit() {

}

static __XMLInit __init;

//================================================================================//
//                                proto-structures                                //
//================================================================================//

struct ProtoSpin {
	int number;

	double x;
	double y;
	double z;

	int element;
	int isotope;
	string label;

	int frame;
};

struct ProtoInteraction {
	int spin1;
	int spin2;
	
	unit _unit;

	Interaction::Type type;
	InteractionPayload payload;

	int payloadFrame;
	int orientFrame;
};

struct ProtoFrame {
	int number;

	double x;
	double y;
	double z;

	Orientation o;
};


//================================================================================//
//                                     UTILS                                      //
//================================================================================//

string dbleToStr(double d) {
	ostringstream s;
	s << d;
	return s.str();
}


//Some element names as std::strings (prevents typos)
string _spin_system_ = "spin_system";
string _spin_ = "spin";
string _interaction_ = "interaction";
string _reference_frame_ = "reference_frame";

string _scalar_ = "scalar";
string _tensor_ = "tensor";
string _eigenvalues_ = "eigenvalues";
string _axrhom_ = "axiality_rhombicity";
string _spanskew_ = "span_skew";

string _orientation_ = "orientation";

string _angle_ = "angle";
string _axis_ = "axis";

string _euler_ = "euler_angles";
string _dcm_ = "dcm";
string _quaternion_ = "quaternion";
string _angle_axis_ = "angle_axis";

//================================================================================//
//                                     LOADING                                    //
//================================================================================//

void Assemble(SpinSystem* ss,
			  const vector<ProtoSpin>& spins,
			  const vector<ProtoInteraction>& interactions,
			  const vector<ProtoFrame>& frames) {
	map<long,Spin*> number2spin;
	foreach(ProtoSpin protoSpin,spins) {
		Vector3d position(protoSpin.x,protoSpin.y,protoSpin.z);
		Spin* spin = new Spin(position,protoSpin.label,protoSpin.element,protoSpin.isotope);
		number2spin[protoSpin.number] = spin;
		ss->InsertSpin(spin);
	}
}

void Guard(int returnCode,const char* error) {
	switch(returnCode) {
	case TIXML_SUCCESS:
		break;
	case TIXML_WRONG_TYPE:
		throw runtime_error(error);
		break;
	case TIXML_NO_ATTRIBUTE:
		throw runtime_error(error);
		break;
	default:
		throw runtime_error("TinyXML::Query*Attribute retunred an unknown error code");
	}
}

const TiXmlElement* Guard(const TiXmlNode* node,const char* error) {
	if(!node) {
		throw runtime_error(error);
	}
	const TiXmlElement* e = node->ToElement();
	if(!e) {
		throw runtime_error(error);
	}
	return e;
}

Matrix3d decodeMatrix(const TiXmlElement* e) {
	double
		xx,xy,xz,
		yx,yy,yz,
		zx,zy,zz;
	Guard(e->QueryDoubleAttribute("xx",&xx),"No xx attribute for matrix");
    Guard(e->QueryDoubleAttribute("xy",&xy),"No xy attribute for matrix");
	Guard(e->QueryDoubleAttribute("xz",&xz),"No xz attribute for matrix");
		   				          			  
	Guard(e->QueryDoubleAttribute("yx",&yx),"No yx attribute for matrix");
	Guard(e->QueryDoubleAttribute("yy",&yy),"No yy attribute for matrix");
	Guard(e->QueryDoubleAttribute("yz",&yz),"No yz attribute for matrix");
		   				          			  
	Guard(e->QueryDoubleAttribute("zx",&zx),"No zx attribute for matrix");
	Guard(e->QueryDoubleAttribute("zy",&zy),"No zy attribute for matrix");
	Guard(e->QueryDoubleAttribute("zz",&zz),"No zz attribute for matrix");

	return MakeMatrix3d(xx,xy,xz,
						yx,yy,yz,
						zx,zy,zz);
}

void decodeOrientation(const TiXmlElement* e,Orientation& o,int& frame) {
	Guard(e->QueryIntAttribute(_reference_frame_,&frame),"Not reference frame in orientation");
	const TiXmlNode* payloadNode = NULL;
	if((payloadNode = e->FirstChild(_euler_))) {
		const TiXmlElement* eulerEl = Guard(payloadNode,"Malformed euler angles element");
		EulerAngles ea(0,0,0);
		Guard(eulerEl->QueryDoubleAttribute("alpha",&ea.alpha),"No alpha attribute for euler angles");
		Guard(eulerEl->QueryDoubleAttribute("beta", &ea.beta ),"No beta  attribute for euler angles");
		Guard(eulerEl->QueryDoubleAttribute("gamma",&ea.gamma),"No gamma attribute for euler angles");
		o = ea;
	} else if((payloadNode = e->FirstChild(_dcm_))) {
		const TiXmlElement* matrixEl = Guard(payloadNode,"Malformed DCM element");
		Matrix3d mat = decodeMatrix(matrixEl);
		o = mat;
	} else if((payloadNode = e->FirstChild(_quaternion_))) {
		const TiXmlElement* quaternionEl = Guard(payloadNode,"Malformed quaternion element");
		double x,y,z,w;
		Guard(quaternionEl->QueryDoubleAttribute("x" ,&x),"No x attribute for quaternion");
		Guard(quaternionEl->QueryDoubleAttribute("y" ,&y),"No y attribute for quaternion");
		Guard(quaternionEl->QueryDoubleAttribute("z" ,&z),"No z attribute for quaternion");
		Guard(quaternionEl->QueryDoubleAttribute("re",&w),"No re attribute for quaternion");

		o = Quaterniond(x,y,z,w);
	} else if((payloadNode = e->FirstChild(_angle_axis_))) {
		const TiXmlElement* angleAxisEl = Guard(payloadNode,"Malformed angle axis element");
		
		const TiXmlElement* angleEl = Guard(angleAxisEl->FirstChild(_angle_),"Malformed or missing angle element for angle axis");
		//TODO, HACK, use of atof
		double angle = atof(angleEl->GetText());
		
		const TiXmlElement* axisEl  = Guard(angleAxisEl->FirstChild(_axis_) ,"Malformed or missing axis element for angle axis");
		double x,y,z;
		Guard(axisEl->QueryDoubleAttribute("x" ,&x),"No x attribute for angle-axis");
		Guard(axisEl->QueryDoubleAttribute("y" ,&y),"No y attribute for angle-axis");
		Guard(axisEl->QueryDoubleAttribute("z" ,&z),"No z attribute for angle-axis");
		o = AngleAxisd(angle,Vector3d(x,y,z));
	} else {
		throw runtime_error("No euler angles or DCM or quaternion or angle axis element for orientation");
	}
}

void decodeInteraction(const TiXmlElement* e,ProtoInteraction& protoInteraction) {
	string xmlKind;
	Guard(e->QueryStringAttribute("kind",&xmlKind),  "missing kind attribute");
	map<string,Interaction::Type>::iterator it = gXMLKind2Type.find(xmlKind);
	if(it != gXMLKind2Type.end()) {
		protoInteraction.type = (*it).second;
	} else {
		throw runtime_error("malformed kind attribute");
	}
	Guard(e->QueryIntAttribute("spin_1",&protoInteraction.spin1),"missing spin1 attribute");
	//TODO: This really *needs* to be optional
	Guard(e->QueryIntAttribute("spin_2",&protoInteraction.spin2),"missing spin2 attribute");
	
	string unitStr;
	Guard(e->QueryStringAttribute("units",&unitStr),"missing units in interaction");
	if(protoInteraction.type!=Interaction::G_TENSER && unitStr != _MHz_) {
		throw runtime_error("Invalid unit in interaction");
	} else if(protoInteraction.type==Interaction::G_TENSER && unitStr != _unitless_) {
		throw runtime_error("Invalid unit in interaction");
	}


	//Be leinient about mixing up the order of eigenvalues etc. and
	//orientation
	const TiXmlNode* mag = NULL; //One of scalar|tensor|eigenvalues|axiality_rhombicity|span_skew
	if((mag = e->FirstChild(_scalar_))) {
		const TiXmlElement* scalar = Guard(mag,"Malformed scalar element");

		//TODO,HACK atof does not report failuar, just 0.0
		protoInteraction.payload = atof(scalar->GetText());
	} else if((mag = e->FirstChild(_tensor_))) {
		const TiXmlElement* matrix = Guard(mag,"Malformed tensor element");

		//TODO,HACK atof does not report failuar, just 0.0
		protoInteraction.payload = decodeMatrix(matrix);
		Guard(matrix->QueryIntAttribute(_reference_frame_,&protoInteraction.payloadFrame),"No reference frame for matrix");
	} else if((mag = e->FirstChild(_eigenvalues_))) {
		const TiXmlElement* evEl = Guard(mag,"Malformed eigenvalue element");

		Eigenvalues ev(0,0,0,EulerAngles(0,0,0));
		Guard(evEl->QueryDoubleAttribute("XX",&ev.xx),"No xx attribute for eigenvalues");
		Guard(evEl->QueryDoubleAttribute("YY",&ev.yy),"No yy attribute for eigenvalues");
		Guard(evEl->QueryDoubleAttribute("ZZ",&ev.zz),"No zz attribute for eigenvalues");

		const TiXmlElement* orientEl = Guard(e->FirstChild(_orientation_),"eigenvalue specification requires an orientation");
		int frame;
		decodeOrientation(orientEl,ev.mOrient,frame);
		protoInteraction.payload = ev;
		protoInteraction.orientFrame = frame;
		
	} else if((mag = e->FirstChild(_axrhom_))) {
		const TiXmlElement* axRhEl = Guard(mag,"Malformed axiality-rhombicity element");
		
		AxRhom ar(0,0,0,EulerAngles(0,0,0));
		Guard(axRhEl->QueryDoubleAttribute("iso",&ar.iso),"No iso attribute for axiality_rhombicity");
		Guard(axRhEl->QueryDoubleAttribute("ax", &ar.ax) ,"No ax attribute for axiality_rhombicity");
		Guard(axRhEl->QueryDoubleAttribute("rh", &ar.rh) ,"No rh attribute for axiality_rhombicity");

		const TiXmlElement* orientEl = Guard(e->FirstChild(_orientation_),"axiality-rhombicity specification requires an orientation");
		int frame;
		decodeOrientation(orientEl,ar.mOrient,frame);
		protoInteraction.payload = ar;
		protoInteraction.orientFrame = frame;

	} else if((mag = e->FirstChild(_spanskew_))) {
		const TiXmlElement* spanSkewEl = Guard(mag,"Malformed span-skew element");

		SpanSkew spanSkew(0,0,0,EulerAngles(0,0,0));
		Guard(spanSkewEl->QueryDoubleAttribute("iso",&spanSkew.iso),  "No iso  attribute for span_skew");
		Guard(spanSkewEl->QueryDoubleAttribute("ax", &spanSkew.span) ,"No span attribute for span_skew");
		Guard(spanSkewEl->QueryDoubleAttribute("rh", &spanSkew.skew) ,"No skew attribute for span_skew");

		const TiXmlElement* orientEl = Guard(e->FirstChild(_orientation_),"span-skew specification requires an orientation");
		int frame;
		decodeOrientation(orientEl,spanSkew.mOrient,frame);
		protoInteraction.payload = spanSkew;
		protoInteraction.orientFrame = frame;

	} else {
		throw runtime_error("interaction missing it's definition in terms of one of scalar,tensor,eigenvalues,axiality_rhombicity,span_skew");
	}
}

void SpinXML::XMLLoader::LoadFile(SpinSystem* ss,const char* filename) const {
	TiXmlDocument doc(filename);
	bool loadOkay = doc.LoadFile(TIXML_ENCODING_UTF8);
	if(!loadOkay) {
		throw runtime_error("Couldn't parse XML");
	}

	TiXmlElement* root = doc.RootElement();
	if(!root) {
		throw runtime_error("Couldn't find the root spin_system node");
	}

	//Okay, we have the <spin_system> bit. The order that entities
	//should appear in the fire according to the spec is spin*
	//interaction* reference_frame* but we the reference_frames
	//frist. We might as well be agnostic about the order we recieve
	//the elements

	//Also, since we'll be throwing exceptions about like they're
	//going out of fassion, new or anything else that needs explicit
	//cleanup is banned in this next section

	vector<ProtoSpin> protoSpins;
	vector<ProtoInteraction> protoInteractions;
	vector<ProtoFrame> protoFrames;

	TiXmlNode* child = NULL;
	while((child = root->IterateChildren(child))) {
		TiXmlElement* e = child->ToElement();
		if(!e) {
			continue;
		}
		if(e->Value() == _reference_frame_) {
		} else if(e->Value() == _spin_) {
			ProtoSpin spin;
			Guard(e->QueryIntAttribute("number",&spin.number),  "missing spin number  attribute");
			Guard(e->QueryIntAttribute("element",&spin.element),"missing spin element attribute");
			Guard(e->QueryIntAttribute("isotope",&spin.isotope),"missing spin isotope attribute");
			Guard(e->QueryStringAttribute("label",&spin.label), "missing label attribute");

			const TiXmlElement* coords = Guard(e->FirstChild("coordinates"),"missing or malformed coordinate");

			Guard(coords->QueryDoubleAttribute("x",&spin.x),"missing x attribute");
			Guard(coords->QueryDoubleAttribute("y",&spin.x),"missing y attribute");
			Guard(coords->QueryDoubleAttribute("z",&spin.x),"missing z attribute");
			Guard(coords->QueryIntAttribute("z",&spin.frame),"missing reference frame");

			protoSpins.push_back(spin);
		} else if(e->Value() == _interaction_) {
		} else {
			//We have something else, perhaps emit a warning?
		}
	}
	Assemble(ss,protoSpins,protoInteractions,protoFrames);
}

//================================================================================//
//                                     SAVING                                     //
//================================================================================//
// For simplicitly we will save everything in angstroms and MHz                   //
//================================================================================//

///Simple module-private class to manage the shared state. Don't
///expect any far flung OO ideas here, this could easily be a buch of
///plain old functions with a few more paramiters passed about (in
///fact, that's how it started)

class XMLSaver {
public:
	void encodeMatrix(Matrix3d mat,TiXmlElement* el) const {
		el->SetAttribute("xx",mat(0,0));
		el->SetAttribute("xy",mat(0,1));
		el->SetAttribute("xz",mat(0,2));

		el->SetAttribute("yx",mat(1,0));
		el->SetAttribute("yy",mat(1,1));
		el->SetAttribute("yz",mat(1,2));

		el->SetAttribute("zx",mat(2,0));
		el->SetAttribute("zy",mat(2,1));
		el->SetAttribute("zz",mat(2,2));
	}

	void encodeOrient(const Orientation& orient,TiXmlElement* el) const {
		switch(orient.GetType()) {
		case Orientation::EULER: {
			TiXmlElement* eaEl = new TiXmlElement(_euler_);
			EulerAngles ea = orient.GetAsEuler();
			eaEl->SetAttribute("alpha",ea.alpha);
			eaEl->SetAttribute("beta" ,ea.beta);
			eaEl->SetAttribute("gamma",ea.gamma);
			eaEl->SetAttribute(_reference_frame_,LAB_FRAME);
			el->LinkEndChild(eaEl);
			break;
		}
		case Orientation::DCM: {
			TiXmlElement* matrixEl = new TiXmlElement(_dcm_);
			encodeMatrix(orient.GetAsMatrix(),matrixEl);
			matrixEl->SetAttribute(_reference_frame_,LAB_FRAME);
			el->LinkEndChild(matrixEl);
			break;
		}
		case Orientation::QUATERNION: {
			TiXmlElement* qEl = new TiXmlElement(_quaternion_);
			Quaterniond q = orient.GetAsQuaternion();
			qEl->SetAttribute("re",q.w());
			qEl->SetAttribute("i",q.x());
			qEl->SetAttribute("j",q.y());
			qEl->SetAttribute("k",q.z());
			qEl->SetAttribute(_reference_frame_,LAB_FRAME);

			el->LinkEndChild(qEl);
			break;
		}
		case Orientation::ANGLE_AXIS: {
			TiXmlElement* aaEl = new TiXmlElement(_angle_axis_);
			AngleAxisd aa = orient.GetAsAngleAxis();

			TiXmlElement* angleEl = new TiXmlElement(_angle_);
			angleEl->LinkEndChild(new TiXmlText(dbleToStr(aa.angle())));
			aaEl->LinkEndChild(angleEl);
		
			TiXmlElement* axisEl = new TiXmlElement(_axis_);
			axisEl->SetAttribute("x",aa.axis().x());
			axisEl->SetAttribute("y",aa.axis().y());
			axisEl->SetAttribute("z",aa.axis().z());
			axisEl->SetAttribute(_reference_frame_,LAB_FRAME);
			aaEl->LinkEndChild(axisEl);

			el->LinkEndChild(aaEl);
			break;
		}
		}
	}


	int lookupFrame(Frame* frame) const {
		Frame* lab = ss->GetLabFrame();

		if(frame == lab || frame == NULL) {
			return LAB_FRAME;
		} else {
			return (*frame2Number.find(frame)).second;
		}
	}

	void saveFrameRecusion(Frame* frame,TiXmlElement* frameEl) const {
		foreach(Frame* childFrame,frame->GetChildren()) {
			TiXmlElement* childEl = new TiXmlElement("reference_frame");
			childEl->SetAttribute("number",(*frame2Number.find(childFrame)).second);
			childEl->SetAttribute("label","FRAME");

			TiXmlElement* originEl = new TiXmlElement("origin");
			originEl->SetAttribute("x",frame->GetTranslation().x() / Angstroms);
			originEl->SetAttribute("y",frame->GetTranslation().y() / Angstroms);
			originEl->SetAttribute("z",frame->GetTranslation().z() / Angstroms);
			originEl->SetAttribute(_reference_frame_,LAB_FRAME);
			childEl->LinkEndChild(originEl);
		
			TiXmlElement* orientEl = new TiXmlElement("orientation");
			encodeOrient(frame->GetOrientation(),orientEl);
			childEl->LinkEndChild(orientEl);

			frameEl->LinkEndChild(childEl);
			saveFrameRecusion(childFrame,childEl);
		}
	}

	int enumerateFrameRecusion(Frame* frame,int counter = 1) {
		foreach(Frame* childFrame,frame->GetChildren()) {
			counter++;
			frame2Number.insert(pair<Frame*,int>(childFrame,counter));
			counter = enumerateFrameRecusion(childFrame,counter);
		}
		return counter;
	}


	void encodeInterStorage(const Interaction* inter,TiXmlElement* interEl) const {
		Orientation o;
		unit u = inter->GetType() == Interaction::G_TENSER ? Unitless : MHz;
		switch(inter->GetStorage()) {
		case Interaction::STORAGE_SCALAR: {
			TiXmlElement* scalarEl = new TiXmlElement(_scalar_);
			scalarEl->SetValue(dbleToStr(inter->AsScalar() * u));
			interEl->LinkEndChild(scalarEl);
			break;
		}
		case Interaction::MATRIX: {
			TiXmlElement* matrixEl = new TiXmlElement(_tensor_);
			encodeMatrix(inter->AsMatrix() * u,matrixEl);
			matrixEl->SetAttribute("reference_frame",0);
			interEl->LinkEndChild(matrixEl);
			break;
		}
		case Interaction::EIGENVALUES: {
			TiXmlElement* evEl = new TiXmlElement(_eigenvalues_);
			Eigenvalues ev = inter->AsEigenvalues();
			evEl->SetAttribute("XX",ev.xx / u);
			evEl->SetAttribute("YY",ev.yy / u);
			evEl->SetAttribute("ZZ",ev.zz / u);
			interEl->LinkEndChild(evEl);

			o = ev.mOrient;
			break;
		}
		case Interaction::AXRHOM: {
			TiXmlElement* arEl = new TiXmlElement(_axrhom_);
			AxRhom ar = inter->AsAxRhom();
			arEl->SetAttribute("rh",ar.rh   / u);
			arEl->SetAttribute("iso",ar.iso / u);
			arEl->SetAttribute("ax",ar.ax   / u);
			interEl->LinkEndChild(arEl);

			o = ar.mOrient;
			break;
		}
		case Interaction::SPANSKEW: {
			TiXmlElement* ssEl = new TiXmlElement(_spanskew_);
			SpanSkew ss = inter->AsSpanSkew();
			ssEl->SetAttribute("span",ss.span  / u);
			ssEl->SetAttribute("skew",ss.skew);
			ssEl->SetAttribute("iso", ss.iso   / u);
			interEl->LinkEndChild(ssEl);

			o = ss.mOrient;
			break;
		}
		}
		if(inter->GetStorage() == Interaction::EIGENVALUES ||
		   inter->GetStorage() == Interaction::AXRHOM ||
		   inter->GetStorage() == Interaction::SPANSKEW) {
			TiXmlElement* orientEl = new TiXmlElement(_orientation_);
			encodeOrient(o,orientEl);
			interEl->LinkEndChild(orientEl);
		}
	}


	void doIt(const char* filename) {
		//Setup the XML document
		TiXmlDocument doc;
		TiXmlElement* root = new TiXmlElement("spin_system");
		doc.LinkEndChild(root);

		//Before we do anything, we need to assign a number to every frame
		enumerateFrameRecusion(ss->GetLabFrame());

		//Save spins
		long counter = 0;
		foreach(Spin* spin,ss->GetSpins()) {
			int frameNumber = lookupFrame(spin->GetPreferedFrame());

			TiXmlElement* spinEl = new TiXmlElement("spin");
			spinEl->SetAttribute("number",counter);
			spinEl->SetAttribute("element",spin->GetElement());
			spinEl->SetAttribute("isotope",spin->GetIsotope());
			spinEl->SetAttribute("label",spin->GetLabel());

			TiXmlElement* coordEl = new TiXmlElement("coordinates");
			coordEl->SetAttribute("x",spin->GetPosition().x() / Angstroms);
			coordEl->SetAttribute("y",spin->GetPosition().y() / Angstroms);
			coordEl->SetAttribute("z",spin->GetPosition().z() / Angstroms);
			coordEl->SetAttribute("reference_frame",frameNumber);

			spinEl->LinkEndChild(coordEl);

			root->LinkEndChild(spinEl);
			counter++;
		}

		//Save Interactions
		foreach(Interaction* inter,ss->GetAllInteractions()) {
			TiXmlElement* interEl = new TiXmlElement("interaction");

			long spin1n = ss->GetSpinNumber(inter->GetSpin1());
			long spin2n = ss->GetSpinNumber(inter->GetSpin2());

			interEl->SetAttribute("kind" ,gType2XMLKind[inter->GetType()]);
			if(inter->GetType() == Interaction::G_TENSER) {
				interEl->SetAttribute("units",_MHz_);
			} else {
				interEl->SetAttribute("units",_unitless_);
			}
			interEl->SetAttribute("spin_1",spin1n);
			interEl->SetAttribute("spin_2",spin2n);

			encodeInterStorage(inter,interEl);

			root->LinkEndChild(interEl);
		}
		saveFrameRecusion(ss->GetLabFrame(),root);

	
		doc.SaveFile(filename);

	}
	XMLSaver(const SpinSystem* _ss) : ss(_ss) {
	}
private:
	map<Frame*,int> frame2Number;
	const SpinSystem* ss;
};

void SpinXML::XMLLoader::SaveFile(const SpinSystem* ss,const char* filename) const {
	XMLSaver o(ss);
	o.doIt(filename);
}

