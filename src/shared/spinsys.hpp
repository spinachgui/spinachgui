
#ifndef SPINSYS_H;
#define SPINSYS_H;

#include <vector>
#include <string>
#include <string.h>
#include <stdexcept>

namespace SpinXML {

void SetSchemaLocation(const char* loc);

const long END=-1;

class SpinSystem;
class Spin;
class Orientation;
class Interaction;
class ReferenceFrame;

  ///Class for storing a 3 dimensional vector
class Vector3 {
  public:
  ///Constructs an uninitalised vector
    Vector3();
  ///Constructs a new vector
    Vector3(double _x,double _y,double _z);

  ///Sets it's arguments to the value of the vector's z,y and z
  ///components
    void GetCoordinates(double* _x,double* _y, double* _z) const;
  ///Sets the vector's x,y and z coordinates
    void SetCoordinates(double _x,double _y, double _z);

  ///Get the X component 
    double GetX() const;
  ///Get the Y component 
    double GetY() const;
  ///Get the Z component 
    double GetZ() const;    

  ///Set the X component 
    void SetX(double _x);
  ///Set the Y component 
    void SetY(double _y);
  ///Set the Z component 
    void SetZ(double _z);


  private:
    double x;
    double y;
    double z;
};

  ///Class for storeing a 3x3 matrix.
class Matrix3 {
  public:
  ///Default constructor. Constructs the identity
    Matrix3();
  ///Constructs a specific 3x3 matrix
    Matrix3(double a00,double a01,double a02,
            double a10,double a11,double a12,
            double a20,double a21,double a22);
  ///Copies the matrix
    Matrix3(const Matrix3& m);

  ///Print the matrix to the standard output in a human readable
  ///format
    void Dump() const;
    
  ///Get a pointer to the matrix in memory. The matrix is stored in
  ///row major form (GetRaw()[0]=a00,GetRaw()[1]=a01, etc.)
    const double* GetRaw() const;
    
  ///Get a specific value by row and column index.
    double Get(long column, long row) const;
  ///Get a specific value by row and column
    double operator() (long column, long row) const;
  ///Add together two matricese in the standard fasion.
    Matrix3 operator+ (const Matrix3& m) const;
  ///Add together two matricese in the standard fasion and set the
  ///value of the first to the result
    Matrix3& operator+= (const Matrix3& m);
  ///Set the value of one matrix to the value of the second.
    Matrix3& operator= (const Matrix3& m);
  ///Set a specific element to the value of val
    void Set(long column,long row,double val);
  private:
    double raw[9];
};

  ///Class for storing a 3 dimentional rotation
class Orientation {
  public:
  ///Constructs and orientaion of type Orientation::UNDEFINED with all
  ///other data uninialised.
    Orientation();
  ///Copies an orientation
    Orientation(const Orientation& orient);
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
    void SetAngleAxis(double angle,Vector3* axis);
  ///Set the value of the rotation in terms of quaternion
  ///paramiters. This function also sets the type value as returned by
  ///GetType()
    void SetQuaternion(double real, double i, double j, double k);
  ///Set the value of the rotation in terms of eigensystem. This
  ///function also sets the type value as returned by GetType()
    void SetEigenSystem(Vector3* XAxis,Vector3* YAxis, Vector3* ZAxis);

  ///Converts to a rotation matrix
    Matrix3 GetAsMatrix() const;

  ///returns a representation of the orientation as a string that is
  ///both machiene parsable and human readable.
    std::string ToString() const;
  ///Parses an orentation from a string representation. The form of
  ///the representation should be the same as returned by ToString
    void FromString(std::string string);
  private:
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

  ///Class representing a reference frame. This class has not been
  ///properly implimented and should not be used yet.
class ReferenceFrame {
  public:
    ReferenceFrame();
    ReferenceFrame(ReferenceFrame* Parent,Vector3* Position,Orientation* Orient);
    ~ReferenceFrame();
    
    long GetChildCount() const;
    ReferenceFrame* GetChildFrame(long n) const;
    std::vector<ReferenceFrame*> GetChildFrames() const;
    void InsertChild(ReferenceFrame* Frame,long Position=END);
    void RemoveChild(long Position);
    void RemoveChild(ReferenceFrame* Child);
    
    Vector3* GetPosition() const;
    void SetPosition(Vector3* Position);
    Orientation* GetOrientation() const;
    
  private:
    ReferenceFrame* mParent;
    std::vector<ReferenceFrame*> mChildren;
    
