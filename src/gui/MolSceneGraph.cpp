
#include <gui/MolSceneGraph.hpp>
#include <shared/nuclear_data.hpp>
#include <cmath>
#include <iostream>
#include <gui/SpinachApp.hpp>

using namespace std;

const double pi=3.141592654;

//============================================================//
// Optimised Records

struct OptRecord {
    OptRecord() {
	mat[3 ]=0;
	mat[7 ]=0;
	mat[11]=0;
	mat[12]=0;
	mat[13]=0;
	mat[14]=0;
	mat[15]=1;
    }
    void SetMatrix(const Matrix3& mat3) {
	//Set the elements of the matrix that matter (others will have been
	//set to 0 in the constructor, so there isn't any need to set them
	//again) 
	/*Calc eigenvectors here*/
	evec_x=Vector3(1.0,0.0,0.0);
	evec_y=Vector3(0.0,1.0,0.0);
	evec_z=Vector3(0.0,0.0,1.0);

	mat[0 ]=abs(mat3.Get(0,0));
	mat[1 ]=abs(mat3.Get(0,1));
	mat[2 ]=abs(mat3.Get(0,2));
			
	mat[4 ]=abs(mat3.Get(1,0));
	mat[5 ]=abs(mat3.Get(1,1));
	mat[6 ]=abs(mat3.Get(1,2));
			
	mat[8 ]=abs(mat3.Get(2,0));
	mat[9 ]=abs(mat3.Get(2,1));
	mat[10]=abs(mat3.Get(2,2));

	trace=mat3.Trace();
    }

    float mat[16];
    Vector3 evec_x;
    Vector3 evec_y;
    Vector3 evec_z;
    double trace;
};


class OptNucCentredDataStore {
public:
    OptNucCentredDataStore() {
    }
    void Build(SpinSystem* ss,Interaction::SubType st) {
	long count=ss->GetSpinCount();
	mOptList.reserve(count);
	for(long i=0;i<count;i++) {
	    Spin* spin=ss->GetSpin(i);    
	    spin->GetTotalInteraction(st);
	}
    }
    const OptRecord& GetRecord(long spinN) {
	return mOptList[spinN];
    }
private:
    long mCount;
    std::vector<OptRecord> mOptList;
};



class OptInterNucDataStore {
public:
    OptInterNucDataStore() {
    }
    void Build(SpinSystem* ss,Interaction::SubType st) {
	long count=ss->GetSpinCount();
	for(long i=0;i<count;i++) {
	    
	}
    }
    const OptRecord& GetRecord(long spin1,long spin2) {
	assert(spin1 > spin2);
	return mOptList[(mCount-1)*(spin1+1)-(spin2-1)*spin2/2 + spin1 - spin2];
    }
private:
    long mCount;
    std::vector<OptRecord> mOptList;
};




//============================================================//

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

        glMaterialfv(GL_FRONT, GL_SPECULAR, white);
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material);
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
    SetTranslucency(0.5,true);
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
    if(!dc.mVisible[mType]) 
        return;
    Vector3& colour=dc.mInterColours[mType];

    LoadInteractionMatrix();
    GLfloat mem_colour[4];
    mem_colour[0]=colour.GetX();
    mem_colour[1]=colour.GetY();
    mem_colour[2]=colour.GetZ();
    mem_colour[3]=0.3f;

    //glMaterialfv(GL_FRONT, GL_SPECULAR, mem_colour);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mem_colour);

    glColor4f(colour.GetX(),colour.GetY(),colour.GetZ(),0.5);
    switch(mType) {
    case Interaction::ST_HFC:
    case Interaction::ST_CUSTOM_LINEAR:
    case Interaction::ST_CUSTOM_QUADRATIC:
    case Interaction::ST_QUADRUPOLAR:
    case Interaction::ST_ZFS:
    case Interaction::ST_G_TENSER:
    case Interaction::ST_SHIELDING:
        glPushMatrix(); {
            double scalling=dc.mScallings[mType];
            //glMultMatrixf(mat);
            glScalef(scalling,scalling,scalling);
            gluSphere(dc.GetSolidQuadric(),1.0,29,37);
        } glPopMatrix();
        break;
    case Interaction::ST_EXCHANGE:
    case Interaction::ST_SCALAR:
    case Interaction::ST_DIPOLAR:
    case Interaction::ST_CUSTOM_BILINEAR:
    case Interaction::ST_ANY:
    case Interaction::ST_NMR:
    case Interaction::ST_EPR:
    default:
      cout << "Trying to draw an interaction with an invalid type: " << Interaction::GetSubTypeName(mType) << endl;
      throw std::logic_error("Trying to draw an interaction with an invalid type");
      break;
    }
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
    for(long i=0;i<count;i++) {
	Spin* spin1=mSS->GetSpin(i);
	if(spin1->GetElement() == 0) {
	    continue;
	}
	for(long j=i+1;j<count;j++) {
	    Spin* spin2=mSS->GetSpin(j);
	    if(spin2->GetElement()==0) {
		continue;
	    }
	    cout << i << " " << j << endl;
	    //Add nodes to draw the bilinear interactions
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
    glMaterialfv(GL_FRONT, GL_SPECULAR, whiteMaterial);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blueMaterial);

    if(dc.drawBonds) {
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

                double len=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2));
			
                //Now we need to find the rotation between the z axis
                double angle=acos((z2-z1)/len);
                glPushMatrix(); {
                    glTranslatef(x1,y1,z1);
                    glRotatef(angle/2/pi*360,y1-y2,x2-x1,0);
                    gluCylinder(dc.GetSolidQuadric(),0.04,0.04,len,7,7);
                } glPopMatrix();
            }
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


