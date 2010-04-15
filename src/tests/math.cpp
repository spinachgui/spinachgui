
#include <cstring>
#include <complex>
#include <iostream>
#include <cmath>
#include <complex>
#include <shared/unit.hpp>

using namespace std;

typedef long double R;
typedef complex<R> C;


template<typename F>
complex<F> acos(complex<F> z) {
    return log(z + complex<F>(0,1) * sqrt(complex<F>(1,0)-z*z))/complex<F>(0,1);
}

template<typename F> 
bool isZero(complex<F> z) {
    return 
        z==complex<F>( 0.0, 0.0) ||
        z==complex<F>(-0.0, 0.0) || 
        z==complex<F>( 0.0,-0.0) || 
        z==complex<F>(-0.0,-0.0);
}

///Solve and overdetermined system via the least squares method.
void overdetermined(C a, C b,
                    C c, C d,
                    C e, C f,
                    C x, C y, C z,
                    C* unknown1, C* unknown2) {
    cout << "==============================" << endl;
    cout << "Solving the overdetermined system" << endl;
    
    cout << "("
         << a << ", " << b << "  \t     \t (" << x        <<  endl
         << c << ", " << d << "  \t(x  =\t "  << y        << endl
         << e << ", " << f << ") \ty)   \t "  << z << ")" << endl;
        
    // x = (A^T A)^-1 A^T b will solve for x in an overdetermined system

    //Find B=A^T A
    C b11=a*a+b*c+c*e;
    C b12=a*b+b*d+c*f;
    C b21=a*d+c*e+f*e;
    C b22=b*d+e*d+f*f;

    //Now find (B)^-1
    C inv_det = C(1.0,0.0)/(b11*b22-b12*b21);
    C invB11  = b22 * inv_det;    C invB12  = -(b12 * inv_det);
    C invB21  = -(b21 * inv_det); C invB22  = b11 * inv_det;

    cout << "Checking the inverse" << endl;
    cout << "("
         << b11*invB11+b12*invB21 << "," << b11*invB12+b12*invB22 << "," << endl
         << b21*invB11+b22*invB21 << "," << b21*invB12+b22*invB22 << "," << ")" << endl;
 

    
    //Finally B^-1 * A^T = D, the pseudo inverse
    
    C d11 = a*invB11+b*invB12; C d12 = c*invB11+d*invB12; C d13 = e*invB11+f*invB12;
    C d21 = a*invB21+b*invB22; C d22 = c*invB21+d*invB22; C d23 = e*invB21+f*invB22;

    *unknown1 = d11*x+d12*y+d13*z;
    *unknown2 = d21*x+d22*y+d23*z;
}


///A class for evaluating and solving quadratic equations

struct quadratic {
    C a,b,c;
    quadratic(C _a, C _b, C _c) 
        : a(_a), b(_b), c(_c) {
    }
    ///Numerically evaluate the quadratic
    C operator()(C x) const {
        return a*x*x + b*x + c;
    }
    ///Numerically evaluate the quadratic
    C eval(C x) const {
        return a*x*x + b*x + c;
    }
    ///Compute the first derivative
    C first_derivitive(C x) const {
        return C(2.0,0.0)*a*x + b;
    }

    void solve(C* x1,C* x2) const {
        std::cout << "Attempting to solve a quadratic f(x)="
                  << a << "x**2 + "
                  << b << "x**1 + "
                  << c << " = 0" << std::endl;
        C inv_denominator = C(1.0,0)/(C(2.0,0.0)*a);
        C radical = sqrt(b*b - C(4,0)*a*c) * inv_denominator;
        C minus_b = -b* inv_denominator;

        *x1 = minus_b + radical;
        *x2 = minus_b - radical;

        cout << "f(" << *x1 << ")=" << eval(*x1) << endl;
        cout << "f(" << *x2 << ")=" << eval(*x2) << endl;
    }
    ///Take a value close to f(x)=0 and find a better value
    C newton_opt(C x,long times=1) const {
        for(long i=0;i<times;i++) {
            C dfdx=first_derivitive(x);
            if(dfdx == C(0,0)) break; //We probably already found
            //the solution, in any case
            //newton can't help us any
            //longer
            x-=(*this)(x)/dfdx;
        }
        return x;
    }

};


