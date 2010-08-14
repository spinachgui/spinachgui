
#include <shared/orientation.hpp>
#include <iostream>
#include <cstring>
#include <sstream>
#include <cmath>
#include <boost/variant/static_visitor.hpp>
#include <boost/variant/apply_visitor.hpp>

using namespace std;
using namespace Eigen;
using namespace SpinXML;


//============================================================//
// Constants

const double SpinXML::hbar=6.626068e-34;
const double SpinXML::bohr_mag=9.27400915e-24;
const double SpinXML::mu0=1.25663706e-6;

Matrix3d SpinXML::MakeMatrix3d(double a00, double a01, double a02,
					  double a10, double a11, double a12,
					  double a20, double a21, double a22) {
	Matrix3d m;
	m(0,0)=a00;		m(0,1)=a01;		m(0,2)=a02;
	m(1,0)=a10;		m(1,1)=a11;		m(1,2)=a12;
	m(2,0)=a20;		m(2,1)=a21;		m(2,2)=a22;

	return m;
}



//==============================================================================//
// Orientation

Orientation::Type Orientation::GetType() const {
    if(get<EulerAngles>(&mData)!=NULL) {
        return EULER;
    } else if(get<AngleAxisd>(&mData)!=NULL) {
        return ANGLE_AXIS;
    } else if(get<Matrix3d>(&mData)!=NULL) {
        return EIGENSYSTEM;
    } else  {
        return QUATERNION;
    }
}


class GetAsMatrixVisitor : public static_visitor<Matrix3d> {
public:
    Matrix3d operator()(const EulerAngles& dat) const {
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
        return MakeMatrix3d(a11,a12,a13,
							a21,a22,a23,
							a31,a32,a33);
    }
    Matrix3d operator()(const Matrix3d& dat) const {
        return dat;
    }
    Matrix3d operator()(const Quaterniond& dat) const {
        double W = dat.w();
        double X = dat.x();
        double Y = dat.y();
        double Z = dat.z();

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
    
        return MakeMatrix3d(1 - 2 * ( yy + zz ), 2 * ( xy - zw )    , 2 * ( xz + yw ),
							2 * ( xy + zw )    , 1 - 2 * ( xx + zz ), 2 * ( yz - xw ),
							2 * ( xz - yw )    , 2 * ( yz + xw )    , 1 - 2 * ( xx + yy ));
    }
    Matrix3d operator()(const AngleAxisd& dat) const {
        return dat.toRotationMatrix();
    }
};
Matrix3d Orientation::GetAsMatrix() const {
    Orientation normalized=Normalized();
    return apply_visitor(GetAsMatrixVisitor(),normalized.mData);
}

void Orientation::GetEuler(double* alpha,double* beta,double* gamma) const {
    *alpha = get<EulerAngles>(mData).alpha;
    *beta =  get<EulerAngles>(mData).beta;
    *gamma = get<EulerAngles>(mData).gamma;
}

void Orientation::GetAngleAxis(double* angle,Vector3d* axis) const {
    *angle = get<AngleAxisd>(mData).angle();
    *axis =  get<AngleAxisd>(mData).axis();
}

void Orientation::GetQuaternion(double* real, double* i, double* j, double* k) const {
    *real = get<Quaterniond>(mData).w();
    *i    = get<Quaterniond>(mData).x();
    *j    = get<Quaterniond>(mData).y();
    *k    = get<Quaterniond>(mData).z();
}

void Orientation::GetEigenSystem(Vector3d* XAxis,Vector3d* YAxis, Vector3d* ZAxis) const {
    Matrix3d mat = get<Matrix3d>(mData);
    *XAxis = Vector3d(mat(0,0),mat(0,1),mat(0,2));
    *YAxis = Vector3d(mat(1,0),mat(1,1),mat(1,2));
    *ZAxis = Vector3d(mat(2,0),mat(2,1),mat(2,2));
}


