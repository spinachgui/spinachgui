
#include <3d/displaySettings.hpp>

#include <sigc++/sigc++.h>
#include <shared/assert.hpp>

#include <map>

using namespace std;
using namespace sigc;
using namespace SpinXML;

//================================================================================//

sigc::signal<void> sig3DChange;


sigc::signal<void,SpinXML::Interaction::Type,bool>      sigInterVisible;
sigc::signal<void,SpinXML::Interaction::Type,ColourRGB> sigInterColour;
sigc::signal<void,SpinXML::Interaction::Type,double>    sigInterSize;

map<SpinXML::Interaction::Type,bool>      visableMap;
map<SpinXML::Interaction::Type,ColourRGB> colourMap;
map<SpinXML::Interaction::Type,double>    sizeMap;


void SetInterVisible(bool b,SpinXML::Interaction::Type t) {
        visableMap[t] = b;
        sigInterVisible(t,b);
        sig3DChange();
}

void SetInterColour (ColourRGB c,SpinXML::Interaction::Type t) {
        colourMap[t] = c;
        sigInterColour(t,c);
        sig3DChange();
}

void SetInterSize   (double s,SpinXML::Interaction::Type t) {
        sizeMap[t] = s;
        sigInterSize(t,s);
        sig3DChange();
}

bool GetInterVisible(SpinXML::Interaction::Type t) {
	spinxml_assert(visableMap.find(t) != visableMap.end());
	return visableMap[t];
}
ColourRGB GetInterColour (SpinXML::Interaction::Type t) {
	spinxml_assert(colourMap.find(t) != colourMap.end());
	return colourMap[t];
}
double    GetInterSize   (SpinXML::Interaction::Type t) {
	spinxml_assert(sizeMap.find(t) != sizeMap.end());
	return sizeMap[t];
}

//================================================================================//
// Wireframe settings

GLUquadric* gActiveQuadric = NULL;
GLUquadric* gSolidQuadric  = NULL;
GLUquadric* gWireQuadric   = NULL;

GLUquadric* GetQuadric() {
        return gActiveQuadric;
}
void SetQuadric(QUADRIC_TYPE type) {
        if(type == QUAD_WIREFRAME) {
                gActiveQuadric = gWireQuadric;
        } else if(type == QUAD_SOLID) {
                gActiveQuadric = gSolidQuadric;
        } else {
                cerr << "Bad quadric type passes to SetQuadric()" << endl;
        }
}


//================================================================================//
// Should bonds be drawn?

bool gShowBonds = true;

void SetShowBonds(bool b) {
    gShowBonds = b;
    sigShowBonds(b);
    sig3DChange();
}
bool GetShowBonds() {
    return gShowBonds;
}
sigc::signal<void,bool> sigShowBonds;

//================================================================================//
// What colour should bonds be drawn

ColourRGB gBondColour(0,0,0.9f);

//================================================================================//
// Supress Interactions

set<Spin*> gSupressed;

const std::set<SpinXML::Spin*>& GetSuppressedSpins() {
    return gSupressed;
}
void SetSupressedSpins(const std::set<SpinXML::Spin*>& spins) {
    gSupressed = spins;
    sigSupressedChange(gSupressed);
    sig3DChange();
}
sigc::signal<void,const std::set<SpinXML::Spin*>&> sigSupressedChange;

//================================================================================//
// Mono draw mode

MONO_DRAW_MODE gMonoDrawMode = MONO_ELIPSOID;

void SetMonoDrawMode(MONO_DRAW_MODE mode) {
    gMonoDrawMode = mode;
    sigDrawMode(mode);
    sig3DChange();
}
MONO_DRAW_MODE GetMonoDrawMode() {
    return gMonoDrawMode;
}
sigc::signal<void,MONO_DRAW_MODE> sigDrawMode;


//================================================================================//
// Interaction Display Settings

__ModInit::__ModInit() {
    gSolidQuadric = gluNewQuadric();
    gWireQuadric  = gluNewQuadric();
    gActiveQuadric = gWireQuadric;

    gluQuadricDrawStyle(gSolidQuadric,GLU_FILL);
    gluQuadricNormals  (gSolidQuadric,GLU_SMOOTH);

    gluQuadricDrawStyle(gWireQuadric,GLU_LINE);
    gluQuadricNormals  (gWireQuadric,GLU_SMOOTH);

    //Set sensible default scallings
    sizeMap[Interaction::HFC             ] = 0.01;
    sizeMap[Interaction::G_TENSER        ] = 12;
    sizeMap[Interaction::ZFS             ] = 0.01;
    sizeMap[Interaction::EXCHANGE        ] = 0.01;
    sizeMap[Interaction::SHIELDING       ] = 0.01;
    sizeMap[Interaction::SCALAR          ] = 5e-6;
    sizeMap[Interaction::QUADRUPOLAR     ] = 0.01;
    sizeMap[Interaction::DIPOLAR         ] = 0.01;
    sizeMap[Interaction::CUSTOM_LINEAR   ] = 0.01;
    sizeMap[Interaction::CUSTOM_BILINEAR ] = 0.01;
    sizeMap[Interaction::CUSTOM_QUADRATIC] = 0.01;

    visableMap[Interaction::HFC             ] = true;
    visableMap[Interaction::G_TENSER        ] = true;
    visableMap[Interaction::ZFS             ] = true;
    visableMap[Interaction::EXCHANGE        ] = true;
    visableMap[Interaction::SHIELDING       ] = true;
    visableMap[Interaction::SCALAR          ] = true;
    visableMap[Interaction::QUADRUPOLAR     ] = true;
    visableMap[Interaction::DIPOLAR         ] = true;
    visableMap[Interaction::CUSTOM_LINEAR   ] = true;
    visableMap[Interaction::CUSTOM_BILINEAR ] = true;
    visableMap[Interaction::CUSTOM_QUADRATIC] = true;

    colourMap[Interaction::HFC             ] = ColourRGB(1.0f,  0.501960784314f,  0.0f);
    colourMap[Interaction::G_TENSER        ] = ColourRGB(0.247058823529f,  0.76862745098f,  0.352941176471f);
    colourMap[Interaction::ZFS             ] = ColourRGB(0.043137254902f,  0.807843137255f,  0.972549019608f);
    colourMap[Interaction::EXCHANGE        ] = ColourRGB(0.1f,0.1f,0.1f);
    colourMap[Interaction::SHIELDING       ] = ColourRGB(0.078431372549f,  0.078431372549f,  0.909803921569f);
    colourMap[Interaction::SCALAR          ] = ColourRGB(0.941176470588f,  0.0196078431373f,  0.0313725490196f);
    colourMap[Interaction::QUADRUPOLAR     ] = ColourRGB(1.0f,  0.501960784314f,  0.0f);
    colourMap[Interaction::DIPOLAR         ] = ColourRGB(0.81568627451f,  0.847058823529f,  0.0941176470588f);
    colourMap[Interaction::CUSTOM_LINEAR   ] = ColourRGB(0.81568627451f,  0.81568627451f,  0.81568627451f);
    colourMap[Interaction::CUSTOM_BILINEAR ] = ColourRGB(0.81568627451f,  0.81568627451f,  0.81568627451f);
    colourMap[Interaction::CUSTOM_QUADRATIC] = ColourRGB(0.81568627451f,  0.81568627451f,  0.81568627451f);
}

__ModInit::~__ModInit() {
    gluDeleteQuadric(gSolidQuadric);
    gluDeleteQuadric(gWireQuadric);
}

static __ModInit __init;
