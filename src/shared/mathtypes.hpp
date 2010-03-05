
#ifndef __MATHTYPES___H_
#define __MATHTYPES___H_

#include <cstring>
#include <string>
#include <cmath>
#include <shared/unit.hpp>
#include <shared/dstegr.hpp>

namespace SpinXML {

    ///Plank's constant in SI units
    extern const double hbar;

    ///Bohr magneton in SI units
    extern const double bohr_mag;
    
    ///Ther permeability of free space
    extern const double mu0;

    ///Class for storing a 3 dimensional vector
    template<typename T>
    class Vector3_t {
    public:
        ///Constructs an uninitalised vector
        Vector3_t(){}
        ///Copy Constructor
        Vector3_t(const Vector3_t& v) 
            :  x(v.x),y(v.y),z(v.z) {
        }
        ///Constructs a new vector
        Vector3_t(T _x,T _y,T _z) 
            : x(_x),y(_y),z(_z) {
        }

        ///Sets it's arguments to the value of the vector's z,y and z
        ///components
        void GetCoordinates(T* _x,T* _y, T* _z) const {
            *_x=x;
            *_y=y;
            *_z=z;
        }
        ///Sets it's arguments to the value of the vector's z,y and z
        ///components in SI units
        void GetCoordinatesSI(double* _x,double* _y, double* _z) const {
            *_x=x.si;
            *_y=y.si;
            *_z=z.si;
        }
        ///Sets the vector's x,y and z coordinates
        void SetCoordinates(T _x,T _y, T _z) {
            x=_x;
            y=_y;
            z=_z;
        }
        ///Sets the vector's x,y and z coordinates in SI units
        void SetCoordinatesSI(double _x,double _y, double _z) {
            x.si=_x;
            y.si=_y;
            z.si=_z;
        }

        ///Get the X component 
        T GetX() const {return x;}
        ///Get the Y component 
        T GetY() const {return y;}
        ///Get the Z component 
        T GetZ() const {return z;}

        ///Set the X component 
        void SetX(T _x) {x=_x;}
        ///Set the Y component 
        void SetY(T _y) {y=_y;}
        ///Set the Z component 
        void SetZ(T _z) {z=_z;}

        ///Returns the magnitude of the vector
        T length() const {
            return sqrt(x*x+y*y+z*z);
        }

        ///normalises the vector
        void normalise() {
            double inv_length=1/sqrt(x.si*x.si+y.si*y.si+z.si*z.si);
            x*=inv_length;
            y*=inv_length;
            z*=inv_length;                
        }

        Vector3_t operator+(const Vector3_t& v) const {
            return Vector3_t(x+v.x,y+v.y,z+v.z);
        }
        Vector3_t operator-(const Vector3_t& v) const {
            return Vector3_t(x-v.x,y-v.y,z-v.z);
        }

    private:
        T x;
        T y;
        T z;
    };
    typedef Vector3_t<double> Vector3;


    ///Class for storeing a 3x3 matrix defined over a field of
    ///double. Behaves as a value type, whihc is probably fine for an
    ///object with only 9 floating point numbers. Obviously you'd want
    ///to be cleverer if you wanted to scale this up.
    class Matrix3 {
    public:
        typedef double field;
        ///Default constructor. Constructs the identity
        Matrix3() {
            static const double IDENTITY3[]={1.0,0.0,0.0,
                                             0.0,1.0,0.0,
                                             0.0,0.0,1.0};
            memcpy(raw,IDENTITY3,9*sizeof(field));
        }
        ///Constructs a specific 3x3 matrix
        Matrix3(field a00,field a01,field a02,
                field a10,field a11,field a12,
                field a20,field a21,field a22) {
            raw[0]=a00;   raw[1]=a01;   raw[2]=a02;
            raw[3]=a10;   raw[4]=a11;   raw[5]=a12;
            raw[6]=a20;   raw[7]=a21;   raw[8]=a22;
        }

