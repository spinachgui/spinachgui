
#include <shared/formats/tinyxml/tinyxml.h>
#include <shared/spinsys.hpp>
#include <shared/interaction.hpp>
#include <shared/formats/xml.hpp>
#include <shared/foreach.hpp>
#include <shared/basic_math.hpp>
#include <shared/panic.hpp>
#include <map>
#include <set>
#include <boost/optional.hpp>
#include <shared/formats/proto.hpp>

using namespace SpinXML;
using namespace std;
using namespace Eigen;
using namespace boost;

#define LAB_FRAME 0
#define INAPPLICABLE_FRAME -1  //Use when writing out a
                               //reference_frame that should be
                               //ignored when reading

//================================================================================//
//                                     Units                                      //
//================================================================================//
// For now, everything in Anstroms, Hz and MHz. If the units field is set
// to anything else it's wrong

string _MHz_ = "MHz";
string _Hz_  = "Hz";
string _Angstrom_ = "Angstroms";
string _unitless_ = "unitless";

//================================================================================//
//                                     INIT                                       //
//================================================================================//

map<Interaction::Type,string> gType2XMLKind;
map<string,Interaction::Type> gXMLKind2Type;

__XMLInit::__XMLInit() {
    gType2XMLKind[Interaction::HFC]              = "hfc";
    gType2XMLKind[Interaction::G_TENSER]         = "g-tenser";
    gType2XMLKind[Interaction::ZFS]              = "zfs";
    gType2XMLKind[Interaction::EXCHANGE]         = "exchange";
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
    gXMLKind2Type["custem"]      = Interaction::CUSTOM_LINEAR;
}


__XMLInit::~__XMLInit() {

}

static __XMLInit __init;


//================================================================================//
//                                     UTILS                                      //
//================================================================================//

string dbleToStr(double d) {
    ostringstream s;
    s << d;
    return s.str();
}

