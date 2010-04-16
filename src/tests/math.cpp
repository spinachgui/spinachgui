
#include <cstring>
#include <complex>
#include <iostream>
#include <cmath>
#include <complex>
#include <shared/unit.hpp>
#include <shared/mathtypes.hpp>

using namespace std;
using namespace SpinXML;


int main() {
    cubic f(C(1.0,0),C(5e-10,0),C(5e-20,0),C(5e-30,0));
    cout << "f(5)=" << f(5) << endl;

    C lambda1,lambda2,lambda3;
    cout << "==========Solve1 method==========" << endl;
    f.solve(&lambda1,&lambda2,&lambda3);
    cout << "Solution1 = " << lambda1 << endl;
    cout << "Solution2 = " << lambda2 << endl;
    cout << "Solution3 = " << lambda3 << endl;
    cout << endl;

    cout << "==========Solve2 method==========" << endl;
    f.solve2(&lambda1,&lambda2,&lambda3);
    cout << "Solution1 = " << lambda1 << endl;
    cout << "Solution2 = " << lambda2 << endl;
    cout << "Solution3 = " << lambda3 << endl;
    cout << endl;

    cout << endl;
    cout << "==========Eigenvectors==========" << endl;

    C 
        e1,e2,e3,
        vx1,vy1,vz1,
        vx2,vy2,vz2,
        vx3,vy3,vz3;
    standalone_eig(C(1.0,0.0),C(0.0,0.0),C(4.0,0.0),
                   C(0.0,0.0),C(2.0,0.0),C(0.0,0.0),
                   C(0.0,0.0),C(0.0,0.0),C(3.0,0.0),
        &e1,&e2,&e3,
        &vx1,&vy1,&vz1,
        &vx2,&vy2,&vz2,
        &vx3,&vy3,&vz3);

    return 0;
}



