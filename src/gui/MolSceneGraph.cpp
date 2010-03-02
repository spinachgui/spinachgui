
#include <gui/MolSceneGraph.hpp>
#include <shared/nuclear_data.hpp>
#include <cmath>
#include <iostream>
#include <gui/SpinachApp.hpp>

using namespace std;

const double pi=3.141592654;

///Scenegraph node that draws a spin
SpinNode::SpinNode(Spin* spin) 
    : mSpin(spin) {

    mHover=false;
    mSelected=false;

    mSpin->sigDying.connect(mem_fun(this,&SpinNode::OnSpinDying));
    if(spin->GetElement() != 0) {
        //Electrons are placed at (0,0,0)
        SetTranslation(mSpin->GetPosition());
    }
    std::vector<Interaction*> mInters=mSpin->GetInteractions();

    static const Interaction::SubType NuclearCentredInterTypes[]=
        {Interaction::ST_HFC,
         Interaction::ST_SHIELDING,
         Interaction::ST_QUADRUPOLAR, 
         Interaction::ST_CUSTOM_LINEAR,
         Interaction::ST_CUSTOM_QUADRATIC};

    static const Interaction::SubType ElectronCentredInterTypes[]=
        {Interaction::ST_G_TENSER,
         Interaction::ST_ZFS,
         Interaction::ST_CUSTOM_LINEAR,
         Interaction::ST_CUSTOM_QUADRATIC};


    //Here we are creating a node to visualise every type of
    //interaction. I'm not sure if this is really the best idea in terms
    //of efficency. We shall see.
    if(spin->GetElement() != 0){
        //Nulcear interactions
        for(long i=0;i<5;i++) {
            AddNode(new InterNode(mSpin,NuclearCentredInterTypes[i]));
        }
    } else {
        //Electron interactions
        for(long i=0;i<4;i++) {
            AddNode(new InterNode(mSpin,ElectronCentredInterTypes[i]));
        }
    }
    SelectionManager::Instance()->sigHover.connect( mem_fun(this,&SpinNode::OnSpinHover));
    SelectionManager::Instance()->sigSelect.connect(mem_fun(this,&SpinNode::OnSpinSelect));

}
void SpinNode::OnSpinHover(Spin* spin) {
    if(spin==mSpin) {
        mHover=true;
        Dirty();
    }
}
void SpinNode::OnSpinSelect(vector<Spin*> spins) {
    for(vector<Spin*>::iterator i=spins.begin();i != spins.end(); ++i) {
        if((*i)==mSpin) {
            mSelected=true;
            Dirty();
        }
    }
}


void SpinNode::RawDraw(SpinachDC& dc) {
    const static GLfloat white[3]={0.5f,0.5f,0.5f};
    if(true) {
        GLfloat material[3];
        if(mSelected==false) {
            material[0] = getElementR(mSpin->GetElement());
            material[1] = getElementG(mSpin->GetElement());
            material[2] = getElementB(mSpin->GetElement());
        } else {
            material[0] = 1.0;
            material[1] = 1.0;
            material[2] = 1.0;
        }

        glMaterialfv(GL_FRONT, GL_SPECULAR, material);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
    }
    if(mSpin->GetElement()==0) {
        glTranslatef(40,dc.height-40,0);
        glMultMatrixf(dc.mRotationMatrix);
        gluSphere(dc.GetSolidQuadric(),9,14,14);

    } else {
        gluSphere(dc.GetSolidQuadric(),0.1,14,14);
    }
}

void SpinNode::ToPovRay(wxString& src) {
    if(mSpin->GetElement()!=0) {
        GLfloat material[3];
        material[0] = getElementR(mSpin->GetElement());
        material[1] = getElementG(mSpin->GetElement());
        material[2] = getElementB(mSpin->GetElement());

        Vector3l pos=mSpin->GetPosition();

        src << wxT("sphere{\n<") 
            << pos.GetX()[Angstroms] << wxT(",")
            << pos.GetY()[Angstroms] << wxT(",")
            << pos.GetZ()[Angstroms] << wxT(">, 0.1 \npigment {color rgb <") 
            << getElementR(mSpin->GetElement()) << wxT(",")
            << getElementG(mSpin->GetElement()) << wxT(",")
            << getElementG(mSpin->GetElement()) << wxT(">}\n")
            << wxT("\n}\n");
    }
}

InterNode::InterNode(SpinXML::Spin* spin, SpinXML::Interaction::SubType st) 
    : mSpin(spin),mType(st) {
    mSpin->sigDying.connect(mem_fun(this,&InterNode::OnSpinDying));
    mat[3 ]=0;
    mat[7 ]=0;
    mat[11]=0;
    mat[12]=0;
    mat[13]=0;
    mat[14]=0;
    mat[15]=1;
    LoadInteractionMatrix();
    spin->sigNewInteraction.connect(mem_fun(this,&InterNode::OnNewInteraction));
}

void InterNode::OnNewInteraction(Interaction* inter) {
    if(inter->GetSubType()==mType) {
        LoadInteractionMatrix();
    }
}

