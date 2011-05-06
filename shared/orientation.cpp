
#include <shared/orientation.hpp>
#include <iostream>
#include <cstring>
#include <sstream>
#include <cmath>
#include <boost/variant/static_visitor.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <Eigen/Dense>
#include <shared/assert.hpp>
#include <shared/panic.hpp>

using namespace std;
using namespace Eigen;
using namespace SpinXML;
using namespace boost;

//============================================================//
// Constants

const double SpinXML::hbar=6.626068e-34;
const double SpinXML::bohr_mag=9.27400915e-24;
const double SpinXML::mu0=1.25663706e-6;

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
		EulerAngles mEuler;
		Eigen::AngleAxisd mAngleAxis;
		Eigen::Quaterniond mQuaternion;
		Eigen::Matrix3d mMatrix;



Orientation::Orientation()
	: mType(QUATERNION), mQuaternion(Quaterniond(1,0,0,0)) {
	Invariant();
}
///Constructs an orientation from euler angles
Orientation::Orientation(const EulerAngles& ea) 
	: mType(EULER),mEuler(ea) {
	Invariant();
}
///Constructs an orientation from a matrix
Orientation::Orientation(const Eigen::Matrix3d& m) 
	: mType(DCM),mMatrix(m) {
	Invariant();
}
///Constructs an orientation from a quaternion
Orientation::Orientation(const Eigen::Quaterniond& q)
	: mType(QUATERNION), mQuaternion(q) {
	Invariant();
}
///Constructs an orientation from an AngleAxis
Orientation::Orientation(const Eigen::AngleAxisd& aa)
	: mType(ANGLE_AXIS), mAngleAxis(aa) {
	Invariant();
}
///Destructor
Orientation::~Orientation() {};

const Orientation& Orientation::operator=(const EulerAngles& ea)        {
	mType = EULER;
	mEuler = ea;
	Invariant();
	return *this;
}
const Orientation& Orientation::operator=(const Eigen::AngleAxisd& aa)  {
	mType = ANGLE_AXIS;
	mAngleAxis = aa;
	Invariant();
	return *this;
}
const Orientation& Orientation::operator=(const Eigen::Matrix3d& m)     {
	mType = DCM;
	mMatrix = m;
	Invariant();
	return *this;
}
const Orientation& Orientation::operator=(const Eigen::Quaterniond& q)  {
	mType = QUATERNION;
	mQuaternion = q;
	Invariant();
	return *this;
}

bool Orientation::operator==(const Orientation& o) const {
	if(mType != o.mType) {
		return false;
	}
	if(mType == EULER) {
		return o.mEuler==mEuler;
	}
	if(mType == DCM) {
		return o.mMatrix == mMatrix;
	}
	if(mType == ANGLE_AXIS) {
		return o.mAngleAxis.angle() == mAngleAxis.angle() &&
			o.mAngleAxis.axis() == mAngleAxis.axis();
	}
	if(mType == QUATERNION) {
		return o.mQuaternion.w()==mQuaternion.w()
			&& o.mQuaternion.x()==mQuaternion.x()
			&& o.mQuaternion.y()==mQuaternion.y()
			&& o.mQuaternion.z()==mQuaternion.z();
	}
	spinxml_assert(false);
	return false;
}




Orientation::Type Orientation::GetType() const {
	return mType;
}

Matrix3d Orientation::GetAsMatrix() const {
    Orientation normalized=Normalized();
	if(mType == EULER) {
        return ConvertToDCM(normalized.mEuler);
	}
	if(mType == DCM) {
        return ConvertToDCM(normalized.mMatrix);
	}
	if(mType == ANGLE_AXIS) {
        return ConvertToDCM(normalized.mAngleAxis);
	}
	if(mType == QUATERNION) {
        return ConvertToDCM(normalized.mQuaternion);
	}
	spinxml_assert(false);
	return Matrix3d();
}

Vector3d Orientation::Apply(Vector3d v) const {
    if(mType == EULER) {
        AngleAxisd one(mEuler.alpha ,Vector3d(0,0,1));
        AngleAxisd two(mEuler.beta  ,Vector3d(0,1,0));
        AngleAxisd three(mEuler.gamma ,Vector3d(0,0,1));
        return three*(two*(one*v));
    } else if(mType == DCM) {
        return mMatrix*v;
    } else if(mType == ANGLE_AXIS) {
        return mAngleAxis*v;
    } else if(mType == QUATERNION)  {
        return mQuaternion*v;
    }
	spinxml_assert(false);
	return Vector3d();
}

void Orientation::GetEuler(double* alpha,double* beta,double* gamma) const {
	spinxml_assert(mType == EULER);
    *alpha = mEuler.alpha;
    *beta =  mEuler.beta;
    *gamma = mEuler.gamma;
}