///A class for evaluation as solving special cubics of the form x**3 + p x - q
struct reduced_cubic {
    C p,q;
    ///Constructor
    reduced_cubic(C _p, C _q)
        : p(_p),q(_q) {
    }
    ///Numerically evaluate the cubic
    C operator()(C x) const {
        return x*x*x + p*x -q;
    }
    ///Numerically evaluate the cubic
    C eval(C x) const {
        return x*x*x + p*x -q;
    }
    void solve(C* z1,C* z2,C* z3) {
        const static C twoPiOverTreeRotation  = C(-0.5,-0.866025403784438596588302061718422);

        cout << "==========x**3 + p x - q=0 Solver=========="<< endl;
        if(isZero(p) && isZero(q)) {
            *z1=*z2*=*z3=C(0,0);
            return;
        } else if(isZero(p)) {
            *z1=pow(q,C(1.0/3.0,0));
            *z2=*z1*twoPiOverTreeRotation;
            *z3=*z2*twoPiOverTreeRotation;
            return;
        }
        quadratic f(C(1.0,0),-q, -p*p*p/C(27,0));
        C w1,w2;
        f.solve(&w1,&w2);

        cout << "The w3 solutions are:" << w1 << " " << w2 << endl;
        
        C w1_1,w1_2,w1_3;
        C w2_1,w2_2,w2_3;

        //There appears to be a bug in the gnu pow(complex,double)
        //function so we use the pow(complex,complex instead)
        w1_1=pow(w1,C(1.0/3.0,0));
        w2_1=pow(w2,C(1.0/3.0,0));
        
        w1_2 = w1_1 * twoPiOverTreeRotation;
        w1_3 = w1_2 * twoPiOverTreeRotation;

        w2_2 = w2_1 * twoPiOverTreeRotation;
        w2_3 = w2_2 * twoPiOverTreeRotation;

        cout << "The six solutions for w are:" << endl
             << "w1_1 = " << w1_1 << endl 
             << "w1_2 = " << w1_2 << endl 
             << "w1_3 = " << w1_3 << endl 
             << "w2_1 = " << w2_1 << endl
             << "w2_2 = " << w2_2 << endl
             << "w2_3 = " << w2_3 << endl;

        *z1 = w1_1 - p/(C(3,0)*w1_1);
        *z2 = w1_2 - p/(C(3,0)*w1_2);
        *z3 = w1_3 - p/(C(3,0)*w1_3);

        std::cout << "f(" << *z1 << ")="  << eval(*z1) << std::endl;
        std::cout << "f(" << *z2  << ")=" << eval(*z2) << std::endl;
        std::cout << "f(" << *z3 << ")="  << eval(*z3) << std::endl;
    }
};



