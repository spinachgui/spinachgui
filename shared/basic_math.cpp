
#include <shared/basic_math.hpp>

using namespace SpinXML;

//============================================================//
//Helper functions

Matrix3d SpinXML::MakeMatrix3d(double a00, double a01, double a02,
					  double a10, double a11, double a12,
					  double a20, double a21, double a22) {
	Matrix3d m;
	m(0,0)=a00;		m(0,1)=a01;		m(0,2)=a02;
	m(1,0)=a10;		m(1,1)=a11;		m(1,2)=a12;
	m(2,0)=a20;		m(2,1)=a21;		m(2,2)=a22;

	return m;
}


Matrix3d SpinXML::MakeMatrix3d(Matrix4d mat) {
	Matrix3d m;
	m(0,0)=mat(0,0);	m(0,1)=mat(0,1);	m(0,2)=mat(0,2);
	m(1,0)=mat(1,0);	m(1,1)=mat(1,1);	m(1,2)=mat(1,2);
	m(2,0)=mat(2,0);	m(2,1)=mat(2,1);	m(2,2)=mat(2,2);
	
	return m;
}


Vector4d SpinXML::MakeVector4d(Vector3d v) {
	return Vector4d(v.x(),v.y(),v.z(),1);
}

Vector3d SpinXML::MakeVector3d(Vector4d v) {
	return Vector3d(v.x(),v.y(),v.z());
}