        ///Copies the matrix
        Matrix3(const Matrix3& m) {
            memcpy(raw,m.raw,9*sizeof(field));
        }

        ///Get the trace of the matrix
        field Trace() const {return raw[0]+raw[4]+raw[8];}

        ///Get the transpose of the matrix
        Matrix3 Transpose() const {
            Matrix3 result;
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
        void Transposed() {
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
    
        ///Get a pointer to the matrix in memory. The matrix is stored in
        ///row major form (GetRaw()[0]=a00,GetRaw()[1]=a01, etc.)
        const field* GetRaw() const {return raw;}
    
        ///Get a specific value by row and column index.
        field Get(long column, long row) const {
            return raw[3*column+row];
        }
        ///Get a specific value by row and column
        field operator() (long column, long row) const {
            return raw[3*column+row];
        }
        ///Add together two matricese in the standard fasion.
        Matrix3 operator+ (const Matrix3& m) const {
            Matrix3 result;
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
        ///Multiply two matricese together
        Matrix3 operator* (const Matrix3& m) const {
            Matrix3 result;
            result.raw[0]=raw[0]*m.raw[0] + raw[1]*m.raw[3] + raw[2]*m.raw[6];
            result.raw[1]=raw[0]*m.raw[1] + raw[1]*m.raw[4] + raw[2]*m.raw[7];
            result.raw[2]=raw[0]*m.raw[2] + raw[1]*m.raw[5] + raw[2]*m.raw[8];

            result.raw[3]=raw[3]*m.raw[0] + raw[4]*m.raw[3] + raw[5]*m.raw[6];
            result.raw[4]=raw[3]*m.raw[1] + raw[4]*m.raw[4] + raw[5]*m.raw[7];
            result.raw[5]=raw[3]*m.raw[2] + raw[4]*m.raw[5] + raw[5]*m.raw[8];

            result.raw[6]=raw[6]*m.raw[0] + raw[7]*m.raw[3] + raw[8]*m.raw[6];
            result.raw[7]=raw[6]*m.raw[1] + raw[7]*m.raw[4] + raw[8]*m.raw[7];
            result.raw[8]=raw[6]*m.raw[2] + raw[7]*m.raw[5] + raw[8]*m.raw[8];
            return result;
        }
        ///Multiply a matrix my a scalar
        Matrix3 operator* (double s) const {
            Matrix3 result;
            result.raw[0]=raw[0]*s;
            result.raw[1]=raw[1]*s;
            result.raw[2]=raw[2]*s;
              	    	 
            result.raw[3]=raw[3]*s;
            result.raw[4]=raw[4]*s;
            result.raw[5]=raw[5]*s;
              	        
            result.raw[6]=raw[6]*s;
            result.raw[7]=raw[7]*s;
            result.raw[8]=raw[8]*s;
            return result;
        }
        ///divide a matrix my a scalar
        Matrix3 operator/ (double s) const {
            double inv_s=1.0/s; 
            Matrix3 result;
            result.raw[0]=raw[0]*inv_s;
            result.raw[1]=raw[1]*inv_s;
            result.raw[2]=raw[2]*inv_s;
              	    	 
            result.raw[3]=raw[3]*inv_s;
            result.raw[4]=raw[4]*inv_s;
            result.raw[5]=raw[5]*inv_s;
              	        
            result.raw[6]=raw[6]*inv_s;
            result.raw[7]=raw[7]*inv_s;
            result.raw[8]=raw[8]*inv_s;
            return result;
        }
        void Dump() const;

        ///Multiply a matrix my a scalar
        Matrix3 operator*= (double s) {
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
        Matrix3& operator+= (const Matrix3& m) {
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
        ///Set the value of one matrix to the value of the second.
        Matrix3& operator= (const Matrix3& m) {
            memcpy(raw,m.raw,9*sizeof(double));
            return *this;
        }
        ///Set a specific element to the value of val
        void Set(long column,long row,field val) {
            raw[3*column+row]=val;
        }
    private:
        field raw[9];
    };


    ///Class for storeing a 3x3 matrix defined over a field with known
    ///units. Behaves as a value type, whihc is probably fine for an
    ///object with only 9 floating point numbers. Obviously you'd want
    ///to be cleverer if you wanted to scale this up.
    template<int energy,int length>
    class Matrix3_t {
    public:
        typedef ddouble<energy,length> field;
        ///Default constructor. Constructs the identity
        Matrix3_t() {
            static const double IDENTITY3[]={1.0,0.0,0.0,
                                             0.0,1.0,0.0,
                                             0.0,0.0,1.0};
            memcpy(raw,IDENTITY3,9*sizeof(field));
        }
        ///Constructs a specific 3x3 matrix
        Matrix3_t(field a00,field a01,field a02,
                  field a10,field a11,field a12,
                  field a20,field a21,field a22) {
            raw[0]=a00;   raw[1]=a01;   raw[2]=a02;
            raw[3]=a10;   raw[4]=a11;   raw[5]=a12;
            raw[6]=a20;   raw[7]=a21;   raw[8]=a22;
        }

        ///Copies the matrix
        Matrix3_t(const Matrix3_t& m) {
            memcpy(raw,m.raw,9*sizeof(field));
        }

        ///Get the trace of the matrix
        field Trace() const {return raw[0]+raw[4]+raw[8];}

        ///Get the transpose of the matrix
        Matrix3_t Transpose() const {
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
        void Transposed() {
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
        ///Multiply two matricese together
        template<int E,int L>
        Matrix3_t<energy+E,length+L> operator* (const Matrix3_t<E,L>& m) const;
        ///Multiply a matrix my a scalar
        template<int E,int L>
        Matrix3_t<energy+E,length+L> operator* (ddouble<E,L> s) const {
            Matrix3_t<energy+E,length+L> result;
            result.raw[0]=raw[0]*s;
            result.raw[1]=raw[1]*s;
            result.raw[2]=raw[2]*s;
              	    	 
            result.raw[3]=raw[3]*s;
            result.raw[4]=raw[4]*s;
            result.raw[5]=raw[5]*s;
              	        
            result.raw[6]=raw[6]*s;
            result.raw[7]=raw[7]*s;
            result.raw[8]=raw[8]*s;
            return result;
        }
        ///Multiply a matrix my a scalar
        Matrix3_t operator*= (double s) {
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
        ///Set the value of one matrix to the value of the second.
        Matrix3_t& operator= (const Matrix3_t& m) {
            memcpy(raw,m.raw,9*sizeof(double));
            return *this;
        }
        ///Set a specific element to the value of val
        void Set(long column,long row,field val) {
            raw[3*column+row]=val;
        }
    private:
        field raw[9];
    };


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

    ///Class for storing a 3 dimentional rotation
    class Orientation {
    public:
        ///Constructs and orientaion of type Orientation::UNDEFINED with all
        ///other data uninialised.
        Orientation();
        ///Copies an orientation
        Orientation(const Orientation& orient);
        ///Assignment Operation
        const Orientation& operator=(const Orientation& orient);
        ///Destructor
        ~Orientation();
    
        ///Enumeration of the four conventions on storing rotations
        enum Type {
            UNDEFINED,
            EULER,
            ANGLE_AXIS,
            QUATERNION,
            EIGENSYSTEM
        };
    
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
    private:
        void Clear();
        union  {
            struct {
                double alpha;
                double beta;
                double gamma;
            } mEuler;
            struct {
                double angle;
                Vector3* axis;
            } mAngleAxis;
            struct {
                double real;
                double i;
                double j;
                double k;
            } mQuaternion;
            struct {
                Vector3* XAxis;
                Vector3* YAxis;
                Vector3* ZAxis;
            } mEigenSystem;
        } mData;
        Type mType;
    };


};
#endif
