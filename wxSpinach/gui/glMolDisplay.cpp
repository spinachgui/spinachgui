#include <gl/gl.h>
#include <gl/glu.h>
#include "glMolDisplay.h"

enum {
    TIMER_GLTICK=1
};

glMolDisplay::glMolDisplay(wxWindow* parent)
: wxGLCanvas(parent,-1,wxDefaultPosition,wxDefaultSize),mZoom(0.05),mTimer(this, TIMER_GLTICK) {
    this->mTimer.Start(1);

    camX=0;
    camY=0;
    camZ=5.0;

    xRotate=0.0;
    yRotate=0.0;

    //Initalize rotationMatrix to the identity
    for(long i=0;i<16;i++) {rotationMatrix[i]=(i%5==0 ? 1.0 : 0.0);}

}


void glMolDisplay::enableGL() {
    this->SetCurrent();

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(0.0, 0.0, 1.0);

    GLUquadricObj* qobj= gluNewQuadric();
    gluQuadricDrawStyle(qobj,GLU_LINE);
    gluQuadricNormals(qobj,GLU_SMOOTH);

    list = glGenLists(1);

    glNewList(list,GL_COMPILE);
      gluSphere(qobj,3,20,20);
    glEndList();
    gluDeleteQuadric(qobj);

    DoReize();
}

void glMolDisplay::glTick() {
 	glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);


  //Take the opertunity to calculate the rotation matrix for the scene
  //TODO: This would be better handled on the CPU, it's only one
  //      matrix. Change when matrix classes have been written
    float dotProduct=(xRotate*xRotate+yRotate*yRotate);
    float norm=sqrt(dotProduct);

    glLoadIdentity();
    glRotatef(dotProduct,yRotate/norm,xRotate/norm,0);
    glMultMatrixf(rotationMatrix);
    glGetFloatv(GL_MODELVIEW_MATRIX,rotationMatrix);

  //Load the Identity, actually start the trasfromations
    glLoadIdentity();
    gluLookAt(camX,camY,camZ,0,0,0,0,1,0);
    glMultMatrixf(rotationMatrix);

    xRotate=0.0;
    yRotate=0.0;
//    glLoadMatrixf(rotationMatrix);

    glPushMatrix();
      glTranslatef(0.0,0.0,-5);
      glCallList(list);
    glPopMatrix();


    GLdouble mvmatrix[16], projmatrix[16];
    GLdouble worldx,worldy,worldz;
    GLint viewport[4];

    glGetIntegerv(GL_VIEWPORT,viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);

    gluUnProject(double(mousex),double(height-mousey-1),0.5,mvmatrix,projmatrix,viewport,&worldx,&worldy,&worldz);

    glTranslatef(worldx,worldy,worldz);
    glCallList(list);

	this->SwapBuffers();
}

//=========================>> Event handlers <<====================//

void glMolDisplay::OnMouseMove(wxMouseEvent& e) {
    if(e.Dragging()) {
        xRotate+=(e.GetX()-mousex);
        yRotate+=(e.GetY()-mousey);
    }

    mousex=e.GetX();
    mousey=e.GetY();

}

void glMolDisplay::DoReize() {
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-width*mZoom, width*mZoom, -height*mZoom, height*mZoom, -10.0, 10.0);
}

void glMolDisplay::OnWheel(wxMouseEvent& e) {
    mZoom-=0.001*e.GetWheelRotation()/e.GetWheelDelta();
    if(mZoom<0.001) {
        mZoom=0.001;
    }
    DoReize();
}

void glMolDisplay::OnResize(wxSizeEvent& e) {
  //This should really be done with a onResize event
    this->GetClientSize(&width,&height);
    glViewport(0,0,width,height);

	DoReize();

    e.Skip();
}

void glMolDisplay::OnTimer(wxTimerEvent& e) {
    glTick();
}


BEGIN_EVENT_TABLE(glMolDisplay, wxGLCanvas)
  EVT_MOTION     (glMolDisplay::OnMouseMove)
  EVT_MOUSEWHEEL (glMolDisplay::OnWheel)
  EVT_SIZE       (glMolDisplay::OnResize)
  EVT_TIMER      (TIMER_GLTICK, glMolDisplay::OnTimer)
END_EVENT_TABLE()

