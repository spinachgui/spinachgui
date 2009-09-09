
#include <cmath>

#include <gui/glDisplay.hpp>

#include <wx/dcclient.h>

#include <gui/SpinachApp.hpp>

const double pi=3.141592654;

using namespace std;

enum MENU_EVENTS {
  MENU_SPIN_DIALOG,
  MENU_COUPLING_DIALOG
};

GLfloat whiteMaterial[] = {0.5, 0.5,  0.5}; 
GLfloat blueMaterial[]  = {0.06,0.06, 0.4}; 
GLfloat redMaterial[]   = {0.9, 0.00, 0.0}; 

glDisplay::glDisplay(wxWindow* parent,wxWindowID id) : wxGLCanvas(parent,id,NULL) {
  mGLContext=NULL;
  mGLEnabled=false;
  mSS=wxGetApp().GetSpinSystem();

  mZoom=0.01;
  mCamX=0.0;
  mCamY=0.0;
  mCamZ=15.0;

  mXTranslate=0;
  mYTranslate=0;
  mXRotate=0;
  mYRotate=0;

  mRotationMatrix[0 ]=1.0;
  mRotationMatrix[1 ]=0.0;
  mRotationMatrix[2 ]=0.0;
  mRotationMatrix[3 ]=0.0;
		    
  mRotationMatrix[4 ]=0.0;
  mRotationMatrix[5 ]=1.0;
  mRotationMatrix[6 ]=0.0;
  mRotationMatrix[7 ]=0.0;
		    
  mRotationMatrix[8 ]=0.0;
  mRotationMatrix[9 ]=0.0;
  mRotationMatrix[10]=1.0;
  mRotationMatrix[11]=0.0;
		    
  mRotationMatrix[12]=0.0;
  mRotationMatrix[13]=0.0;
  mRotationMatrix[14]=0.0;
  mRotationMatrix[15]=1.0;
}

void glDisplay::CreateBondList() {
  glNewList(mDLBonds,GL_COMPILE);

  GLUquadric* qobj = gluNewQuadric();
  gluQuadricDrawStyle(qobj,GLU_FILL);
  gluQuadricNormals(qobj,GLU_SMOOTH);

  for(long i=0;i<mSS->GetSpinCount();i++) {   //Draw the spins and the bonds
    Spin* thisSpin=mSS->GetSpin(i);
    double x1,y1,z1;
    thisSpin->GetCoordinates(&x1,&y1,&z1);
        
    //draw in bonds to nearby atoms

    glMaterialfv(GL_FRONT, GL_SPECULAR, blueMaterial);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, whiteMaterial);
    vector<Spin*> nearby=thisSpin->GetNearbySpins(1.8);
    glColor3f(1.0, 0.0, 0.0);
    for(long j=0;j<nearby.size();j++) {
      Spin* nearbySpin=nearby[j];
      double x2,y2,z2;
      nearbySpin->GetCoordinates(&x2,&y2,&z2);
      double bondLength=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2));

	//Now we need to find the rotation between the z axis
      double angle=acos((z2-z1)/bondLength);
      glPushMatrix();
      glTranslatef(x1,y1,z1);
      glRotatef(angle/2/pi*360,y1-y2,x2-x1,0);
      gluCylinder(qobj,0.1,0.1,bondLength,7,7);
      glPopMatrix();
      glEndList();
    }
  }
}

