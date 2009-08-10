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

%typemap(out) std::vector<long> 
{
  $result=PyList_New($1.size());
  for(long i=0;i < $1.size(); i++) {
    PyObject* n=PyInt_FromLong($1[i]);
    PyList_SetItem($result,i,n);
  }
}

%typemap(out) std::complex<double>
{
  $result=PyComplex_FromDoubles($1.real(),$1.imag());
}



class SpinsysXMLRoot {
public:
  SpinsysXMLRoot();
  void clear();

  void loadFromFile(const char* filename);
  void loadFromG03File(const char* filename);
  void loadFromXYZFile(const char* filename);
  void saveToFile(const char* filename) const;

  SpinachSpinsys getRoot() const;
  void setRoot(SpinachSpinsys ss);
};

class Spin_system {
public:
  Spin_system();
};

class SpinachSpinsys : public Spin_system {
  friend class SpinsysXMLRoot;
public:
  SpinachSpinsys();
  SpinachSpinsys(Spin_system& s);

  void dump() const;
  long getSpinCount() const;
  SpinachSpin getSpinByIndex(long n);

  long getInteractionCount() const;  
  SpinachInteraction getInteractionByIndex(long n);

  std::vector<long> getNearbySpins(long spinNumber,double distance);
  Matrix3 GetTotalInteractionOnSpinAsMatrix(long n);
  double GetTotalIsotropicInteractionOnSpinPair(long n,long m);
  void addSpin();
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

  std::complex<double> getEigenValX() const {if(!foundEigenVals){calcEigenvalues();} return eigx;}
  std::complex<double> getEigenValY() const {if(!foundEigenVals){calcEigenvalues();} return eigy;}
  std::complex<double> getEigenValZ() const {if(!foundEigenVals){calcEigenvalues();} return eigz;}

  Vector getEigenvectorX() const;
  Vector getEigenvectorY() const;
  Vector getEigenvectorZ() const;

  double det() const;
};