void InterNode::LoadInteractionMatrix() {
    //Set the elements of the matrix that matter (others will have been
    //set to 0 in the constructor, so there isn't any need to set them
    //again) 

    //TODO: This would be a great place to calculate the eigenvalues
    Matrix3 mat3=mSpin->GetTotalInteraction(mType);
    mat[0 ]=abs(mat3.Get(0,0));
    mat[1 ]=abs(mat3.Get(0,1));
    mat[2 ]=abs(mat3.Get(0,2));
			
    mat[4 ]=abs(mat3.Get(1,0));
    mat[5 ]=abs(mat3.Get(1,1));
    mat[6 ]=abs(mat3.Get(1,2));
			
    mat[8 ]=abs(mat3.Get(2,0));
    mat[9 ]=abs(mat3.Get(2,1));
    mat[10]=abs(mat3.Get(2,2));
    Dirty();
}


void InterNode::RawDraw(SpinachDC& dc) {
    LoadInteractionMatrix();
    const static GLfloat white[3]={0.5f,0.5f,0.5f};
    const static GLfloat blue[3]={0.0,0.0,0.5};
    glColor4f(0.0,0.0,0.0,0.5);
    glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    switch(mType) {
        //Draw as a ellipsoid around a nucleus
    case Interaction::ST_HFC:
        glMaterialfv(GL_FRONT, GL_SPECULAR, blue);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
        goto nuclear_centred_drawing;  //I know, I know, see http://xkcd.com/292/
    case Interaction::ST_CUSTOM_LINEAR:
        glMaterialfv(GL_FRONT, GL_SPECULAR, white);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
        goto nuclear_centred_drawing;
    case Interaction::ST_CUSTOM_QUADRATIC:
        glMaterialfv(GL_FRONT, GL_SPECULAR, white);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
        goto nuclear_centred_drawing;
    case Interaction::ST_QUADRUPOLAR:
        glMaterialfv(GL_FRONT, GL_SPECULAR, white);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
    nuclear_centred_drawing:
        //Apply the transformation matrix to warp the sphere
        glPushMatrix(); {
            double scalling=dc.mScallings[mType];
            glMultMatrixf(mat);
            glScalef(scalling,scalling,scalling);
            gluSphere(dc.GetSolidQuadric(),1.0,29,37);
        } glPopMatrix();
        break;
        //Draw as a cyclinder
    case Interaction::ST_EXCHANGE:
        glMaterialfv(GL_FRONT, GL_SPECULAR, white);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
        goto cyclinder_drawing;
    case Interaction::ST_SHIELDING:
        glMaterialfv(GL_FRONT, GL_SPECULAR, white);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
        goto cyclinder_drawing;
    case Interaction::ST_SCALAR:
        glMaterialfv(GL_FRONT, GL_SPECULAR, white);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
        goto cyclinder_drawing;
    case Interaction::ST_DIPOLAR:
        glMaterialfv(GL_FRONT, GL_SPECULAR, white);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
        goto cyclinder_drawing;
    case Interaction::ST_CUSTOM_BILINEAR:
        glMaterialfv(GL_FRONT, GL_SPECULAR, white);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
    cyclinder_drawing:
    
        break;
        //EPR electron type interactions
    case Interaction::ST_ZFS:
        glMaterialfv(GL_FRONT, GL_SPECULAR, white);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
        goto electron_cenred_drawing;
    case Interaction::ST_G_TENSER:
        glMaterialfv(GL_FRONT, GL_SPECULAR, white);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
    electron_cenred_drawing:
        break;
    case Interaction::ST_ANY:
    case Interaction::ST_NMR:
    case Interaction::ST_EPR:
    default:
        throw std::logic_error("Trying to draw an interaction with an invalid type");
        break;
    }
    glDisable(GL_BLEND);
}
void InterNode::ToPovRay(wxString& str) {
    if(mSpin->GetElement()==0) {
        /* Do nothing, for now */
    } else {
        str << wxT("sphere {<0,0,0> 0.04\n");
        str << wxT("matrix<")
            << mat[0 ] << wxT(",") << mat[1 ] << wxT(",") << mat[2 ]
            << mat[4 ] << wxT(",") << mat[5 ] << wxT(",") << mat[6 ]
            << mat[8 ] << wxT(",") << mat[9 ] << wxT(",") << mat[10]
            << mat[12] << wxT(",") << mat[13] << wxT(",") << mat[14]
            << wxT(">\n");
        str << wxT("pigment {color rgbf <0.8,0.8,0.8,.8>}\n");
        str << wxT("finish {reflection 0.1 refraction 0.9 phong 1.0}\n");
        str << wxT("}\n");
    }

}



void CyclinderNode::RawDraw(SpinachDC& dc) {
    double x1=mR1.GetX(),y1=mR1.GetY(),z1=mR1.GetZ();
    double x2=mR2.GetX(),y2=mR1.GetY(),z2=mR1.GetZ();

    double length=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2));
			
    //Now we need to find the rotation between the z axis
    double angle=acos((z2-z1)/length);
    glPushMatrix();
    glTranslatef(x1,y1,z1);
    glRotatef(angle/2/pi*360,y1-y2,x2-x1,0);
    gluCylinder(dc.GetSolidQuadric(),mRadius,mRadius,length,7,7);
    glPopMatrix();
}