double strToDble(string str) {
    istringstream s(str);
    double d;
    if(!(s >> d)) {
        throw runtime_error(string("Couldn't parse ") + str + string(" as a floating point number"));
    }
    return d;
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

string _origin_ = "origin";

//================================================================================//
//                                     LOADING                                    //
//================================================================================//


optional<string> OptionalString(const TiXmlElement* el,const string& name) {
    if(el->Attribute(name) == NULL) {
        return optional<string>();
    }
    string value;
    if(el->QueryStringAttribute(name.c_str(),&value) != TIXML_SUCCESS) {
        throw runtime_error("Error reading an attribute (wrong type?)");
    }
    return value;
}

optional<int> OptionalInt(const TiXmlElement* el,const string& name) {
    optional<int> ovalue;
    if(el->Attribute(name) == NULL) {
        return ovalue;
    }
    int value;
    if(el->QueryIntAttribute(name,&value) != TIXML_SUCCESS) {
        throw runtime_error("Error reading an attribute (wrong type?)");
    }
    ovalue = value;
    return ovalue;
}
optional<double> OptionalDouble(const TiXmlElement* el,const string& name) {
    if(el->Attribute(name) == NULL) {
        return optional<double>();
    }
    double value;
    if(el->QueryDoubleAttribute(name,&value) != TIXML_SUCCESS) {
        throw runtime_error("Error reading an attribute (wrong type?)");
    }
    return value;
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
    const TiXmlNode* payloadNode = NULL;
    if((payloadNode = e->FirstChild(_euler_))) {
        const TiXmlElement* eulerEl = Guard(payloadNode,"Malformed euler angles element");
        EulerAngles ea(0,0,0);
        Guard(eulerEl->QueryDoubleAttribute("alpha",&ea.alpha),"No alpha attribute for euler angles");
        Guard(eulerEl->QueryDoubleAttribute("beta", &ea.beta ),"No beta  attribute for euler angles");
        Guard(eulerEl->QueryDoubleAttribute("gamma",&ea.gamma),"No gamma attribute for euler angles");
        Guard(eulerEl->QueryIntAttribute(_reference_frame_,&frame),"No reference frame in euler angles");
        o = ea;
    } else if((payloadNode = e->FirstChild(_dcm_))) {
        const TiXmlElement* matrixEl = Guard(payloadNode,"Malformed DCM element");
        Guard(matrixEl->QueryIntAttribute(_reference_frame_,&frame),"No reference frame in matrix");
        Matrix3d mat = decodeMatrix(matrixEl);
        o = mat;
    } else if((payloadNode = e->FirstChild(_quaternion_))) {
        const TiXmlElement* quaternionEl = Guard(payloadNode,"Malformed quaternion element");
        double x,y,z,w;
        Guard(quaternionEl->QueryDoubleAttribute("i" ,&x),"No x attribute for quaternion");
        Guard(quaternionEl->QueryDoubleAttribute("j" ,&y),"No y attribute for quaternion");
        Guard(quaternionEl->QueryDoubleAttribute("k" ,&z),"No z attribute for quaternion");
        Guard(quaternionEl->QueryDoubleAttribute("re",&w),"No re attribute for quaternion");
        
        if(x == 0 && y == 0 && z == 0 && w == 0) {
            throw runtime_error("All for elements of a quaternions were 0, cannot be normalised");
        }
        
        Guard(quaternionEl->QueryIntAttribute(_reference_frame_,&frame),"No reference frame in quaternion");

        o = Quaterniond(x,y,z,w);
    } else if((payloadNode = e->FirstChild(_angle_axis_))) {
        const TiXmlElement* angleAxisEl = Guard(payloadNode,"Malformed angle axis element");
        
        const TiXmlElement* angleEl = Guard(angleAxisEl->FirstChild(_angle_),"Malformed or missing angle element for angle axis");
        double angle = strToDble(angleEl->GetText());
        
        const TiXmlElement* axisEl  = Guard(angleAxisEl->FirstChild(_axis_) ,"Malformed or missing axis element for angle axis");
        double x,y,z;
        Guard(axisEl->QueryDoubleAttribute("x" ,&x),"No x attribute for angle-axis");
        Guard(axisEl->QueryDoubleAttribute("y" ,&y),"No y attribute for angle-axis");
        Guard(axisEl->QueryDoubleAttribute("z" ,&z),"No z attribute for angle-axis");
        Guard(axisEl->QueryIntAttribute(_reference_frame_,&frame),"No reference frame in axis");

        if(x == 0 && y == 0 && z == 0) {
            throw runtime_error("Vector for an angle-axis rotation is the zero vector, cannot normalise");
        }

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

    Guard(e->QueryIntAttribute("spin_1",&protoInteraction.spin1),"Missing spin_1 attribute, only spin_2 is optional");
    protoInteraction.spin2 = OptionalInt(e,"spin_2");

    string unitStr;
    Guard(e->QueryStringAttribute("units",&unitStr),"missing units in interaction");
	if(protoInteraction.type==Interaction::G_TENSER || protoInteraction.type == Interaction::SHIELDING) {
		if(unitStr != _unitless_) {
            throw runtime_error("Invalid unit in interaction");
		}
	} else if(protoInteraction.type==Interaction::SCALAR) {
		if(unitStr != _Hz_) {
            throw runtime_error("Invalid unit in interaction");
		}
	} else {
		if(unitStr != _MHz_) {
            throw runtime_error("Invalid unit in interaction");
		}
    }
	unit u = protoInteraction.type==Interaction::G_TENSER || protoInteraction.type==Interaction::SHIELDING ? Unitless : MHz;
	if(protoInteraction.type==Interaction::SCALAR) {
        u = Hz;
	}
    //Be leinient about mixing up the order of eigenvalues etc. and
    //orientation
    const TiXmlNode* mag = NULL; //One of scalar|tensor|eigenvalues|axiality_rhombicity|span_skew
    if((mag = e->FirstChild(_scalar_))) {
        const TiXmlElement* scalarEl = Guard(mag,"Malformed scalar element");

        double scalar = strToDble(scalarEl->GetText());
        protoInteraction.payload = (scalar * u);
        
        protoInteraction.frame = 0;
    } else if((mag = e->FirstChild(_tensor_))) {
        const TiXmlElement* matrixEl = Guard(mag,"Malformed tensor element");

        Matrix3d matrix = decodeMatrix(matrixEl);
        protoInteraction.payload = (matrix * u);
        Guard(matrixEl->QueryIntAttribute(_reference_frame_,&protoInteraction.frame),"No reference frame for matrix");
    } else if((mag = e->FirstChild(_eigenvalues_))) {
        const TiXmlElement* evEl = Guard(mag,"Malformed eigenvalue element");

        Eigenvalues ev(0,0,0,EulerAngles(0,0,0));
        Guard(evEl->QueryDoubleAttribute("XX",&ev.xx),"No xx attribute for eigenvalues");
        Guard(evEl->QueryDoubleAttribute("YY",&ev.yy),"No yy attribute for eigenvalues");
        Guard(evEl->QueryDoubleAttribute("ZZ",&ev.zz),"No zz attribute for eigenvalues");

        ev.xx = ev.xx * u;
        ev.yy = ev.yy * u;
        ev.zz = ev.zz * u;

        const TiXmlElement* orientEl = Guard(e->FirstChild(_orientation_),"eigenvalue specification requires an orientation");
        int frame;
        decodeOrientation(orientEl,ev.mOrient,frame);
        protoInteraction.payload = ev;
        protoInteraction.frame = frame;
        
    } else if((mag = e->FirstChild(_axrhom_))) {
        const TiXmlElement* axRhEl = Guard(mag,"Malformed axiality-rhombicity element");
        
        AxRhom ar(0,0,0,EulerAngles(0,0,0));
        Guard(axRhEl->QueryDoubleAttribute("iso",&ar.iso),"No iso attribute for axiality_rhombicity");
        Guard(axRhEl->QueryDoubleAttribute("ax", &ar.ax) ,"No ax attribute for axiality_rhombicity");
        Guard(axRhEl->QueryDoubleAttribute("rh", &ar.rh) ,"No rh attribute for axiality_rhombicity");

        ar.iso = ar.iso * u;
        ar.ax  = ar.ax *  u;
        ar.rh  = ar.rh *  u;

        const TiXmlElement* orientEl = Guard(e->FirstChild(_orientation_),"axiality-rhombicity specification requires an orientation");
        int frame;
        decodeOrientation(orientEl,ar.mOrient,frame);
        protoInteraction.payload = ar;
        protoInteraction.frame = frame;

    } else if((mag = e->FirstChild(_spanskew_))) {
        const TiXmlElement* spanSkewEl = Guard(mag,"Malformed span-skew element");

        SpanSkew spanSkew(0,0,0,EulerAngles(0,0,0));
        Guard(spanSkewEl->QueryDoubleAttribute("iso",&spanSkew.iso),  "No iso  attribute for span_skew");
        Guard(spanSkewEl->QueryDoubleAttribute("ax", &spanSkew.span) ,"No span attribute for span_skew");
        Guard(spanSkewEl->QueryDoubleAttribute("rh", &spanSkew.skew) ,"No skew attribute for span_skew");

        spanSkew.iso  = spanSkew.iso  * u;
        spanSkew.span = spanSkew.skew * u;

        const TiXmlElement* orientEl = Guard(e->FirstChild(_orientation_),"span-skew specification requires an orientation");
        int frame;
        decodeOrientation(orientEl,spanSkew.mOrient,frame);
        protoInteraction.payload = spanSkew;
        protoInteraction.frame = frame;

    } else {
        throw runtime_error("interaction missing it's definition in terms of one of scalar,tensor,eigenvalues,axiality_rhombicity,span_skew");
    }
}

void decodeFrame(const TiXmlElement* e,vector<ProtoFrame>& protoFrames,int parent = 0) {
    ProtoFrame frame;
    frame.parent = parent;
    Guard(e->QueryIntAttribute("number",  &frame.number),  "No number attribute for frame");
    optional<string> label = OptionalString(e,"label");
    if(!label) {
        label = "";
    }
    frame.label = label.get();

    const TiXmlElement* originEl = Guard(e->FirstChild(_origin_),"Reference frame missing an origin");
    double x,y,z;
    Guard(originEl->QueryDoubleAttribute("x",  &x),  "Missing x attribute of origin");
    Guard(originEl->QueryDoubleAttribute("y",  &y),  "Missing y attribute of origin");
    Guard(originEl->QueryDoubleAttribute("z",  &z),  "Missing z attribute of origin");

    frame.x = x * Angstroms;
    frame.y = y * Angstroms;
    frame.z = z * Angstroms;

    const TiXmlElement* orientationEl = Guard(e->FirstChild(_orientation_),"Reference frame missing an orientation");
    int dummy;
    decodeOrientation(orientationEl,frame.o,dummy);

    protoFrames.push_back(frame);

    const TiXmlNode* child = NULL;
    while((child = e->IterateChildren(child))) {
        const TiXmlElement* childEl = child->ToElement();
        if(!childEl) {
            continue;
        }
        if(childEl->Value() == _reference_frame_) {
            decodeFrame(childEl,protoFrames,frame.number);
        }
    }
    return;
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
        if(e->Value() == _spin_) {
            ProtoSpin spin;
            Guard(e->QueryIntAttribute("number",&spin.number),  "missing spin number  attribute");
            Guard(e->QueryIntAttribute("element",&spin.element),"missing spin element attribute");
            Guard(e->QueryIntAttribute("isotope",&spin.isotope),"missing spin isotope attribute");
            optional<string> label = OptionalString(e,"label");
            if(!label) {
                label = "";
            }
            spin.label = label.get();

            const TiXmlElement* coords = Guard(e->FirstChild("coordinates"),"missing or malformed coordinate");

            double x,y,z;
            Guard(coords->QueryDoubleAttribute("x",&x),"missing x attribute");
            Guard(coords->QueryDoubleAttribute("y",&y),"missing y attribute");
            Guard(coords->QueryDoubleAttribute("z",&z),"missing z attribute");
            Guard(coords->QueryIntAttribute("reference_frame",&spin.frame),"missing reference frame");

            spin.x = x * Angstroms;
            spin.y = y * Angstroms;
            spin.z = z * Angstroms;

            protoSpins.push_back(spin);
        } else if(e->Value() == _interaction_) {
            ProtoInteraction inter;
            decodeInteraction(e,inter);
            protoInteractions.push_back(inter);
        } else if(e->Value() == _reference_frame_) {
            decodeFrame(e,protoFrames);
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
        el->SetDoubleAttribute("xx",mat(0,0));
        el->SetDoubleAttribute("xy",mat(0,1));
        el->SetDoubleAttribute("xz",mat(0,2));

        el->SetDoubleAttribute("yx",mat(1,0));
        el->SetDoubleAttribute("yy",mat(1,1));
        el->SetDoubleAttribute("yz",mat(1,2));

        el->SetDoubleAttribute("zx",mat(2,0));
        el->SetDoubleAttribute("zy",mat(2,1));
        el->SetDoubleAttribute("zz",mat(2,2));
    }

    void encodeOrient(const Orientation& orient,TiXmlElement* el,int frameNumber) const {
        switch(orient.GetType()) {
        case Orientation::EULER: {
            TiXmlElement* eaEl = new TiXmlElement(_euler_);
            EulerAngles ea = orient.GetAsEuler();
            eaEl->SetDoubleAttribute("alpha",ea.alpha);
            eaEl->SetDoubleAttribute("beta" ,ea.beta);
            eaEl->SetDoubleAttribute("gamma",ea.gamma);
            eaEl->SetAttribute(_reference_frame_,frameNumber);
            el->LinkEndChild(eaEl);
            break;
        }
        case Orientation::DCM: {
            TiXmlElement* matrixEl = new TiXmlElement(_dcm_);
            encodeMatrix(orient.GetAsMatrix(),matrixEl);
            matrixEl->SetAttribute(_reference_frame_,frameNumber);
            el->LinkEndChild(matrixEl);
            break;
        }
        case Orientation::QUATERNION: {
            TiXmlElement* qEl = new TiXmlElement(_quaternion_);
            Quaterniond q = orient.GetAsQuaternion();

            if(q.x() == 0 && q.y() == 0 && q.z() == 0 && q.w() == 0) {
                PANIC("About to write out the zero quaternion (0,0,0,0)");
            }

            qEl->SetDoubleAttribute("re",q.w());
            qEl->SetDoubleAttribute("i",q.x());
            qEl->SetDoubleAttribute("j",q.y());
            qEl->SetDoubleAttribute("k",q.z());
            qEl->SetAttribute(_reference_frame_,frameNumber);

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
            Vector3d axis = aa.axis();

            if(axis.x() == 0 && axis.y() == 0 && axis.z() == 0) {
                PANIC("About to write out an angle-axis axis that is the zero vector");
            }

            axisEl->SetDoubleAttribute("x",axis.x());
            axisEl->SetDoubleAttribute("y",axis.y());
            axisEl->SetDoubleAttribute("z",axis.z());
            axisEl->SetAttribute(_reference_frame_,frameNumber);
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
            originEl->SetDoubleAttribute("x",childFrame->GetTranslation().x() / Angstroms);
            originEl->SetDoubleAttribute("y",childFrame->GetTranslation().y() / Angstroms);
            originEl->SetDoubleAttribute("z",childFrame->GetTranslation().z() / Angstroms);
            originEl->SetAttribute(_reference_frame_,LAB_FRAME);
            childEl->LinkEndChild(originEl);
        
            TiXmlElement* orientEl = new TiXmlElement("orientation");
            encodeOrient(childFrame->GetOrientation(),orientEl,INAPPLICABLE_FRAME);
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
        unit u = inter->GetType() == Interaction::G_TENSER ||
			     inter->GetType() == Interaction::SHIELDING ? Unitless : MHz;
		if(inter->GetType() == Interaction::SCALAR) {
            u = Hz;
		}
        Frame* frame = inter->GetPreferedFrame();
        int frameNumber = lookupFrame(frame);

        switch(inter->GetStorage()) {
        case Interaction::STORAGE_SCALAR: {
            TiXmlElement* scalarEl = new TiXmlElement(_scalar_);
			scalarEl->SetValue("scalar");
            scalarEl->LinkEndChild(new TiXmlText(dbleToStr(inter->AsScalar() / u)));

            interEl->LinkEndChild(scalarEl);
            break;
        }
        case Interaction::MATRIX: {
            TiXmlElement* matrixEl = new TiXmlElement(_tensor_);
            Matrix3d mat3 = inter->AsMatrix();
            if(frame != NULL) {
                mat3 = FromLabMatrix3d(frame,mat3);
            }
            encodeMatrix(mat3 * u,matrixEl);
            matrixEl->SetAttribute("reference_frame",frameNumber);
            interEl->LinkEndChild(matrixEl);
            break;
        }
        case Interaction::EIGENVALUES: {
            TiXmlElement* evEl = new TiXmlElement(_eigenvalues_);
            Eigenvalues ev = inter->AsEigenvalues();
            evEl->SetDoubleAttribute("XX",ev.xx / u);
            evEl->SetDoubleAttribute("YY",ev.yy / u);
            evEl->SetDoubleAttribute("ZZ",ev.zz / u);
            interEl->LinkEndChild(evEl);

            o = ev.mOrient;
            break;
        }
        case Interaction::AXRHOM: {
            TiXmlElement* arEl = new TiXmlElement(_axrhom_);
            AxRhom ar = inter->AsAxRhom();
            arEl->SetDoubleAttribute("rh",ar.rh   / u);
            arEl->SetDoubleAttribute("iso",ar.iso / u);
            arEl->SetDoubleAttribute("ax",ar.ax   / u);
            interEl->LinkEndChild(arEl);

            o = ar.mOrient;
            break;
        }
        case Interaction::SPANSKEW: {
            TiXmlElement* ssEl = new TiXmlElement(_spanskew_);
            SpanSkew ss = inter->AsSpanSkew();
            ssEl->SetDoubleAttribute("span",ss.span  / u);
            ssEl->SetDoubleAttribute("skew",ss.skew);
            ssEl->SetDoubleAttribute("iso", ss.iso   / u);
            interEl->LinkEndChild(ssEl);

            o = ss.mOrient;
            break;
        }
        }
        if(inter->GetStorage() == Interaction::EIGENVALUES ||
           inter->GetStorage() == Interaction::AXRHOM ||
           inter->GetStorage() == Interaction::SPANSKEW) {
            TiXmlElement* orientEl = new TiXmlElement(_orientation_);
            if(frame != NULL) {
                o = FromLabOrient(frame,o);
            }
            encodeOrient(o,orientEl,frameNumber);
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
            spinEl->SetAttribute("isotope",spin->GetIsotope()+spin->GetElement());
            spinEl->SetAttribute("label",spin->GetLabel());

            TiXmlElement* coordEl = new TiXmlElement("coordinates");
            Vector3d position = spin->GetPosition();
            if(spin->GetPreferedFrame() != NULL) {
                position = FromLabVec3d(spin->GetPreferedFrame(),position);
            }
            coordEl->SetDoubleAttribute("x",position.x() / Angstroms);
            coordEl->SetDoubleAttribute("y",position.y() / Angstroms);
            coordEl->SetDoubleAttribute("z",position.z() / Angstroms);
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

            if(spin1n == -1) {
                spin1n = spin2n; spin2n = -1;
            }

            interEl->SetAttribute("kind" ,gType2XMLKind[inter->GetType()]);
            if(inter->GetType() == Interaction::G_TENSER || inter->GetType() == Interaction::SHIELDING) {
                interEl->SetAttribute("units",_unitless_);
            } else if(inter->GetType() == Interaction::SCALAR) {
                interEl->SetAttribute("units",_Hz_);
			} else {
                interEl->SetAttribute("units",_MHz_);
			}
            interEl->SetAttribute("spin_1",spin1n);
            if(spin2n != -1) {
                interEl->SetAttribute("spin_2",spin2n);
            }

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


