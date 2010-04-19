
#ifndef __MATHTYPES___H_
#define __MATHTYPES___H_

#include <cstring>
#include <string>
#include <cmath>
#include <complex>
#include <shared/unit.hpp>
#include <shared/polynomial.hpp>
#include <boost/variant.hpp>

#define PI 3.141592653589793238462643
using std::complex;

namespace SpinXML {

    ///Plank's constant in SI units
    extern const double hbar;

    ///Bohr magneton in SI units
    extern const double bohr_mag;
    
    ///Ther permeability of free space
    extern const double mu0;


    void standalone_eig(C a, C b, C c,
                        C d, C e, C f,
                        C g, C h, C i,
                        C* e1,C* e2, C* e3,
                        C* vx1,C* vy1, C* vz1,
                        C* vx2,C* vy2, C* vz2,
                        C* vx3,C* vy3, C* vz3);



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
            
            //==============================//
            // Bookeeping

            C a   = C(raw[0].si,0.0), b = C(raw[1].si,0.0), c = C(raw[2].si,0.0),
                d = C(raw[3].si,0.0), e = C(raw[4].si,0.0), f = C(raw[5].si,0.0),
                g = C(raw[6].si,0.0), h = C(raw[7].si,0.0), i = C(raw[8].si,0.0);
 
            C xx,yy,zz,
                vx1, vy1,  vz1,
                vx2, vy2,  vz2,
                vx3, vy3,  vz3;
            standalone_eig(a,b,c,
                           d,e,f,
                           g,h,i,
                           &xx,  &yy,  &zz,
                           &vx1, &vy1, &vz1,
                           &vx2, &vy2, &vz2,
                           &vx3, &vy3, &vz3);

            //==============================//
            // Output

            //We will assume real eigenvectors for now
            v1=Vector3T(field(real(vx1)),
                        field(real(vy1)),
                        field(real(vz1))).normalised();
            v2=Vector3T(field(real(vx2)),
                        field(real(vy2)),
                        field(real(vz2))).normalised();
            v3=Vector3T(field(real(vx3)),
                        field(real(vy3)),
                        field(real(vz3))).normalised();

            /*v1=Vector3T(field(1),
                        field(0),
                        field(0));
            v2=Vector3T(field(0),
                        field(1),
                        field(0));
            v3=Vector3T(field(0),
                        field(0),
                        field(1));*/