//============================================================
// The New 3D Code
//============================================================

void DrawCylinder(Vector3l R1,Vector3l R2,length width,length_unit u,SpinachDC& dc) {
    //If the spin is an electron, it should be drawn outside of the
    //molecule
    double x1=R1.GetX()[u],y1=R1.GetY()[u],z1=R1.GetZ()[u];
    double x2=R2.GetX()[u],y2=R2.GetY()[u],z2=R2.GetZ()[u];

    double len=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2));
			
    //Now we need to find the rotation between the z axis
    double angle=acos((z2-z1)/len);
    glPushMatrix(); {
	glTranslatef(x1,y1,z1);
	glRotatef(angle/2/pi*360,y1-y2,x2-x1,0);
	gluCylinder(dc.GetSolidQuadric(),width[u],width[u],len,7,7);
    } glPopMatrix();
}
//============================================================//


MoleculeNodeNew::MoleculeNodeNew(SpinSystem* ss) 
    : mSS(ss) {
	//Add spin drawer node, bond drawer node, interaction drawer
	//node. This node provides a common center for them to rotate
	//about.
    AddNode(new SpinDrawerNode(mSS));
    AddNode(new BondDrawerNode(mSS));
    AddNode(new InteractionDrawerNode(mSS));
}

void MoleculeNodeNew::RawDraw(SpinachDC& dc) {
	//In this case, do nothing
}

void MoleculeNodeNew::ToPovRay(wxString& src) {
	//Not implimented
}

//============================================================//

SpinDrawerNode::SpinDrawerNode(SpinSystem* ss) 
    : mSS(ss) {
    
}
void SpinDrawerNode::RawDraw(SpinachDC& dc) {
    long count=mSS->GetSpinCount();
    for(long i=0;i<count;i++) {
	Spin* spin=mSS->GetSpin(i);
	if(spin->GetElement() != 0){
	    glPushMatrix();
	    glTranslatef(spin->GetPosition().GetX()[Angstroms],
			 spin->GetPosition().GetY()[Angstroms],
			 spin->GetPosition().GetZ()[Angstroms]);
	}
	const static GLfloat white[3]={0.8f,0.8f,0.8f};
	GLfloat material[4]; material[3]=0.0f;

	material[0] = getElementR(spin->GetElement());
	material[1] = getElementG(spin->GetElement());
	material[2] = getElementB(spin->GetElement());

	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material);
	if(spin->GetElement()==0) {
	    glTranslatef(40,dc.height-40,0);
	    glMultMatrixf(dc.mRotationMatrix);
	    gluSphere(dc.GetSolidQuadric(),9,14,14);

	} else {
	    gluSphere(dc.GetSolidQuadric(),0.1,14,14);
	}
	if(spin->GetElement() != 0){
	    glPopMatrix();
	}

    }
}
void SpinDrawerNode::ToPovRay(wxString& src) {
	
}

//============================================================//


BondDrawerNode::BondDrawerNode(SpinSystem* ss)
    : mSS(ss) {
    const static GLfloat blueMaterial[]  = {0.06,0.06,0.4,0.0f}; 
    SetMaterial(blueMaterial);	
}

void BondDrawerNode::RawDraw(SpinachDC& dc) {
    if(!dc.drawBonds) {
	return;
    }
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

	    double len=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2));
			
	    //Now we need to find the rotation between the z axis
	    double angle=acos((z2-z1)/len);
	    glPushMatrix(); {
		glTranslatef(x1,y1,z1);
		glRotatef(angle/2/pi*360,y1-y2,x2-x1,0);
		gluCylinder(dc.GetSolidQuadric(),0.04,0.04,len,7,7);
	    } glPopMatrix();
	}
    }
}
void BondDrawerNode::ToPovRay(wxString& src) {
	
}


