

#include <shared/nuclear_data.hpp>
#include <cmath>
#include <iostream>

#include <3d/glgeometry.hpp>
#include <3d/glmode.hpp>

#include <gui/SpinachApp.hpp>

#include <shared/foreach.hpp>

#include <Eigen/Eigenvalues> 

#include <3d/displaySettings.hpp>

#include <shared/spinsys.hpp>


using namespace std;
using namespace SpinXML;
using namespace Eigen;

const double pi=3.141592654;





///An abstract class who's job is to visualise part of a spin system
///such as spins, linear interactions or bonds.


Renderer::Renderer(GLuint glName) 
    : mName(glName) {
}

Renderer::~Renderer() {
}

void Renderer::DrawWith(GLMode& mode) const {
    mode.On();
    Geometary();
    mode.Off();
}

void Renderer::Draw() const {
    if(mName != NAME_NONE)
        glPushName(mName);
    Geometary();
    if(mName != NAME_NONE)
        glPopName();
}



//============================================================//


MoleculeFG::MoleculeFG() 
    : Renderer(NAME_FG) {
    long count=GetRawSS()->GetSpinCount();
    for(long i=0;i<count;i++) {
        Spin* spin=GetRawSS()->GetSpin(i);
        if(spin->GetElement() == 0) {
            //Now we are only drawing electrons
            cout << "No longer adding an electron to the forground" << endl;
        }
    }
}


void DrawCylinder(Vector3d R1,Vector3d R2,length width) {
    //If the spin is an electron, it should be drawn outside of the
    //molecule
    double x1=R1.x(),y1=R1.y(),z1=R1.z();
    double x2=R2.x(),y2=R2.y(),z2=R2.z();

    double len=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2));
                        
    //Now we need to find the rotation between the z axis
    double angle=acos((z2-z1)/len);
    glPushMatrix(); {
        glTranslatef(x1,y1,z1);
        glRotatef(angle/2/pi*360,y1-y2,x2-x1,0);
        gluCylinder(GetQuadric(),width,width,len,7,7);
    } glPopMatrix();
}

//============================================================//
// Generic function used in bother the electron and nuclear spin
// drawers to draw a spin

void DrawSpin(Spin* spin,long name) {
    const set<Spin*>& selection = GetSelection();
    Spin* hover = GetHover();

    const static GLfloat white[4]={0.8f,0.8f,0.8f,0.0f};
    GLfloat material[4]; material[3]=0.0f;

    if(spin == hover) {
        material[0] = 200/255.0f;
        material[1] = 1.0;
        material[2] = 200/255.0f;
    } else if(selection.find(spin) != selection.end()) {
        material[0] = 255/255.0f;
        material[1] = 100/255.0f;
        material[2] = 100/255.0f;
    } else {
        material[0] = getElementR(spin->GetElement());
        material[1] = getElementG(spin->GetElement());
        material[2] = getElementB(spin->GetElement());
    }

    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material);

    glPushName(name);
    gluSphere(GetQuadric(),1.0,14,14);
    glPopName();
}

//============================================================//

ElectronScene::ElectronScene(Camera* camera)
    : Renderer(NAME_SPINS), mCamera(camera) {
}

void ElectronScene::Geometary() const {
    long count=GetRawSS()->GetSpinCount();
    for(long i=0;i<count;i++) {
        Spin* spin=GetRawSS()->GetSpin(i);
        if(spin->GetElement() != 0){
            continue;
        }
        glPushMatrix();

        Affine3f t;
        t = mCamera->GetTransform();
        Matrix3f m3x3 = t.rotation();
        Matrix4f m4x4 = Matrix4f::Identity();
        m4x4.block(0,0,3,3) = m3x3;
                
        glMultMatrixf(m4x4.data());
        glScalef(10,10,10);

		glPushName(i);
        DrawSpin(spin,i);
		glPopName();

        glPopMatrix();
    }
}



//============================================================//

SpinDrawer::SpinDrawer() 
 : Renderer(NAME_SPINS) {
}

void SpinDrawer::Geometary() const {
    long count=GetRawSS()->GetSpinCount();
    for(long i=0;i<count;i++) {
        Spin* spin=GetRawSS()->GetSpin(i);
        if(spin->GetElement() == 0){
            continue;
        }
        glPushMatrix();
        glTranslatef(spin->GetPosition().x() / Angstroms,
                     spin->GetPosition().y() / Angstroms,
                     spin->GetPosition().z() / Angstroms);

        glScalef(0.4f,0.4f,0.4f);
        DrawSpin(spin,i);
        glPopMatrix();

    }
}

//============================================================//


BondDrawer::BondDrawer() 
    : Renderer(NAME_BONDS) {
}

