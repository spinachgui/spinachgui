
#include <shared/panic.hpp>

#include <iostream>

using namespace std;

bool (*gPanicHandler)(const string& s) = NULL;

void SetPanicHandler(bool (*handler)(const string& s)) {
    gPanicHandler = handler;
}

void PANIC(const string& s) {
    bool abandon = false;
    if(gPanicHandler != NULL) {
        abandon = gPanicHandler(s);
    }
    cout << "Panicking, " << s << endl;
    int* x = NULL;
    x++;
    x--;
    //Make use we use x with a side effect
    if(abandon) {
        cout << (*x) << endl;
    }
}

void NaNPANIC(double n, const std::string& s) {
    if(n != n) {
        PANIC(s);
    }
}

void NaNPANIC(const Eigen::Vector3d& v, const std::string& s) {
    NaNPANIC(v.x(),s);
    NaNPANIC(v.y(),s);
    NaNPANIC(v.z(),s);
}

void NaNPANIC(const Eigen::Matrix3d& m, const std::string& s) {
    NaNPANIC(m(0,0),s);
    NaNPANIC(m(0,1),s);
    NaNPANIC(m(0,2),s);

    NaNPANIC(m(1,0),s);
    NaNPANIC(m(1,1),s);
    NaNPANIC(m(1,2),s);

    NaNPANIC(m(2,0),s);
    NaNPANIC(m(2,1),s);
    NaNPANIC(m(2,2),s);
}


