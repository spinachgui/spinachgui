
#include <gui/MolSceneGraph.hpp>
#include <shared/nuclear_data.hpp>
#include <cmath>
#include <iostream>

using namespace std;

const double pi=3.141592654;

///Scenegraph node that draws a spin
SpinNode::SpinNode(Spin* spin) 
  : mSpin(spin) {
  mSpin->sigDying.connect(mem_fun(this,&SpinNode::OnSpinDying));
  SetTranslation(mSpin->GetPosition());
  std::vector<Interaction*> mInters=mSpin->GetInteractions();
  for(long i=0;i<mInters.size();i++) {
    AddNode(new InterNode(mInters[i]));
  }
}


void SpinNode::RawDraw(const SpinachDC& dc) {
  const static GLfloat white[3]={0.5f,0.5f,0.5f};
  const static GLfloat blue[3]={0.0,0.0,0.5};
  if(true) {
    GLfloat material[3];
    material[0] = getElementR(mSpin->GetElement());
    material[1] = getElementG(mSpin->GetElement());
    material[2] = getElementB(mSpin->GetElement());

    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
  }
  gluSphere(dc.GetSolidQuadric(),0.2,14,14);
}

InterNode::InterNode(Interaction* inter) 
  : mInter(inter) {
  mInter->sigDying.connect(mem_fun(this,&InterNode::OnInterDying));
  mat[3 ]=0;
  mat[7 ]=0;
  mat[11]=0;
  mat[12]=0;
  mat[13]=0;
  mat[14]=0;
  mat[15]=1;
  LoadInteractionMatrix();
}

void InterNode::LoadInteractionMatrix() {
  //Set the elements of the matrix that matter (others will have been
  //set to 0 in the constructor, so there isn't any need to set them
  //again) 

  //TODO: This would be a great place to calculate the eigenvalues
  Matrix3 mat3=mInter->GetAsMatrix();
  mat[0 ]=abs(mat3.Get(0,0));
  mat[1 ]=abs(mat3.Get(0,1));
  mat[2 ]=abs(mat3.Get(0,2));
			
  mat[4 ]=abs(mat3.Get(1,0));
  mat[5 ]=abs(mat3.Get(1,1));
  mat[6 ]=abs(mat3.Get(1,2));
			
  mat[8 ]=abs(mat3.Get(2,0));
  mat[9 ]=abs(mat3.Get(2,1));
  mat[10]=abs(mat3.Get(2,2));
}


void InterNode::RawDraw(const SpinachDC& dc) {
  const static GLfloat white[3]={0.5f,0.5f,0.5f};
  const static GLfloat blue[3]={0.0,0.0,0.5};

  switch(mInter->GetSubType()) {
    //Draw as a ellipsoid around a nucleus
  case Interaction::ST_HFC:
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
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
      glPushMatrix(); {
	glMultMatrixf(mat);
	glScalef(0.04,0.04,0.04);
	gluSphere(dc.GetWireQuadric(),1.0,14,14);
      } glPopMatrix();
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
  }
}



void CyclinderNode::RawDraw(const SpinachDC& dc) {
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
  long count=ss->GetSpinCount();
  for(long i=0;i<count;i++) {
    AddNode(new SpinNode(ss->GetSpin(i)));
  }
}

void MoleculeNode::OnNewSpin(Spin* newSpin,long number) {
  //Somehow, somewhere a new spin has been created, so create a new
  //spin renderer
  AddNode(new SpinNode(newSpin));
}


void MoleculeNode::RawDraw(const SpinachDC& dc) {
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  static const GLfloat white[] = {0.5, 0.5,  0.5}; 
  glMaterialfv(GL_FRONT, GL_SPECULAR, white);
			
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
}



