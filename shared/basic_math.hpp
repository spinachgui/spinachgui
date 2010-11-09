#ifndef BASIC_MATH_HPP
#define BASIC_MATH_HPP

#include <cmath>
#include <complex>
#include <Eigen/Dense>

using namespace Eigen;

#define pi M_PI

namespace SpinXML {
    using std::complex;

    typedef std::complex<long double> C;

    ///Returns true of the complex number is exactly zero
    template<typename F> 
    bool isZero(complex<F> z) {
        return 
            z==complex<F>( 0.0, 0.0) ||
            z==complex<F>(-0.0, 0.0) || 
            z==complex<F>( 0.0,-0.0) || 
            z==complex<F>(-0.0,-0.0);
    }

    ///Helper function to make a matrix from a bunch of doubles
    Matrix3d MakeMatrix3d(double a00, double a01, double a02,
                          double a10, double a11, double a12,
                          double a20, double a21, double a22);

	///Extract out the rotation potion of a 4x4 isometary matrix
	Matrix3d MakeMatrix3d(Matrix4d mat);

	///Helper function to make a vector4 from a vector3
	Vector4d MakeVector4d(Vector3d v);

	///Helper function to make project a vector4 back to a vector3
	Vector3d MakeVector3d(Vector4d v);


    ///Returns true of the complex number is close to zero
    template<typename F> 
    bool isNearlyZero(complex<F> z) {
        return abs(z) < 1e-15;
    }
    ///Returns true of the complex number is close to zero
    template<typename F> 
    complex<F> clampToZero(complex<F> z) {
        return abs(z) < 1e-15 ? complex<F>(0) : z;
    }


    ///The complex acos function
    template<typename F>
    std::complex<F> acos(complex<F> z) {
        return log(z + complex<F>(0,1) * sqrt(complex<F>(1,0)-z*z))/complex<F>(0,1);
    }

}

#endif
