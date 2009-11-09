
#include <shared/mathtypes.hpp>
#include <iostream>
#include <cstring>
#include <sstream>

using namespace std;
using namespace SpinXML;

//==============================================================================//
// Vector3

Vector3::Vector3() {
    
}

Vector3::Vector3(double _x,double _y,double _z) : x(_x),y(_y),z(_z) {

}

Vector3::Vector3(const Vector3& v) :  x(v.x),y(v.y),z(v.z) {

}

double Vector3::GetX() const {
  return x;
}
double Vector3::GetY() const {
  return y;
}

double Vector3::GetZ() const {
  return z;
}

void Vector3::SetX(double _x) {
  x=_x;
  return;
}

void Vector3::SetY(double _y) {
  y=_y;
  return;
}

void Vector3::SetZ(double _z) {
  z=_z;
  return;
}

void Vector3::GetCoordinates(double* _x,double* _y, double* _z) const {
  *_x=x;
  *_y=y;
  *_z=z;
  return;
}

void Vector3::SetCoordinates(double _x,double _y, double _z) {
  x=_x;
  y=_y;
  z=_z;
  return;
}


//==============================================================================//
// Matrix3
    
const double IDENTITY3[]={1.0,0.0,0.0,
	  		0.0,1.0,0.0,
			  0.0,0.0,1.0};


Matrix3::Matrix3() {
  memcpy(raw,IDENTITY3,9*sizeof(double));
}

Matrix3::Matrix3(double a00,double a01,double a02,double a10,double a11,double a12,double a20,double a21,double a22) {
  raw[0]=a00;   raw[1]=a01;   raw[2]=a02;
  raw[3]=a10;   raw[4]=a11;   raw[5]=a12;
  raw[6]=a20;   raw[7]=a21;   raw[8]=a22;
}

Matrix3::Matrix3(const Matrix3& m) {
  memcpy(raw,m.raw,9*sizeof(double));
}
    
const double* Matrix3::GetRaw() const {
  return raw;
}
    
double Matrix3::Get(long column, long row) const {
  return raw[3*column+row];
}

double Matrix3::operator() (long column, long row) const {
  return raw[3*column+row];
}

Matrix3 Matrix3::operator+ (const Matrix3& m) const {
  Matrix3 result;
  result.raw[0]=raw[0]+m.raw[0];
  result.raw[1]=raw[1]+m.raw[1];
  result.raw[2]=raw[2]+m.raw[2];
	      	     	       
  result.raw[3]=raw[3]+m.raw[3];
  result.raw[4]=raw[4]+m.raw[4];
  result.raw[5]=raw[5]+m.raw[5];
	      	     	       
  result.raw[6]=raw[6]+m.raw[6];
  result.raw[7]=raw[7]+m.raw[7];
  result.raw[8]=raw[8]+m.raw[8];
  return result;
}

Matrix3& Matrix3::operator+= (const Matrix3& m) {
  raw[0]=raw[0]+m.raw[0];
  raw[1]=raw[1]+m.raw[1];
  raw[2]=raw[2]+m.raw[2];
       	     	       
  raw[3]=raw[3]+m.raw[3];
  raw[4]=raw[4]+m.raw[4];
  raw[5]=raw[5]+m.raw[5];
       	     	       
  raw[6]=raw[6]+m.raw[6];
  raw[7]=raw[7]+m.raw[7];
  raw[8]=raw[8]+m.raw[8];
  return *this;
}

Matrix3& Matrix3::operator= (const Matrix3& m) {
  memcpy(raw,m.raw,9*sizeof(double));
}

void Matrix3::Set(long column,long row,double val) {
  raw[3*column+row]=val;
}

void Matrix3::Dump() const {
  cout << "Matrix3:" << endl;
  cout << " (" << raw[0] << " " <<  raw[1] << " " <<  raw[2] << endl;
  cout << "  " << raw[3] << " " <<  raw[4] << " " <<  raw[5] << endl;
  cout << "  " << raw[6] << " " <<  raw[7] << " " <<  raw[8] << ")" << endl;
}

