
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
//                                     UTILS                                      //
//================================================================================//

string dbleToStr(double d) {
	ostringstream s;
	s << d;
	return s.str();
}


//================================================================================//
//                                     LOADING                                    //
//================================================================================//


void SpinXML::XMLLoader::LoadFile(SpinSystem* ss,const char* filename) const {
	TiXmlDocument doc(filename);
	bool loadOkay = doc.LoadFile();
	if(!loadOkay) {
		throw runtime_error("Couldn't parse XML");
	}
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

