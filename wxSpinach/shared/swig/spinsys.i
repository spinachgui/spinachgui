%module spinsys
%{
/* Put header files here or function declarations like below */
#include <shared/spinsys.hpp>
#include <shared/spinsys_spec.hpp>
%}

%typemap(out) Vector
{
  PyObject* x=PyFloat_FromDouble($1.getX());
  PyObject* y=PyFloat_FromDouble($1.getY());
  PyObject* z=PyFloat_FromDouble($1.getZ());
  $result=PyTuple_Pack(3,x,y,z);
}

//=========>> Spinsys class <<=========//

class Spinsys {
public:
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
  ///Get a reference to a spin
  void addSpin();
  ///Get a particular spin number
  SpinachSpin getSpin(long n);
  ///Get the total number of spins in the spin system
  long getSpinCount() const;
  ///Get a particular interaction
  SpinachInteraction getInteraction(long n);
  ///Get the total number of interactions
  long getInteractionCount() const;  
};

//=========>> Spin Class =========//

//Swig just needs to know that there is a spin class and it has
//a default constructior
class Spin {
public:
  Spin();
};


class SpinachSpin : public Spin {
public:
  SpinachSpin();
  void sayHello();
  void dump();
  Vector getCoords();
};

//============================================================>>
// Interaction class

class Interaction {};

class Interaction1 : public Interaction {
};

class SpinachInteraction : public Interaction1 {
public:
  SpinachInteraction() : Interaction1() {}
  SpinachInteraction(const Interaction1& _Int) : Interaction1(_Int) {}
  SpinachOrientation getOrientation();
  const char* getFormAsString() const;
};


//=========>> Orientation Class <<=====//

class Orientation {
public:
  Orientation();
};

class SpinachOrientation : public Orientation{
public:
  SpinachOrientation() : Orientation() {}
  SpinachOrientation(const Orientation& _O) : Orientation(_O) {}
};


//=========>> Reference Frame Class <<=========//

class Reference_frame {
public:
  Reference_frame();
};

class SpinachFrame : public Reference_frame {
public:
  SpinachFrame() : Reference_frame() {}
  SpinachFrame(const Reference_frame& _rf) : Reference_frame(_rf) {}
  SpinachOrientation getOrientation();
};