//==============================================================================//
// Orientation

Orientation::Orientation() : mType(UNDEFINED) {
}

Orientation::Orientation(const Orientation& orient) :
  mType(orient.mType) {
  if(mType==ANGLE_AXIS) {
    mData.mAngleAxis.axis = new Vector3(*orient.mData.mAngleAxis.axis);
  } else if(mType==EIGENSYSTEM) {
    mData.mEigenSystem.XAxis = new Vector3(*orient.mData.mEigenSystem.XAxis);
    mData.mEigenSystem.YAxis = new Vector3(*orient.mData.mEigenSystem.YAxis);
    mData.mEigenSystem.ZAxis = new Vector3(*orient.mData.mEigenSystem.ZAxis);
  } else if(mType==QUATERNION) {
    mData.mQuaternion.real = orient.mData.mQuaternion.real;
    mData.mQuaternion.i    = orient.mData.mQuaternion.i;
    mData.mQuaternion.j    = orient.mData.mQuaternion.j;
    mData.mQuaternion.k    = orient.mData.mQuaternion.k;
  } else if(mType==EULER) {
    mData.mEuler.alpha =     orient.mData.mEuler.alpha;
    mData.mEuler.beta  =     orient.mData.mEuler.beta;
    mData.mEuler.gamma =     orient.mData.mEuler.gamma;
  }
}

const Orientation& Orientation::operator=(const Orientation& orient) {
  mType=orient.mType;
  if(mType==ANGLE_AXIS) {
    mData.mAngleAxis.axis = new Vector3(*orient.mData.mAngleAxis.axis);
  } else if(mType==EIGENSYSTEM) {
    mData.mEigenSystem.XAxis = new Vector3(*orient.mData.mEigenSystem.XAxis);
    mData.mEigenSystem.YAxis = new Vector3(*orient.mData.mEigenSystem.YAxis);
    mData.mEigenSystem.ZAxis = new Vector3(*orient.mData.mEigenSystem.ZAxis);
  } else if(mType==QUATERNION) {
    mData.mQuaternion.real = orient.mData.mQuaternion.real;
    mData.mQuaternion.i    = orient.mData.mQuaternion.i;
    mData.mQuaternion.j    = orient.mData.mQuaternion.j;
    mData.mQuaternion.k    = orient.mData.mQuaternion.k;
  } else if(mType==EULER) {
    mData.mEuler.alpha =     orient.mData.mEuler.alpha;
    mData.mEuler.beta  =     orient.mData.mEuler.beta;
    mData.mEuler.gamma =     orient.mData.mEuler.gamma;
  }

}


Orientation::~Orientation() {
  Clear();
}

void Orientation::Clear() {
  if(mType==ANGLE_AXIS && mData.mAngleAxis.axis != NULL) {
    delete mData.mAngleAxis.axis;
  } else if(mType==EIGENSYSTEM) {
    delete mData.mEigenSystem.XAxis;
    delete mData.mEigenSystem.YAxis;
    delete mData.mEigenSystem.ZAxis;
  }
  mType=UNDEFINED;
}
 
Orientation::Type Orientation::GetType() const {
  return mType;
}

Matrix3 Orientation::GetAsMatrix() const {
  if(mType==EIGENSYSTEM) {
    const Vector3* xa=mData.mEigenSystem.XAxis;
    const Vector3* ya=mData.mEigenSystem.YAxis;
    const Vector3* za=mData.mEigenSystem.ZAxis;

    return Matrix3(xa->GetX(),ya->GetX(),za->GetX(),
		   xa->GetY(),ya->GetY(),za->GetY(),
		   xa->GetZ(),ya->GetZ(),za->GetZ());
  }
  return Matrix3(1,0,0,0,1,0,0,0,1);
}