InteractionDrawerNode::InteractionDrawerNode(SpinSystem* ss) 
    : mSS(ss) {
    //OptNucCentredDataStore;
}

void InteractionDrawerNode::RawDrawInterType(SpinachDC& dc,Interaction::SubType st) {
    GLfloat material[4];
    material[0]=dc.mInterColours[st].GetX();
    material[1]=dc.mInterColours[st].GetY();
    material[2]=dc.mInterColours[st].GetZ();
    material[3]=0.5f;
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material);    

    long count = mSS->GetSpinCount();
    for(long i=0;i<count;i++) {
	Spin* spin=mSS->GetSpin(i);
        glPushMatrix(); {
	    if(spin->GetElement()!=0) {
		if(st==Interaction::ST_G_TENSER) continue;
		if(st==Interaction::ST_ZFS)      continue;
		length x,y,z;
		spin->GetCoordinates(&x,&y,&z);
		glTranslatef(x[Angstroms],y[Angstroms],z[Angstroms]);
	    } else {
		if(st==Interaction::ST_QUADRUPOLAR) continue;
		if(st==Interaction::ST_SHIELDING  ) continue;
		if(st==Interaction::ST_HFC        ) continue;
	    }

            double scalling=dc.mScallings[st];
            //glMultMatrixf(mat);
            glScalef(scalling,scalling,scalling);
            gluSphere(dc.GetSolidQuadric(),1.0,29,37);
        } glPopMatrix();
    }
}

void InteractionDrawerNode::RawDrawBilinInterType(SpinachDC& dc, Interaction::SubType st) {
    GLfloat material[4];
    material[0]=dc.mInterColours[st].GetX();
    material[1]=dc.mInterColours[st].GetY();
    material[2]=dc.mInterColours[st].GetZ();
    material[3]=0.5f;
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material);    

    long count = mSS->GetSpinCount();
    for(long i=0;i<count;i++) {
	Spin* spin1=mSS->GetSpin(i);
	if(spin1->GetElement()==0) {
	    continue;
	}
	for(long j=i+1;j<count;j++) {
	    Spin* spin2=mSS->GetSpin(j);
	    if(spin2->GetElement()==0) {
		continue;
	    }
	    double scalling=dc.mScallings[st];
	    double deltaScalling=2*scalling - scalling;
	    double trace=i*count+j;//spin1->GetTotalInteractionTrace(st,spin2);

	    if(scalling < trace) {
		double width=(trace > scalling*2) ? 0.04 : (((trace-scalling)/deltaScalling)*0.04);
		DrawCylinder(spin1->GetPosition(),
			     spin2->GetPosition(),
			     width*Angstroms,
			     Angstroms,dc);
	    }
	}
    }
}

void InteractionDrawerNode::RawDraw(SpinachDC& dc) {
    SetTranslucency(0.5,true);
    ///Draw the elipsoid interaction types
    if(dc.mVisible[Interaction::ST_HFC             ]) RawDrawInterType(dc,Interaction::ST_HFC             );
    if(dc.mVisible[Interaction::ST_SHIELDING       ]) RawDrawInterType(dc,Interaction::ST_SHIELDING       );
    if(dc.mVisible[Interaction::ST_QUADRUPOLAR     ]) RawDrawInterType(dc,Interaction::ST_QUADRUPOLAR     );
    //if(dc.mVisible[Interaction::ST_G_TENSER        ]) RawDrawInterType(dc,Interaction::ST_G_TENSER        );
    //if(dc.mVisible[Interaction::ST_ZFS             ]) RawDrawInterType(dc,Interaction::ST_ZFS             );
    if(dc.mVisible[Interaction::ST_CUSTOM_LINEAR   ]) RawDrawInterType(dc,Interaction::ST_CUSTOM_LINEAR   );
    if(dc.mVisible[Interaction::ST_CUSTOM_QUADRATIC]) RawDrawInterType(dc,Interaction::ST_CUSTOM_QUADRATIC);

    if(dc.mVisible[Interaction::ST_EXCHANGE       ]) RawDrawBilinInterType(dc,Interaction::ST_EXCHANGE       );
    if(dc.mVisible[Interaction::ST_SCALAR         ]) RawDrawBilinInterType(dc,Interaction::ST_SCALAR         );
    if(dc.mVisible[Interaction::ST_DIPOLAR        ]) RawDrawBilinInterType(dc,Interaction::ST_DIPOLAR        );
    if(dc.mVisible[Interaction::ST_CUSTOM_BILINEAR]) RawDrawBilinInterType(dc,Interaction::ST_CUSTOM_BILINEAR);
}



void InteractionDrawerNode::ToPovRay(wxString& src) {
	
}



