
#include <vector>
#include <string>

using namespace std;

const long END=-1;

class SpinSystem;
class Spin;
class Orientation;
class Interaction;
class ReferenceFrame;

class SpinSystemElement {};

class Vector3 {
    Vector3();
    Vector3(double _x,double _y,double _z);

    double GetX();
    double GetY();
    double GetZ();    
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
    
    const double* GetRaw();
    
    double Get(long column, long row);
    double operator() (long column, long row);
    void Set(long i,long j,double val);
  private:
    double raw[9];
};

class SpinSystem {
  public:
    SpinSystem();
    ~SpinSystem();
    
    long GetSpinCount();    
    Spin* GetSpin(long n);
    vector<Spin*> GetSpins();
    void InsertSpin(Spin* _Spin,long Position=END);
    void RemoveSpin(long Position);
    void RemoveSpin(Spin* _Spin);

    const ReferenceFrame* GetRootFrame();
    
    void LoadFromGamesFile(const char* filename);
    void LoadFromG03File(const char* filename);
    void LoadFromXMLFile(const char* filename);
  private:
    friend class Spin;
    
     vector<Spin*> mSpins;
     vector<Interaction*> mInteractions;
     ReferenceFrame mLabFrame; 
};

class Spin {
  public:
    Spin(SpinSystem* Parent);
    ~Spin();

    Vector3 GetPosition();
    void SetPosition(Vector3 Position);

    void SetLabel(string Label);
    const char* GetLabel();
    
    long GetInteractionCount();
    Interaction* GetInteraction(long n);
    vector<Interaction*> GetInteractions();
    void InsertInteraction(Interaction* _Interaction,long Position=END);
    void RemoveInteraction(long Position);
    void RemoveInteraction(Interaction* _Interaction);
    
    ReferenceFrame* GetFrame();
    void SetFrame(ReferenceFrame* Frame);
  private:
    SpinSystem* mParent;
    Vector3 mPosition;
    string mLabel;
    ReferenceFrame* mFrame;
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
    
    Type GetType();
    Matrix3 GetAsMatrix3();

    void GetEuler(double* alpha,double* beta,double* gamma);
    void GetAngleAxis(double* angle,Vector3* axis);
    void GetQuaternion(double* real, double* i, double* j, double* k);
    void GetEigenSystem(Vector3* XAxis,Vector3* YAxis, Vector3* ZAxis);

    void SetEuler(double alpha,double beta,double gamma);
    void SetAngleAxis(double angle,Vector3 axis);
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
    
    Type GetType();
    Matrix3 GetAsMatrix3();
    
    void GetScalar(double* Scalar);
    void GetMatrix(Matrix3* Matrix);
    void GetEigenvalues(double* XX,double* YY, double* ZZ, Orientation* Orient);
    void GetAxRhom(double* iso,double* ax, double* rh, Orientation* Orient);
    void GetSpanSkew(double* iso,double* Span, double* Skew, Orientation* Orient);

    void SetScalar(double Scalar);
    void SetMatrix(Matrix3* Matrix);
    void SetEigenvalues(double XX,double YY, double ZZ, Orientation* Orient);
    void SetAxRhom(double iso,double ax, double rh, Orientation* Orient);
    void SetSpanSkew(double iso,double Span, double Skew, Orientation* Orient);
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
};

class ReferenceFrame {
  public:
    ReferenceFrame();
    ReferenceFrame(ReferenceFrame* Parent);
    ~ReferenceFrame();
    
    long GetChildCount();
    ReferenceFrame* GetChildFrame(long n);
    vector<ReferenceFrame*> GetChildFrames();
    void InsertChild(ReferenceFrame* Frame,long Position=END);
    void RemoveChild(long Position);
    void RemoveChild(ReferenceFrame* Child);
    
    Vector3 GetPosition();
    void SetPosition(Vector3 Position);
    Orientation* GetOrientation();
    
  private:
    ReferenceFrame* mParent;
    vector<ReferenceFrame*> mChildren;
    
    Vector3 mPosition;
    Orientation mOrient;
};



