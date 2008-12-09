#include <gl/gl.h>
#include <gl/glu.h>
#include "glMolDisplay.h"

enum {
    TIMER_GLTICK=1
};

glMolDisplay::glMolDisplay(wxWindow* parent)
: wxGLCanvas(parent,-1,wxDefaultPosition,wxDefaultSize),mZoom(0.05),mTimer(this, TIMER_GLTICK) {
    this->mTimer.Start(1);
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

    glLoadIdentity();
    gluLookAt(0,2,4,0,0,0,0,1,0);

    glTranslatef(0.0,0.0,-5);
    glCallList(list);


  //Get the mouse coordinates in normalized device coordinates
//    double xNDC=2*(double(mousex)/double(w))-1.0;
//    double yNDC=2*(double(mousey)/double(h))-1.0;

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