///A class for evaluating and solving cubic equations
struct cubic {
    C a,b,c,d;
    ///Constructor
    cubic(C _a,C _b,C _c,C _d) 
        : a(_a),b(_b),c(_c),d(_d){
    }
    ///Numerically evaluate the cubic
    C operator()(C x) const {
        return a*x*x*x + b*x*x + c*x + d;
    }
    ///Numerically evaluate the cubic
    C eval(C x) const {
        return a*x*x*x + b*x*x + c*x + d;
    }
    ///Compute the first derivative
    C first_derivitive(C x) const {
        return C(3.0,0.0)*a*x*x + C(2.0,0.0)*b*x + c;
    }
    void solve(C* lambda1,C* lambda2,C* lambda3) const {
        std::cout << "Attempting to solve a cubic f(x)="
                  << a << "x**3 + "
                  << b << "x**2 + "
                  << c << "x**1 + "
                  << d << " = 0" << std::endl;
            
        C delta2=(b*b - C(3.0,0)*a*c)/(C(9.0,0)*a*a);
        C h = -C(2.0,0)*a*sqrt(delta2)*sqrt(delta2)*sqrt(delta2);
        C Xn = -b/(C(3.0,0)*a);
        C Yn = (*this)(Xn);

        std::cout << "delta2=" << delta2 << std::endl;
        std::cout << "h="      << h << std::endl;
        std::cout << "Xn="     << Xn << std::endl;
        std::cout << "Yn="     << Yn << std::endl;

        //Cind the complex arccosine
            
        C theta=acos(Yn/h)/C(3.0,0.0);

        C alpha=Xn + C(2.0,0.0) * sqrt(delta2) * cos(theta);
        C beta =Xn + C(2.0,0.0) * sqrt(delta2) * cos(theta + C(2*M_PI/3,0));
        C gamma=Xn + C(2.0,0.0) * sqrt(delta2) * cos(theta + C(4*M_PI/3,0));

        std::cout << "f(" << alpha << ")=" << (*this)(alpha) << std::endl;
        std::cout << "f(" << beta  << ")=" <<  (*this)(beta) << std::endl;
        std::cout << "f(" << gamma << ")=" << (*this)(gamma) << std::endl;

        alpha=newton_opt(alpha,5);
        beta =newton_opt(beta, 5);
        gamma=newton_opt(gamma,5);

        std::cout << "Solutions after additional newton optimisation" << std::endl;
        std::cout << "f(" << alpha << ")=" << (*this)(alpha) << std::endl;
        std::cout << "f(" << beta  << ")=" <<  (*this)(beta) << std::endl;
        std::cout << "f(" << gamma << ")=" << (*this)(gamma) << std::endl;

        *lambda1 = alpha;
        *lambda2 = beta;
        *lambda3 = gamma;
        return;
    }
    void solve2(C* z1,C* z2,C* z3) const {
        std::cout << "Attempting to solve a cubic f(x)="
                  << a << "x**3 + "
                  << b << "x**2 + "
                  << c << "x**1 + "
                  << d << " = 0" << std::endl;

        //Map the general cubic to z**3 + a2 z**2 + a1 z + a0
        C inv_a = C(1.0,0)/a;
        C a2 = b*inv_a;
        C a1 = c*inv_a;
        C a0 = d*inv_a;

        C p =(C(3,0)*a1 - a2*a2)/C(3,0);
        C q =(C(9,0)*a1*a2 -C(27,0)*a0 - C(2,0)*a2*a2*a2)/C(27,0);
        // Reduce to a quadratic equation in w
        cout << "The shifted cubic is "
             << "x**3 + "
             << p << "x**1 + "
             << -q << " = 0" << std::endl;
        reduced_cubic f(p,q);
        C x1,x2,x3;
        f.solve(&x1,&x2,&x3);
        
        cout << "========== Solved the reduced cubic" << endl;

        *z1 = x1 - a2/C(3.0,0);
        *z2 = x2 - a2/C(3.0,0);
        *z3 = x3 - a2/C(3.0,0);

        std::cout << "f(" << *z1 << ")="  << eval(*z1) << std::endl;
        std::cout << "f(" << *z2  << ")=" << eval(*z2) << std::endl;
        std::cout << "f(" << *z3 << ")="  << eval(*z3) << std::endl;
        
        *z1=newton_opt(*z1,5);
        *z2=newton_opt(*z2,5);
        *z3=newton_opt(*z3,5);

        cout << "Solutions after newton method cleanup" << endl;
        std::cout << "f(" << *z1 << ")="  << eval(*z1) << std::endl;
        std::cout << "f(" << *z2  << ")=" << eval(*z2) << std::endl;
        std::cout << "f(" << *z3 << ")="  << eval(*z3) << std::endl;
        

        return;
    }
    ///Take a value close to f(x)=0 and find a better value
    C newton_opt(C x,long times=1) const {
        for(long i=0;i<times;i++) {
            C dfdx=first_derivitive(x);
            if(dfdx == C(0,0)) break; //We probably already found
            //the solution, in any case
            //newton can't help us any
            //longer
            x-=(*this)(x)/dfdx;
        }
        return x;
    }
};