void BondDrawer::Geometary() const {
    gBondColour.SetMaterial(1.0);
    if(!GetShowBonds()) {
        return;
    }
    long count=GetRawSS()->GetSpinCount();
    for(long i=0;i<count;i++) {
        Spin* spin=GetRawSS()->GetSpin(i);
        if(spin->GetElement() == 0) {
            continue;
        }
        //If the spin is an electron, it should be drawn outside of the
        //molecule
        vector<Spin*> nearby=GetRawSS()->GetNearbySpins(spin->GetPosition(),1.6*Angstroms,spin);
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
                gluCylinder(GetQuadric(),0.04,0.04,len,7,7);
            } glPopMatrix();
        }
    }
}

//============================================================//

void DrawMonoInter(Spin* spin,Interaction* inter) {
    Interaction::Type t = inter->GetType();
    GetInterColour(t).SetMaterial(0.5);

    Eigenvalues ev = inter->AsEigenvalues();

                        
    double xx = ev.xx;
    double yy = ev.yy;
    double zz = ev.zz;

    Matrix3d mat3 = ev.mOrient.GetAsDCM();

    GLfloat mat[16];
    mat[3 ]=0;
    mat[7 ]=0;
    mat[11]=0;
    mat[12]=0;
    mat[13]=0;
    mat[14]=0;
    mat[15]=1;

    mat[0 ]=mat3(0,0);
    mat[1 ]=mat3(0,1);
    mat[2 ]=mat3(0,2);
                                                    
    mat[4 ]=mat3(1,0);
    mat[5 ]=mat3(1,1);
    mat[6 ]=mat3(1,2);
                                                    
    mat[8 ]=mat3(2,0);
    mat[9 ]=mat3(2,1);
    mat[10]=mat3(2,2);

    glMultMatrixf(mat);

    //We need to treat the g_tensor specially
	unit u = inter->GetType() == Interaction::G_TENSER || inter->GetType() == Interaction::SHIELDING ? Unitless : MHz;
    glScaled(xx / u,yy /u, zz / u);
                        
    double size = GetInterSize(t);
    glScaled(size,size,size);

    glPushName(GetRawSS()->GetSpinNumber(spin));

    if(GetMonoDrawMode() == MONO_ELIPSOID) {
        gluSphere(GetQuadric(),1,11,13);
    } else {
        glBegin(GL_LINES); {
            glVertex3f(0,0,0);
            glVertex3f(1,0,0);
        }glEnd();

        glBegin(GL_LINES); {
            glVertex3f(0,0,0);
            glVertex3f(0,1,0);
        } glEnd();

        glBegin(GL_LINES); {
            glVertex3f(0,0,0);
            glVertex3f(0,0,1);
        } glEnd();
    }
    glPopName();
        
}

//============================================================//

ElectronInterDrawer::ElectronInterDrawer(Camera* camera)
        : Renderer(NAME_MONO_INTERACTIONS),mCamera(camera) {
}

void ElectronInterDrawer::Geometary() const {
    vector<Interaction*> inters = GetRawSS()->GetAllInteractions();
        
    const set<Spin*> supressed = GetSuppressedSpins();
    foreach(Interaction* inter,inters) {
        Interaction::Type t = inter->GetType();
        if(!GetInterVisible(t)) {
            continue;
        }

        Spin* spin = inter->GetSpin1();
        if(!spin->GetElement() == 0) {
            continue;
        }

        if(t != Interaction::G_TENSER) {
            continue;
        }

        if(supressed.find(spin) != supressed.end()) {
            return;
        }

        if(inter->GetIsBilinear()) {
            continue;
        }
        glPushMatrix();

        Affine3f transfomration;
        transfomration = mCamera->GetTransform();
        Matrix3f m3x3 = transfomration.rotation();
        Matrix4f m4x4 = Matrix4f::Identity();
        m4x4.block(0,0,3,3) = m3x3;
                
        glMultMatrixf(m4x4.data());
        DrawMonoInter(spin,inter);
        glPopMatrix();


    }
        
}

//============================================================//

MonoInterDrawer::MonoInterDrawer() 
    :  Renderer(NAME_MONO_INTERACTIONS) {
}

