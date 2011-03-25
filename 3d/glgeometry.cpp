

#include <shared/nuclear_data.hpp>
#include <cmath>
#include <iostream>

#include <3d/glgeometry.hpp>
#include <3d/glmode.hpp>

#include <gui/SpinachApp.hpp>

using namespace std;
using namespace SpinXML;
using namespace Eigen;

const double pi=3.141592654;





///An abstract class who's job is to visualise part of a spin system
///such as spins, linear interactions or bonds.


Renderer::Renderer() {
}

Renderer::~Renderer() {
}

void Renderer::DrawWith(GLMode& mode,const DisplaySettings& settings, PASS pass) const {
    mode.On();
    Geometary(settings,pass);
    mode.Off();
}

void Renderer::Draw(const DisplaySettings& settings, PASS pass) const {
    Geometary(settings,pass);
}


//============================================================//

///Keeps a collection of renderers and manages gl state common to a
///scene, such as camera position, global rotation and lighting.

Scene::Scene(const TRenderVec& renderers) 
	: mRenderers(renderers) {
}

Scene::~Scene() {
	for(TRenderIt i = mRenderers.begin();i != mRenderers.end();++i) {
	    delete (*i);
	}
}

void Scene::Geometary(const DisplaySettings& displaySettings,PASS pass) const {
	//loop and render
	for(TRenderIt i = mRenderers.begin();i != mRenderers.end();++i) {
	    (*i)->Draw(displaySettings,pass);
	}
}

//============================================================//


MoleculeFG::MoleculeFG()  {
    long count=GetSS()->GetSpinCount();
    for(long i=0;i<count;i++) {
        Spin* spin=GetSS()->GetSpin(i);
        if(spin->GetElement() == 0) {
            //Now we are only drawing electrons
            cout << "No longer adding an electron to the forground" << endl;
        }
    }
}


//============================================================
// The New 3D Code
//============================================================

void DrawCylinder(Vector3d R1,Vector3d R2,length width,unit u,const DisplaySettings& settings) {
    //If the spin is an electron, it should be drawn outside of the
    //molecule
    double x1=R1.x() * u,y1=R1.y() * u,z1=R1.z() * u;
    double x2=R2.x() * u,y2=R2.y() * u,z2=R2.z() * u;

    double len=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2));
			
    //Now we need to find the rotation between the z axis
    double angle=acos((z2-z1)/len);
    glPushMatrix(); {
		glTranslatef(x1,y1,z1);
		glRotatef(angle/2/pi*360,y1-y2,x2-x1,0);
		gluCylinder(settings.mSolidQuadric,width * u,width * u,len,7,7);
    } glPopMatrix();
}

//============================================================//

SpinDrawer::SpinDrawer() {
}

void SpinDrawer::Geometary(const DisplaySettings& settings, PASS pass) const {
    cout << "SpinDrawer::Geometary" << endl;
    long count=GetSS()->GetSpinCount();
    for(long i=0;i<count;i++) {
	Spin* spin=GetSS()->GetSpin(i);
	if(spin->GetElement() != 0){
	    glPushMatrix();
	    glTranslatef(spin->GetPosition().x() / Angstroms,
			 spin->GetPosition().y() / Angstroms,
			 spin->GetPosition().z() / Angstroms);
	}
	const static GLfloat white[4]={0.8f,0.8f,0.8f,0.0f};
	GLfloat material[4]; material[3]=0.0f;

	material[0] = getElementR(spin->GetElement());
	material[1] = getElementG(spin->GetElement());
	material[2] = getElementB(spin->GetElement());

	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material);
	if(spin->GetElement()==0) {
	    //Bug resulting in drawing no content after a g03 load tracked to this line:
	    glTranslatef(40.0f,settings.height-40.0f,0.0f);

	    gluSphere(settings.mSolidQuadric,9,14,14);

	} else {
            glPushName(i);
	    gluSphere(settings.mSolidQuadric,0.1,14,14);
            glPopName();
	}
	if(spin->GetElement() != 0){
	    glPopMatrix();
	}

    }
}

//============================================================//


BondDrawer::BondDrawer() {
    //const static GLfloat blueMaterial[]  = {0.06,0.06,0.4,0.0f}; 
    //SetMaterial(blueMaterial);	
}

void BondDrawer::Geometary(const DisplaySettings& settings, PASS passm) const {
    long count=GetSS()->GetSpinCount();
    for(long i=0;i<count;i++) {
	Spin* spin=GetSS()->GetSpin(i);
	if(spin->GetElement() == 0) {
	    continue;
	}
	//If the spin is an electron, it should be drawn outside of the
	//molecule
	vector<Spin*> nearby=GetSS()->GetNearbySpins(spin->GetPosition(),1.8*Angstroms,spin);
	for(unsigned long j=0;j<nearby.size();j++) {
	    if(nearby[j]->GetElement()==0) {
		continue;
	    }
	    Vector3d mR1=spin->GetPosition();
	    Vector3d mR2=nearby[j]->GetPosition();

	    double x1=mR1.x() / Angstroms,y1=mR1.y() / Angstroms,z1=mR1.z() / Angstroms;
	    double x2=mR2.x() / Angstroms,y2=mR2.y() / Angstroms,z2=mR2.z() / Angstroms;

	    double len=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2));
			
	    //Now we need to find the rotation between the z axis
	    double angle=acos((z2-z1)/len);
	    glPushMatrix(); {
		glTranslatef(x1,y1,z1);
		glRotatef(angle/2/pi*360,y1-y2,x2-x1,0);
		gluCylinder(settings.mSolidQuadric,0.04,0.04,len,7,7);
	    } glPopMatrix();
	}
    }
}


LinearInterDrawer::LinearInterDrawer() {
}

void LinearInterDrawer::Geometary(const DisplaySettings& settings, PASS pass) const {

}

//============================================================//

FrameDrawer::FrameDrawer() {

}

void DrawFrameRecursive(Frame* frame) {
	glPushMatrix();
	glMultMatrixd(frame->getTransformFromLab().data());
    //Draw some coordiante axese
    glBegin(GL_LINES); {
        glVertex3f(0,0,0);
        glVertex3f(5,0,0);
    }glEnd();

    glBegin(GL_LINES); {
        glVertex3f(0,0,0);
        glVertex3f(0,5,0);
    } glEnd();

    glBegin(GL_LINES); {
        glVertex3f(0,0,0);
        glVertex3f(0,0,5);
    } glEnd();

	const vector<Frame*>& children = frame->GetChildren();
	for(vector<Frame*>::const_iterator i = children.begin();i != children.end();++i) {
		DrawFrameRecursive(*i);
	}

	glPopMatrix();
}

void FrameDrawer::Geometary(const DisplaySettings&, PASS) const {
    static const GLfloat white[] = {0.5, 0.5, 0.5};
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	Frame* frame = GetRawSS()->GetLabFrame();
	DrawFrameRecursive(frame);
}