    Vector3* mPosition;
    Orientation* mOrient;
};

  ///Class representing one of the shielding paramiters such as the
  ///chemical shift or J coupling
class Interaction {
  friend class SpinSystem;
  public:
  ///Constructs an undefined interaction. The type are returned by
  ///GetType() is null UNDEFINED.
    Interaction();
  ///Copy constructor
    Interaction(const Interaction& inter, SpinSystem* newSystem=NULL);
  ///Destructor
    ~Interaction();
    
  ///Print the interaction to the strandard output in a human readable form.
     void Dump() const;

  ///Enumeration of the storage conventions used by this interaction
    enum Type {
        UNDEFINED,
        SCALAR,
        MATRIX,
        EIGENVALUES,
        AXRHOM,
        SPANSKEW
    };

  ///Enumeration what the physical source of this interaction is. Can
  ///be used as a hint for simulation software or it might determine
  ///how a partcular interaction is visualised.
    enum SubType {
      ST_ANY,
      ST_NMR,
      ST_EPR,

      //EPR INTERACTIONS
      ST_HFC,
      ST_G_TENSER,
      ST_ZFS,
      ST_EXCHANGE,

      //NMR INTERACTIONS
      ST_SHIELDING,
      ST_SCALAR,   

      //Interactions relevent to both nmr and epr
      ST_QUADRUPOLAR,
      ST_DIPOLAR,
      ST_CUSTOM
    };

  ///Get a human readable name for a member of the enum Type
    static const char* GetTypeName(Type t);
  ///Get a human readable name for a member of the enum SubType
    static const char* GetSubTypeName(SubType st);

  ///Get the storage convention being used
    Type GetType() const;
  ///Get the physical source of this interaction
    SubType GetSubType() const;
  ///Set a flag indicating the physical source of this interaction.
    void SetSubType(SubType st);
  ///Returns true if the physical source of this interaction is t. The
  ///members ST_NMR, ST_EPR and ST_ANY may be used here and will be
  ///interpreted coorectly. For example, if inter is of SubType
  ///ST_SCALAR then inter.IsSubType(ST_NMR) is true.
    bool IsSubType(SubType t) const;
    
  ///If the scalar sorage convention being used then this function will set
  ///the value of its argument to the appropreate value. Otherwise the
  ///result is undefined.
    void GetScalar(double* Scalar) const;
  ///If the Matrix sorage convention being used then this function will set
  ///the value of its argument to the appropreate value. Otherwise the
  ///result is undefined.
    void GetMatrix(Matrix3* OutMatrix) const;
  ///If the Eigenvalues sorage convention being used then this function will set
  ///the value of its arguments to the appropreate value. Otherwise the
  ///result is undefined.
    void GetEigenvalues(double* XX,double* YY, double* ZZ, Orientation** OrientOut) const;
  ///If the Axiality-Rhombicity sorage convention being used then this function will set
  ///the value of its arguments to the appropreate value. Otherwise the
  ///result is undefined.
    void GetAxRhom(double* iso,double* ax, double* rh, Orientation** OrientOut) const;
  ///If the Span-Skew sorage convention being used then this function will set
  ///the value of its arguments to the appropreate value. Otherwise the
  ///result is undefined.
    void GetSpanSkew(double* iso,double* Span, double* Skew, Orientation** OrientOut) const;

  ///Set the value of the interaction using the scalar covention.
    void SetScalar(double Scalar);
  ///Set the value of the interaction using the matrix covention.
    void SetMatrix(Matrix3* InMatrix);
  ///Set the value of the interaction using the eigenvalue covention.
    void SetEigenvalues(double XX,double YY, double ZZ, Orientation* Orient);
  ///Set the value of the interaction using the axiality-rhombicity covention.
    void SetAxRhom(double iso,double ax, double rh, Orientation* Orient);
  ///Set the value of the interaction using the span-skew covention.
    void SetSpanSkew(double iso,double Span, double Skew, Orientation* Orient);

  ///Get a pointer to the first spin this interaction acts on. This
  ///function is always defined.
    Spin* GetSpin1() const;
  ///Set the first spin this interaction acts on.
    void SetSpin1(Spin* Spin1);
  ///Get the second spin this interaction acts on. If GetSpin2()==NULL
  ///then this interaction is liner and only acts on one spin. If
  ///GetSpin2()==GetSpin1() this interaction is quadratic
    Spin* GetSpin2() const;
  ///Set the value of second spin the interaction acts
  ///on. SetSpin2(NULL) will result in a linear interaction and
  ///SetSpin2(GetSpin1()) will result in a quadratic interaction.
    void SetSpin2(Spin* Spin2);

