
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <Eigen/Dense>
#include <Eigen/Geometry>

class Camera {
public:
	Camera();
	void SetZoom(float zoom);
	void DeltaZoom(float delta);
	void Rotate(float deltaX,float deltaY);
	void Set(int width,int height) const;
private:
	//The camera starts at the origin
	Eigen::Vector3f mEyeLocation;
	Eigen::Vector3f mEyeTarget;

	float mZoom;
};

#endif
