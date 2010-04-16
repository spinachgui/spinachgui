
#include <shared/mathtypes.hpp>
#include <iostream>
#include <cstring>
#include <sstream>
#include <cmath>
#include <boost/variant/static_visitor.hpp>
#include <boost/variant/apply_visitor.hpp>

using namespace std;
using namespace SpinXML;


//============================================================//
// Constants

const double SpinXML::hbar=6.626068e-34;
const double SpinXML::bohr_mag=9.27400915e-24;
const double SpinXML::mu0=1.25663706e-6;




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

Orientation::Orientation() : mData(EulerAngles(0,0,0)) {
}

Orientation::Type Orientation::GetType() const {
    if(get<EulerAngles>(&mData)!=NULL) {
        return EULER;
    } else if(get<AngleAxis>(&mData)!=NULL) {
        return ANGLE_AXIS;
    } else if(get<Matrix3>(&mData)!=NULL) {
        return EIGENSYSTEM;
    } else  {
        return QUATERNION;
    }
}


class GetAsMatrixVisitor : public static_visitor<Matrix3> {
public:
    Matrix3 operator()(const EulerAngles& dat) const {
        double cos_alpha = cos(dat.alpha);
        double cos_beta  = cos(dat.beta); 
        double cos_gamma = cos(dat.gamma);

        double sin_alpha = sin(dat.alpha);
        double sin_beta  = sin(dat.beta);
        double sin_gamma = sin(dat.gamma);

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
    Matrix3 operator()(const Matrix3& dat) const {
        return dat;
    }
    Matrix3 operator()(const Quaternion& dat) const {
        double W = dat.w;
        double X = dat.x;
        double Y = dat.y;
        double Z = dat.z;

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
    
        Matrix3 mat3(1 - 2 * ( yy + zz ), 2 * ( xy - zw )    , 2 * ( xz + yw ),
                     2 * ( xy + zw )    , 1 - 2 * ( xx + zz ), 2 * ( yz - xw ),
                     2 * ( xz - yw )    , 2 * ( yz + xw )    , 1 - 2 * ( xx + yy ));

        return mat3;

    }
    Matrix3 operator()(const AngleAxis& dat) const {
        double angle=dat.angle;
        double x=dat.axis.GetX();
        double y=dat.axis.GetY();
        double z=dat.axis.GetZ();

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
    
        return Matrix3(1 - 2 * ( yy + zz ), 2 * ( xy - zw )     ,  2 * ( xz + yw ),
                       2 * ( xy + zw )    ,  1 - 2 * ( xx + zz ),  2 * ( yz - xw ),
                       2 * ( xz - yw )    ,  2 * ( yz + xw )    ,   1 - 2 * ( xx + yy ));

    }
};
Matrix3 Orientation::GetAsMatrix() const {
    return apply_visitor(GetAsMatrixVisitor(),mData);
}

void Orientation::GetEuler(double* alpha,double* beta,double* gamma) const {
    *alpha = get<EulerAngles>(mData).alpha;
    *beta =  get<EulerAngles>(mData).beta;
    *gamma = get<EulerAngles>(mData).gamma;
}

void Orientation::GetAngleAxis(double* angle,Vector3* axis) const {
    *angle = get<AngleAxis>(mData).angle;
    *axis =  get<AngleAxis>(mData).axis;
}

void Orientation::GetQuaternion(double* real, double* i, double* j, double* k) const {
    *real = get<Quaternion>(mData).w;
    *i    = get<Quaternion>(mData).x;
    *j    = get<Quaternion>(mData).y;
    *k    = get<Quaternion>(mData).z;
}

void Orientation::GetEigenSystem(Vector3* XAxis,Vector3* YAxis, Vector3* ZAxis) const {
    Matrix3 mat = get<Matrix3>(mData);
    *XAxis = Vector3(mat(0,0),mat(0,1),mat(0,2));
    *YAxis = Vector3(mat(1,0),mat(1,1),mat(1,2));
    *ZAxis = Vector3(mat(2,0),mat(2,1),mat(2,2));
}

void Orientation::SetEuler(double alpha,double beta,double gamma) {
  mData=EulerAngles(alpha,beta,gamma);
}

void Orientation::SetAngleAxis(double angle,const Vector3& axis) {
  mData=AngleAxis(angle,axis);
}

void Orientation::SetQuaternion(double real, double i, double j, double k) {
  mData=Quaternion(real,i,j,k);
}

void Orientation::SetEigenSystem(const Vector3& XAxis,const Vector3& YAxis, const Vector3& ZAxis) {
  mData=Matrix3(XAxis.GetX(),XAxis.GetY(),XAxis.GetZ(),
                YAxis.GetX(),YAxis.GetY(),YAxis.GetZ(),
                ZAxis.GetX(),ZAxis.GetY(),ZAxis.GetZ());
}

class ToStringVisitor : public static_visitor<string> {
public:
    string operator()(const EulerAngles& dat) const {
        ostringstream oss;
        oss << "eu:" << dat.alpha << "," << dat.beta << "," << dat.gamma;
        return oss.str();
    }
    string operator()(const Matrix3& dat) const {
        ostringstream oss;
        oss << "es:"
            << dat(0,0) << "," << dat(0,1) << "," << dat(0,2) 
            << dat(1,0) << "," << dat(1,1) << "," << dat(1,2)
            << dat(2,0) << "," << dat(2,1) << "," << dat(2,2);
        return oss.str();
    }
    string operator()(const Quaternion& dat) const {
        ostringstream oss;
        oss << "q:"
            << dat.w << ","
            << dat.x    << ","
            << dat.y    << ","
            << dat.z;
        return oss.str();
    }
    string operator()(const AngleAxis& dat) const {
        ostringstream oss;
        oss << "aa:"
            << dat.angle << ","
            << dat.axis.GetX() << ","
            << dat.axis.GetY() << ","
            << dat.axis.GetZ();
        return oss.str();
    }
};
string Orientation::ToString() const {
    return apply_visitor(ToStringVisitor(),mData);
}

void Orientation::FromString(std::string string) {
  cerr << "Not Implimented" << endl;
}


class AsEulerVisitor : public static_visitor<Orientation> {
public:
    Orientation operator()(const EulerAngles& dat) const {
        return Orientation(dat);
    }
    Orientation operator()(const Matrix3& dat) const {
        Orientation q=Orientation(dat).GetAsQuaternion();
        return q.GetAsEuler();
    }
    Orientation operator()(const Quaternion& dat) const {
        Quaternion q(dat.w,dat.x,dat.y,dat.z);
        return Orientation(q.ToEuler());
    }
    Orientation operator()(const AngleAxis& dat) const {
        return Orientation(dat).GetAsQuaternion().GetAsEuler();
    }
};
Orientation Orientation::GetAsEuler() const {
    return apply_visitor(AsEulerVisitor(),mData);
}

class AsDCMVisitor : public static_visitor<Orientation> {
public:
    Orientation operator()(const EulerAngles& dat) const {
        return Orientation(dat).GetAsQuaternion().GetAsEigenSystem();
    }
    Orientation operator()(const Matrix3& dat) const {
        return Orientation(dat);
    }
    Orientation operator()(const Quaternion& dat) const {
        double W = dat.w;
        double X = dat.x;
        double Y = dat.y;
        double Z = dat.z;

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
        
        return Orientation(Matrix3(1 - 2 * ( yy + zz ),2 * ( xy - zw ),2 * ( xz + yw ),
                                   2 * ( xy + zw ),1 - 2 * ( xx + zz ),2 * ( yz - xw ),
                                   2 * ( xz - yw ),2 * ( yz + xw ),1 - 2 * ( xx + yy )));
    }
    Orientation operator()(const AngleAxis& dat) const {
        return Orientation(dat).GetAsQuaternion().GetAsEigenSystem();
    }
};
Orientation Orientation::GetAsEigenSystem() const {
    return apply_visitor(AsDCMVisitor(),mData);
}

class AsAngleAxisVisitor : public static_visitor<Orientation> {
public:
    Orientation operator()(const EulerAngles& dat) const {
        return Orientation(dat).GetAsQuaternion().GetAsAngleAxis();
    }
    Orientation operator()(const Matrix3& dat) const {
        return Orientation(dat).GetAsQuaternion().GetAsAngleAxis();
    }
    Orientation operator()(const Quaternion& dat) const {
        double angle=2*acos(dat.w);
        double inv_sin_a=1/(sqrt(1-dat.w*dat.w)*2);
        double vx=dat.x*inv_sin_a;
        double vy=dat.y*inv_sin_a;
        double vz=dat.z*inv_sin_a;
        return Orientation(AngleAxis(angle,Vector3(vx,vy,vz)));
    }
    Orientation operator()(const AngleAxis& dat) const {
        return Orientation(dat);
    }
};
Orientation Orientation::GetAsAngleAxis() const {
    return apply_visitor(AsAngleAxisVisitor(),mData);
}

class AsQuaternionVisitor : public static_visitor<Orientation> {
public:
    Orientation operator()(const EulerAngles& dat) const {
	Quaternion q1(dat.alpha,Vector3(0,0,1));
	Quaternion q2(dat.beta, Vector3(0,1,0));
	Quaternion q3(dat.gamma,Vector3(0,0,1));
        Quaternion q=q3*q2*q1;
        return Orientation(q);
    }
    Orientation operator()(const Matrix3& dat) const {
        double W,X,Y,Z;
        double m00=dat(0,0);
        double m01=dat(0,1);
        double m02=dat(0,2);

        double m10=dat(1,0);
        double m11=dat(1,1);
        double m12=dat(1,2);

        double m20=dat(2,0);
        double m21=dat(2,1);
        double m22=dat(2,2);

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
        return Orientation(Quaternion(W,X,Y,Z));
    }
    Orientation operator()(const Quaternion& dat) const {
        return Orientation(dat);
    }
    Orientation operator()(const AngleAxis& dat) const {
        double vx=dat.axis.GetX();
        double vy=dat.axis.GetY();
        double vz=dat.axis.GetZ();

	double sin_a=sin(dat.angle/2);
	double w=cos(dat.angle/2);
	double x=vx*sin_a;
	double y=vy*sin_a;
	double z=vz*sin_a;
        return Orientation(Quaternion(w,x,y,z));
    }
};
Orientation Orientation::GetAsQuaternion() const {
    return apply_visitor(AsQuaternionVisitor(),mData);
}

class NormaliseVisitor : public static_visitor<> {
public:
    void operator()(EulerAngles& dat) const {
        dat.Normalize();
    }
    void operator()(Matrix3& dat) const {
        dat.Othogonalise();
    }
    void operator()(Quaternion& dat) const {
        dat.Normalise();
    }
    void operator()(AngleAxis& dat) const {
        dat.Normalise();
    }
};
void Orientation::Normalise() {
    apply_visitor(NormaliseVisitor(),mData);
}

class NormalisedVisitor : public static_visitor<Orientation> {
public:
    Orientation operator()(const EulerAngles& dat) const {
        return Orientation(dat.Normalized());
    }
    Orientation operator()(const Matrix3& dat) const {
        return Orientation(dat.Othogonalised());
    }
    Orientation operator()(const Quaternion& dat) const {
        return Orientation(dat.Normalised());
    }
    Orientation operator()(const AngleAxis& dat) const {
        return Orientation(dat.Normalised());
    }
};
Orientation Orientation::Normalised() const {
    return apply_visitor(NormalisedVisitor(),mData);
}


void SpinXML::standalone_eig(C a, C b, C c,
                    C d, C e, C f,
                    C g, C h, C i,
                    C* e1,C* e2, C* e3,
                    C* vx1,C* vy1, C* vz1,
                    C* vx2,C* vy2, C* vz2,
                    C* vx3,C* vy3, C* vz3) {

    std::cout << "Finding the eigenvalues of this matrix:" << std::endl;
    std::cout << "  (" << a << " " << b << " " << c <<  std::endl
              << "   " << d << " " << e << " " << f <<  std::endl
              << "   " << g << " " << h << " " << i << ")" <<  std::endl;
    //==============================//
    // Special cases
    if(isZero(b) && isZero(c) && isZero(f) &&
       isZero(d) && isZero(h) && isZero(g)) {
        //Diagonal matrix. Life is easy
        *e1=a;
        *e2=e;
        *e3=i;
        *vx1 = a;
        *vy2 = e;
        *vz3 = i;
        *vx2 = *vy3 = *vz1 = C(0.0);
        *vx3 = *vy1 = *vz2 = C(0.0);
        return;
    }
    
    //==============================//
    // Eigenvalues

    C poly_a = C(-1,0);
    C poly_b = a+e+i;
    C poly_c = d*b+g*c+f*h-a*e-a*i-e*i;
    C poly_d = a*e*i - a*f*h - d*b*i + d*c*h + g*b*f - g*c*e;
    
    cubic characteristic_polynomial(poly_a,poly_b,poly_c,poly_d);
    C lambda1,lambda2,lambda3;
    std::cout << "==========Finding the eigenvalues==========" << std::endl;
    characteristic_polynomial.solve2(&lambda1,&lambda2,&lambda3);
    
    //==============================//
    // Eigenvectors
    std::cout << "==========Finding the eigenvectors==========" << std::endl;
    //Assume the first element of the eigenvector is 1;

    C x1 = C(1.0,0), y1, z1;
    C x2 = C(1.0,0), y2, z2;
    C x3 = C(1.0,0), y3, z3;
    overdetermined
        (b        ,c,
         e-lambda1,f,
         h        ,i-lambda1,
         lambda1-a,-d,-g,
         &y1,&z1);
    overdetermined
        (b        ,c,
         e-lambda2,f,
         h        ,i-lambda2,
         lambda2-a,-d,-g,
         &y2,&z2);
    overdetermined
        (b        ,c,
         e-lambda3,f,
         h        ,i-lambda3,
         lambda3-a,-d,-g,
         &y3,&z3);
    /*
    //First eigenvector
    C x1 = C(1.0,0.0);
    C y1 = (f*lambda1-a*f+d*c)/(c*lambda1 - e*c + b*f);
    C z1 = (h*lambda1-a*h+g*b)/(b*lambda1 - i*b + c*h);

    //Second eigenvector
    C x2 = C(1.0,0.0);
    C y2 = (f*lambda2-a*f+d*c)/(c*lambda2 - e*c + b*f);
    C z2 = (h*lambda2-a*h+g*b)/(b*lambda2 - i*b + c*h);

    //Third eigenvector
    C x3 = C(1.0,0.0);
    C y3 = (f*lambda3-a*f+d*c)/(c*lambda3 - e*c + b*f);
    C z3 = (h*lambda3-a*h+g*b)/(b*lambda3 - i*b + c*h);
    */

    std::cout << "First Eigenvector (Before norming)    : ("
              << x1 << "," << y1 << "," << z1 << ")" << std::endl;
    std::cout << "Second Eigenvector (Before norming)   : ("
              << x2 << "," << y2 << "," << z2 << ")" << std::endl;
    std::cout << "Third Eigenvector (Before norming)    : ("
              << x3 << "," << y3 << "," << z3 << ")" << std::endl;

            

    //==============================//
    // Normalisation

    C norm1=sqrt(x1*std::conj(x1) + y1*std::conj(y1) + z1*std::conj(z1));
    C norm2=sqrt(x2*std::conj(x2) + y2*std::conj(y2) + z2*std::conj(z2));
    C norm3=sqrt(x3*std::conj(x3) + y3*std::conj(y3) + z3*std::conj(z3));

    std::cout << "norm1=" << norm1 << std::endl;
    std::cout << "norm2=" << norm2 << std::endl;
    std::cout << "norm3=" << norm3 << std::endl;

    x1/=norm1;
    y1/=norm1;
    z1/=norm1;

    x2/=norm2;
    y2/=norm2;
    z2/=norm2;

    x3/=norm3;
    y3/=norm3;
    z3/=norm3;

    //==============================//
    // Output

    //We will assume real eigenvectors for now
    *vx1=x1;
    *vy1=y1;
    *vz1=z1;

    *vx2=x2;
    *vy2=y2;
    *vz2=z2;

    *vx3=x3;
    *vy3=y3;
    *vz3=z3;

    *e1 = real(lambda1);
    *e2 = real(lambda2);
    *e3 = real(lambda3);

    //==============================//
    // Sanity checks (can be removed safely)

    C 
        t11 = a*x1+b*y1+c*z1,
        t12 = d*x1+e*y1+f*z1,
        t13 = g*x1+h*y1+i*z1; 
    C 
        t21 = a*x2+b*y2+c*z2,
        t22 = d*x2+e*y2+f*z2,
        t23 = g*x2+h*y2+i*z2; 
    C 
        t31 = a*x3+b*y3+c*z3,
        t32 = d*x3+e*y3+f*z3,
        t33 = g*x3+h*y3+i*z3; 

    std::cout << "First Eigenvector           : ("
              << x1 << "," << y1 << "," << z1 << ")" << std::endl;
    std::cout << "First Eigenvector tranformed: ("
              << t11/lambda1 << "," << t12/lambda1 << "," << t13/lambda1 << ")" << std::endl;

    std::cout << "Second Eigenvector           : ("
              << x2 << "," << y2 << "," << z2 << ")" << std::endl;
    std::cout << "Second Eigenvector tranformed: ("
              << t21/lambda2 << "," << t22/lambda2 << "," << t23/lambda2 << ")" << std::endl;
    std::cout << "Third Eigenvector           : ("
              << x3 << "," << y3 << "," << z3 << ")" << std::endl;
    std::cout << "Third Eigenvector tranformed: ("
              << t31/lambda3 << "," << t32/lambda3 << "," << t33/lambda3 << ")" << std::endl;
}