class ToStringVisitor : public static_visitor<string> {
public:
    string operator()(const EulerAngles& dat) const {
        ostringstream oss;
        oss << "eu:" << dat.alpha << "," << dat.beta << "," << dat.gamma;
        return oss.str();
    }
    string operator()(const Matrix3d& dat) const {
        ostringstream oss;
        oss << "es:"
            << dat << endl;
        return oss.str();
    }
    string operator()(const Quaterniond& dat) const {
        ostringstream oss;
        oss << "q:"
            << dat.w() << ","
            << dat.x() << ","
            << dat.y() << ","
            << dat.z();
        return oss.str();
    }
    string operator()(const AngleAxisd& dat) const {
        ostringstream oss;
        oss << "aa:"
            << dat.angle() << ","
            << dat.axis().x() << ","
            << dat.axis().y() << ","
            << dat.axis().z();
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
    Orientation operator()(const Matrix3d& dat) const {
        Orientation q=Orientation(dat).GetAsQuaternion();
        return q.GetAsEuler();
    }
    Orientation operator()(const Quaterniond& dat) const {
        Quaterniond q(dat.w(),dat.x(),dat.y(),dat.z());
        return Orientation(EulerAngles(0,0,0)); //TODO
    }
    Orientation operator()(const AngleAxisd& dat) const {
        return Orientation(dat).GetAsQuaternion().GetAsEuler();
    }
};
Orientation Orientation::GetAsEuler() const {
    Orientation normalised=Normalized();
    return apply_visitor(AsEulerVisitor(),normalised.mData);
}

class AsDCMVisitor : public static_visitor<Orientation> {
public:
    Orientation operator()(const EulerAngles& dat) const {
        return Orientation(dat).GetAsQuaternion().GetAsEigenSystem();
    }
    Orientation operator()(const Matrix3d& dat) const {
        return Orientation(dat);
    }
    Orientation operator()(const Quaterniond& dat) const {
		return Orientation(dat.toRotationMatrix());
    }
    Orientation operator()(const AngleAxisd& dat) const {
        return Orientation(dat).GetAsQuaternion().GetAsEigenSystem();
    }
};
Orientation Orientation::GetAsEigenSystem() const {
    Orientation o=Normalized();
    return apply_visitor(AsDCMVisitor(),o.mData);
}

class AsAngleAxisVisitor : public static_visitor<Orientation> {
public:
    Orientation operator()(const EulerAngles& dat) const {
        return Orientation(dat).GetAsQuaternion().GetAsAngleAxis();
    }
    Orientation operator()(const Matrix3d& dat) const {
        return Orientation(dat).GetAsQuaternion().GetAsAngleAxis();
    }
    Orientation operator()(const Quaterniond& dat) const {
        double angle=2*acos(dat.w());
        if(angle == 0.0 || angle == -0.0) {
            //Singularty at the identity
            return Orientation(AngleAxisd(angle,Vector3d(0,0,0)));
        }
        double inv_sin_a=1/(sqrt(1-dat.w()*dat.w())*2);
        double vx=dat.x()*inv_sin_a;
        double vy=dat.y()*inv_sin_a;
        double vz=dat.z()*inv_sin_a;
        return Orientation(AngleAxisd(angle,Vector3d(vx,vy,vz)));
    }
    Orientation operator()(const AngleAxisd& dat) const {
        return Orientation(dat);
    }
};
Orientation Orientation::GetAsAngleAxis() const {
    Orientation o=Normalized();
    return apply_visitor(AsAngleAxisVisitor(),o.mData);
}

class AsQuaternionVisitor : public static_visitor<Orientation> {
public:
    Orientation operator()(const EulerAngles& dat) const {
		Quaterniond q1(AngleAxisd(dat.alpha,Vector3d(0,0,1)));
		Quaterniond q2(AngleAxisd(dat.beta, Vector3d(0,1,0)));
		Quaterniond q3(AngleAxisd(dat.gamma,Vector3d(0,0,1)));
		Quaterniond q=q3*q2*q1;
		return Orientation(q);
    }
    Orientation operator()(const Matrix3d& dat) const {
        return Orientation(Quaterniond(dat));
    }
    Orientation operator()(const Quaterniond& dat) const {
        return Orientation(dat);
    }
    Orientation operator()(const AngleAxisd& dat) const {
        double vx=dat.axis().x();
        double vy=dat.axis().y();
        double vz=dat.axis().z();

		double sin_a=sin(dat.angle()/2);
		double w=cos(dat.angle()/2);
		double x=vx*sin_a;
		double y=vy*sin_a;
		double z=vz*sin_a;
        return Orientation(Quaterniond(w,x,y,z));
    }
};
Orientation Orientation::GetAsQuaternion() const {
    cout << "Unnormalised" << ToString() << endl;
    Orientation o=Normalized();
    cout << "Normalised" << o.ToString() << endl;
    return apply_visitor(AsQuaternionVisitor(),o.mData);
}

class NormalizeVisitor : public static_visitor<> {
public:
    void operator()(EulerAngles& dat) const {
        dat.Normalize();
    }
    void operator()(Matrix3d& dat) const {
    }
    void operator()(Quaterniond& dat) const {
        dat.normalize();
    }
    void operator()(AngleAxisd& dat) const {
        dat.axis()=dat.axis().normalized();
    }
};
void Orientation::Normalize() {
    apply_visitor(NormalizeVisitor(),mData);
}

class NormalisedVisitor : public static_visitor<Orientation> {
public:
    Orientation operator()(const EulerAngles& dat) const {
        return Orientation(dat.Normalized());
    }
    Orientation operator()(const Matrix3d& dat) const {
		return dat;
    }
    Orientation operator()(const Quaterniond& dat) const {
        return Orientation(dat.normalized());
    }
    Orientation operator()(const AngleAxisd& dat) const {
        return Orientation(AngleAxisd(dat.angle(),dat.axis().normalized()));
    }
};
Orientation Orientation::Normalized() const {
    return apply_visitor(NormalisedVisitor(),mData);
}


