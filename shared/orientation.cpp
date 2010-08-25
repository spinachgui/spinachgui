
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

//============================================================//
// Rotation Conversions bank

EulerAngles SpinXML::ConvertToEuler(const EulerAngles& rot) {return rot;}
EulerAngles SpinXML::ConvertToEuler(const Matrix3d&    rot) {return ConvertToEuler(ConvertToQuaternion(rot));}
EulerAngles SpinXML::ConvertToEuler(const Quaterniond& rot) {
	Vector3d z_axis=Vector3d(0,0,1);
	Vector3d x_axis=Vector3d(1,0,0);
	z_axis=rot*z_axis;
	x_axis=rot*x_axis;
    
	double gamma=atan2(z_axis.y(),z_axis.x());
	double beta=atan2(sqrt(z_axis.x()*z_axis.x() + z_axis.y()*z_axis.y()),z_axis.z());

	//Use γ and β to rotate V2 back onto the point 0,0,1
	Quaterniond betaTwist(AngleAxisd(-beta, Vector3d(0,1,0)));
	Quaterniond gammaTwist (AngleAxisd(-gamma,Vector3d(0,0,1)));

	x_axis=(betaTwist*gammaTwist)*x_axis;

	double alpha = atan2(x_axis.y(),x_axis.x());
	if(alpha < 0 || alpha >= 2*PI)  alpha = alpha-2*PI*floor(alpha/(2*PI));
	if(alpha >= 2*PI) alpha = 0;
	if(beta < 0 || beta >= PI)    beta = beta-  PI*floor(beta/PI);
	if(gamma < 0 || gamma >= 2*PI)  gamma = gamma-2*PI*floor(gamma/(2*PI));
	if(gamma >= 2*PI) gamma = 0;
	return EulerAngles(alpha,beta,gamma);
}
EulerAngles SpinXML::ConvertToEuler(const AngleAxisd&   rot) {return ConvertToEuler(ConvertToQuaternion(rot));}

Matrix3d    SpinXML::ConvertToDCM(const EulerAngles& rot) {return ConvertToDCM(ConvertToQuaternion(rot));}
Matrix3d    SpinXML::ConvertToDCM(const Matrix3d&    rot) {return rot;}
Matrix3d    SpinXML::ConvertToDCM(const Quaterniond& rot) {return rot.toRotationMatrix();}
Matrix3d    SpinXML::ConvertToDCM(const AngleAxisd&   rot){return ConvertToDCM(ConvertToQuaternion(rot));}

Quaterniond SpinXML::ConvertToQuaternion(const EulerAngles& rot) {
	Quaterniond q1(AngleAxisd(rot.alpha,Vector3d(0,0,1)));
	Quaterniond q2(AngleAxisd(rot.beta, Vector3d(0,1,0)));
	Quaterniond q3(AngleAxisd(rot.gamma,Vector3d(0,0,1)));
	return q3*q2*q1;
}
Quaterniond SpinXML::ConvertToQuaternion(const Matrix3d&    rot) {return Quaterniond(rot);}
Quaterniond SpinXML::ConvertToQuaternion(const Quaterniond& rot) {return rot;}
Quaterniond SpinXML::ConvertToQuaternion(const AngleAxisd&   rot) {return Quaterniond(rot);}

AngleAxisd  SpinXML::ConvertToAngleAxis(const EulerAngles& rot) {return ConvertToAngleAxis(ConvertToQuaternion(rot));}
AngleAxisd  SpinXML::ConvertToAngleAxis(const Matrix3d&    rot) {return ConvertToAngleAxis(ConvertToQuaternion(rot));}
AngleAxisd  SpinXML::ConvertToAngleAxis(const Quaterniond& rot) {return AngleAxisd(rot);}
AngleAxisd  SpinXML::ConvertToAngleAxis(const AngleAxisd&   rot) {return rot;}

//============================================================//
// Rotation Normalizer bank
EulerAngles SpinXML::NormalizeRotation(const EulerAngles& rot) {return rot.Normalized();}							   
Matrix3d    SpinXML::NormalizeRotation(const Matrix3d& rot)    {return rot;}
Quaterniond SpinXML::NormalizeRotation(const Quaterniond& rot) {return rot.normalized();}							   
AngleAxisd  SpinXML::NormalizeRotation(const AngleAxisd& rot)  {return AngleAxisd(rot.angle(),rot.axis().normalized());}



//==============================================================================//
// Orientation

Orientation::Type Orientation::GetType() const {
    if(get<EulerAngles>(&mData)!=NULL) {
        return EULER;
    } else if(get<AngleAxisd>(&mData)!=NULL) {
        return ANGLE_AXIS;
    } else if(get<Matrix3d>(&mData)!=NULL) {
        return DCM;
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

void Orientation::GetDCM(Matrix3d* matrix) const {
    *matrix = get<Matrix3d>(mData);
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

#define DEFINE_CONVERTER(name,return_type,function)		\
	struct name : public static_visitor<return_type> {		\
		return_type operator()(const EulerAngles& dat) const {return function(dat);} \
		return_type operator()(const Matrix3d& dat) const    {return function(dat);} \
		return_type operator()(const Quaterniond& dat) const {return function(dat);} \
		return_type operator()(const AngleAxisd& dat) const  {return function(dat);} \
	};

DEFINE_CONVERTER(AsEulerVisitor,EulerAngles,ConvertToEuler)
EulerAngles Orientation::GetAsEuler() const {
    Orientation normalised=Normalized();
    return apply_visitor(AsEulerVisitor(),normalised.mData);
}

DEFINE_CONVERTER(AsDCMVisitor,Matrix3d,ConvertToDCM)
Matrix3d Orientation::GetAsDCM() const {
    Orientation o=Normalized();
    return apply_visitor(AsDCMVisitor(),o.mData);
}

DEFINE_CONVERTER(AsAngleAxisVisitor,AngleAxisd,ConvertToAngleAxis)
AngleAxisd Orientation::GetAsAngleAxis() const {
    Orientation o=Normalized();
    return apply_visitor(AsAngleAxisVisitor(),o.mData);
}

DEFINE_CONVERTER(AsQuaternionVisitor,Quaterniond,ConvertToQuaternion)
Quaterniond Orientation::GetAsQuaternion() const {
    Orientation o=Normalized();
    return apply_visitor(AsQuaternionVisitor(),o.mData);
}

struct NormalizeVisitor : public static_visitor<> {
    void operator()(EulerAngles& dat) const {dat.Normalize();}
    void operator()(Matrix3d& dat) const { }
    void operator()(Quaterniond& dat) const {dat.normalize();}
    void operator()(AngleAxisd& dat) const {dat.axis()=dat.axis().normalized();}
};
void Orientation::Normalize() {
    apply_visitor(NormalizeVisitor(),mData);
}

struct NormalisedVisitor : public static_visitor<Orientation> {
    Orientation operator()(const EulerAngles& dat) const {return Orientation(dat.Normalized());}
    Orientation operator()(const Matrix3d& dat) const    {return dat;}
    Orientation operator()(const Quaterniond& dat) const {return Orientation(dat.normalized());}
    Orientation operator()(const AngleAxisd& dat) const  {return Orientation(AngleAxisd(dat.angle(),dat.axis().normalized()));}
};
Orientation Orientation::Normalized() const {
    return apply_visitor(NormalisedVisitor(),mData);
}