void eig(C a, C b, C c,
         C d, C e, C f,
         C g, C h, C i,
         C* e1,C* e2, C* e3,
         C* vx1,C* vy1, C* vz1,
         C* vx2,C* vy2, C* vz2,
         C* vx3,C* vy3, C* vz3) {

            std::cout << "Finding the eigenvalues of this matrix:" << std::endl;
            std::cout << "  (" << a << " " << b << " " << c <<  std::endl
                      << "   " << d << " " << e << " " << f <<  std::endl
                      << "   " << g << " " << h << " " << i << ")" <<  std::endl;
            //==============================//
            // Special cases
            if(isZero(b) && isZero(c) && isZero(f) &&
               isZero(d) && isZero(h) && isZero(g)) {
                //Diagonal matrix. Life is easy
                *vx1 = a;
                *vy2 = e;
                *vz3 = i;
                *vx2 = *vy3 = *vz1 = C(0.0);
                *vx3 = *vy1 = *vz2 = C(0.0);
                return;
            }
    
            //==============================//
            // Eigenvalues

            C poly_a = C(-1,0);
            C poly_b = a+e+i;
            C poly_c = d*b+g*c+f*h-a*e-a*i-e*i;
            C poly_d = a*e*i - a*f*h - d*b*i + d*c*h + g*b*f - g*c*e;
    
            cubic characteristic_polynomial(poly_a,poly_b,poly_c,poly_d);
            C lambda1,lambda2,lambda3;
            cout << "==========Finding the eigenvalues==========" << endl;
            characteristic_polynomial.solve2(&lambda1,&lambda2,&lambda3);
    
            //==============================//
            // Eigenvectors
            cout << "==========Finding the eigenvectors==========" << endl;
            //Assume the first element of the eigenvector is 1;

            C x1 = C(1.0,0), y1, z1;
            C x2 = C(1.0,0), y2, z2;
            C x3 = C(1.0,0), y3, z3;
            overdetermined
                (b        ,c,
                 e-lambda1,f,
                 h        ,i-lambda1,
                 lambda1-a,-d,-g,
                 &y1,&z1);
            overdetermined
                (b        ,c,
                 e-lambda2,f,
                 h        ,i-lambda2,
                 lambda2-a,-d,-g,
                 &y2,&z2);
            overdetermined
                (b        ,c,
                 e-lambda3,f,
                 h        ,i-lambda3,
                 lambda3-a,-d,-g,
                 &y3,&z3);
            /*
            //First eigenvector
            C x1 = C(1.0,0.0);
            C y1 = (f*lambda1-a*f+d*c)/(c*lambda1 - e*c + b*f);
            C z1 = (h*lambda1-a*h+g*b)/(b*lambda1 - i*b + c*h);

            //Second eigenvector
            C x2 = C(1.0,0.0);
            C y2 = (f*lambda2-a*f+d*c)/(c*lambda2 - e*c + b*f);
            C z2 = (h*lambda2-a*h+g*b)/(b*lambda2 - i*b + c*h);

            //Third eigenvector
            C x3 = C(1.0,0.0);
            C y3 = (f*lambda3-a*f+d*c)/(c*lambda3 - e*c + b*f);
            C z3 = (h*lambda3-a*h+g*b)/(b*lambda3 - i*b + c*h);
            */

            std::cout << "First Eigenvector (Before norming)    : ("
                 << x1 << "," << y1 << "," << z1 << ")" << std::endl;
            std::cout << "Second Eigenvector (Before norming)   : ("
                 << x2 << "," << y2 << "," << z2 << ")" << std::endl;
            std::cout << "Third Eigenvector (Before norming)    : ("
                 << x3 << "," << y3 << "," << z3 << ")" << std::endl;

            

            //==============================//
            // Normalisation

            C norm1=sqrt(x1*std::conj(x1) + y1*std::conj(y1) + z1*std::conj(z1));
            C norm2=sqrt(x2*std::conj(x2) + y2*std::conj(y2) + z2*std::conj(z2));
            C norm3=sqrt(x3*std::conj(x3) + y3*std::conj(y3) + z3*std::conj(z3));

            std::cout << "norm1=" << norm1 << std::endl;
            std::cout << "norm2=" << norm2 << std::endl;
            std::cout << "norm3=" << norm3 << std::endl;

            x1/=norm1;
            y1/=norm1;
            z1/=norm1;

            x2/=norm2;
            y2/=norm2;
            z2/=norm2;

            x3/=norm3;
            y3/=norm3;
            z3/=norm3;

            //==============================//
            // Output

            //We will assume real eigenvectors for now
            *vx1=x1;
            *vy1=y1;
            *vz1=z1;

            *vx2=x2;
            *vy2=y2;
            *vz2=z2;

            *vx3=x3;
            *vy3=y3;
            *vz3=z3;

            *e1 = real(lambda1);
            *e2 = real(lambda2);
            *e3 = real(lambda3);

            //==============================//
            // Sanity checks (can be removed safely)

            C 
                t11 = a*x1+b*y1+c*z1,
                t12 = d*x1+e*y1+f*z1,
                t13 = g*x1+h*y1+i*z1; 
            C 
                t21 = a*x2+b*y2+c*z2,
                t22 = d*x2+e*y2+f*z2,
                t23 = g*x2+h*y2+i*z2; 
            C 
                t31 = a*x3+b*y3+c*z3,
                t32 = d*x3+e*y3+f*z3,
                t33 = g*x3+h*y3+i*z3; 

            std::cout << "First Eigenvector           : ("
                 << x1 << "," << y1 << "," << z1 << ")" << std::endl;
            std::cout << "First Eigenvector tranformed: ("
                 << t11/lambda1 << "," << t12/lambda1 << "," << t13/lambda1 << ")" << std::endl;

            std::cout << "Second Eigenvector           : ("
                 << x2 << "," << y2 << "," << z2 << ")" << std::endl;
            std::cout << "Second Eigenvector tranformed: ("
                 << t21/lambda2 << "," << t22/lambda2 << "," << t23/lambda2 << ")" << std::endl;
            std::cout << "Third Eigenvector           : ("
                 << x3 << "," << y3 << "," << z3 << ")" << std::endl;
            std::cout << "Third Eigenvector tranformed: ("
                 << t31/lambda3 << "," << t32/lambda3 << "," << t33/lambda3 << ")" << std::endl;
}


template<typename T,typename D>
class Vector3_t {
public:
    typedef dreal<T,D> field;
    Vector3_t() {}
    Vector3_t(const Vector3_t& v) 
        : x(v.x),y(v.y),z(v.z) {
    }
    Vector3_t(field _x,field _y,field _z)
        : x(_x),y(_y),z(_z) {
    }

