
#include <shared/mathtypes.hpp>
#include <iostream>
#include <cstring>
#include <sstream>
#include <cmath>

using namespace std;
using namespace SpinXML;

//============================================================//
// Constants

const double SpinXML::hbar=6.626068e-34;
const double SpinXML::bohr_mag=9.27400915e-24;
const double SpinXML::mu0=1.25663706e-6;


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

Matrix3 Matrix3::operator* (double s) const {
    Matrix3 result;
    result.raw[0]=raw[0]*s;
    result.raw[1]=raw[1]*s;
    result.raw[2]=raw[2]*s;
              	    	 
    result.raw[3]=raw[3]*s;
    result.raw[4]=raw[4]*s;
    result.raw[5]=raw[5]*s;
              	        
    result.raw[6]=raw[6]*s;
    result.raw[7]=raw[7]*s;
    result.raw[8]=raw[8]*s;
    return result;
}

Matrix3 Matrix3::operator* (const Matrix3& m) const {
  Matrix3 result;
  result.raw[0]=raw[0]*m.raw[0] + raw[1]*m.raw[3] + raw[2]*m.raw[6];
  result.raw[1]=raw[0]*m.raw[1] + raw[1]*m.raw[4] + raw[2]*m.raw[7];
  result.raw[2]=raw[0]*m.raw[2] + raw[1]*m.raw[5] + raw[2]*m.raw[8];

  result.raw[3]=raw[3]*m.raw[0] + raw[4]*m.raw[3] + raw[5]*m.raw[6];
  result.raw[4]=raw[3]*m.raw[1] + raw[4]*m.raw[4] + raw[5]*m.raw[7];
  result.raw[5]=raw[3]*m.raw[2] + raw[4]*m.raw[5] + raw[5]*m.raw[8];

  result.raw[6]=raw[6]*m.raw[0] + raw[7]*m.raw[3] + raw[8]*m.raw[6];
  result.raw[7]=raw[6]*m.raw[1] + raw[7]*m.raw[4] + raw[8]*m.raw[7];
  result.raw[8]=raw[6]*m.raw[2] + raw[7]*m.raw[5] + raw[8]*m.raw[8];
  return result;
}

Matrix3 Matrix3::Transpose() const {
  Matrix3 result;
  result.raw[0]=raw[0];
  result.raw[4]=raw[4];
  result.raw[8]=raw[8];

  result.raw[1]=raw[3];
  result.raw[2]=raw[6];
  result.raw[5]=raw[7];

  result.raw[3]=raw[1];
  result.raw[6]=raw[2];
  result.raw[7]=raw[5];
  return result;
}