void glDisplay::Geometary() {
  int width,height;
  GetClientSize(&width,&height);

  long spinCount=mSS->GetSpinCount();

  GLUquadric* qobj = gluNewQuadric();
  gluQuadricDrawStyle(qobj,GLU_FILL);
  gluQuadricNormals(qobj,GLU_SMOOTH);

  //Move these to some sort of config file
  float radius=0.3;
  float radius2=radius*radius;

  
  //Find out if any of the spins are under the mouse and record the closest
  mHover=-1;     //The index of the spin the user is hovering over
  mHoverDist=0;  //This distance to that spin

  GLint viewport[4];
  GLdouble mvmatrix[16],projmatrix[16];
  glGetIntegerv(GL_VIEWPORT,viewport);
  glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
  glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);

  GLdouble worldFarX,  worldFarY ,worldFarZ;
  GLdouble worldNearX, worldNearY,worldNearZ; 
  gluUnProject(mMouseX,height-mMouseY-1,1.0,mvmatrix,projmatrix,viewport,&worldFarX, &worldFarY ,&worldFarZ);
  gluUnProject(mMouseX,height-mMouseY-1,0.0,mvmatrix,projmatrix,viewport,&worldNearX,&worldNearY,&worldNearZ);

  for(long i=0;i<spinCount;i++) {  //Decide which spin is selected
    double x,y,z;
    Spin* thisSpin=mSS->GetSpin(i);
    thisSpin->GetCoordinates(&x,&y,&z);

    //The distance from the near clipping plane is reused in the colision detection
    //print x,y,z
    double clipDist2=(worldNearX-x)*(worldNearX-x) +    (worldNearY-y)*(worldNearY-y) + (worldNearZ-z)*(worldNearZ-z);
    if( clipDist2 > mHoverDist && mHover!=-1) {
      continue; //We already found a closer spin, so there isn't any point in checking this one
    }
    //Cast a ray from the eye to worldx,worldy,worldz and see if it collides with anything.
    //There four equations which combine into quadratic equation. If the descrimiate indicates a real
    //solution then the ray hits the sphere.
    double Rx=worldNearX-worldFarX;
    double Ry=worldNearY-worldFarY;
    double Rz=worldNearZ-worldFarZ;

    double A =    Rx*Rx+                         Ry*Ry                    + Rz*Rz;
    double B = 2*(Rx*(worldNearX-x) +            Ry*(worldNearY-y) +        Rz*(worldNearZ-z));
    double C =    clipDist2 - radius2;

    double desc=B*B-4*A*C;
    if (desc > 0) {
      mHover=i;
      mHoverDist=clipDist2;
    }
            
    for(long i=0;i<spinCount;i++) {   //Draw the spins and the bonds
      Spin* thisSpin=mSS->GetSpin(i);
      double x,y,z;
      thisSpin->GetCoordinates(&x,&y,&z);

      glColor3f(1.0, 1.0, 1.0);
      //Draw in the single spin interaction tensor
      Matrix3 mat3=thisSpin->GetLinearInteractionAsMatrix();
      //Convert to a openGL 4x4 matrix
      float mat[16];
      mat[0 ]=abs(mat3.Get(0,0));
      mat[1 ]=abs(mat3.Get(0,1));
      mat[2 ]=abs(mat3.Get(0,2));
      mat[3 ]=0;
	    
      mat[4 ]=abs(mat3.Get(0,0));
      mat[5 ]=abs(mat3.Get(0,1));
      mat[6 ]=abs(mat3.Get(0,2));
      mat[7 ]=0;
	    
      mat[8 ]=abs(mat3.Get(0,0));
      mat[9 ]=abs(mat3.Get(0,1));
      mat[10]=abs(mat3.Get(0,2));
      mat[11]=0;
	    
      mat[12]=0;
      mat[13]=0;
      mat[14]=0;
      mat[15]=1;


      //Apply the transformation matrix to warp the sphere
      glPushMatrix();

      glTranslatef(x,y,z);

      glPushMatrix();
      glMultMatrixf(mat);
      glScalef(0.04,0.04,0.04);
      glCallList(mDLSphereWire);
      glPopMatrix();

      double eValX=1;//self.ss.getEigenValX(i).real;
      double eValY=1;//self.ss.getEigenValY(i).real;
      double eValZ=1;//self.ss.getEigenValZ(i).real;

      float eVecX[]={1,0,0};
      float eVecY[]={0,1,0};
      float eVecZ[]={0,0,1};

      //Draw the three eigenvectors of the interactionx
      glBegin(GL_LINES);
      glVertex3f(0,0,0);
      glVertex3f(eVecX[0]*eValX,eVecX[1]*eValX,eVecX[2]*eValX);
      glEnd();

      glBegin(GL_LINES);
      glVertex3f(0,0,0);
      glVertex3f(eVecY[0]*eValY,eVecY[1]*eValY,eVecY[2]*eValY);
      glEnd();

      glBegin(GL_LINES);
      glVertex3f(0,0,0);
      glVertex3f(eVecZ[0]*eValZ,eVecZ[1]*eValZ,eVecZ[2]*eValZ);
      glEnd();

      glPopMatrix();


      if(mHover==i) {
	glMaterialfv(GL_FRONT, GL_SPECULAR, redMaterial);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, whiteMaterial);
      }/* else if(i in self.selected) {
	glMaterialfv(GL_FRONT, GL_SPECULAR, selectedMaterial);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, whiteMaterial);                
      }	else {
	letter,num=splitSymbol("H1");
	if (letter in self.colourDict) {
	  glMaterialfv(GL_FRONT, GL_SPECULAR, self.colourDict[letter]);
	  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, self.colourDict[letter]);
	}
	}*/
      glPushMatrix();
      glTranslatef(x,y,z);
      glScalef(radius,radius,radius);
      glCallList(mDLSphereSolid);
      glPopMatrix();
    }
    glCallList(mDLBonds);

 
    glDisable(GL_LIGHTING);   
    glDisable(GL_LIGHT0);     
    glDisable(GL_LIGHT1);     
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for(long i=0;i<spinCount;i++) { // #Draw the J couplings
      //Do the two spin couplings
      Spin* thisSpin=mSS->GetSpin(i);
      double x1,y1,z1;
      thisSpin->GetCoordinates(&x1,&y1,&z1);  
      for (long j=i+1;j<spinCount;j++) {
	Spin* jSpin=mSS->GetSpin(j);
	double scalar=abs(thisSpin->GetBilinearInteractionAsScalar(jSpin))/300;
	if (scalar < 20) {
	  continue;
	}
	double x2,y2,z2;
	jSpin->GetCoordinates(&x2,&y2,&z2);
	glColor4f(scalar,0,0,scalar);
	glBegin(GL_LINES);
	glVertex3f(x1,y1,z1);
	glVertex3f(x2,y2,z2);
	glEnd();
      }
    }
    glDisable(GL_BLEND);
  }
}