void MonoInterDrawer::Geometary() const {
    vector<Interaction*> inters = GetRawSS()->GetAllInteractions();
        
    const set<Spin*> supressed = GetSuppressedSpins();
    foreach(Interaction* inter,inters) {
        Interaction::Type t = inter->GetType();
        if(!GetInterVisible(t)) {
            continue;
        }

        if(t == Interaction::G_TENSER) {
            continue;
        }

        Spin* spin = inter->GetSpin1();

        if(supressed.find(spin) != supressed.end()) {
            return;
        }

        if(inter->GetIsBilinear()) {
            if(inter->GetType() == Interaction::HFC) {
                if(spin->GetElement() == 0) {
                    spin = inter->GetSpin2();
                }
            } else {
                continue;
            }
        }
        GetInterColour(t).SetMaterial(0.5);

        glPushMatrix(); {
            glTranslatef(spin->GetPosition().x() / Angstroms,
                         spin->GetPosition().y() / Angstroms,
                         spin->GetPosition().z() / Angstroms);
            DrawMonoInter(spin,inter);

        } glPopMatrix();
    }
}

//============================================================//

BilinearInterDrawer::BilinearInterDrawer() 
        : Renderer(NAME_BINARY_INTERACTIONS) {
}

void BilinearInterDrawer::Geometary() const {
    vector<Interaction*> inters = GetRawSS()->GetAllInteractions();
        
    foreach(Interaction* inter,inters) {
        Interaction::Type t = inter->GetType();
        if(!GetInterVisible(t)) {
            continue;
        }

        if(!inter->GetIsBilinear()) {
            continue;
        }
        if(inter->GetType() == Interaction::HFC) {
            continue;
        }

        Spin* spin1 = inter->GetSpin1();
        Spin* spin2 = inter->GetSpin2();

        //We don't currently visualise bilinear interactions involving
        //electrons
        if(spin1->GetElement() == 0) {
            continue;
        }
        if(spin2->GetElement() == 0) {
            continue;
        }

        GetInterColour(t).SetMaterial(0.5);

        double minCuttoff=GetInterSize(t);
        double maxCuttoff=minCuttoff*2;

		energy norm;
		if(inter->GetType() == Interaction::DIPOLAR) {
			norm = inter->AsMatrix().norm()/MHz;
		} else {
			norm = inter->AsMatrix().trace()/MHz;
		}
		norm=abs(norm);

        if(minCuttoff < norm) {
            double width=(norm > maxCuttoff) ? 1.0 : (norm-minCuttoff)/(maxCuttoff-minCuttoff);
            double realWidth = width * 0.04;

            DrawCylinder(spin1->GetPosition() * Angstroms.get_from_si(), spin2->GetPosition() * Angstroms.get_from_si(), realWidth);
        }

    }
}

//============================================================//


FrameDrawer::FrameDrawer() 
    : Renderer(NAME_FRAME),mX(wxT("X")),mY(wxT("Y")),mZ(wxT("Z")) {
}

void FrameDrawer::DrawFrameRecursive(const Frame* frame) const {
    glPushMatrix();
    double A = Angstroms.get_from_si();
    const double* d = frame->getTransformFromLab().data();
    double buffer[16];
    buffer[ 0] = d[ 0];
    buffer[ 1] = d[ 1];
    buffer[ 2] = d[ 2];
    buffer[ 3] = d[ 3];
                             
    buffer[ 4] = d[ 4];
    buffer[ 5] = d[ 5];
    buffer[ 6] = d[ 6];
    buffer[ 7] = d[ 7];
                             
    buffer[ 8] = d[ 8];
    buffer[ 9] = d[ 9];
    buffer[10] = d[10];
    buffer[11] = d[11];
                             
    buffer[12] = d[12]*A;
    buffer[13] = d[13]*A;
    buffer[14] = d[14]*A;
    buffer[15] = d[15];

    glMultMatrixd(buffer);
        
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

    //Lable them
    glRasterPos3f(5,0,0);
    mX.glStamp();
    glRasterPos3f(0,5,0);
    mY.glStamp();
    glRasterPos3f(0,0,5);
    mZ.glStamp();

    const vector<Frame*>& children = frame->GetChildren();
    for(vector<Frame*>::const_iterator i = children.begin();i != children.end();++i) {
        DrawFrameRecursive(*i);
    }

    glPopMatrix();
}

void FrameDrawer::Geometary() const {
    static const GLfloat white[] = {0.5, 0.5, 0.5};
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    Frame* frame = GetRawSS()->GetLabFrame();
    DrawFrameRecursive(frame);
}

//================================================================================//
// Collections of Geometary

SpinSysScene::SpinSysScene() 
    : Renderer(NAME_NONE) {
}

void SpinSysScene::Geometary() const {
    mSpinDrawer.Draw();
    mBondDrawer.Draw();
    mFrameDrawer.Draw();
}


InteractionScene::InteractionScene() 
  : Renderer(NAME_NONE) {
}

void InteractionScene::Geometary() const {
    mMonoDrawer.Draw();
    mBinaryDrawer.Draw();
}
