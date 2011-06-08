
#include <gui/OrientDisplay.hpp>


#include <3d/glmode.hpp>
#include <3d/camera.hpp>
#include <3d/displaySettings.hpp>
#include <shared/basic_math.hpp>

static GLLighting lighting;

OrientDisplay3D::OrientDisplay3D(wxWindow* parent) 
    : Display3D(parent) {
}

void OrientDisplay3D::DrawScene() {
    lighting.On();
    gluCylinder(GetQuadric(),0.5,0.5,1,8,8);
    gluSphere(GetQuadric(),0.3,30,30);
    lighting.Off();
}

void OrientDisplay3D::DrawForeground() {
    
}

void OrientDisplay3D::OnMouseOver3D(int stackLength,const GLuint* ClosestName) {
    
}

