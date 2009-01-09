#ifndef SPIN_H
#define SPIN_H

#include <mtl/matrix.h>

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>


using namespace std;

typedef mtl::matrix<double,
                    mtl::rectangle<>,
                    mtl::dense<mtl::external>,
                    mtl::column_major>::type Matrix;

class Spin {
public:
    Spin();
    Spin(const string& name,double _x,double _y,double _z,double isotropic): mName(name),x(_x),y(_y),z(_z),mIsotropic(isotropic) {
        double dA[] = { 1, 3, 2, 1.5, 2.5, 3.5, 4.5, 9.5, 5.5 };
        Matrix A(dA, 3, 3);
    };

  //Clerical Stuff
    string mName;

  //TODO: Replace with proper vector notation?
    double x;
    double y;
    double z;

  //Shielding
    double mIsotropic;
    Matrix mAnisotropic;

  //Relaxation
    bool mRedfield;
    double mRelaxationRate;

private:
    Spin(Spin&);
};

typedef boost::shared_ptr<Spin> SpinPnt;

#endif


