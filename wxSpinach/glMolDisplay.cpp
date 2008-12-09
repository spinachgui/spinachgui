#include <gl/gl.h>
#include <gl/glu.h>
#include "glMolDisplay.h"

glMolDisplay::glMolDisplay(wxWindow* parent)
: wxGLCanvas(parent,-1,wxDefaultPosition,wxDefaultSize) {
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
}

void glMolDisplay::glTick() {
  //This should really be done with a onResize event
    int w,h;
    this->GetClientSize(&w,&h);
    glViewport(0,0,w,h);


 	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w/10, w/10, -h/10, h/10, -10.0, 10.0);

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

    gluUnProject(double(mousex),double(h-mousey-1),0.5,mvmatrix,projmatrix,viewport,&worldx,&worldy,&worldz);

    glTranslatef(worldx,worldy,worldz);
    glCallList(list);

	this->SwapBuffers();
}

//=========================>> Event handlers <<====================//

void glMolDisplay::OnMouseMove(wxMouseEvent& e) {
    mousex=e.GetX();
    mousey=e.GetY();
}

void glMolDisplay::OnResize(wxSizeEvent& e) {
    this->GetClientSize(&width,&height);
    e.Skip();
}


BEGIN_EVENT_TABLE(glMolDisplay, wxGLCanvas)
  EVT_MOTION  (glMolDisplay::OnMouseMove)
  EVT_SIZE    (glMolDisplay::OnResize)
END_EVENT_TABLE()

