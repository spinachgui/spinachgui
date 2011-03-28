
#include <3d/camera.hpp>
#include <3d/opengl.hpp>

#include <iostream>

using namespace Eigen;
using namespace std;

Camera::Camera()
	: mEyeLocation(0,0,5),
	  mEyeTarget(0,0,0),
	  mUp(0,1,0),
	  mZoom(0.001) {
}
void Camera::SetZoom(float zoom) {
	mZoom = zoom;
}

void Camera::DeltaZoom(float deltaZoom) {
    mZoom+=deltaZoom;
    if(mZoom<0.001) {
        mZoom=0.001;
    }
}

void Camera::Translate(float deltaX,float deltaY) {
	mEyeLocation += Vector3f(0,deltaY*0.003,0);
	mEyeTarget   += Vector3f(0,deltaY*0.003,0);
}

void Camera::Rotate(float deltaX,float deltaY) {
    cout << "Rotate(" << deltaX << "," << deltaY << ")" << endl;
    //The angle is proptional to the distance moved by the mouse
    float dotProduct=(deltaX*deltaX+deltaY*deltaY);
    float norm=sqrt(dotProduct);

    //The axis is perpendicular to the eye-object vector from the
    //world space point and is perpendicular to the direction the
    //mouse is moving when the start end end coordinates have been
    //translated into world coordinates

	//We can find this second vector by transforming mEyeLocation into
	//window coordinates, adding the vector (deltaX,deltaY,0) and
	//transforming back
    GLint viewport[4];
    GLdouble mvmatrix[] = {1,0,0,0,
						   0,1,0,0,
						   0,0,1,0,
						   0,0,0,1};
	GLdouble projmatrix[16];
    glGetIntegerv(GL_VIEWPORT,viewport);
    glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);

    GLdouble eyeX = mEyeLocation.x(), eyeY = mEyeLocation.y(),eyeZ = mEyeLocation.z();
    GLdouble winEyeX, winEyeY, winEyeZ;
    GLdouble nextEyeX,nextEyeY,nextEyeZ;
    //gluUnProject(eyeX,eyeY,eyeZ,mvmatrix,projmatrix,viewport,&winEyeX, &winEyeY ,&winEyeZ);
    //gluProject(winEyeX - deltaX, winEyeY + deltaY,winEyeZ,mvmatrix,projmatrix,viewport,&nextEyeX,&nextEyeY,&nextEyeZ);
	gluProject(-deltaX, deltaY, 0,mvmatrix,projmatrix,viewport,&nextEyeX,&nextEyeY,&nextEyeZ);	

    Vector3f axis = Vector3f(nextEyeX-winEyeX,nextEyeY-winEyeY,nextEyeZ-winEyeZ).cross(mEyeLocation-mEyeTarget);

	Matrix3f rotation = AngleAxisf(norm*0.01,axis.normalized()).toRotationMatrix();
    mEyeLocation = rotation * mEyeLocation;

	cout << mUp.normalized().dot(Vector3f(nextEyeX-winEyeX,nextEyeY-winEyeY,nextEyeZ-winEyeZ).normalized()) << endl;
	
    mUp = rotation * mUp;

	cout << "===" << endl << mEyeLocation << endl << "===" << endl << mUp << endl;
}
void Camera::Set(int width,int height) const {
    glMatrixMode(GL_PROJECTION);
    //NB: glOrtho multiplies the current matrix so glLoadIdentity is
    //vital
    glLoadIdentity();
    glOrtho(-width*mZoom ,width*mZoom,
			-height*mZoom,height*mZoom,
			1.0, 40.0);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    gluLookAt(mEyeLocation.x(),mEyeLocation.y(),mEyeLocation.z(),
			  mEyeTarget.x()  ,mEyeTarget.y()  ,mEyeTarget.z(),
			  mUp.x()         ,mUp.y()         ,mUp.z());
}
