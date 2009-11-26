
#ifndef SPINSYS_H;
#define SPINSYS_H;

#include <vector>
#include <string>
#include <string.h>
#include <stdexcept>
#include <shared/mathtypes.hpp>

#ifdef SPINXML_EVENTS
#include <wx/string.h>
#include <gui/Event.hpp>
#endif

namespace SpinXML {


#ifdef SPINXML_EVENTS

  enum SPINSYS_PART {
    PART_SYSTEM,
    PART_SPIN,
    PART_INTERACTION
  };

#endif


void SetSchemaLocation(const char* loc);

const long END=-1;

class SpinSystem;
class Spin;
class Orientation;
class Interaction;
class ReferenceFrame;


  ///Class representing a reference frame. This class has not been
  ///properly implimented and should not be used yet.
class ReferenceFrame {
  public:
    ReferenceFrame();
    ReferenceFrame(ReferenceFrame* Parent,const Vector3& Position,const Orientation& Orient);
    ~ReferenceFrame();
    
    long GetChildCount() const;
    ReferenceFrame* GetChildFrame(long n) const;
    std::vector<ReferenceFrame*> GetChildFrames() const;
    void InsertChild(ReferenceFrame* Frame,long Position=END);
    void RemoveChild(long Position);
    void RemoveChild(ReferenceFrame* Child);
    
    Vector3& GetPosition();
    void SetPosition(const Vector3& Position);

    Orientation& GetOrientation();
    
  private:
    ReferenceFrame* mParent;
    std::vector<ReferenceFrame*> mChildren;
    
    Vector3 mPosition;
    Orientation mOrient;
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
    
  ///Print the interaction to the strandard output in a human readable
  ///form.
     void Dump() const;

  ///Enumeration of the agebrake forms
  enum Form {
    LINEAR,
    BILINEAR,
    QUADRATIC,
    ANY_FORM
  };

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
    void GetEigenvalues(double* XX,double* YY, double* ZZ, Orientation* OrientOut) const;
  ///If the Axiality-Rhombicity sorage convention being used then this function will set
  ///the value of its arguments to the appropreate value. Otherwise the
  ///result is undefined.
    void GetAxRhom(double* iso,double* ax, double* rh, Orientation* OrientOut) const;
  ///If the Span-Skew sorage convention being used then this function will set
  ///the value of its arguments to the appropreate value. Otherwise the
  ///result is undefined.
    void GetSpanSkew(double* iso,double* Span, double* Skew, Orientation* OrientOut) const;

  ///Set the value of the interaction using the scalar covention.
    void SetScalar(double Scalar);
  ///Set the value of the interaction using the matrix covention.
    void SetMatrix(const Matrix3& InMatrix);
  ///Set the value of the interaction using the eigenvalue covention.
    void SetEigenvalues(double XX,double YY, double ZZ, const Orientation& Orient);
  ///Set the value of the interaction using the axiality-rhombicity covention.
    void SetAxRhom(double iso,double ax, double rh, const Orientation& Orient);
  ///Set the value of the interaction using the span-skew covention.
    void SetSpanSkew(double iso,double Span, double Skew, const Orientation& Orient);

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
      } mEigenvalues;
      struct {
	double iso;
	double ax;
	double rh;
      } mAxRhom;
      struct {
          double iso;
          double span;
          double skew;
      } mSpanSkew;
   } mData;

  Orientation mOrient;

   Type mType;
   SubType mSubType;
   Spin* mSpin1;
   Spin* mSpin2;

#ifdef SPINXML_EVENTS
  EventNode* mNode;
public:
  EventNode* GetNode() const {return mNode;}
#endif
};

  ///A class representing a spin in a spin system
class Spin {
  friend class SpinSystem;
  public:  
    Spin(SpinSystem* Parent,Vector3 mPosition,std::string mLabel,ReferenceFrame* mFrame,long atomicNumber=1);
    Spin(const Spin& s,SpinSystem* newParent=NULL);
    ~Spin();
  
    void Dump() const;

    Vector3& GetPosition();
    void SetPosition(Vector3 Position);
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
    Vector3 mPosition;
    std::string mLabel;
    ReferenceFrame* mFrame;
    long mElement;
    std::vector<long> mIsotopes;

#ifdef SPINXML_EVENTS
  EventNode* mNode;
public:
  EventNode* GetNode() const {return mNode;}
#endif
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

    void LoadFromXYZFile(const char* filename);
    void SaveToXYZFile(const char* filename) const;

    std::vector<Interaction*>& GetInteractions() {return mInteractions;}
  
  private:
    friend class Spin;

    std::vector<Spin*> mSpins;
    std::vector<Interaction*> mInteractions;
    ReferenceFrame* mLabFrame; 

#ifdef SPINXML_EVENTS
  EventNode* mNode;
public:
  EventNode* GetNode() const {return mNode;}
#endif

};

}; //End Namespace


#endif