void glDisplay::OnPaint(wxPaintEvent& e) {
  if(!mGLEnabled) {
    mGLEnabled=true;
    EnableGL();
  }

  wxPaintDC dc(this);

  glClear(GL_COLOR_BUFFER_BIT);
  glClear(GL_DEPTH_BUFFER_BIT);


  glColor3f(0.0, 0.0, 1.0);

  int width,height;
  GetClientSize(&width,&height);


  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-width*mZoom, width*mZoom, -height*mZoom, height*mZoom, -40.0, 40.0);
  //gluPerspective(45.0,float(width)/float(height),0.1, 200.0);
  glMatrixMode(GL_MODELVIEW);


  //Take the opertunity to calculate the rotation matrix for the scene
  //TODO: This would be better handled on the CPU, it's only one
  //matrix. Change when matrix classes have been written
  float dotProduct=(mXRotate*mXRotate+mYRotate*mYRotate);
  float norm=sqrt(dotProduct);
  glLoadIdentity();

  if(norm != 0){ //Prevent division by zero errors
    glRotatef(dotProduct,mYRotate/norm,mXRotate/norm,0);
  }
  glTranslatef(mXTranslate*mZoom,mYTranslate*mZoom,0);
  mXRotate=0;
  mYRotate=0;
  mXTranslate=0;
  mYTranslate=0;
  glMultMatrixf(mRotationMatrix);
  glGetFloatv(GL_MODELVIEW_MATRIX,mRotationMatrix);
  

  glLoadIdentity();
  gluLookAt(mCamX,mCamY,mCamZ,0,0,-1,0,1,0);
  glMultMatrixf(mRotationMatrix);


  glClear(GL_COLOR_BUFFER_BIT);
  glClear(GL_DEPTH_BUFFER_BIT);
  glClearDepth(1.0);

  glEnable(GL_DEPTH_TEST);

  Geometary();
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,mTexDepth);
  glCopyTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,0,0,width,height,0);
  glDisable(GL_TEXTURE_2D);

  //glEnable(GL_TEXTURE_2D);
  //glBindTexture(GL_TEXTURE_2D,self.tex);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  //glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);


  //glAlphaFunc(GL_GEQUAL, 0.99);
  //glEnable(GL_ALPHA_TEST);

  glEnable(GL_LIGHTING);   
  glEnable(GL_LIGHT0);     
  glEnable(GL_LIGHT1);     
  glClear(GL_COLOR_BUFFER_BIT);
  glClear(GL_DEPTH_BUFFER_BIT);
  Geometary();
  glDisable(GL_TEXTURE_2D);

  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
  glBindTexture(GL_TEXTURE_2D,mTexDepth);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);


  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0,width,height,0);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity();
        
  long littleWidth=120;
  long littleHeight=round(littleWidth*height/float(width));
        
  glBegin(GL_QUADS);
  glTexCoord2f(0,0);
  glVertex2f(0,0);
        
  glTexCoord2f(1.0,0);
  glVertex2f(littleWidth,0);
    
  glTexCoord2f(1.0,1.0);
  glVertex2f(littleWidth,littleHeight);
        
  glTexCoord2f(0,1.0);
  glVertex2f(0,littleHeight);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  SwapBuffers();
}