    ///Sets it's arguments to the value of the vector's z,y and z
    ///components
    void GetCoordinates(field* _x,field* _y, field* _z) const {
        *_x=x;
        *_y=y;
        *_z=z;
    }
    ///Sets it's arguments to the value of the vector's z,y and z
    ///components in SI units
    void GetCoordinatesSI(T* _x,T* _y, T* _z) const {
        *_x=x.si;
        *_y=y.si;
        *_z=z.si;
    }
    ///Sets the vector's x,y and z coordinates
    void SetCoordinates(field _x,field _y, field _z) {
        x=_x;
        y=_y;
        z=_z;
    }

    ///Sets the vector's x,y and z coordinates in SI units
    void SetCoordinatesSI(T _x,T _y, T _z) {
        x.si=_x;
        y.si=_y;
        z.si=_z;
    }
    ///normalises the vector in place
    void normalise() {
        double inv_length=1/sqrt(x.si*x.si+y.si*y.si+z.si*z.si);
        x.si*=inv_length;
        y.si*=inv_length;
        z.si*=inv_length;                
    }
    ///normalises the vector
    Vector3_t normalised() const {
        double inv_length=1/sqrt(x.si*x.si+y.si*y.si+z.si*z.si);
        Vector3_t retVal;
        retVal.x.si=x.si*inv_length;
        retVal.y.si=y.si*inv_length;
        retVal.z.si=z.si*inv_length;
        return retVal;
    }

    Vector3_t operator+(const Vector3_t& v) const {
        return Vector3_t(x+v.x,y+v.y,z+v.z);
    }
    Vector3_t operator-(const Vector3_t& v) const {
        return Vector3_t(x-v.x,y-v.y,z-v.z);
    }
    ///Get the X component 
    field GetX() const {return x;}
    ///Get the Y component 
    field GetY() const {return y;}
    ///Get the Z component 
    field GetZ() const {return z;}

    ///Set the X component 
    void SetX(field _x) {x=_x;}
    ///Set the Y component 
    void SetY(field _y) {y=_y;}
    ///Set the Z component 
    void SetZ(field _z) {z=_z;}

    ///Returns the magnitude of the vector
    field GetLength() const {
        sqrt(x*x+y*y+z*z);
        return field(3.0);
    }


private:
    field x;
    field y;
    field z;
};
    typedef Vector3_t<double,_unitless> Vector3;

///Class for storeing a 3x3 matrix defined over a field with known
///units. Behaves as a value type, whihc is probably fine for an
///object with only 9 floating point numbers. Obviously you'd want
///to be cleverer if you wanted to scale this up.
template<typename T,typename D>
class Matrix3_t {
public:
    typedef dreal<T,D> field;
    typedef complex<field> cplex;
    typedef Vector3_t<T,D> Vector3T;
    ///Default constructor. Constructs the identity
    Matrix3_t() {
        static const double IDENTITY3[]={1.0,0.0,0.0,
                                         0.0,1.0,0.0,
                                         0.0,0.0,1.0};
        memcpy(raw,IDENTITY3,9*sizeof(field));
    }
    ///Constructs a specific 3x3 matrix
    Matrix3_t(const field a00,const field a01,const field a02,
              const field a10,const field a11,const field a12,
              const field a20,const field a21,const field a22) {
        raw[0]=a00;   raw[1]=a01;   raw[2]=a02;
        raw[3]=a10;   raw[4]=a11;   raw[5]=a12;
        raw[6]=a20;   raw[7]=a21;   raw[8]=a22;
    }
    template <class OtherDimensions>
    Matrix3_t(Matrix3_t<T,OtherDimensions> const& rhs) {
        memcpy(raw,rhs.GetRaw(),9*sizeof(field));
        BOOST_STATIC_ASSERT((mpl::equal<D,OtherDimensions>::type::value));
    }

    ///Copies the matrix
    Matrix3_t(const Matrix3_t& m) {
        memcpy(raw,m.raw,9*sizeof(field));
    }
    ///Set the value of one matrix to the value of the second.
    const Matrix3_t& operator=(const Matrix3_t& m) {
        memcpy(raw,m.raw,9*sizeof(field));
        return *this;
    }

    ///Get the trace of the matrix
    field Trace() const {return raw[0]+raw[4]+raw[8];}

