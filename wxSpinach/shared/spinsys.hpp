
#include <shared/spinsys_spec.hpp>
#include <iostream>
#include <vector>

///Having "Vector" mean a 3D vector and "vector" mean a type of array
///would be just too confusing and error prone. Typedef the problem away

using namespace std;

//Forward declare the classes in this file
class Spinsys;
class SpinachSpin;
class SpinachInteraction;
class SpinachFrame;
class SpinachOrientation;

class Spinsys  {
public:
  ///Default contructor. Creates an empty object
  Spinsys();
  ///Create an empty spin system
  void createNew();
  ///Load a spin system from an XML file located at filename
  void loadFromFile(const char* filename);
  ///Save the spin system to an XML file at filename
  void saveToFile(const char* filename);
  ///Output the spin system in a human readable format to the standard
  ///output for debugging purposes.
  void dump() const;
  ///Get a list of references to spins
  Spin_system::SpinSequence& getSpins() {return mXMLSpinSys->getSpin();}
  ///Get a reference to a spin
  SpinachSpin getSpin(long n);
  long getSpinCount() {return 10;}

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

class SpinachInteraction : public Interaction1 {
public:
  SpinachInteraction() : Interaction1() {}
  SpinachInteraction(const Interaction1& _Int) : Interaction1(_Int) {}
};

class SpinachSpin : public Spin {
public:
  SpinachSpin() : Spin() {}
  SpinachSpin(const Spin& _Spin) : Spin(_Spin) {}
  void sayHello(){cout << "Hello world from C++" << endl;}
  void dump();
  Vector getCoords();
};


class SpinachOrientation : public Orientation{
public:
  SpinachOrientation() : Orientation() {}
  SpinachOrientation(const Orientation& _O) : Orientation(_O) {}
};


class SpinachFrame : public Reference_frame {
public:
  SpinachFrame() : Reference_frame() {}
  SpinachFrame(const Reference_frame& _rf) : Reference_frame(_rf) {}
protected:
};

