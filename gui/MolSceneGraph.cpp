

#include <gui/MolSceneGraph.hpp>
#include <shared/nuclear_data.hpp>
#include <cmath>
#include <iostream>
#include <gui/SpinachApp.hpp>

using namespace std;
using namespace SpinXML;

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
    void SetMatrix(const Matrix3d& mat3) {
	//Set the elements of the matrix that matter (others will have been
	//set to 0 in the constructor, so there isn't any need to set them
	//again) 
	/*Calc eigenvectors here*/
	evec_x=Vector3d(1.0,0.0,0.0);
	evec_y=Vector3d(0.0,1.0,0.0);
	evec_z=Vector3d(0.0,0.0,1.0);

	mat[0 ]=abs(mat3(0,0));
	mat[1 ]=abs(mat3(0,1));
	mat[2 ]=abs(mat3(0,2));
			
	mat[4 ]=abs(mat3(1,0));
	mat[5 ]=abs(mat3(1,1));
	mat[6 ]=abs(mat3(1,2));
			
	mat[8 ]=abs(mat3(2,0));
	mat[9 ]=abs(mat3(2,1));
	mat[10]=abs(mat3(2,2));

	trace=mat3.trace();
    }

    float mat[16];
    Vector3d evec_x;
    Vector3d evec_y;
    Vector3d evec_z;
    double trace;
};


