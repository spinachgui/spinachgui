
#include <shared/basic_math.hpp>
#include <iostream>

using namespace SpinXML;
using namespace std;

void SpinXML::overdetermined(C a, C b,
                             C c, C d,
                             C e, C f,
                             C x, C y, C z,
                             C* unknown1, C* unknown2) {
    cout << "==============================" << endl;
    cout << "Solving the overdetermined system" << endl;
    
    cout << "("
              << a << ", " << b << endl
              << c << ", " << d << endl
              << e << ", " << f << endl;
    cout << "With x = " << x << ", y=" << y << ", z=" << z << endl;
        
    // x = (A^T A)^-1 A^T b will solve for x in an overdetermined system

    //Find B=A^T A
    C b11=a*a+c*c+e*e;      C b12=a*b+c*d+e*f;
    C b21=b12;              C b22=b*b+d*d+f*f;

    cout << "B matrix:" << endl;
    cout << "("
         << b11 << "," << b12 << endl
         << b21 << "," << b22 << ")" << endl;

    //Now find (B)^-1
    C det=(b11*b22-b12*b21);
    if(isZero(det)) {cout << "WARNING: matrix has no inverse" << endl;}
    C inv_det = C(1.0,0.0)/det;

    C invB11  = b22 * inv_det;    C invB12  = -(b12 * inv_det);
    C invB21  = -(b21 * inv_det); C invB22  = b11 * inv_det;

    cout << "Checking the inverse" << endl;
    cout << "("
              << b11*invB11+b12*invB21 << "," << b11*invB12+b12*invB22 << "," << endl
              << b21*invB11+b22*invB21 << "," << b21*invB12+b22*invB22 << "," << ")" << endl;
 

    
    //Finally B^-1 * A^T = D, the pseudo inverse
    
    C d11 = a*invB11+b*invB12; C d12 = c*invB11+d*invB12; C d13 = e*invB11+f*invB12;
    C d21 = a*invB21+b*invB22; C d22 = c*invB21+d*invB22; C d23 = e*invB21+f*invB22;

    cout << "Pseudo inverse:" << endl;
    cout << "("
         << d11 << "," << d12 << "," << d13 << endl
         << d21 << "," << d22 << "," << d23 << ")" << endl;

    //Check that the unknowns solve the system

    C u1=*unknown1 = d11*x+d12*y+d13*z;
    C u2=*unknown2 = d21*x+d22*y+d23*z;

    C xs=a*u1+b*u2;
    C ys=c*u1+d*u2;
    C zs=e*u1+f*u2;

    cout << "Solution:" << endl;
    cout << "u1" << u1 << endl;
    cout << "u2" << u2 << endl;
    cout << endl;
    
    cout << "Checking the solution" << endl;
    cout << "x=" << x << "  xs=" << xs << endl;
    cout << "y=" << y << "  ys=" << ys << endl;
    cout << "z=" << z << "  zs=" << zs << endl;
    cout << endl;
}


