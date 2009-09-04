
#ifndef SPINSYS_H;
#define SPINSYS_H;

#include <vector>
#include <string>
#include <string.h>
#include <stdexcept>

namespace SpinXML {


const long END=-1;

class SpinSystem;
class Spin;
class Orientation;
class Interaction;
class ReferenceFrame;

class Vector3 {
  public:
    Vector3();
    Vector3(double _x,double _y,double _z);

    void GetCoordinates(double* _x,double* _y, double* _z) const;
    void SetCoordinates(double _x,double _y, double _z);

    double GetX() const;
    double GetY() const;
    double GetZ() const;    

    void SetX(double _x);
    void SetY(double _y);
    void SetZ(double _z);


  private:
    double x;
    double y;
    double z;
};

class Matrix3 {
  public:
    Matrix3();
    Matrix3(double a00,double a01,double a02,
            double a10,double a11,double a12,
            double a20,double a21,double a22);
    Matrix3(const Matrix3& m);

    void Dump() const;
    
    const double* GetRaw() const;
    
    double Get(long column, long row) const;
    double operator() (long column, long row) const;
    Matrix3 operator+ (const Matrix3& m) const;
    Matrix3& operator+= (const Matrix3& m);
    Matrix3& operator= (const Matrix3& m);
    void Set(long i,long j,double val);
  private:
    double raw[9];
};


class Orientation {
  public:
    Orientation();
    ~Orientation();
    
    enum Type {
        UNDEFINED,
        EULER,
        ANGLE_AXIS,
        QUATERNION,
        EIGENSYSTEM
    };
    
    Type GetType() const;
 
    void GetEuler(double* alpha,double* beta,double* gamma) const;
    void GetAngleAxis(double* angle,Vector3* axis) const;
    void GetQuaternion(double* real, double* i, double* j, double* k) const;
    void GetEigenSystem(Vector3* XAxis,Vector3* YAxis, Vector3* ZAxis) const;

    void SetEuler(double alpha,double beta,double gamma);
    void SetAngleAxis(double angle,Vector3* axis);
    void SetQuaternion(double real, double i, double j, double k);
    void SetEigenSystem(Vector3* XAxis,Vector3* YAxis, Vector3* ZAxis);

    Matrix3 GetAsMatrix() const;
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

class Interaction {
  public:
    Interaction();
    Interaction(const Interaction& inter);
    ~Interaction();
    
     void Dump() const;

    enum Type {
        UNDEFINED,
        SCALAR,
        MATRIX,
        EIGENVALUES,
        AXRHOM,
        SPANSKEW
    };

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
    
    Type GetType() const;
    SubType GetSubType() const;
    void SetSubType(SubType st);
    bool IsSubType(SubType t) const;
    
    void GetScalar(double* Scalar) const;
    void GetMatrix(Matrix3* OutMatrix) const;
    void GetEigenvalues(double* XX,double* YY, double* ZZ, Orientation** OrientOut) const;
    void GetAxRhom(double* iso,double* ax, double* rh, Orientation** OrientOut) const;
    void GetSpanSkew(double* iso,double* Span, double* Skew, Orientation** OrientOut) const;

    void SetScalar(double Scalar);
    void SetMatrix(Matrix3* InMatrix);
    void SetEigenvalues(double XX,double YY, double ZZ, Orientation* Orient);
    void SetAxRhom(double iso,double ax, double rh, Orientation* Orient);
    void SetSpanSkew(double iso,double Span, double Skew, Orientation* Orient);

    Spin* GetSpin1() const;
    void SetSpin1(Spin* Spin1);
    Spin* GetSpin2() const;
    void SetSpin2(Spin* Spin2);

    void SetQuadratic();
    void SetLinear();
    bool GetIsQuadratic() const;

    double GetAsScalar() const;
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

class Spin {
  public:
    Spin(SpinSystem* Parent,Vector3* mPosition,std::string mLabel,ReferenceFrame* mFrame);
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

    
    ReferenceFrame* GetFrame() const;
    void SetFrame(ReferenceFrame* Frame);

    std::vector<Spin*> GetNearbySpins(double distance);

  private:
    SpinSystem* mParent;
    Vector3* mPosition;
    std::string mLabel;
    ReferenceFrame* mFrame;
};




class SpinSystem {
  public:
    SpinSystem();
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