void Orientation::GetAngleAxis(double* angle,Vector3d* axis) const {
	spinxml_assert(mType == ANGLE_AXIS);
    *angle = mAngleAxis.angle();
    *axis =  mAngleAxis.axis();
}

void Orientation::GetQuaternion(double* real, double* i, double* j, double* k) const {
	spinxml_assert(mType == QUATERNION);
    *real = mQuaternion.w();
    *i    = mQuaternion.x();
    *j    = mQuaternion.y();
    *k    = mQuaternion.z();
}

void Orientation::GetDCM(Matrix3d* matrix) const {
	spinxml_assert(mType == DCM);
    *matrix = mMatrix;
}

string Orientation::ToString() const {
	if(mType == EULER) {
        ostringstream oss;
        oss << "eu:" << mEuler.alpha << "," << mEuler.beta << "," << mEuler.gamma;
        return oss.str();
	}
	if(mType == DCM) {
        ostringstream oss;
        oss << "es:"
            << mMatrix << endl;
        return oss.str();
	}
	if(mType == QUATERNION) {
        ostringstream oss;
        oss << "q:"
            << mQuaternion.w() << ","
            << mQuaternion.x() << ","
            << mQuaternion.y() << ","
            << mQuaternion.z();
        return oss.str();
	}
	if(mType == ANGLE_AXIS) {
        ostringstream oss;
        oss << "aa:"
            << mAngleAxis.angle() << ","
            << mAngleAxis.axis().x() << ","
            << mAngleAxis.axis().y() << ","
            << mAngleAxis.axis().z();
        return oss.str();
	}
	spinxml_assert(false);
	return "ERROR";
}

void Orientation::FromString(std::string string) {
    cerr << "Not Implimented" << endl;
}

#define DEFINE_CONVERTER(name,return_type,function)                     \
    Orientation normalized=Normalized();								\
	if(mType == EULER) {												\
        return function(normalized.mEuler);								\
	}																	\
	if(mType == DCM) {													\
        return function(normalized.mMatrix);							\
	}																	\
	if(mType == ANGLE_AXIS) {											\
		return function(normalized.mAngleAxis);							\
	}																	\
	if(mType == QUATERNION) {											\
        return function(normalized.mQuaternion);						\
	}																	\
	spinxml_assert(false);												

EulerAngles Orientation::GetAsEuler() const {
	DEFINE_CONVERTER(AsEulerVisitor,EulerAngles,ConvertToEuler);
	return EulerAngles();
}

Matrix3d Orientation::GetAsDCM() const {
	DEFINE_CONVERTER(AsDCMVisitor,Matrix3d,ConvertToDCM)
	return Matrix3d();
}

AngleAxisd Orientation::GetAsAngleAxis() const {
	DEFINE_CONVERTER(AsAngleAxisVisitor,AngleAxisd,ConvertToAngleAxis)
	return AngleAxisd();		
}

Quaterniond Orientation::GetAsQuaternion() const {
	DEFINE_CONVERTER(AsQuaternionVisitor,Quaterniond,ConvertToQuaternion)
	return Quaterniond();
}

struct NormalizeVisitor : public static_visitor<> {
    void operator()(EulerAngles& dat) const {
		dat.Normalize();
	}
    void operator()(Matrix3d& dat) const { }
    void operator()(Quaterniond& dat) const {dat.normalize();}
    void operator()(AngleAxisd& dat) const {}
};
void Orientation::Normalize() {
	if(mType == EULER) {						
		mEuler.Normalize();
	} else if(mType == DCM) {	
		//TODO
	} else if(mType == QUATERNION) {					
		mQuaternion.normalize();	
	} else if(mType == ANGLE_AXIS) {
        mAngleAxis.axis()=mAngleAxis.axis().normalized();
	} else {
		spinxml_assert(false);
	}
	Invariant();
}

Orientation Orientation::Normalized() const {
	if(mType == EULER) {						
		return mEuler.Normalized();
	}
	if(mType == DCM) {	
		return mMatrix;
	}
	if(mType == QUATERNION) {					
		return mQuaternion.normalized();	
	}
	if(mType == ANGLE_AXIS) {
        return AngleAxisd(mAngleAxis.angle(),mAngleAxis.axis().normalized());
	}
	spinxml_assert(false);
	return Orientation();
}


void Orientation::Invariant() const {
    if(mType == ANGLE_AXIS) {
		Vector3d axis = mAngleAxis.axis();
		if(axis.x() == 0 && axis.y() == 0 && axis.z() == 0) {
			PANIC("An orientation in angle-axis notation has (0,0,0) as it's axis vector");
		}
    } else if(mType == QUATERNION) {
		if(mQuaternion.x() == 0 && mQuaternion.y() == 0 && mQuaternion.z() == 0 && mQuaternion.w() == 0) {
			PANIC("An orientation in quaternion notation is (0,0,0,0)");
		}
	}
	
}
