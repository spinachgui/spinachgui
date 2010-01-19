
#include <gui/MolSceneGraph.hpp>
#include <cmath>

const double pi=3.141592654;

void SphereNode::RawDraw(const SpinachDC& dc) {
  gluSphere(dc.GetSolidQuadric(),mRadius,14,14);
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


EllipsoidNode::EllipsoidNode() {
  mat[0 ]=1;  mat[1 ]=0;  mat[2 ]=0;  mat[3 ]=0;
  mat[4 ]=0;  mat[5 ]=1;  mat[6 ]=0;  mat[7 ]=0;
  mat[8 ]=0;  mat[9 ]=0;  mat[10]=1;  mat[11]=0;
  mat[12]=0;  mat[13]=0;  mat[14]=0;  mat[15]=1;
}

EllipsoidNode::EllipsoidNode(const Matrix3& mat3) {
  //Some elements of this matrix will always be set to specific
  //values, so we might as well keep them
  mat[3 ]=0;
  mat[7 ]=0;
  mat[11]=0;
  mat[12]=0;
  mat[13]=0;
  mat[14]=0;
  mat[15]=1;

  SetMatrix(mat3);
}

void EllipsoidNode::SetMatrix(const Matrix3& mat3) {
  //Set the elements of the matrix that matter (others will have been
  //set to 0 in the constructor, so there isn't any need to set them
  //again) 

  //TODO: This would be a great place to calculate the eigenvalues
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

void EllipsoidNode::RawDraw(const SpinachDC& dc) {
  ///Unpack a 3x3 matrix into a 4x4 opengl matrix
  //Apply the transformation matrix to warp the sphere
  glPushMatrix(); {
    glPushMatrix(); {
      glMultMatrixf(mat);
      glScalef(0.04,0.04,0.04);
      gluSphere(dc.GetWireQuadric(),1.0,14,14);
    } glPopMatrix();
			
    double eValX=1;//self.ss.getEigenValX(i).real;
    double eValY=1;//self.ss.getEigenValY(i).real;
    double eValZ=1;//self.ss.getEigenValZ(i).real;
			
    float eVecX[]={1,0,0};
    float eVecY[]={0,1,0};
    float eVecZ[]={0,0,1};
			
    //Draw the three eigenvectors of the interactionx
    glBegin(GL_LINES); {
      glVertex3f(0,0,0);
      glVertex3f(eVecX[0]*eValX,eVecX[1]*eValX,eVecX[2]*eValX);
    }glEnd();
			
    glBegin(GL_LINES); {
      glVertex3f(0,0,0);
      glVertex3f(eVecY[0]*eValY,eVecY[1]*eValY,eVecY[2]*eValY);
    } glEnd();
			
    glBegin(GL_LINES); {
      glVertex3f(0,0,0);
      glVertex3f(eVecZ[0]*eValZ,eVecZ[1]*eValZ,eVecZ[2]*eValZ);
    } glEnd();
			
  } glPopMatrix();

  float mMat[16];
}

GLfloat white[] = {0.5, 0.5,  0.5}; 



void MoleculeNode::RawDraw(const SpinachDC& dc) {
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

  glMaterialfv(GL_FRONT, GL_SPECULAR, white);

  double eValX=1;//self.ss.getEigenValX(i).real;
  double eValY=1;//self.ss.getEigenValY(i).real;
  double eValZ=1;//self.ss.getEigenValZ(i).real;
			
  float eVecX[]={1,0,0};
  float eVecY[]={0,1,0};
  float eVecZ[]={0,0,1};
			
  //Draw the three eigenvectors of the interactionx
  glBegin(GL_LINES); {
    glVertex3f(0,0,0);
    glVertex3f(eVecX[0]*eValX,eVecX[1]*eValX,eVecX[2]*eValX);
  }glEnd();
			
  glBegin(GL_LINES); {
    glVertex3f(0,0,0);
    glVertex3f(eVecY[0]*eValY,eVecY[1]*eValY,eVecY[2]*eValY);
  } glEnd();
			
  glBegin(GL_LINES); {
    glVertex3f(0,0,0);
    glVertex3f(eVecZ[0]*eValZ,eVecZ[1]*eValZ,eVecZ[2]*eValZ);
  } glEnd();
}



