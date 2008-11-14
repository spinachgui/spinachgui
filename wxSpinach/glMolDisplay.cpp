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
	glLoadIdentity();

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
 	glClear(GL_COLOR_BUFFER_BIT);

  //This should really be done with a onResize event
    int w,h;
    this->GetClientSize(&w,&h);
    glViewport(0,0,w,h);

    glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
 //   gluLookAt(0,0,5.0,0,0,0,0,1,0);   //Move the camera to 0,0,5 and aim at 0,0,0

	glLoadIdentity();
    glTranslatef(0,0,5.0);
    glCallList(list);  //Draw a sphere at 5,0,0 (global coordinates)

	glLoadIdentity();
    glTranslatef(0,0,-5.0);
    glCallList(list);  //Draw a sphere at 5,0,0 (global coordinates)


	this->SwapBuffers();
}

//=========================>> Event handlers <<====================//

void glMolDisplay::OnMouseMove(wxMouseEvent& e) {
    int x=e.GetX();
    int y=e.GetY();
}

BEGIN_EVENT_TABLE(glMolDisplay, wxGLCanvas)
  EVT_MOTION  (glMolDisplay::OnMouseMove)
END_EVENT_TABLE()

