#ifndef SPIN_H
#define SPIN_H


#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>


using namespace std;


class Spin {
public:
    Spin();
    Spin(const string& name,double _x,double _y,double _z,double isotropic): mName(name),x(_x),y(_y),z(_z),mIsotropic(isotropic) {
      //Eigenvalues 1 2 3 rotated about x by pi/4
        double dA[] = { 1.5, 0.5, 0,
                       -0.5, 1.5, 0,
                        0,   0,   3};
        //Matrix A(dA, 3, 3);
    };

  //Clerical Stuff
    string mName;

  //TODO: Replace with proper vector notation?
    double x;
    double y;
    double z;

  //Shielding
    double mIsotropic;
//    Matrix mAnisotropic;

  //Relaxation
    bool mRedfield;
    double mRelaxationRate;

private:
    Spin(Spin&);
};

typedef boost::shared_ptr<Spin> SpinPnt;

#endif