    ///Get the transpose of the matrix
    Matrix3_t Transposed() const {
        Matrix3_t result;
        result.raw[0]=raw[0];
        result.raw[4]=raw[4];
        result.raw[8]=raw[8];

        result.raw[1]=raw[3];
        result.raw[2]=raw[6];
        result.raw[5]=raw[7];

        result.raw[3]=raw[1];
        result.raw[6]=raw[2];
        result.raw[7]=raw[5];
        return result;
    }
    ///Transpose the matrix in place
    void Transpose() {
        field one =raw[1];
        field two =raw[2];
        field five=raw[5];
        raw[1]=raw[3];
        raw[2]=raw[6];
        raw[5]=raw[7];

        raw[3]=raw[1];
        raw[6]=raw[2];
        raw[7]=raw[5];
    }

    typedef dreal<T,typename mpl::transform<D,_by_three,times_by_f>::type > det_t;
    det_t det() const {
        return
            raw[0]*(raw[8]*raw[4]-raw[7]*raw[5])
            -raw[3]*(raw[8]*raw[1]-raw[7]*raw[2])
            +raw[6]*(raw[5]*raw[1]-raw[4]*raw[2]);
    }

    ///Return a copy of the matrix inverted.
    typedef Matrix3_t<T,typename mpl::transform<D,negate_f>::type> inverse_t;
    inverse_t Inverted() {
        det_t Det = det();

        inverse_t mat;
        mat.Set(0,0,  (raw[8]*raw[4]-raw[7]*raw[5]) / Det);
        mat.Set(0,1, -(raw[8]*raw[3]-raw[6]*raw[5]) / Det);
        mat.Set(0,2,  (raw[7]*raw[3]-raw[6]*raw[4]) / Det);
                                                        
        mat.Set(1,0, -(raw[8]*raw[1]-raw[7]*raw[2]) / Det);
        mat.Set(1,1,  (raw[8]*raw[0]-raw[6]*raw[2]) / Det);
        mat.Set(1,2, -(raw[7]*raw[0]-raw[6]*raw[1]) / Det);
                                                      
        mat.Set(2,0,  (raw[5]*raw[1]-raw[4]*raw[2]) / Det);
        mat.Set(2,1, -(raw[5]*raw[0]-raw[3]*raw[2]) / Det);
        mat.Set(2,2,  (raw[4]*raw[0]-raw[3]*raw[1]) / Det);

        return mat;
    }

