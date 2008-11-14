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
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);

    GLUquadricObj* qobj= gluNewQuadric();
    gluQuadricDrawStyle(qobj,GLU_LINE);
    gluQuadricNormals(qobj,GLU_SMOOTH);

    list = glGenLists(1);

    glNewList(list,GL_COMPILE);
      gluSphere(qobj,0.2,20,20);
    glEndList();
    gluDeleteQuadric(qobj);
}

void glMolDisplay::glTick() {
 	glClear(GL_COLOR_BUFFER_BIT);

  //This should really be done with a onResize event
    int w,h;
    this->GetClientSize(&w,&h);
    glViewport(0,0,w,h);

    glLoadIdentity();
    glTranslatef(0.0,0.0,-0.5);
    glCallList(list);


  //Get the mouse coordinates in normalized device coordinates
    double xNDC=2*(double(mousex)/double(w))-1.0;
    double yNDC=2*(double(mousey)/double(h))-1.0;
    glTranslatef(xNDC,-yNDC,0.0);
    glCallList(list);
  //

	this->SwapBuffers();
}

//=========================>> Event handlers <<====================//

void glMolDisplay::OnMouseMove(wxMouseEvent& e) {
    mousex=e.GetX();
    mousey=e.GetY();
}

BEGIN_EVENT_TABLE(glMolDisplay, wxGLCanvas)
  EVT_MOTION  (glMolDisplay::OnMouseMove)
END_EVENT_TABLE()