void glDisplay::ChangeViewport() {
  int width,height;
  GetClientSize(&width,&height);
  glViewport(0,0,width,height);

  //From the documentation:
  //glDeleteTextures silently ignores 0's and names that do not correspond to existing textures.
  //so it's okay that self.tex will not exist at first
  glDeleteTextures(1,&mTexDepth);
  glGenTextures(1,&mTexDepth);

  glBindTexture(GL_TEXTURE_2D,mTexDepth);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

  glTexImage2D(GL_PROXY_TEXTURE_2D,0,GL_RGBA,
	       height,width,
	       0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);

}


void glDisplay::EnableGL() {
  if(mGLContext == NULL) {
    mGLContext = new wxGLContext(this);
  }
  this->SetCurrent(*mGLContext);

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClearDepth(1.0);

  mDLSphereWire = glGenLists(1);
       
  GLUquadric* qobj = gluNewQuadric();

  gluQuadricDrawStyle(qobj,GLU_LINE);
  gluQuadricNormals(qobj,GLU_SMOOTH);

  glNewList(mDLSphereWire,GL_COMPILE);
  gluSphere(qobj,1.0,14,14);
  glEndList();
  gluDeleteQuadric(qobj);

  mDLSphereSolid = glGenLists(1);

  qobj = gluNewQuadric();
  gluQuadricDrawStyle(qobj,GLU_FILL);
  gluQuadricNormals(qobj,GLU_SMOOTH);

  glNewList(mDLSphereSolid,GL_COMPILE);
  gluSphere(qobj,1.0,12,12);
  glEndList();
  gluDeleteQuadric(qobj);



  glDepthFunc(GL_LEQUAL);

  //Reserve Space for the display lists
  mDLBonds=glGenLists(1);

  //glShadeModel(GL_SMOOTH);
  //Adpated from a detailed tutorail on opengl lighting located at
  //http://www.falloutsoftware.com/tutorials/gl/gl8.htm


  // We're setting up two light sources. One of them is located
  // on the left side of the model (x = -1.5f) and emits white light. The
  // second light source is located on the right side of the model (x = 1.5f)
  // emitting red light.

  // GL_LIGHT0: the white light emitting light source
  // Create light components for GL_LIGHT0
  GLfloat ambientLight0[] =  {0.0, 0.0, 0.0, 1.0};
  GLfloat diffuseLight0[] =  {0.5, 0.5, 0.5, 1.0};
  GLfloat specularLight0[] = {0.6, 0.6, 0.6, 1.0};
  GLfloat position0[] =      {-1.5, 1.0,-4.0, 1.0};	
  // Assign created components to GL_LIGHT0
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight0);
  glLightfv(GL_LIGHT0, GL_POSITION, position0);

  // GL_LIGHT1: the red light emitting light source
  // Create light components for GL_LIGHT1
  GLfloat ambientLight1[] =  {0.1, 0.1, 0.1, 1.0};
  GLfloat diffuseLight1[] =  {0.1, 0.1, 0.1, 1.0};
  GLfloat specularLight1[] = {0.3, 0.3, 0.3, 1.0};
  GLfloat position1[] =      {1.5, 1.0, 4.0, 1.0};	
  // Assign created components to GL_LIGHT1
  glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight1);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight1);
  glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight1);
  glLightfv(GL_LIGHT1, GL_POSITION, position1);

  //glEnable(GL_COLOR_MATERIAL);
  //glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

  //Create a dictionary of colours

  glGenTextures(1,&mTexDepth);
  CreateBondList();

  ChangeViewport();
}