MoleculeNode::MoleculeNode(SpinSystem* ss) 
    : mSS(ss) {
    OnReload();
    mSS->sigReloaded.connect(mem_fun(this,&MoleculeNode::OnReload));
}

void MoleculeNode::OnReload() {
    long count=mSS->GetSpinCount();
    for(long i=0;i<count;i++) {
        Spin* spin=mSS->GetSpin(i);
        if(spin->GetElement() != 0) {
            //If the spin is an electron, it should be drawn outside of the
            //molecule
            AddNode(new SpinNode(spin));
        }
    }
}

void MoleculeNode::OnNewSpin(Spin* newSpin,long number) {
    //Somehow, somewhere a new spin has been created, so create a new
    //spin renderer
    if(newSpin->GetElement() != 0) {
        Dirty();
        AddNode(new SpinNode(newSpin));
    }
    cout << "MoleculeNode::OnNewSpin" << endl;
}

void MoleculeNode::ToPovRay(wxString& str) {
    long count=mSS->GetSpinCount();
    for(long i=0;i<count;i++) {
        Spin* spin=mSS->GetSpin(i);
        if(spin->GetElement() == 0) {
            continue;
        }
        //If the spin is an electron, it should be drawn outside of the
        //molecule
        vector<Spin*> nearby=mSS->GetNearbySpins(spin->GetPosition(),1.8*Angstroms,spin);
        for(unsigned long j=0;j<nearby.size();j++) {
            if(nearby[j]->GetElement()==0) {
                continue;
            }
            Vector3l mR1=spin->GetPosition();
            Vector3l mR2=nearby[j]->GetPosition();

            length x1=mR1.GetX(),y1=mR1.GetY(),z1=mR1.GetZ();
            length x2=mR2.GetX(),y2=mR2.GetY(),z2=mR2.GetZ();

			
            //Now we need to find the rotation between the z axis
            str << wxT("cylinder {")
                << wxT("<") << x1[Angstroms] 
                << wxT(",") << y1[Angstroms] 
                << wxT(",") << z1[Angstroms]
                << wxT(">")
                << wxT("<") << x2[Angstroms]
                << wxT(",") << y2[Angstroms]
                << wxT(",") << z2[Angstroms]
                << wxT(">") 
                << 0.04 << wxT(" \npigment{color rgb <0.0,0.0,0.7>}\n}\n");
        }
    }

}

void MoleculeNode::RawDraw(SpinachDC& dc) {

    static const GLfloat darkgreen[] = {0.0, 0.9,  0.0}; 
    static const GLfloat lightgreen[] = {0.0, 0.9,  0.0}; 
    glMaterialfv(GL_FRONT, GL_AMBIENT, lightgreen);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, darkgreen);

			
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

    GLfloat whiteMaterial[] = {0.5, 0.5,  0.5}; 
    GLfloat blueMaterial[]  = {0.06,0.06, 0.4}; 
    glMaterialfv(GL_FRONT, GL_SPECULAR, blueMaterial);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, whiteMaterial);

#warning "This version contains really dump bond drawing code! Replace as soon as feasable"
    long count=mSS->GetSpinCount();
    for(long i=0;i<count;i++) {
        Spin* spin=mSS->GetSpin(i);
        if(spin->GetElement() == 0) {
            continue;
        }
        //If the spin is an electron, it should be drawn outside of the
        //molecule
        vector<Spin*> nearby=mSS->GetNearbySpins(spin->GetPosition(),1.8*Angstroms,spin);
        for(unsigned long j=0;j<nearby.size();j++) {
            if(nearby[j]->GetElement()==0) {
                continue;
            }
            Vector3l mR1=spin->GetPosition();
            Vector3l mR2=nearby[j]->GetPosition();

            double x1=mR1.GetX()[Angstroms],y1=mR1.GetY()[Angstroms],z1=mR1.GetZ()[Angstroms];
            double x2=mR2.GetX()[Angstroms],y2=mR2.GetY()[Angstroms],z2=mR2.GetZ()[Angstroms];

            double length=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2));
			
            //Now we need to find the rotation between the z axis
            double angle=acos((z2-z1)/length);
            glPushMatrix(); {
                glTranslatef(x1,y1,z1);
                glRotatef(angle/2/pi*360,y1-y2,x2-x1,0);
                gluCylinder(dc.GetSolidQuadric(),0.04,0.04,length,7,7);
            } glPopMatrix();
        }
    }
}



MoleculeFG::MoleculeFG(SpinSystem* ss) 
    : mSS(ss) {
    long count=ss->GetSpinCount();
    for(long i=0;i<count;i++) {
        Spin* spin=ss->GetSpin(i);
        if(spin->GetElement() == 0) {
            //Now we are only drawing electrons
            cout << "Added an electron to the forground" << endl;
            AddNode(new SpinNode(spin));
        }
    }
}
