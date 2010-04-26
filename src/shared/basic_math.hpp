
#include <cmath>
#include <complex>

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

    ///Solve and overdetermined system via the least squares method.
    void overdetermined(C a, C b,
                        C c, C d,
                        C e, C f,
                        C x, C y, C z,
                        C* unknown1, C* unknown2);
}

