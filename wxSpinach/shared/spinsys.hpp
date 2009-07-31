
#include <shared/spinsys_spec.hpp>
#include <iostream>
#include <vector>


using namespace std;

//Forward declare the classes in this file
class Spinsys;
class SpinachSpin;
class SpinachInteraction;
class SpinachFrame;
class SpinachOrientation;
class Matrix3;
class intList;

///Enumeration of the various types of interaction
enum INTERACTION {
  INTER_SCALER,
  INTER_MATRIX,
  INTER_EIGENVALUES,
  INTER_AXIALITY_RHOMBICITY,
  INTER_SPAN_SKEW
};

enum ORIENTATION {
  ORIENT_EULER,
  ORIENT_ANGLE_AXIS,
  ORIENT_QUATERNION,
  ORIENT_EIGENSYSTEM
};


class Spinsys  {
public:
  ///Default contructor. Creates an empty object
  Spinsys();
  ///Create an empty spin system
  void createNew();
  ///Load a spin system from an XML file located at filename
  void loadFromFile(const char* filename);
  ///Load a spin system from a g03 output file
  void loadFromG03File(const char* filename);
  ///Save the spin system to an XML file at filename
  void saveToFile(const char* filename);
  ///Output the spin system in a human readable format to the standard
  ///output for debugging purposes.
  void dump() const;
  ///Get a particular spin number
  SpinachSpin getSpin(long n);
  ///Get the total number of spins in the spin system
  long getSpinCount() const;
  ///Get a particular interaction
  SpinachInteraction getInteraction(long n);
  ///Get the total number of interactions
  long getInteractionCount() const;  
  ///Get all spins within distance of spin spinNumber with a higher number
  ///than spinNumber (useful for drawing bonds. If you loop though the structure,
  ///you only need to draw a bond once)
  std::vector<long> getNearbySpins(long spinNumber,double distance);
  ///Sum all the interaction matricese involving a given spin and return as a single
  ///matrix
  Matrix3 GetTotalInteractionOnSpinAsMatrix(long n);


  ///Attach a spin
  void addSpin();
protected:
  auto_ptr<Spin_system> mXMLSpinSys;
  typedef std::vector<SpinachSpin> spinarray;
  typedef std::vector<SpinachInteraction> interactionarray;
  typedef std::vector<SpinachFrame> framearray;
  spinarray mSpins;
  interactionarray mInteractions;
  framearray mFrames;
};


class SpinachOrientation : public Orientation{
public:
  SpinachOrientation() : Orientation() {}
  SpinachOrientation(const Orientation& _O) : Orientation(_O) {}
  Matrix3 getAsMatrix() const;
  long getForm() const;
};


class SpinachInteraction : public Interaction1 {
public:
  SpinachInteraction() : Interaction1() {}
  SpinachInteraction(const Interaction1& _Int) : Interaction1(_Int) {}
  SpinachOrientation getSpinachOrientation();
  const char* getFormAsString() const;
  long getForm() const;
  long getSpin1Number() const {return Interaction1::getSpin_1();}
  double get(long x,long y) const {
    //Danger!
    Matrix m=getMatrix().get();
    return m.getElement()[3*y+x];
  }
  Matrix3 getAsMatrix() const;
};

class SpinachSpin : public Spin {
public:
  SpinachSpin() : Spin() {}
  SpinachSpin(const Spin& _Spin) : Spin(_Spin) {}
  long getNumber() {return Spin::getNumber();}
  const char* getIsotope() {return Spin::getIsotope().c_str();}
  const char* getLabel() {
    LabelOptional la = Spin::getLabel();
    if(la.present()) {
      return la.get().c_str();
    } else {
      return "";
    }
  }

  Matrix3 GetTotalInteractionAsMatrix();
  void dump();
  Vector getCoords();
};



class SpinachFrame : public Reference_frame {
public:
  SpinachFrame() : Reference_frame() {}
  SpinachFrame(const Reference_frame& _rf);
  SpinachOrientation getOrientation() {SpinachOrientation orient(Reference_frame::getOrientation()); return orient;};
};

class Matrix3 : public Matrix {
public:
  Matrix3() : Matrix() {}
  Matrix3(const Matrix& _m) : Matrix(_m) {}
  Matrix3(double a00,double a01,double a02,
	  double a10,double a11,double a12,
	  double a20,double a21,double a22);
  double get(long i1,long i2) const {return getElement()[3*i1+i2];}
  void set(long i1,long i2, double a) {getElement()[3*i1+i2]=a;}
  void dump() const;  
  ///Add two matricese together in the expected way.
  Matrix3 operator+(const Matrix3& m) const;
};