  ///Tells the interaction that it is quadratic
    void SetQuadratic();
  ///Tells the interaction that it is linear
    void SetLinear();

  ///True if this interaction is linear
    bool GetIsLinear() const;
  ///True if this interaction is bilinear
    bool GetIsBilinear() const;
  ///True if this interaction is quadratic.
    bool GetIsQuadratic() const;

  ///Get the isotropic value of the interaction
    double GetAsScalar() const;
  ///Get the interaction as a full matrix
    Matrix3 GetAsMatrix() const /*throw(logic_error)*/;

  private:
    union  {
      double mScalar;
      Matrix3* mMatrix;
      struct {
        double XX;
        double YY;
        double ZZ;
        Orientation* Orient;
      } mEigenvalues;
      struct {
          double iso;
          double ax;
          double rh;
          Orientation* Orient;
      } mAxRhom;
      struct {
          double iso;
          double span;
          double skew;
          Orientation* Orient;
      } mSpanSkew;
   } mData;
   Type mType;
   SubType mSubType;
   Spin* mSpin1;
   Spin* mSpin2;
};

  ///A class representing a spin in a spin system
class Spin {
  friend class SpinSystem;
  public:  
    Spin(SpinSystem* Parent,Vector3* mPosition,std::string mLabel,ReferenceFrame* mFrame,long atomicNumber=1);
    Spin(const Spin& s,SpinSystem* newParent=NULL);
    ~Spin();
  
    void Dump() const;

    Vector3* GetPosition() const;
    void SetPosition(Vector3* Position);
    void GetCoordinates(double* _x,double* _y, double* _z) const;
    void SetCoordinates(double _x,double _y, double _z);

    void SetLabel(std::string Label);
    const char* GetLabel() const;

    long GetInteractionCount() const;
    Interaction* GetInteraction(long n) const;
    std::vector<Interaction*> GetInteractions() const;
    void InsertInteraction(Interaction* _Interaction,long Position=END);
    void RemoveInteraction(long Position);
    void RemoveInteraction(Interaction* _Interaction);

    double GetLinearInteractionAsScalar(Interaction::SubType t=Interaction::ST_ANY) const;
    double GetBilinearInteractionAsScalar(Spin* OtherSpin,Interaction::SubType t=Interaction::ST_ANY) const;
    double GetQuadrapolarInteractionAsScalar(Interaction::SubType t=Interaction::ST_ANY) const;

    Matrix3 GetLinearInteractionAsMatrix(Interaction::SubType t=Interaction::ST_ANY) const;
    Matrix3 GetBilinearInteractionAsMatrix(Spin* OtherSpin,Interaction::SubType t=Interaction::ST_ANY) const;
    Matrix3 GetQuadrapolarInteractionAsMatrix(Interaction::SubType t=Interaction::ST_ANY) const;

    long GetElement() const;
    void SetElement(long element);
    std::vector<long> GetIsotopes() const;
    void SetIsotopes(std::vector<long> isotopes) const;


    ReferenceFrame* GetFrame() const;
    void SetFrame(ReferenceFrame* Frame);

    std::vector<Spin*> GetNearbySpins(double distance);

  private:
    SpinSystem* mParent;
    Vector3* mPosition;
    std::string mLabel;
    ReferenceFrame* mFrame;
    long mElement;
    std::vector<long> mIsotopes;
};




class SpinSystem {
  public:
    SpinSystem();
    SpinSystem(const SpinSystem& system);
    ~SpinSystem();

    void Clear();
    
    void Dump() const;

    long GetSpinCount() const;
    Spin* GetSpin(long n) const;
    long GetSpinNumber(Spin* spin) const;

    std::vector<Spin*> GetSpins() const;
    void InsertSpin(Spin* _Spin,long Position=END);
    void RemoveSpin(long Position);
    void RemoveSpin(Spin* _Spin);

    ReferenceFrame* GetRootFrame() const;
    
    void LoadFromGamesFile(const char* filename);

    void LoadFromG03File(const char* filename) throw(std::runtime_error);

    void LoadFromXMLFile(const char* filename);
    void SaveToXMLFile(const char* filename) const;
  
  private:
    friend class Spin;

    std::vector<Spin*> mSpins;
    std::vector<Interaction*> mInteractions;
    ReferenceFrame* mLabFrame; 
};

}; //End Namespace


#endif