Matrix3 Matrix3::operator*= (double s) {
  raw[0]=raw[0]*s;
  raw[1]=raw[1]*s;
  raw[2]=raw[2]*s;
       	     	 
  raw[3]=raw[3]*s;
  raw[4]=raw[4]*s;
  raw[5]=raw[5]*s;
       	         
  raw[6]=raw[6]*s;
  raw[7]=raw[7]*s;
  raw[8]=raw[8]*s;
  return *this;
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
  return *this;
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
// Matrix4
    
const double IDENTITY4[]={1.0,0.0,0.0,0.0,
                          0.0,1.0,0.0,0.0,
			  0.0,0.0,1.0,0.0,
                          0.0,0.0,0.0,1.0};


Matrix4::Matrix4() {
  memcpy(raw,IDENTITY4,16*sizeof(double));
}

Matrix4:: Matrix4(double a00,double a01,double a02,double a03,
                  double a10,double a11,double a12,double a13,
                  double a20,double a21,double a22,double a23,
                  double a30,double a31,double a32,double a33) {
    raw[0 ]=a00;  raw[1 ]=a01;   raw[2 ]=a02;  raw[3 ]=a03;
    raw[4 ]=a10;  raw[5 ]=a11;   raw[6 ]=a12;  raw[7 ]=a13;
    raw[8 ]=a20;  raw[9 ]=a21;   raw[10]=a22;  raw[11]=a23;
    raw[12]=a30;  raw[13]=a31;   raw[14]=a32;  raw[15]=a33;
}

Matrix4::Matrix4(const Matrix4& m) {
  memcpy(raw,m.raw,16*sizeof(double));
}
    
const double* Matrix4::GetRaw() const {
  return raw;
}
    
double Matrix4::Get(long column, long row) const {
  return raw[4*column+row];
}

double Matrix4::operator() (long column, long row) const {
  return raw[4*column+row];
}

Matrix4 Matrix4::operator+ (const Matrix4& m) const {
  Matrix4 result;
  result.raw[0 ]=raw[0 ]+m.raw[0 ];
  result.raw[1 ]=raw[1 ]+m.raw[1 ];
  result.raw[2 ]=raw[2 ]+m.raw[2 ];
  result.raw[3 ]=raw[3 ]+m.raw[3 ];

  result.raw[4 ]=raw[4 ]+m.raw[4 ];
  result.raw[5 ]=raw[5 ]+m.raw[5 ];
  result.raw[6 ]=raw[6 ]+m.raw[6 ];
  result.raw[7 ]=raw[7 ]+m.raw[7 ];

  result.raw[8 ]=raw[8 ]+m.raw[8 ];
  result.raw[9 ]=raw[9 ]+m.raw[9 ];
  result.raw[10]=raw[10]+m.raw[10];
  result.raw[11]=raw[11]+m.raw[11];

  result.raw[12]=raw[12]+m.raw[12];
  result.raw[13]=raw[13]+m.raw[13];
  result.raw[14]=raw[14]+m.raw[14];
  result.raw[15]=raw[15]+m.raw[15];
  return result;
}

Matrix4 Matrix4::operator* (const Matrix4& m) const {
  Matrix4 result;
  result.raw[ 0]=raw[ 0]*m.raw[ 0] + raw[ 1]*m.raw[ 4] + raw[ 2]*m.raw[ 8] + raw[ 3]*m.raw[12];
  result.raw[ 1]=raw[ 0]*m.raw[ 1] + raw[ 1]*m.raw[ 5] + raw[ 2]*m.raw[ 9] + raw[ 3]*m.raw[13];
  result.raw[ 2]=raw[ 0]*m.raw[ 2] + raw[ 1]*m.raw[ 6] + raw[ 2]*m.raw[10] + raw[ 3]*m.raw[14];
  result.raw[ 3]=raw[ 0]*m.raw[ 3] + raw[ 1]*m.raw[ 7] + raw[ 2]*m.raw[11] + raw[ 3]*m.raw[15];

  result.raw[ 4]=raw[ 4]*m.raw[ 0] + raw[ 5]*m.raw[ 4] + raw[6 ]*m.raw[ 8] + raw[7 ]*m.raw[12];
  result.raw[ 5]=raw[ 4]*m.raw[ 1] + raw[ 5]*m.raw[ 5] + raw[6 ]*m.raw[ 9] + raw[7 ]*m.raw[13];
  result.raw[ 6]=raw[ 4]*m.raw[ 2] + raw[ 5]*m.raw[ 6] + raw[6 ]*m.raw[10] + raw[7 ]*m.raw[14];
  result.raw[ 7]=raw[ 4]*m.raw[ 3] + raw[ 5]*m.raw[ 7] + raw[6 ]*m.raw[11] + raw[7 ]*m.raw[15];
                                                                                        
  result.raw[ 8]=raw[ 8]*m.raw[ 0] + raw[9 ]*m.raw[ 4] + raw[10]*m.raw[ 8] + raw[11]*m.raw[12];
  result.raw[ 9]=raw[ 8]*m.raw[ 1] + raw[9 ]*m.raw[ 5] + raw[10]*m.raw[ 9] + raw[11]*m.raw[13];
  result.raw[10]=raw[ 8]*m.raw[ 2] + raw[9 ]*m.raw[ 6] + raw[10]*m.raw[10] + raw[11]*m.raw[14];
  result.raw[11]=raw[ 8]*m.raw[ 3] + raw[9 ]*m.raw[ 7] + raw[10]*m.raw[11] + raw[11]*m.raw[15];

  result.raw[12]=raw[12]*m.raw[ 0] + raw[13]*m.raw[ 4] + raw[14]*m.raw[ 8] + raw[15]*m.raw[12];
  result.raw[13]=raw[12]*m.raw[ 1] + raw[13]*m.raw[ 5] + raw[14]*m.raw[ 9] + raw[15]*m.raw[13];
  result.raw[14]=raw[12]*m.raw[ 2] + raw[13]*m.raw[ 6] + raw[14]*m.raw[10] + raw[15]*m.raw[14];
  result.raw[15]=raw[12]*m.raw[ 3] + raw[13]*m.raw[ 7] + raw[14]*m.raw[11] + raw[15]*m.raw[15];
  return result;
}

Matrix4 Matrix4::Transpose() const {
  Matrix4 result;
  result.raw[0 ]=raw[0 ];
  result.raw[5 ]=raw[5 ];
  result.raw[10]=raw[10];
  result.raw[15]=raw[15];

  result.raw[1]=raw[4] ;    result.raw[4]=raw[1];
  result.raw[2]=raw[8];     result.raw[8]=raw[2];
  result.raw[3]=raw[12];    result.raw[12]=raw[3];

  result.raw[6]=raw[9];     result.raw[9]=raw[6];
  result.raw[7]=raw[13];    result.raw[13]=raw[7];

  result.raw[11]=raw[14];   result.raw[14]=raw[11];

  return result;
}

Matrix4& Matrix4::operator+= (const Matrix4& m) {
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

Matrix4& Matrix4::operator= (const Matrix4& m) {
  memcpy(raw,m.raw,16*sizeof(double));
  return *this;
}

void Matrix4::Set(long column,long row,double val) {
  raw[4*column+row]=val;
}

void Matrix4::Dump() const {
  cout << "Matrix4:" << endl;
  cout << " (" << raw[ 0] << " " <<  raw[ 1] << " " <<  raw[ 2] << " " <<  raw[ 3] << endl;
  cout << "  " << raw[ 4] << " " <<  raw[ 5] << " " <<  raw[ 6] << " " <<  raw[ 7] << endl;
  cout << "  " << raw[ 8] << " " <<  raw[ 9] << " " <<  raw[10] << " " <<  raw[11] << endl;
  cout << "  " << raw[12] << " " <<  raw[13] << " " <<  raw[14] << " " <<  raw[15] << ")" << endl;
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
    mData.mAngleAxis.angle = orient.mData.mAngleAxis.angle;
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
  return *this;
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
  } else if(mType==QUATERNION) {
    double W = mData.mQuaternion.real;
    double X = mData.mQuaternion.i;
    double Y = mData.mQuaternion.j;
    double Z = mData.mQuaternion.k;

    //Normalise the quaternion
    double inv_mag = 1/(X*X + Y*Y + Z*Z + W*W);
    X=X*inv_mag;
    Y=Y*inv_mag;
    Z=Z*inv_mag;
    W=W*inv_mag;
    
    double xx      = X * X;
    double xy      = X * Y;
    double xz      = X * Z;
    double xw      = X * W;

    double yy      = Y * Y;
    double yz      = Y * Z;
    double yw      = Y * W;

    double zz      = Z * Z;
    double zw      = Z * W;
    
    Matrix3 mat3(
		 1 - 2 * ( yy + zz ),
		 2 * ( xy - zw ),
		 2 * ( xz + yw ),

		 2 * ( xy + zw ),
		 1 - 2 * ( xx + zz ),
		 2 * ( yz - xw ),

		 2 * ( xz - yw ),
		 2 * ( yz + xw ),
		 1 - 2 * ( xx + yy )
		 );

    return mat3;
  } else if(mType==ANGLE_AXIS) {
    double angle=mData.mAngleAxis.angle;
    double x=mData.mAngleAxis.axis->GetX();
    double y=mData.mAngleAxis.axis->GetY();
    double z=mData.mAngleAxis.axis->GetZ();

    double sin_a = sin(angle / 2);
    double cos_a = cos(angle / 2);

    double X = x * sin_a;
    double Y = y * sin_a;
    double Z = z * sin_a;
    double W = cos_a;
    //Normalise the quaternion
    double inv_mag = 1/(X*X + Y*Y + Z*Z + W*W);
    X=X*inv_mag;
    Y=Y*inv_mag;
    Z=Z*inv_mag;
    W=W*inv_mag;
    double xx      = X * X;
    double xy      = X * Y;
    double xz      = X * Z;
    double xw      = X * W;

    double yy      = Y * Y;
    double yz      = Y * Z;
    double yw      = Y * W;

    double zz      = Z * Z;
    double zw      = Z * W;
    
    return Matrix3(
		   1 - 2 * ( yy + zz ),
		   2 * ( xy - zw ),
		   2 * ( xz + yw ),

		   2 * ( xy + zw ),
		   1 - 2 * ( xx + zz ),
		   2 * ( yz - xw ),

		   2 * ( xz - yw ),
		   2 * ( yz + xw ),
		   1 - 2 * ( xx + yy )
		   );

  } else if(mType==EULER) {
    double cos_alpha = cos(mData.mEuler.alpha);
    double cos_beta  = cos(mData.mEuler.beta); 
    double cos_gamma = cos(mData.mEuler.gamma);

    double sin_alpha = sin(mData.mEuler.alpha);
    double sin_beta  = sin(mData.mEuler.beta);
    double sin_gamma = sin(mData.mEuler.gamma);

    double a11 = cos_alpha*cos_gamma - cos_beta * sin_gamma * sin_alpha;
    double a22 =-sin_alpha*sin_gamma + cos_beta * cos_gamma * cos_alpha;
    double a33 = cos_beta;
    
    double a12 = cos_alpha*sin_gamma + cos_beta * cos_gamma * sin_alpha;
    double a21 =-sin_alpha*cos_gamma + cos_beta * sin_gamma * sin_alpha;
    
    double a13 = sin_alpha*sin_beta;
    double a31 = sin_beta *sin_gamma;
    
    double a23 = cos_alpha* sin_beta;
    double a32 =-sin_beta * cos_gamma;
    return Matrix3(a11,a12,a13,
		   a21,a22,a23,
		   a31,a32,a33);
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
  mData.mEigenSystem.XAxis=new Vector3(XAxis);
  mData.mEigenSystem.YAxis=new Vector3(YAxis);
  mData.mEigenSystem.ZAxis=new Vector3(ZAxis);
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

