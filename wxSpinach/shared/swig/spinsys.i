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
  Vector getCoords();
  long getNumber();
  const char* getIsotope();
  const char* getLabel();
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
  SpinachOrientation getSpinachOrientation();
  long getSpin1Number() const;
  const char* getFormAsString() const;
  double get(long x, long y) const; //get matrix elements. This really needs replacing
  Matrix3 getAsMatrix() const;
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

class Matrix {
public:
  Matrix();
  Matrix(const Matrix& _m);
};

class Matrix3 {
public:
  Matrix3();
  Matrix3(const Matrix& _m);
  Matrix3(const Matrix3& _m);
  Matrix3(double a00,double a01,double a02,
	  double a10,double a11,double a12,
	  double a20,double a21,double a22);
  double get(long i1,long i2) const;
  void set(long i1,long i2, double a);
  void dump() const;  

};
