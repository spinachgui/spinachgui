
#include <vector>
#include <string>
#include <string.h>

using namespace std;

const long END=-1;

class SpinSystem;
class Spin;
class Orientation;
class Interaction;
class ReferenceFrame;

class SpinSystemElement {};

class Vector3 {
  public:
    Vector3();
    Vector3(double _x,double _y,double _z);

    double GetX() const;
    double GetY() const;
    double GetZ() const;    
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
    
    const double* GetRaw() const;
    
    double Get(long column, long row) const;
    double operator() (long column, long row) const;
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
    Matrix3 GetAsMatrix3() const;

    void GetEuler(double* alpha,double* beta,double* gamma) const;
    void GetAngleAxis(double* angle,Vector3* axis) const;
    void GetQuaternion(double* real, double* i, double* j, double* k) const;
    void GetEigenSystem(Vector3* XAxis,Vector3* YAxis, Vector3* ZAxis) const;

    void SetEuler(double alpha,double beta,double gamma);
    void SetAngleAxis(double angle,Vector3* axis);
    void SetQuaternion(double real, double i, double j, double k);
    void SetEigenSystem(Vector3* XAxis,Vector3* YAxis, Vector3* ZAxis);
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
    vector<ReferenceFrame*> GetChildFrames() const;
    void InsertChild(ReferenceFrame* Frame,long Position=END);
    void RemoveChild(long Position);
    void RemoveChild(ReferenceFrame* Child);
    
    Vector3* GetPosition() const;
    void SetPosition(Vector3* Position);
    Orientation* GetOrientation() const;
    
  private:
    ReferenceFrame* mParent;
    vector<ReferenceFrame*> mChildren;
    
    Vector3* mPosition;
    Orientation* mOrient;
};


class Spin {
  public:
    Spin(SpinSystem* Parent,Vector3* mPosition,string mLabel,ReferenceFrame* mFrame);
    ~Spin();

    Vector3* GetPosition() const;
    void SetPosition(Vector3* Position);

    void SetLabel(string Label);
    const char* GetLabel() const;
    
    long GetInteractionCount() const;
    Interaction* GetInteraction(long n) const;
    vector<Interaction*> GetInteractions() const;
    void InsertInteraction(Interaction* _Interaction,long Position=END);
    void RemoveInteraction(long Position);
    void RemoveInteraction(Interaction* _Interaction);
    
    ReferenceFrame* GetFrame() const;
    void SetFrame(ReferenceFrame* Frame);
  private:
    SpinSystem* mParent;
    Vector3* mPosition;
    string mLabel;
    ReferenceFrame* mFrame;
};


class Interaction {
  public:
    Interaction();
    ~Interaction();
    
    enum Type {
        UNDEFINED,
        SCALAR,
        MATRIX,
        EIGENVALUES,
        AXRHOM,
        SPANSKEW
    };
    
    Type GetType() const;
    Matrix3 GetAsMatrix3() const;
    
    void GetScalar(double* Scalar) const;
    void GetMatrix(Matrix3* Matrix) const;
    void GetEigenvalues(double* XX,double* YY, double* ZZ, Orientation* Orient) const;
    void GetAxRhom(double* iso,double* ax, double* rh, Orientation* Orient) const;
    void GetSpanSkew(double* iso,double* Span, double* Skew, Orientation* Orient) const;

    void SetScalar(double Scalar);
    void SetMatrix(Matrix3* Matrix);
    void SetEigenvalues(double XX,double YY, double ZZ, Orientation* Orient);
    void SetAxRhom(double iso,double ax, double rh, Orientation* Orient);
    void SetSpanSkew(double iso,double Span, double Skew, Orientation* Orient);

    Spin* GetSpin1() const;
    void SetSpin1(Spin* Spin1);
    Spin* GetSpin2() const;
    void SetSpin2(Spin* Spin2);

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
   Spin* mSpin1;
   Spin* mSpin2;
};


class SpinSystem {
  public:
    SpinSystem();
    ~SpinSystem();
    
    long GetSpinCount() const;
    Spin* GetSpin(long n) const;
    vector<Spin*> GetSpins() const;
    void InsertSpin(Spin* _Spin,long Position=END);
    void RemoveSpin(long Position);
    void RemoveSpin(Spin* _Spin);

    ReferenceFrame* GetRootFrame() const;
    
    void LoadFromGamesFile(const char* filename);
    void LoadFromG03File(const char* filename);
    void LoadFromXMLFile(const char* filename);
  private:
    friend class Spin;
    
     vector<Spin*> mSpins;
     vector<Interaction*> mInteractions;
     ReferenceFrame* mLabFrame; 
};

