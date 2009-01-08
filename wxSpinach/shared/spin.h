#ifndef SPIN_H
#define SPIN_H


#include <string>
#include <vector>

using namespace std;

class Spin {
public:
    Spin();
    Spin(const string& name,double _x,double _y,double _z): mName(name),x(_x),y(_y),z(_z) {};

  //Clerical Stuff
    string mName;

  //TODO: Replace with proper vector notation?
    double x;
    double y;
    double z;

  //Shielding
    double mIsotropic;

  //Relaxation
    bool mRedfield;
    double mRelaxationRate;

private:
};

#endif