void glDisplay::OnMouseMove(wxMouseEvent& e) {
  if(e.Dragging() && e.RightIsDown()) {
    mXTranslate=mXTranslate+(e.GetX()-mMouseX);
    mYTranslate=mYTranslate-(e.GetY()-mMouseY);
  }  else if(e.Dragging() && e.LeftIsDown()) {
    mXRotate=mXRotate+(e.GetX()-mMouseX);
    mYRotate=mYRotate+(e.GetY()-mMouseY);
  }
  mMouseX=e.GetX();
  mMouseY=e.GetY();
  Refresh();
}


void glDisplay::OnWheel(wxMouseEvent& e) {
  mZoom=mZoom-0.001*e.GetWheelRotation()/e.GetWheelDelta();
  if(mZoom<0.001) {
    mZoom=0.001;
  }
  Refresh();
}

void glDisplay::OnRightClick(wxMouseEvent& e) {
  if(not e.Dragging()) {
    wxMenu* menu = new wxMenu();
    menu->Append(-1, wxT("Test Menu Item"));

    if(mHover>=0) {
      menu->Append(MENU_SPIN_DIALOG, wxT("Spin Properties..."));
     }
    //  if(len(self.selected)==2) {
    //  menu->Append(-1, wxT("Coupling Properties"));
    // }
    PopupMenu(menu);
    delete menu;
  }
}

void glDisplay::OnLeftClick(wxMouseEvent& e) {
  if(! e.ShiftDown()) {
    //selected=[];
  }
  if (mHover != -1) {
    //selected.append(self.hover);
    Refresh();
  }

}

void glDisplay::OnResize(wxSizeEvent& e) {
  ChangeViewport();
}

void glDisplay::OnDisplaySpinDialog(wxCommandEvent& e) {
  
}

BEGIN_EVENT_TABLE(glDisplay,wxGLCanvas)

EVT_PAINT     (                 glDisplay::OnPaint)
EVT_MOTION    (                 glDisplay::OnMouseMove)
EVT_MOUSEWHEEL(                 glDisplay::OnWheel)
EVT_RIGHT_UP  (                 glDisplay::OnRightClick)
EVT_LEFT_UP   (                 glDisplay::OnLeftClick)
EVT_MENU      (MENU_SPIN_DIALOG,glDisplay::OnDisplaySpinDialog)

END_EVENT_TABLE()




