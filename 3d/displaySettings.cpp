
#include <3d/displaySettings.hpp>

#include <sigc++/sigc++.h>

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

bool      GetInterVisible(SpinXML::Interaction::Type t) {
	return visableMap[t];
}
ColourRGB GetInterColour (SpinXML::Interaction::Type t) {
	return colourMap[t];
}
double    GetInterSize   (SpinXML::Interaction::Type t) {
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

ColourRGB gBondColour(0,0,0.9);

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
	sizeMap[Interaction::G_TENSER        ] = 1;
	sizeMap[Interaction::ZFS             ] = 1;
	sizeMap[Interaction::EXCHANGE        ] = 1;
	sizeMap[Interaction::SHIELDING       ] = 1;
	sizeMap[Interaction::SCALAR          ] = 1;
	sizeMap[Interaction::QUADRUPOLAR     ] = 1;
	sizeMap[Interaction::DIPOLAR         ] = 1;
	sizeMap[Interaction::CUSTOM_LINEAR   ] = 1;
	sizeMap[Interaction::CUSTOM_BILINEAR ] = 1;
	sizeMap[Interaction::CUSTOM_QUADRATIC] = 1;

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

	colourMap[Interaction::G_TENSER        ] = ColourRGB(0.0,0.0,0.0);
	colourMap[Interaction::ZFS             ] = ColourRGB(0.0,0.0,0.0);
	colourMap[Interaction::EXCHANGE        ] = ColourRGB(0.0,0.0,0.0);
	colourMap[Interaction::SHIELDING       ] = ColourRGB(0.0,0.0,0.0);
	colourMap[Interaction::SCALAR          ] = ColourRGB(0.0,0.0,0.0);
	colourMap[Interaction::QUADRUPOLAR     ] = ColourRGB(0.0,0.0,0.0);
	colourMap[Interaction::DIPOLAR         ] = ColourRGB(0.0,0.0,0.0);
	colourMap[Interaction::CUSTOM_LINEAR   ] = ColourRGB(0.0,0.0,0.0);
	colourMap[Interaction::CUSTOM_BILINEAR ] = ColourRGB(0.0,0.0,0.0);
	colourMap[Interaction::CUSTOM_QUADRATIC] = ColourRGB(0.0,0.0,0.0);
}

__ModInit::~__ModInit() {
	gluDeleteQuadric(gSolidQuadric);
	gluDeleteQuadric(gWireQuadric);
}

static __ModInit __init;
