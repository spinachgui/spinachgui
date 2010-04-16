
#include <cmath>
#include <complex>
#include <shared/basic_math.hpp>

namespace SpinXML {

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

            std::cout << "f(" << *x1 << ")=" << eval(*x1) << std::endl;
            std::cout << "f(" << *x2 << ")=" << eval(*x2) << std::endl;
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

            std::cout << "==========x**3 + p x - q=0 Solver=========="<< std::endl;
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

            std::cout << "The w3 solutions are:" << w1 << " " << w2 << std::endl;
        
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

            std::cout << "The six solutions for w are:" << std::endl
                 << "w1_1 = " << w1_1 << std::endl 
                 << "w1_2 = " << w1_2 << std::endl 
                 << "w1_3 = " << w1_3 << std::endl 
                 << "w2_1 = " << w2_1 << std::endl
                 << "w2_2 = " << w2_2 << std::endl
                 << "w2_3 = " << w2_3 << std::endl;

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

        ///Find the complex roots of the polynomial
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
        ///Find the complex roots of the polynomial
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
            std::cout << "The shifted cubic is "
                 << "x**3 + "
                 << p << "x**1 + "
                 << -q << " = 0" << std::endl;
            reduced_cubic f(p,q);
            C x1,x2,x3;
            f.solve(&x1,&x2,&x3);
        
            std::cout << "========== Solved the reduced cubic" << std::endl;

            *z1 = x1 - a2/C(3.0,0);
            *z2 = x2 - a2/C(3.0,0);
            *z3 = x3 - a2/C(3.0,0);

            std::cout << "f(" << *z1 << ")="  << eval(*z1) << std::endl;
            std::cout << "f(" << *z2  << ")=" << eval(*z2) << std::endl;
            std::cout << "f(" << *z3 << ")="  << eval(*z3) << std::endl;
        
            *z1=newton_opt(*z1,5);
            *z2=newton_opt(*z2,5);
            *z3=newton_opt(*z3,5);

            std::cout << "Solutions after newton method cleanup" << std::endl;
            std::cout << "f(" << *z1 << ")="  << eval(*z1) << std::endl;
            std::cout << "f(" << *z2  << ")=" << eval(*z2) << std::endl;
            std::cout << "f(" << *z3 << ")="  << eval(*z3) << std::endl;
        

            return;
        }
        ///Take a value close to f(x)=0 and find a better value
        C newton_opt(C x,long times) const {
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

}