            e1 = field(real(xx));
            e2 = field(real(yy));
            e3 = field(real(zz));
        }

        ///Project the matrix into SO(3) in place
        void Othogonalise() {
            //TODO
        }

        ///Project the matrix into SO(3)
        Matrix3_t Othogonalised() const {
            //TODO
            return Matrix3_t(*this);
        }

        field FrobeniusNorm() const {
            return sqrt(
                        raw[0]*raw[0]+
                        raw[1]*raw[1]+
                        raw[2]*raw[2]+

                        raw[3]*raw[3]+
                        raw[4]*raw[4]+
                        raw[5]*raw[5]+

                        raw[6]*raw[6]+
                        raw[7]*raw[7]+
                        raw[8]*raw[8]);
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
    typedef Matrix3_t<double,_unitless> Matrix3;
    typedef Matrix3_t<complex<long double>, _unitless> Matrix3cl;
    ///Class for storeing a 4x4 matrix. Mostly used for graphics, but
    ///could be useful elsewhere...
    class Matrix4 {
    public:
	///Default constructor. Constructs the identity
	Matrix4();
	///Constructs a specific 3x3 matrix
	Matrix4(double a00,double a01,double a02,double a03,
		double a10,double a11,double a12,double a13,
		double a20,double a21,double a22,double a23,
		double a30,double a31,double a32,double a33);
	///Copies the matrix
	Matrix4(const Matrix4& m);

	///Constructs the matrix from a 3x3 matrix
	Matrix4(const Matrix3&m);

	///Print the matrix to the standard output in a human readable
	///format
	void Dump() const;

	///Get the trace of the matrix
	double Trace() const {return raw[0]+raw[5]+raw[10]+raw[15];}

	///Get the transpose of the matrix
	Matrix4 Transpose() const;
    
	///Get a pointer to the matrix in memory. The matrix is stored in
	///row major form (GetRaw()[0]=a00,GetRaw()[1]=a01, etc.)
	const double* GetRaw() const;
    
	///Get a specific value by row and column index.
	double Get(long column, long row) const;
	///Get a specific value by row and column
	double operator() (long column, long row) const;
	///Add together two matricese in the standard fasion.
	Matrix4 operator+ (const Matrix4& m) const;
	///Multiply two matricese together
	Matrix4 operator* (const Matrix4& m) const;
	///Add together two matricese in the standard fasion and set the
	///value of the first to the result
	Matrix4& operator+= (const Matrix4& m);
	///Set the value of one matrix to the value of the second.
	Matrix4& operator= (const Matrix4& m);
	///Set a specific element to the value of val
	void Set(long column,long row,double val);
    private:
	double raw[16];
    };


    ///Euler angle type
    struct EulerAngles {
        double alpha,beta,gamma;
        EulerAngles(double _alpha,double _beta, double _gamma) 
            : alpha(_alpha),beta(_beta),gamma(_gamma) {

        }
        void Normalize() {
            
        }
        EulerAngles Normalized() const {
            
        }

    };

    struct AngleAxis {
        double angle;
        Vector3 axis;
        AngleAxis(double _angle,const Vector3& _axis) 
            : angle(_angle),axis(_axis) {
        }
        void Normalise() {
            axis.normalise();
        }
        AngleAxis Normalised() const {
            return AngleAxis(angle,axis.normalised());
        }

    };


    /// Simple Quaternion Type
    struct Quaternion {
        double w,x,y,z;
        Quaternion(){}
        Quaternion(double _w,double _x, double _y, double _z) 
            : w(_w),x(_x),y(_y),z(_z){
        }
        Quaternion(double angle,const Vector3& axis) {
            SetAngleAxis(angle,axis);
        }
        Quaternion operator*(const Quaternion& q2) {
            Quaternion res;
            double w1=w; double w2=q2.w;
            double x1=x; double x2=q2.x;
            double y1=y; double y2=q2.y;
            double z1=z; double z2=q2.z;
            res.w=w1*w2 - x1*x2 - y1*y2 - z1*z2;
            res.x=w1*x2 + x1*w2 + y1*z2 - z1*y2;
            res.y=w1*y2 - x1*z2 + y1*w2 + z1*x2;
            res.z=w1*z2 + x1*y2 - y1*x2 + z1*w2;	

            return res;
        }
        Vector3 Transform(Vector3 v) {
            Quaternion v_as_q;
            v_as_q.w=0;
            v_as_q.x=v.GetX();
            v_as_q.y=v.GetY();
            v_as_q.z=v.GetZ();

            v_as_q=(*this)*v_as_q*Quaternion(w,-x,-y,-z);

            Vector3 ret;
            ret.SetX(v_as_q.x);
            ret.SetY(v_as_q.y);
            ret.SetZ(v_as_q.z);
            return ret;
        }
        void SetAngleAxis(double a,const Vector3& v) {
            double _x=v.GetX();
            double _y=v.GetY();
            double _z=v.GetZ();

            double norm = sqrt(_x*_x+_y*_y+_z*_z);
            _x/=norm;
            _y/=norm;
            _z/=norm;

            double sin_a=sin(a/2);
            w=cos(a/2);
            x=_x*sin_a;
            y=_y*sin_a;
            z=_z*sin_a;
        }
        double norm() {
            return sqrt(w*w+x*x+y*y+z*z);
        }
        void Normalise() {
            double norm=sqrt(w*w+x*x+y*y+z*z);
            w/=norm;
            x/=norm;
            y/=norm;
            z/=norm;
        }
        Quaternion Normalised() const {
            double norm=sqrt(w*w+x*x+y*y+z*z);
            w*norm;
            x*norm;
            y*norm;
            z*norm;
            return Quaternion(w,x,y,z);
        }

        Quaternion Conjugulate() {
            return Quaternion(w,-x,-y,-z);
        }
        EulerAngles ToEuler() {
            Vector3 z_axis=Vector3(0,0,1);
            Vector3 x_axis=Vector3(1,0,0);
            z_axis=Transform(z_axis);
            x_axis=Transform(x_axis);
    
            double gamma=atan2(z_axis.GetY(),z_axis.GetX());
            double beta=atan2(sqrt(z_axis.GetX()*z_axis.GetX() + z_axis.GetY()*z_axis.GetY()).si,z_axis.GetZ().si);

            //Use γ and β to rotate V2 back onto the point 0,0,1
            Quaternion gammaTwist,betaTwist;
            betaTwist.SetAngleAxis(-beta,Vector3(0,1,0));
            gammaTwist.SetAngleAxis(-gamma,Vector3(0,0,1));

            x_axis=(betaTwist*gammaTwist).Transform(x_axis);

            double alpha = atan2(x_axis.GetY(),x_axis.GetX());
            if(alpha < 0 || alpha >= 2*PI)  alpha = alpha-2*PI*floor(alpha/(2*PI));
            if(alpha >= 2*PI) alpha = 0;
            if(beta < 0 || beta >= PI)    beta = beta-  PI*floor(beta/PI);
            if(gamma < 0 || gamma >= 2*PI)  gamma = gamma-2*PI*floor(gamma/(2*PI));
            if(gamma >= 2*PI) gamma = 0;
            return EulerAngles(alpha,beta,gamma);
        }

    };



    ///Class for storing a 3 dimentional rotation
    class Orientation {
    public:
	///Constructs and orientaion of type Orientation::UNDEFINED with all
	///other data uninialised.
	Orientation();
	///Constructs an orientation from euler angles
	Orientation(const EulerAngles& ea) : mData(ea) {}
	///Constructs an orientation from a matrix
	Orientation(const Matrix3& m) : mData(m) {}
	///Constructs an orientation from a quaternion
	Orientation(const Quaternion& q) : mData(q) {}
	///Constructs an orientation from an AngleAxis
	Orientation(const AngleAxis& aa) : mData(aa) {}
	///Destructor
	~Orientation() {};
    
	///Enumeration of the four conventions on storing rotations
	enum Type {
	    UNDEFINED,
	    EULER,
	    ANGLE_AXIS,
	    QUATERNION,
	    EIGENSYSTEM
	};
    
        ///Normalise the contained rotation in place.
        void Normalise();
        ///Normalise the contained rotation.
        Orientation Normalised() const;

	///Returns the convention that is being used to store the rotation
	Type GetType() const;

	///If the euler angle convention is being used then this function
	///will set its arguments to the stored values otherwise the result
	///is undefined.
	void GetEuler(double* alpha,double* beta,double* gamma) const;
	///If the angle axis convention being used then this function will
	///set its arugments to those values otherwise the result is
	///undefined.
	void GetAngleAxis(double* angle,Vector3* axis) const;
	///If the quaternion convention being used then this function will
	///set its arguments to those values otherwise the result is
	///undefined.
	void GetQuaternion(double* real, double* i, double* j, double* k) const;
	///If the eigensystem convention is being used then this function
	///will set its arguments to the appropriate values otherwise the
	///result is undefined.
	void GetEigenSystem(Vector3* XAxis,Vector3* YAxis, Vector3* ZAxis) const;

	///Set the value of the rotation in terms of euler angles. This
	///function also sets the type value as returned by GetType()
	void SetEuler(double alpha,double beta,double gamma);
	///Set the value of the rotation in terms of the angle axis
	///paramiters. This function also sets the type value as returned by
	///GetType()
	void SetAngleAxis(double angle,const Vector3& axis);
	///Set the value of the rotation in terms of quaternion
	///paramiters. This function also sets the type value as returned by
	///GetType()
	void SetQuaternion(double real, double i, double j, double k);
	///Set the value of the rotation in terms of eigensystem. This
	///function also sets the type value as returned by GetType()
	void SetEigenSystem(const Vector3& XAxis,const Vector3& YAxis, const Vector3& ZAxis);

	///Converts to a rotation matrix
	Matrix3 GetAsMatrix() const;

	///returns a representation of the orientation as a string that is
	///both machiene parsable and human readable.
	std::string ToString() const;
	///Parses an orentation from a string representation. The form of
	///the representation should be the same as returned by ToString
	void FromString(std::string string);

        Orientation GetAsEuler() const;
        Orientation GetAsEigenSystem() const;
        Orientation GetAsAngleAxis() const;
        Orientation GetAsQuaternion() const;

    private:
        typedef boost::variant<EulerAngles,AngleAxis,Quaternion,Matrix3> var_t;
        var_t mData;
    };
};
#endif