void Orientation::GetEuler(double* alpha,double* beta,double* gamma) const {
  *alpha = mData.mEuler.alpha;
  *beta = mData.mEuler.beta;
  *gamma = mData.mEuler.gamma;
  return;
}

void Orientation::GetAngleAxis(double* angle,Vector3* axis) const {
  *angle = mData.mAngleAxis.angle;
  *axis = *mData.mAngleAxis.axis;
  return;
}

void Orientation::GetQuaternion(double* real, double* i, double* j, double* k) const {
  *real = mData.mQuaternion.real;
  *i = mData.mQuaternion.i;
  *j = mData.mQuaternion.j;
  *k = mData.mQuaternion.k;
  return;
}

void Orientation::GetEigenSystem(Vector3* XAxis,Vector3* YAxis, Vector3* ZAxis) const {
  *XAxis = *mData.mEigenSystem.XAxis;
  *YAxis = *mData.mEigenSystem.YAxis;
  *ZAxis = *mData.mEigenSystem.ZAxis;
  return;
}

void Orientation::SetEuler(double alpha,double beta,double gamma) {
  Clear();
  mType=EULER;
  mData.mEuler.alpha=alpha;
  mData.mEuler.beta =beta;
  mData.mEuler.gamma=gamma;
  return;
}

void Orientation::SetAngleAxis(double angle,const Vector3& axis) {
  Clear();
  mType=ANGLE_AXIS;
  mData.mAngleAxis.angle=angle;
  mData.mAngleAxis.axis=new Vector3(axis);
  return;

}

void Orientation::SetQuaternion(double real, double i, double j, double k) {
  Clear();
  mType=QUATERNION;
  mData.mQuaternion.real=real;
  mData.mQuaternion.i=i;
  mData.mQuaternion.j=j;
  mData.mQuaternion.k=k;
  return;
}

void Orientation::SetEigenSystem(const Vector3& XAxis,const Vector3& YAxis, const Vector3& ZAxis) {
  Clear();
  mType=EIGENSYSTEM;
  mData.mEigenSystem.XAxis=new Vector3(1.0,0.0,0.0);
  mData.mEigenSystem.YAxis=new Vector3(0.0,1.0,0.0);
  mData.mEigenSystem.ZAxis=new Vector3(0.0,0.0,1.0);
  return;
}

string Orientation::ToString() const {
  ostringstream oss;
  switch(mType) {
  case EULER:
    oss << "eu:" << mData.mEuler.alpha << "," << mData.mEuler.beta << "," << mData.mEuler.gamma;
    break;
  case ANGLE_AXIS:
    oss << "aa:"
	<< mData.mAngleAxis.angle << ","
	<< mData.mAngleAxis.axis->GetX() << ","
	<< mData.mAngleAxis.axis->GetY() << ","
	<< mData.mAngleAxis.axis->GetZ();
    break;
  case QUATERNION:
    oss << "q:"
	<< mData.mQuaternion.real << ","
	<< mData.mQuaternion.i    << ","
	<< mData.mQuaternion.j    << ","
	<< mData.mQuaternion.k;
    break;
  case EIGENSYSTEM:
    oss << "es:"
	<< mData.mEigenSystem.XAxis->GetX() << "," << mData.mEigenSystem.XAxis->GetY() << "," <<  mData.mEigenSystem.XAxis->GetZ() 
	<< mData.mEigenSystem.YAxis->GetX() << "," << mData.mEigenSystem.YAxis->GetY() << "," <<  mData.mEigenSystem.YAxis->GetZ() 
	<< mData.mEigenSystem.ZAxis->GetX() << "," << mData.mEigenSystem.ZAxis->GetY() << "," <<  mData.mEigenSystem.ZAxis->GetZ();
    break;
  case UNDEFINED:
    oss << "undefined";
    break;
  }
  return oss.str();
}

void Orientation::FromString(std::string string) {
  cerr << "Not Implimented" << endl;
}

