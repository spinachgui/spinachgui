
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

    //Random Matrix
    {   C 
            e1,e2,e3,
            vx1,vy1,vz1,
            vx2,vy2,vz2,
            vx3,vy3,vz3;
        standalone_eig(C(1),C(0),C(4),
                       C(0),C(2),C(0),
                       C(4),C(0),C(3),
                       &e1,&e2,&e3,
                       &vx1,&vy1,&vz1,
                       &vx2,&vy2,&vz2,
                       &vx3,&vy3,&vz3);
    }

    //Matrix with degenerate eigenvalues
    {
        C 
            e1,e2,e3,
            vx1,vy1,vz1,
            vx2,vy2,vz2,
            vx3,vy3,vz3;
        standalone_eig(C(1.7081),C(0.4546),C(     0),
                       C(0.4546),C(1.2919),C(     0),
                       C(     0),C(     0),C(2.0000),
                       &e1,&e2,&e3,
                       &vx1,&vy1,&vz1,
                       &vx2,&vy2,&vz2,
                       &vx3,&vy3,&vz3);

        cout << "e1=" << e1 << endl;
        cout << "e2=" << e2 << endl;
        cout << "e3=" << e3 << endl;

        cout << real(vx1) << " " << real(vy1) << " "  << real(vz1) << endl;
        cout << real(vx2) << " " << real(vy2) << " "  << real(vz2) << endl;
        cout << real(vx3) << " " << real(vy3) << " "  << real(vz3) << endl;
    }
    return 0;
}



