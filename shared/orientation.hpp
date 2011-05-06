
#ifndef __MATHTYPES___H_
#define __MATHTYPES___H_

#include <cstring>
#include <string>
#include <shared/unit.hpp>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <shared/objcounter.hpp>

#define PI 3.141592653589793238462643
using std::complex;

namespace SpinXML {

    ///Plank's constant in SI units
    extern const double hbar;

    ///Bohr magneton in SI units
    extern const double bohr_mag;

    ///Ther permeability of free space
    extern const double mu0;


    ///Euler angle type
    struct EulerAngles {
        double alpha,beta,gamma;
		EulerAngles()
            : alpha(0),beta(0),gamma(0) {

        }
        EulerAngles(double _alpha,double _beta, double _gamma)
            : alpha(_alpha),beta(_beta),gamma(_gamma) {

        }
        void Set(double _alpha,double _beta, double _gamma) {
            alpha=_alpha;
            beta= _beta;
            gamma=_gamma;
        }
        bool operator==(const EulerAngles& o) const {
            return alpha == o.alpha &&  beta == o.beta && gamma == o.gamma;
        }
        bool operator!=(const EulerAngles& o) const {
            return alpha != o.alpha ||  beta != o.beta || gamma != o.gamma;
        }

        void Normalize() {

        }
        EulerAngles Normalized() const {
            return *this;
        }

    };

    //Rotation Converions

    EulerAngles ConvertToEuler(const EulerAngles& rot);
    EulerAngles ConvertToEuler(const Eigen::Matrix3d&    rot);
    EulerAngles ConvertToEuler(const Eigen::Quaterniond& rot);
    EulerAngles ConvertToEuler(const Eigen::AngleAxisd&   rot);

    Eigen::Matrix3d    ConvertToDCM(const EulerAngles& rot);
    Eigen::Matrix3d    ConvertToDCM(const Eigen::Matrix3d&    rot);
    Eigen::Matrix3d    ConvertToDCM(const Eigen::Quaterniond& rot);
    Eigen::Matrix3d    ConvertToDCM(const Eigen::AngleAxisd&   rot);

    Eigen::Quaterniond ConvertToQuaternion(const EulerAngles& rot);
    Eigen::Quaterniond ConvertToQuaternion(const Eigen::Matrix3d&    rot);
    Eigen::Quaterniond ConvertToQuaternion(const Eigen::Quaterniond& rot);
    Eigen::Quaterniond ConvertToQuaternion(const Eigen::AngleAxisd&   rot);

    Eigen::AngleAxisd  ConvertToAngleAxis(const EulerAngles& rot);
    Eigen::AngleAxisd  ConvertToAngleAxis(const Eigen::Matrix3d&    rot);
    Eigen::AngleAxisd  ConvertToAngleAxis(const Eigen::Quaterniond& rot);
    Eigen::AngleAxisd  ConvertToAngleAxis(const Eigen::AngleAxisd&   rot);

    //Uniform Rotation Normalisation
    EulerAngles NormalizeRotation(const EulerAngles& rot);
    Eigen::Matrix3d    NormalizeRotation(const Eigen::Matrix3d& rot);
    Eigen::Quaterniond NormalizeRotation(const Eigen::Quaterniond& rot);
    Eigen::AngleAxisd  NormalizeRotation(const Eigen::AngleAxisd& rot);

    ///Class for storing a 3 dimentional rotation
    class Orientation : private Counter<Orientation> {
    public:
        using Counter<Orientation>::objCount;

        EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
        ///Constructs the identity rotation
        Orientation();
        ///Constructs an orientation from euler angles
        Orientation(const EulerAngles& ea);
        ///Constructs an orientation from a matrix
        Orientation(const Eigen::Matrix3d& m);
        ///Constructs an orientation from a quaternion
        Orientation(const Eigen::Quaterniond& q);
        ///Constructs an orientation from an AngleAxis
        Orientation(const Eigen::AngleAxisd& aa);
        ///Destructor
        ~Orientation();

        ///Enumeration of the four conventions on storing rotations
        enum Type {
            EULER,
            ANGLE_AXIS,
            QUATERNION,
            DCM
        };

        ///Normalise the contained rotation in place.
        void Normalize();
        ///Normalise the contained rotation.
        Orientation Normalized() const;

        ///Returns the convention that is being used to store the rotation
        Type GetType() const;

        ///If the euler angle convention is being used then this function
        ///will set its arguments to the stored values otherwise the result
        ///is undefined.
        void GetEuler(double* alpha,double* beta,double* gamma) const;
        ///If the angle axis convention being used then this function will
        ///set its arugments to those values otherwise the result is
        ///undefined.
        void GetAngleAxis(double* angle,Eigen::Vector3d* axis) const;
        ///If the quaternion convention being used then this function will
        ///set its arguments to those values otherwise the result is
        ///undefined.
        void GetQuaternion(double* real, double* i, double* j, double* k) const;
        ///If the eigensystem convention is being used then this function
        ///will set its arguments to the appropriate values otherwise the
        ///result is undefined.
        void GetDCM(Eigen::Matrix3d* matrix) const;

        Eigen::Vector3d Apply(Eigen::Vector3d v) const;

        const Orientation& operator=(const EulerAngles& ea);
        const Orientation& operator=(const Eigen::AngleAxisd& aa);
        const Orientation& operator=(const Eigen::Matrix3d& m);   
        const Orientation& operator=(const Eigen::Quaterniond& q);

        bool operator==(const Orientation& o) const;

        ///Converts to a rotation matrix
        Eigen::Matrix3d GetAsMatrix() const;

        ///returns a representation of the orientation as a string that is
        ///both machiene parsable and human readable.
        std::string ToString() const;
        ///Parses an orentation from a string representation. The form of
        ///the representation should be the same as returned by ToString
        void FromString(std::string string);

        EulerAngles GetAsEuler() const;
        Eigen::Matrix3d    GetAsDCM() const;
        Eigen::AngleAxisd  GetAsAngleAxis() const;
        Eigen::Quaterniond GetAsQuaternion() const;

    private:
		Type mType;
		EulerAngles mEuler;
		Eigen::AngleAxisd mAngleAxis;
		Eigen::Quaterniond mQuaternion;
		Eigen::Matrix3d mMatrix;

		void Invariant() const;
    };
};
#endif
