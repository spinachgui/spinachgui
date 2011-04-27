
#include <shared/formats/tinyxml/tinyxml.h>
#include <shared/spinsys.hpp>
#include <shared/interaction.hpp>
#include <shared/formats/xml.hpp>
#include <shared/foreach.hpp>
#include <map>

using namespace SpinXML;
using namespace std;
using namespace Eigen;

#define LAB_FRAME 0

//================================================================================//
//                                     INIT                                       //
//================================================================================//

map<Interaction::Type,string> gType2XMLKind;

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
}


__XMLInit::~__XMLInit() {

}

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

TiXmlElement* Guard(TiXmlNode* node,const char* error) {
	if(!node) {
		throw runtime_error(error);
	}
	TiXmlElement* e = node->ToElement();
	if(!e) {
		throw runtime_error(error);
	}
	return e;
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
	while(child = root->IterateChildren(child) ) {
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

			TiXmlElement* coords = Guard(e->FirstChild("coordinates"),"missing or malformed coordinate");

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

void encodeMatrix(Matrix3d mat,TiXmlElement* el) {
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

void encodeOrient(const Orientation& orient,TiXmlElement* el) {
	switch(orient.GetType()) {
	case Orientation::EULER: {
		TiXmlElement* eaEl = new TiXmlElement("euler_angles");
		EulerAngles ea = orient.GetAsEuler();
		eaEl->SetAttribute("alpha",ea.alpha);
		eaEl->SetAttribute("beta" ,ea.beta);
		eaEl->SetAttribute("gamma",ea.gamma);
		el->LinkEndChild(eaEl);
		break;
	}
	case Orientation::DCM: {
		TiXmlElement* matrixEl = new TiXmlElement("dcm");
		encodeMatrix(orient.GetAsMatrix(),matrixEl);
		el->LinkEndChild(matrixEl);
		break;
	}
	case Orientation::QUATERNION: {
		TiXmlElement* qEl = new TiXmlElement("quaternion");
		Quaterniond q = orient.GetAsQuaternion();
		qEl->SetAttribute("re",q.w());
		qEl->SetAttribute("i",q.x());
		qEl->SetAttribute("j",q.y());
		qEl->SetAttribute("k",q.z());

		el->LinkEndChild(qEl);
		break;
	}
	case Orientation::ANGLE_AXIS: {
		TiXmlElement* aaEl = new TiXmlElement("Angle-Axis");
		AngleAxisd aa = orient.GetAsAngleAxis();

		TiXmlElement* angleEl = new TiXmlElement("angle");
		angleEl->SetValue(dbleToStr(aa.angle()));
		aaEl->LinkEndChild(angleEl);
		
		TiXmlElement* axisEl = new TiXmlElement("axis");
		axisEl->SetAttribute("x",aa.axis().x());
		axisEl->SetAttribute("y",aa.axis().y());
		axisEl->SetAttribute("z",aa.axis().z());
		aaEl->LinkEndChild(axisEl);

		el->LinkEndChild(aaEl);
		break;
	}
	}
	el->SetAttribute("reference_frame",LAB_FRAME);
}

TiXmlElement* saveFrameRecusion(Frame* frame) {
	TiXmlElement* frameEl = new TiXmlElement("reference_frame");
	foreach(Frame* childFrame,frame->GetChildren()) {
		TiXmlElement* childEl = saveFrameRecusion(childFrame);
		childEl->SetAttribute("label","FRAME");

		TiXmlElement* originEl = new TiXmlElement("origin");
		originEl->SetAttribute("x",frame->GetTranslation().x());
		originEl->SetAttribute("y",frame->GetTranslation().y());
		originEl->SetAttribute("z",frame->GetTranslation().z());
		childEl->LinkEndChild(originEl);
		
		TiXmlElement* orientEl = new TiXmlElement("orientation");
		encodeOrient(frame->GetOrientation(),orientEl);
		childEl->LinkEndChild(orientEl);

		frameEl->LinkEndChild(childEl);
	}
	return frameEl;
}



void encodeInterStorage(const Interaction* inter,TiXmlElement* interEl) {
	Orientation o;
	switch(inter->GetStorage()) {
	case Interaction::STORAGE_SCALAR: {
		TiXmlElement* scalarEl = new TiXmlElement("scalar");
		scalarEl->SetValue(dbleToStr(inter->AsScalar()));
		interEl->LinkEndChild(scalarEl);
		break;
	}
	case Interaction::MATRIX: {
		TiXmlElement* matrixEl = new TiXmlElement("tensor");
		encodeMatrix(inter->AsMatrix(),matrixEl);
		matrixEl->SetAttribute("reference_frame",0);
		interEl->LinkEndChild(matrixEl);
		break;
	}
	case Interaction::EIGENVALUES: {
		TiXmlElement* evEl = new TiXmlElement("eigenvalues");
		Eigenvalues ev = inter->AsEigenvalues();
		evEl->SetAttribute("XX",ev.xx);
		evEl->SetAttribute("YY",ev.yy);
		evEl->SetAttribute("ZZ",ev.zz);
		interEl->LinkEndChild(evEl);

		o = ev.mOrient;
		break;
	}
	case Interaction::AXRHOM: {
		TiXmlElement* arEl = new TiXmlElement("axiality_rhombicity");
		AxRhom ar = inter->AsAxRhom();
		arEl->SetAttribute("rh",ar.rh);
		arEl->SetAttribute("iso",ar.iso);
		arEl->SetAttribute("ax",ar.ax);
		interEl->LinkEndChild(arEl);

		o = ar.mOrient;
		break;
	}
	case Interaction::SPANSKEW: {
		TiXmlElement* ssEl = new TiXmlElement("eigenvalues");
		SpanSkew ss = inter->AsSpanSkew();
		ssEl->SetAttribute("span",ss.span);
		ssEl->SetAttribute("skew",ss.skew);
		ssEl->SetAttribute("iso", ss.iso);
		interEl->LinkEndChild(ssEl);

		o = ss.mOrient;
		break;
	}
	}
	if(inter->GetStorage() == Interaction::EIGENVALUES ||
	   inter->GetStorage() == Interaction::AXRHOM ||
	   inter->GetStorage() == Interaction::SPANSKEW) {
		TiXmlElement* orientEl = new TiXmlElement("orientation");
		encodeOrient(o,orientEl);
		interEl->LinkEndChild(orientEl);
	}
}


void SpinXML::XMLLoader::SaveFile(const SpinSystem* ss,const char* filename) const {
    TiXmlDocument doc;
	TiXmlElement* root = new TiXmlElement("spin_system");
	doc.LinkEndChild(root);

	long counter = 0;
	foreach(Spin* spin,ss->GetSpins()) {
		TiXmlElement* spinEl = new TiXmlElement("spin");
		spinEl->SetAttribute("number",counter);
		spinEl->SetAttribute("element",spin->GetElement());
		spinEl->SetAttribute("isotope",spin->GetIsotope());
		spinEl->SetAttribute("label",spin->GetLabel());

		TiXmlElement* coordEl = new TiXmlElement("coordinates");
		coordEl->SetAttribute("x",spin->GetPosition().x());
		coordEl->SetAttribute("y",spin->GetPosition().y());
		coordEl->SetAttribute("z",spin->GetPosition().z());
		coordEl->SetAttribute("reference_frame",LAB_FRAME);

		spinEl->LinkEndChild(coordEl);

		root->LinkEndChild(spinEl);
		counter++;
	}
	foreach(Interaction* inter,ss->GetAllInteractions()) {
		TiXmlElement* interEl = new TiXmlElement("interaction");

		long spin1n = ss->GetSpinNumber(inter->GetSpin1());
		long spin2n = ss->GetSpinNumber(inter->GetSpin2());

		interEl->SetAttribute("kind" ,gType2XMLKind[inter->GetType()]);
		interEl->SetAttribute("units","MHz");
		interEl->SetAttribute("spin_1",spin1n);
		interEl->SetAttribute("spin_2",spin2n);

		encodeInterStorage(inter,interEl);

		root->LinkEndChild(interEl);
	}
	TiXmlElement* labFrameEl = new TiXmlElement("reference_frame");
	labFrameEl->LinkEndChild(saveFrameRecusion(ss->GetLabFrame()));
	root->LinkEndChild(labFrameEl);
	
	doc.SaveFile(filename);
}

