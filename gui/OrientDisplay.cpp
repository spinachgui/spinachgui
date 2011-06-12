
#include <gui/OrientDisplay.hpp>


#include <3d/glmode.hpp>
#include <3d/camera.hpp>
#include <3d/displaySettings.hpp>
#include <shared/basic_math.hpp>

static GLLighting lighting;

OrientDisplay3D::OrientDisplay3D(wxWindow* parent) 
    : Display3D(parent),
      mTX(wxString(wxT("X"))),
      mTY(wxString(wxT("Y"))),
      mTZ(wxString(wxT("Z"))) {
}

void OrientDisplay3D::DrawScene() {
    lighting.On();
    gluCylinder(GetQuadric(),0.5,0.5,1,8,8);
    gluSphere(GetQuadric(),0.3,30,30);
    lighting.Off();

    //Draw some coordiante axese
    glColor3f(1,0,0);
    glBegin(GL_LINES); {
        glVertex3f(0,0,0);
        glVertex3f(5,0,0);
    }glEnd();

    glColor3f(0,1,0);
    glBegin(GL_LINES); {
        glVertex3f(0,0,0);
        glVertex3f(0,5,0);
    } glEnd();

    glColor3f(0,0,1);
    glBegin(GL_LINES); {
        glVertex3f(0,0,0);
        glVertex3f(0,0,5);
    } glEnd();

    glColor3f(0,0,0);
    glRasterPos3f(5,0,0);
    mTX.glStamp();
    glRasterPos3f(0,5,0);
    mTY.glStamp();
    glRasterPos3f(0,0,5);
    mTZ.glStamp();


}

void OrientDisplay3D::DrawForeground() {
    
}

void OrientDisplay3D::OnMouseOver3D(int stackLength,const GLuint* ClosestName) {
    
}

