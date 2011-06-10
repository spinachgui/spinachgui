
#include <3d/camera.hpp>
#include <3d/opengl.hpp>

#include <iostream>

using namespace Eigen;
using namespace std;

Camera::Camera()
    :  mZoom(0.01f) {
    mTransform = Matrix4f::Identity();
}
void Camera::SetZoom(float zoom) {
    mZoom = zoom;
}

void Camera::DeltaZoom(float deltaZoom) {
    mZoom+=deltaZoom;
    if(mZoom<0.001) {
        mZoom=0.001f;
    }
}

void Camera::Translate(float deltaX,float deltaY) {
    Affine3f t(Affine3f::Identity());
    t.translate(Vector3f(3*deltaX*mZoom,-3*deltaY*mZoom,0));
    mTransform = t.matrix() * mTransform;
}

void Camera::Rotate(float deltaX,float deltaY) {
    if(abs(deltaX) < 0.001 && abs(deltaY) < 0.001) {
        //If deltaX and deltaY are both small, it appears the
        //transform operation becomes invalid. Don't let this happen.
        return;
    }
    //The angle is proptional to the distance moved by the mouse
    float dotProduct=(deltaX*deltaX+deltaY*deltaY);
    float norm=sqrt(dotProduct);
    mTransform = Affine3f(AngleAxisf(dotProduct/300,Vector3f(deltaY/norm,deltaX/norm,0))).matrix() * mTransform;
}

void Camera::SetRotation(Eigen::Matrix3f& rotation) {
    mTransform(0,0) = rotation(0,0);
    mTransform(0,1) = rotation(0,1);
    mTransform(0,2) = rotation(0,2);
                                  
    mTransform(1,0) = rotation(1,0);
    mTransform(1,1) = rotation(1,1);
    mTransform(1,2) = rotation(1,2);
                                  
    mTransform(2,0) = rotation(2,0);
    mTransform(2,1) = rotation(2,1);
    mTransform(2,2) = rotation(2,2);
}

Eigen::Matrix3f Camera::GetRotation() {
    Matrix3f rotation;

    rotation(0,0) = mTransform(0,0);
    rotation(0,1) = mTransform(0,1);
    rotation(0,2) = mTransform(0,2);
                                  
    rotation(1,0) = mTransform(1,0);
    rotation(1,1) = mTransform(1,1);
    rotation(1,2) = mTransform(1,2);
                                  
    rotation(2,0) = mTransform(2,0);
    rotation(2,1) = mTransform(2,1);
    rotation(2,2) = mTransform(2,2);
    return rotation;
}


void Camera::Set(int width,int height) const {
    glMatrixMode(GL_PROJECTION);
    //NB: glOrtho multiplies the current matrix so glLoadIdentity is
    //vital
    glLoadIdentity();
    glOrtho(-width*mZoom ,width*mZoom,
            -height*mZoom,height*mZoom,
            -40.0, 40.0);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    gluLookAt(0,0,15,0,0,0,0,1,0);
    glMultMatrixf(mTransform.data());
}
