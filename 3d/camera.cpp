
#include <3d/camera.hpp>
#include <3d/opengl.hpp>

Camera::Camera()
	: mEyeLocation(5,5,5), mEyeTarget(0,0,0),mZoom(0.001) {
}
void Camera::SetZoom(double zoom) {

}

void Camera::Rotate(float deltaX,float deltaY) {
	float dotProduct=(deltaX*deltaX+deltaY*deltaY);
	float norm=sqrt(dotProduct);
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
			  mEyeTarget.x(),mEyeTarget.y(),mEyeTarget.z(),
			  0,1,0);
}