    ///Obtain the complex eigenvalues and
    ///eigenvectors. Correctness and simplicity is favored over
    ///speed here so it is recomended that the result is cached
    void eig(field& e1,
             field& e2,
             field& e3,
             Vector3T& v1,
             Vector3T& v2,
             Vector3T& v3
             ) const {
            
        //Perform all calculations at long double precision
        //because rounding errors do build up.
        typedef complex<long double> C;

        //==============================//
        // Bookeeping

        C a   = C(raw[0].si,0.0), b = C(raw[1].si,0.0), c = C(raw[2].si,0.0),
            d = C(raw[3].si,0.0), e = C(raw[4].si,0.0), f = C(raw[5].si,0.0),
            g = C(raw[6].si,0.0), h = C(raw[7].si,0.0), i = C(raw[8].si,0.0);

        std::cout << "Finding the eigenvalues of this matrix:" << std::endl;
        std::cout << "  (" << a << " " << b << " " << c <<  std::endl
                  << "   " << d << " " << e << " " << f <<  std::endl
                  << "   " << g << " " << h << " " << i << ")" <<  std::endl;
    
        //==============================//
        // Eigenvalues

        C poly_a = C(-1,0);
        C poly_b = a+e+i;
        C poly_c = d*b+g*c+f*h-a*e-a*i-e*i;
        C poly_d = a*e*i - a*f*h - d*b*i + d*c*h + g*b*f - g*c*e;
    
        cubic characteristic_polynomial(poly_a,poly_b,poly_c,poly_d);
        C lambda1,lambda2,lambda3;
        characteristic_polynomial.solve(&lambda1,&lambda2,&lambda3);
    
        //==============================//
        // Eigenvectors

        //Assume the first element of the eigenvector is 1;
            
        //First eigenvector
        C x1 = C(1.0,0.0);
        C y1 = (f*lambda1-a*f+d*c)/(c*lambda1 - e*c + b*f);
        C z1 = (h*lambda1-a*h+g*b)/(b*lambda1 - i*b + c*h);

        //Second eigenvector
        C x2 = C(1.0,0.0);
        C y2 = (f*lambda2-a*f+d*c)/(c*lambda2 - e*c + b*f);
        C z2 = (h*lambda2-a*h+g*b)/(b*lambda2 - i*b + c*h);

        //Third eigenvector
        C x3 = C(1.0,0.0);
        C y3 = (f*lambda3-a*f+d*c)/(c*lambda3 - e*c + b*f);
        C z3 = (h*lambda3-a*h+g*b)/(b*lambda3 - i*b + c*h);

        //==============================//
        // Normalisation

        C norm1=sqrt(x1*std::conj(x1) + y1*std::conj(y1) + z1*std::conj(z1));
        C norm2=sqrt(x2*std::conj(x2) + y2*std::conj(y2) + z2*std::conj(z2));
        C norm3=sqrt(x3*std::conj(x3) + y3*std::conj(y3) + z3*std::conj(z3));

        std::cout << "norm1=" << norm1 << std::endl;
        std::cout << "norm2=" << norm1 << std::endl;
        std::cout << "norm3=" << norm1 << std::endl;

        x1/=norm1;
        y1/=norm1;
        z1/=norm1;

        x2/=norm2;
        y2/=norm2;
        z2/=norm2;

        x3/=norm3;
        y3/=norm3;
        z3/=norm3;

        //==============================//
        // Output

        //We will assume real eigenvectors for now
        v1=Vector3T(field(real(x1)),
                    field(real(y1)),
                    field(real(z1)));
        v2=Vector3T(field(real(x2)),
                    field(real(y2)),
                    field(real(z2)));
        v3=Vector3T(field(real(x3)),
                    field(real(y3)),
                    field(real(z3)));

        e1 = field(real(lambda1));
        e2 = field(real(lambda2));
        e3 = field(real(lambda3));

        //==============================//
        // Sanity checks (can be removed safely)

        C 
            t11 = a*x1+b*y1+c*z1,
            t12 = d*x1+e*y1+f*z1,
            t13 = g*x1+h*y1+i*z1; 
        C 
            t21 = a*x2+b*y2+c*z2,
            t22 = d*x2+e*y2+f*z2,
            t23 = g*x2+h*y2+i*z2; 
        C 
            t31 = a*x3+b*y3+c*z3,
            t32 = d*x3+e*y3+f*z3,
            t33 = g*x3+h*y3+i*z3; 

        std::cout << "First Eigenvector           : ("
                  << x1 << "," << y1 << "," << z1 << ")" << std::endl;
        std::cout << "First Eigenvector tranformed: ("
                  << t11/lambda1 << "," << t12/lambda1 << "," << t13/lambda1 << ")" << std::endl;

        std::cout << "Second Eigenvector           : ("
                  << x2 << "," << y2 << "," << z2 << ")" << std::endl;
        std::cout << "Second Eigenvector tranformed: ("
                  << t21/lambda2 << "," << t22/lambda2 << "," << t23/lambda2 << ")" << std::endl;
        std::cout << "Third Eigenvector           : ("
                  << x3 << "," << y3 << "," << z3 << ")" << std::endl;
        std::cout << "Third Eigenvector tranformed: ("
                  << t31/lambda3 << "," << t32/lambda3 << "," << t33/lambda3 << ")" << std::endl;

    }

    ///Project the matrix into SO(3) in place
    void Othogonalise() {
        //TODO
    }

    ///Project the matrix into SO(3)
    Matrix3_t Othogonalised() const {
        //TODO
        return Matrix3_t();
    }

    
    ///Get a pointer to the matrix in memory. The matrix is stored in
    ///row major form (GetRaw()[0]=a00,GetRaw()[1]=a01, etc.)
    const field* GetRaw() const {return raw;}
    