class OptNucCentredDataStore {
public:
    OptNucCentredDataStore() {
    }
    void Build(SpinSystem* ss,Interaction::Type t) {
		long count=ss->GetSpinCount();
		mOptList.reserve(count);
		for(long i=0;i<count;i++) {
			Spin* spin=ss->GetSpin(i);
			#warning "Function Missing"
			//ss->GetTotalInteraction(t);
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
    void Build(SpinSystem* ss,Interaction::Type t) {
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





MoleculeFG::MoleculeFG(SpinSystem* ss) 
    : mSS(ss) {
    long count=ss->GetSpinCount();
    for(long i=0;i<count;i++) {
        Spin* spin=ss->GetSpin(i);
        if(spin->GetElement() == 0) {
            //Now we are only drawing electrons
            cout << "No longer adding an electron to the forground" << endl;
        }
    }
}


//============================================================
// The New 3D Code
//============================================================

void DrawCylinder(Vector3d R1,Vector3d R2,length width,unit u,SpinachDC& dc) {
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
		gluCylinder(dc.GetSolidQuadric(),width * u,width * u,len,7,7);
    } glPopMatrix();
}
//============================================================//


MoleculeNodeNew::MoleculeNodeNew(SpinSystem* ss) 
    : mSS(ss) {
	//Add spin drawer node, bond drawer node, interaction drawer
	//node. This node provides a common center for them to rotate
	//about.
	mPickingName = LAYER_AXIS;
	AddNode(new FrameDrawerNode(mSS));
    AddNode(new SpinDrawerNode(mSS));
    AddNode(new BondDrawerNode(mSS));
    AddNode(new InteractionDrawerNode(mSS));
}

void MoleculeNodeNew::RawDraw(SpinachDC& dc) {
}

void MoleculeNodeNew::ToPovRay(wxString& src) {
	//Not implimented
}

//============================================================//

SpinDrawerNode::SpinDrawerNode(SpinSystem* ss) 
    : mSS(ss) {
    mPickingName=LAYER_SPINS;
}

void SpinDrawerNode::RawDraw(SpinachDC& dc) {
    long count=mSS->GetSpinCount();
    for(long i=0;i<count;i++) {
		Spin* spin=mSS->GetSpin(i);
		if(spin->GetElement() != 0){
			glPushMatrix();
			glTranslatef(spin->GetPosition().x(),
						 spin->GetPosition().y(),
						 spin->GetPosition().z());
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
			glTranslatef(40.0f,dc.height-40.0f,0.0f);
			glMultMatrixf(dc.mRotationMatrix);
			gluSphere(dc.GetSolidQuadric(),9 * OPENGL_SCALE,14,14);

		} else {
            glPushName(i);
			gluSphere(dc.GetSolidQuadric(),0.1 * OPENGL_SCALE,14,14);
            glPopName();
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
    mPickingName=LAYER_BONDS;
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
	    Vector3d mR1=spin->GetPosition();
	    Vector3d mR2=nearby[j]->GetPosition();

	    double x1=mR1.x() * Angstroms,y1=mR1.y() * Angstroms,z1=mR1.z() * Angstroms;
	    double x2=mR2.x() * Angstroms,y2=mR2.y() * Angstroms,z2=mR2.z() * Angstroms;

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
    mPickingName=LAYER_INTERACTIONS;
    //OptNucCentredDataStore;
}

void InteractionDrawerNode::RawDrawInterType(SpinachDC& dc,Interaction::Type t) {
    //cout << "==================================================" << endl;
    GLfloat material[4];
    material[0]=dc.mInterColours[t].x();
    material[1]=dc.mInterColours[t].y();
    material[2]=dc.mInterColours[t].z();
    material[3]=0.5f;
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material);    

    long count = mSS->GetSpinCount();
    for(long i=0;i<count;i++) {
		Spin* spin=mSS->GetSpin(i);
		#warning "Missing function"
		if(!false /*spin->GetHasInteractionOfType(t)*/) {
            continue;
        }

        bool electron = spin->GetElement()==0;
        if(electron) {
            if(t==Interaction::QUADRUPOLAR ||
               t==Interaction::SHIELDING  ||
               t==Interaction::HFC        ) {
                continue; //Do not draw these types around an electron
            }
        } else { //Nucleous
            if(t==Interaction::G_TENSER || t==Interaction::ZFS) {
                continue; //Do not draw these types around a nucleous
            } 
        }
        glPushMatrix(); {
            if(!electron) {
                length x,y,z;
                spin->GetCoordinates(&x,&y,&z);
                glTranslatef(x * Angstroms,y * Angstroms,z * Angstroms);
            }
#warning "MissingFunction"
            Matrix3d mat3;//spin->GetTotalInteraction(st);
            /*           Orientation R(AngleAxis(3.141592654/2*0.4,Vector3(0,0.3,1)));
						 Matrix3e mat3(10.0*MHz,0.0*MHz,0.0*MHz,
						 0.0*MHz ,3.0*MHz,0.0*MHz,
						 0.0*MHz ,0.0*MHz,2.0*MHz);
						 mat3=R.GetAsMatrix().Inverted()*mat3*R.GetAsMatrix();

						 energy xx,yy,zz;
						 Vector3 vx,vy,vz;
						 mat3.eig(xx,yy,zz,vx,vy,vz);
						 vx.normalise();
						 vy.normalise();
						 vz.normalise();

						 Matrix3 Rot(vx.GetX(),vy.GetX(),vz.GetX(),
						 vx.GetY(),vy.GetY(),vz.GetY(),
						 vx.GetZ(),vy.GetZ(),vz.GetZ());
						 if(Rot.det().si<0) {
						 Rot=Rot*(-1.0*no_unit);
						 }
						 Matrix3e Scale(xx      ,energy(0.0),energy(0.0),
						 energy(0.0),yy        ,energy(0.0),
						 energy(0.0),energy(0.0),zz        );

						 mat3=Rot*Scale;
			*/
            GLfloat mat[16];
            mat[3 ]=0;
            mat[7 ]=0;
            mat[11]=0;
            mat[12]=0;
            mat[13]=0;
            mat[14]=0;
            mat[15]=1;

            mat[0 ]=abs(mat3(0,0) * MHz);
            mat[1 ]=abs(mat3(0,1) * MHz);
            mat[2 ]=abs(mat3(0,2) * MHz);
		                
            mat[4 ]=abs(mat3(1,0) * MHz);
            mat[5 ]=abs(mat3(1,1) * MHz);
            mat[6 ]=abs(mat3(1,2) * MHz);
		                
            mat[8 ]=abs(mat3(2,0) * MHz);
            mat[9 ]=abs(mat3(2,1) * MHz);
            mat[10]=abs(mat3(2,2) * MHz);

            cout << mat[ 0] << " " << mat[ 1] << " " << mat[ 2] << " " << mat[ 3] << endl 
                 << mat[ 4] << " " << mat[ 5] << " " << mat[ 6] << " " << mat[ 7] << endl
                 << mat[ 8] << " " << mat[ 9] << " " << mat[10] << " " << mat[11] << endl
                 << mat[12] << " " << mat[13] << " " << mat[14] << " " << mat[15] << endl << endl;

            glMultMatrixf(mat);
            double scalling=dc.mScallings[t];
            glScalef(scalling,scalling,scalling);
            gluSphere(dc.GetSolidQuadric(),1.0,29,37);
        } glPopMatrix();
    }
}

void InteractionDrawerNode::RawDrawBilinInterType(SpinachDC& dc, Interaction::Type t) {
    GLfloat material[4];
    material[0]=dc.mInterColours[t].x();
    material[1]=dc.mInterColours[t].y();
    material[2]=dc.mInterColours[t].z();
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
			double scalling=dc.mScallings[t];
			double deltaScalling=2*scalling - scalling;
			#warning "Missing Function"
			energy norm=0;//spin1->GetTotalInteractionNorm(t,spin2);

			if(scalling < norm * MHz) {
			double width=(norm * MHz > scalling*2) ? 0.04 : (((norm * MHz-scalling)/deltaScalling)*0.04);
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
    if(dc.mVisible[Interaction::HFC             ]) RawDrawInterType(dc,Interaction::HFC             );
    if(dc.mVisible[Interaction::SHIELDING       ]) RawDrawInterType(dc,Interaction::SHIELDING       );
    if(dc.mVisible[Interaction::QUADRUPOLAR     ]) RawDrawInterType(dc,Interaction::QUADRUPOLAR     );
    //if(dc.mVisible[Interaction::G_TENSER        ]) RawDrawInterType(dc,Interaction::G_TENSER        );
    //if(dc.mVisible[Interaction::ZFS             ]) RawDrawInterType(dc,Interaction::ZFS             );
    if(dc.mVisible[Interaction::CUSTOM_LINEAR   ]) RawDrawInterType(dc,Interaction::CUSTOM_LINEAR   );
    if(dc.mVisible[Interaction::CUSTOM_QUADRATIC]) RawDrawInterType(dc,Interaction::CUSTOM_QUADRATIC);

    if(dc.mVisible[Interaction::EXCHANGE       ]) RawDrawBilinInterType(dc,Interaction::EXCHANGE       );
    if(dc.mVisible[Interaction::SCALAR         ]) RawDrawBilinInterType(dc,Interaction::SCALAR         );
    if(dc.mVisible[Interaction::DIPOLAR        ]) RawDrawBilinInterType(dc,Interaction::DIPOLAR        );
    if(dc.mVisible[Interaction::CUSTOM_BILINEAR]) RawDrawBilinInterType(dc,Interaction::CUSTOM_BILINEAR);
}



void InteractionDrawerNode::ToPovRay(wxString& src) {
	
}


//============================================================//

FrameDrawerNode::FrameDrawerNode(SpinSystem*) {

}

void DrawFrameRecursive(Frame* frame) {
	glPushMatrix();
	glMultMatrixd(frame->getTransformFromLab().data());
    //Draw some coordiante axese
    glBegin(GL_LINES); {
        glVertex3f(0,0,0);
        glVertex3f(5 * OPENGL_SCALE,0,0);
    }glEnd();

    glBegin(GL_LINES); {
        glVertex3f(0,0,0);
        glVertex3f(0,5 * OPENGL_SCALE,0);
    } glEnd();

    glBegin(GL_LINES); {
        glVertex3f(0,0,0);
        glVertex3f(0,0,5 * OPENGL_SCALE);
    } glEnd();

	const vector<Frame*>& children = frame->GetChildren();
	for(vector<Frame*>::const_iterator i = children.begin();i != children.end();++i) {
		DrawFrameRecursive(*i);
	}

	glPopMatrix();
}

void FrameDrawerNode::RawDraw(SpinachDC& dc) {
    static const GLfloat white[] = {0.5, 0.5, 0.5};
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	Frame* frame = GetRawSS()->GetLabFrame();
	DrawFrameRecursive(frame);
}
