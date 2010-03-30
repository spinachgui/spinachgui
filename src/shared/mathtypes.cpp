
#include <shared/mathtypes.hpp>
#include <iostream>
#include <cstring>
#include <sstream>
#include <cmath>

using namespace std;
using namespace SpinXML;

#define PI 3.141592653589793238462643

//============================================================//
// Constants

const double SpinXML::hbar=6.626068e-34;
const double SpinXML::bohr_mag=9.27400915e-24;
const double SpinXML::mu0=1.25663706e-6;

//============================================================//
// Simple Euler Angles Type


struct EulerAngles {
    double alpha,beta,gamma;
    EulerAngles(double _alpha,double _beta, double _gamma) 
        : alpha(_alpha),beta(_beta),gamma(_gamma) {

    }
};

//============================================================//
// Simple Quaternion Type


struct Quaternion {
    double w,x,y,z;
    Quaternion(){}
    Quaternion(double _w,double _x, double _y, double _z) 
        : w(_w),x(_x),y(_y),z(_z){
    }
    Quaternion(double angle,const Vector3& axis) {
        SetAngleAxis(angle,axis);
    }
    Quaternion operator*(const Quaternion& q2) {
	Quaternion res;
	double w1=w; double w2=q2.w;
	double x1=x; double x2=q2.x;
	double y1=y; double y2=q2.y;
	double z1=z; double z2=q2.z;
	res.w=w1*w2 - x1*x2 - y1*y2 - z1*z2;
	res.x=w1*x2 + x1*w2 + y1*z2 - z1*y2;
	res.y=w1*y2 - x1*z2 + y1*w2 + z1*x2;
	res.z=w1*z2 + x1*y2 - y1*x2 + z1*w2;	

	return res;
    }
    Vector3 Transform(Vector3 v) {
        Quaternion v_as_q;
        v_as_q.w=0;
        v_as_q.x=v.GetX();
        v_as_q.y=v.GetY();
        v_as_q.z=v.GetZ();

        v_as_q=(*this)*v_as_q*Quaternion(w,-x,-y,-z);

        Vector3 ret;
        ret.SetX(v_as_q.x);
        ret.SetY(v_as_q.y);
        ret.SetZ(v_as_q.z);
        return ret;
    }
    void SetAngleAxis(double a,const Vector3& v) {
        double _x=v.GetX();
        double _y=v.GetY();
        double _z=v.GetZ();

	double norm = sqrt(_x*_x+_y*_y+_z*_z);
	_x/=norm;
	_y/=norm;
	_z/=norm;

	double sin_a=sin(a/2);
	w=cos(a/2);
	x=_x*sin_a;
	y=_y*sin_a;
	z=_z*sin_a;
    }
    double norm() {
	return sqrt(w*w+x*x+y*y+z*z);
    }
    Quaternion Conjugulate() {
        return Quaternion(w,-x,-y,-z);
    }
    EulerAngles ToEuler() {
        Vector3 z_axis=Vector3(0,0,1);
        Vector3 x_axis=Vector3(1,0,0);
        z_axis=Transform(z_axis);
        x_axis=Transform(x_axis);
    
        double gamma=atan2(z_axis.GetY(),z_axis.GetX());
        double beta=atan2(sqrt(z_axis.GetX()*z_axis.GetX() + z_axis.GetY()*z_axis.GetY()).si,z_axis.GetZ().si);

        //Use γ and β to rotate V2 back onto the point 0,0,1
        Quaternion gammaTwist,betaTwist;
        betaTwist.SetAngleAxis(-beta,Vector3(0,1,0));
        gammaTwist.SetAngleAxis(-gamma,Vector3(0,0,1));

        x_axis=(betaTwist*gammaTwist).Transform(x_axis);

        double alpha = atan2(x_axis.GetY(),x_axis.GetX());
        if(alpha < 0 || alpha >= 2*PI)  alpha = alpha-2*PI*floor(alpha/(2*PI));
        if(alpha >= 2*PI) alpha = 0;
        if(beta < 0 || beta >= PI)    beta = beta-  PI*floor(beta/PI);
        if(gamma < 0 || gamma >= 2*PI)  gamma = gamma-2*PI*floor(gamma/(2*PI));
        if(gamma >= 2*PI) gamma = 0;
	return EulerAngles(alpha,beta,gamma);
    }

};




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