    ///Get a specific value by row and column index.
    field Get(long column, long row) const {
        return raw[3*column+row];
    }
    ///Get a specific value by row and column index.
    field GetSI(long column, long row) const {
        return raw[3*column+row].si;
    }
    ///Get a specific value by row and column
    field operator() (long column, long row) const {
        return raw[3*column+row];
    }
    ///Add together two matricese in the standard fasion.
    Matrix3_t operator+ (const Matrix3_t& m) const {
        Matrix3_t result;
        result.raw[0]=raw[0]+m.raw[0];
        result.raw[1]=raw[1]+m.raw[1];
        result.raw[2]=raw[2]+m.raw[2];
	      	     	       
        result.raw[3]=raw[3]+m.raw[3];
        result.raw[4]=raw[4]+m.raw[4];
        result.raw[5]=raw[5]+m.raw[5];
	      	     	       
        result.raw[6]=raw[6]+m.raw[6];
        result.raw[7]=raw[7]+m.raw[7];
        result.raw[8]=raw[8]+m.raw[8];
        return result;
    }
    ///Subtract together two matricese in the standard fasion.
    Matrix3_t operator-(const Matrix3_t& m) const {
        Matrix3_t result;
        result.raw[0]=raw[0]-m.raw[0];
        result.raw[1]=raw[1]-m.raw[1];
        result.raw[2]=raw[2]-m.raw[2];
	      	     	       
        result.raw[3]=raw[3]-m.raw[3];
        result.raw[4]=raw[4]-m.raw[4];
        result.raw[5]=raw[5]-m.raw[5];
	      	     	       
        result.raw[6]=raw[6]-m.raw[6];
        result.raw[7]=raw[7]-m.raw[7];
        result.raw[8]=raw[8]-m.raw[8];
        return result;
    }
    ///Multiply two matricese together
    template<typename D2>
    Matrix3_t<T,typename mpl::transform<D,D2,plus_f>::type>
    operator*(const Matrix3_t<T,D2>& m) const {
        typedef typename mpl::transform<D,D2,plus_f>::type dim;
        const dreal<T,D2>* mraw = m.GetRaw();
        Matrix3_t<T,dim> 
            result(raw[0]*mraw[0] + raw[1]*mraw[3] + raw[2]*mraw[6],
                   raw[0]*mraw[1] + raw[1]*mraw[4] + raw[2]*mraw[7],
                   raw[0]*mraw[2] + raw[1]*mraw[5] + raw[2]*mraw[8],
		       		      		      	 	       	        
                   raw[3]*mraw[0] + raw[4]*mraw[3] + raw[5]*mraw[6],
                   raw[3]*mraw[1] + raw[4]*mraw[4] + raw[5]*mraw[7],
                   raw[3]*mraw[2] + raw[4]*mraw[5] + raw[5]*mraw[8],
		       		      		      	 	       	        
                   raw[6]*mraw[0] + raw[7]*mraw[3] + raw[8]*mraw[6],
                   raw[6]*mraw[1] + raw[7]*mraw[4] + raw[8]*mraw[7],
                   raw[6]*mraw[2] + raw[7]*mraw[5] + raw[8]*mraw[8]);
        return result;
    }
    ///Multiply a matrix my a scalar
    template<typename D2>
    Matrix3_t<T,typename mpl::transform<D,D2,plus_f>::type>
    operator* (const dreal<T,D2> s) const {
        Matrix3_t<T,typename mpl::transform<D,D2,plus_f>::type>
            result(raw[0]*s,raw[1]*s,raw[2]*s,
                   raw[3]*s,raw[4]*s,raw[5]*s,
                   raw[6]*s,raw[7]*s,raw[8]*s);
        return result;
    }
    ///Multiply a matrix by a scalar
    Matrix3_t operator*= (dreal<T,_unitless> s) {
        raw[0]=raw[0]*s;
        raw[1]=raw[1]*s;
        raw[2]=raw[2]*s;
       	     	 
        raw[3]=raw[3]*s;
        raw[4]=raw[4]*s;
        raw[5]=raw[5]*s;
       	         
        raw[6]=raw[6]*s;
        raw[7]=raw[7]*s;
        raw[8]=raw[8]*s;
    }
    ///Add together two matricese in the standard fasion and set the
    ///value of the first to the result
    Matrix3_t& operator+= (const Matrix3_t& m) {
        raw[0]=raw[0]+m.raw[0];
        raw[1]=raw[1]+m.raw[1];
        raw[2]=raw[2]+m.raw[2];
       	     	       
        raw[3]=raw[3]+m.raw[3];
        raw[4]=raw[4]+m.raw[4];
        raw[5]=raw[5]+m.raw[5];
       	     	       
        raw[6]=raw[6]+m.raw[6];
        raw[7]=raw[7]+m.raw[7];
        raw[8]=raw[8]+m.raw[8];
        return *this;
    }
    ///Set a specific element to the value of val
    void Set(long column,long row,field val) {
        raw[3*column+row]=val;
    }
    void Dump(unit<D> u) const {
        std::cout << "Matrix3:" << std::endl
                  << " (" << raw[0][u] << " " <<  raw[1][u] << " " <<  raw[2][u] << std::endl
                  << "  " << raw[3][u] << " " <<  raw[4][u] << " " <<  raw[5][u] << std::endl
                  << "  " << raw[6][u] << " " <<  raw[7][u] << " " <<  raw[8][u] << ")" << std::endl;
    }

private:
    field raw[9];
};
typedef Matrix3_t<complex<double>, _unitless> Matrix3c;
typedef Matrix3_t<complex<long double>, _unitless> Matrix3cl;





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
    eig(C(1.0,0.0),C(2.0,0.0),C(0.0,0.0),
        C(0.0,0.0),C(2.0,0.0),C(3.0,0.0),
        C(0.0,0.0),C(0.0,0.0),C(3.0,0.0),
        &e1,&e2,&e3,
        &vx1,&vy1,&vz1,
        &vx2,&vy2,&vz2,
        &vx3,&vy3,&vz3);

    return 0;
}