Matrix4::Matrix4(const Matrix3&m) {
    const dreal<double,_unitless>* _raw=m.GetRaw();
    raw[0 ]=_raw[0].si; raw[1 ]=_raw[1].si; raw[2 ]=_raw[2].si; raw[3 ]=0;
    raw[4 ]=_raw[3].si; raw[5 ]=_raw[4].si; raw[6 ]=_raw[5].si; raw[7 ]=0;
    raw[8 ]=_raw[6].si; raw[9 ]=_raw[7].si; raw[10]=_raw[8].si; raw[11]=0;
    raw[12]=0;       raw[13]=0;       raw[14]=0;       raw[15]=1;
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
    mData.mAngleAxis.angle = mData.mAngleAxis.angle;
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



Orientation Orientation::GetAsEuler() const {
    switch(mType) {
    case EULER: {
        return Orientation(*this);
    }
    case ANGLE_AXIS: {
        Orientation q=GetAsQuaternion();
        return q.GetAsEuler();
    }
    case QUATERNION: {
        Quaternion q(mData.mQuaternion.real,
                     mData.mQuaternion.i,
                     mData.mQuaternion.j,
                     mData.mQuaternion.k);
        EulerAngles ea = q.ToEuler();
        Orientation o;
        o.SetEuler(ea.alpha,ea.beta,ea.gamma);
        return o;
    }
    case EIGENSYSTEM: {
        Orientation q=GetAsQuaternion();
        return q.GetAsEuler();
    }
    case UNDEFINED:
    default:
        cerr << "Warning, cannot convert an undefined orientation" << endl;
        return Orientation();
    }
}

Orientation Orientation::GetAsEigenSystem() const {
    switch(mType) {
    case EULER: {
        Orientation o = GetAsQuaternion();
        return o.GetAsEigenSystem();
    }
    case ANGLE_AXIS: {
        Orientation o = GetAsQuaternion();
        return o.GetAsEigenSystem();
    }
    case QUATERNION: {
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
    
        double xx = X * X;
        double xy = X * Y;
        double xz = X * Z;
        double xw = X * W;

        double yy = Y * Y;
        double yz = Y * Z;
        double yw = Y * W;

        double zz = Z * Z;
        double zw = Z * W;
        
        Orientation o;
        o.SetEigenSystem(Vector3(1 - 2 * ( yy + zz ),2 * ( xy - zw ),2 * ( xz + yw )),
                         Vector3(2 * ( xy + zw ),1 - 2 * ( xx + zz ),2 * ( yz - xw )),
                         Vector3(2 * ( xz - yw ),2 * ( yz + xw ),1 - 2 * ( xx + yy )));
        return o;
    }
    case EIGENSYSTEM:
        return Orientation(*this);
        break;
    case UNDEFINED:
    default:
        cerr << "Warning, cannot convert an undefined orientation" << endl;
            return Orientation();
    }
}

Orientation Orientation::GetAsAngleAxis() const {
    switch(mType) {
    case EULER: {
        Orientation o=GetAsQuaternion();
        return o.GetAsAngleAxis();
    }
    case ANGLE_AXIS: {
        return Orientation(*this);
    }
    case QUATERNION: {
        double angle=2*acos(mData.mQuaternion.real);
        double inv_sin_a=1/(sqrt(1-mData.mQuaternion.real*mData.mQuaternion.real)*2);
        double vx=mData.mQuaternion.i*inv_sin_a;
        double vy=mData.mQuaternion.j*inv_sin_a;
        double vz=mData.mQuaternion.k*inv_sin_a;
        Orientation o;
        o.SetAngleAxis(angle,Vector3(vx,vy,vz));
        return o;
    }
    case EIGENSYSTEM: {
        Orientation o=GetAsQuaternion();
        return o.GetAsAngleAxis();
    }
    case UNDEFINED:
    default: {
        cerr << "Warning, cannot convert an undefined orientation" << endl;
        return Orientation();
    }
    }
}

Orientation Orientation::GetAsQuaternion() const {
    switch(mType) {
    case EULER: {
	Quaternion q1(mData.mEuler.alpha,Vector3(0,0,1));
	Quaternion q2(mData.mEuler.beta, Vector3(0,1,0));
	Quaternion q3(mData.mEuler.gamma,Vector3(0,0,1));
        Quaternion q=q3*q2*q1;
        Orientation o;
        o.SetQuaternion(q.w,q.x,q.y,q.z);
        return o;
    }
    case ANGLE_AXIS: {
	double sin_a=sin(mData.mAngleAxis.angle/2);
	double w=cos(mData.mAngleAxis.angle/2);
	double x=mData.mAngleAxis.axis->GetX()*sin_a;
	double y=mData.mAngleAxis.axis->GetY()*sin_a;
	double z=mData.mAngleAxis.axis->GetZ()*sin_a;
        Orientation o;
        o.SetQuaternion(w,x,y,z);
        return o;
    }
    case QUATERNION: {
        return Orientation(*this);
    }
    case EIGENSYSTEM: {
        double W,X,Y,Z;
        double m00=mData.mEigenSystem.XAxis->GetX();
        double m01=mData.mEigenSystem.XAxis->GetY();
        double m02=mData.mEigenSystem.XAxis->GetZ();

        double m10=mData.mEigenSystem.YAxis->GetX();
        double m11=mData.mEigenSystem.YAxis->GetY();
        double m12=mData.mEigenSystem.YAxis->GetZ();

        double m20=mData.mEigenSystem.ZAxis->GetX();
        double m21=mData.mEigenSystem.ZAxis->GetY();
        double m22=mData.mEigenSystem.ZAxis->GetZ();

        double trace=m00+m11+m22 + 1;
        if(trace > 0) {
            double S = 0.5 / sqrt(trace);
            W = 0.25 / S;
            X = (m21 - m12)*S;
            Y = (m02 - m20)*S;
            Z = (m10 - m01)*S;
        } else {
            if(m00 > m11 && m00 > m22) {
                double S  = sqrt(1.0 + m00 - m11 - m22)*2;
                X = 0.5 / S;
                Y = (m01 + m10)/S;
                Z = (m02 + m21)/S;
                W = (m12 + m21)/S;
            } else if(m11 > m22) {
                double S  = sqrt(1.0 + m11 - m22 - m00)*2;
                X = (m01 + m10)/S;
                Y = 0.5 / S;
                Z = (m12 + m21)/S;
                W = (m02 + m20)/S;
            } else {
                double S  = sqrt(1.0 + m22 - m11 - m00)*2;
                X = (m02 + m20)/S;
                Y = (m12 + m21)/S;
                Z = 0.5 / S;
                W = (m01 + m10)/S;
            }
        }
        Orientation o;
        o.SetQuaternion(W,X,Y,Z);
        return o;
    }
    case UNDEFINED:
    default: {
        cerr << "Warning, cannot convert an undefined orientation" << endl;
        return Orientation();
    }
    }
}

