
#include <shared/spinsys_spec.hpp>
#include <iostream>

using namespace std;

//Forward declare the classes in this file
class Spinsys;
class OrientationWrap;

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
  ///Get a reference to a spin
  void getSpin();
  ///Get a list of references to spins
  Spin_system::SpinSequence& getSpins() {return mXMLSpinSys->getSpin();}

  ///Attach a spin
  void addSpin();
protected:
  auto_ptr<Spin_system> mXMLSpinSys;
};

class SpinachSpin : public Spin {
public:
  void sayHello(){cout << "Hello world from C++" << endl;}
  void dump();
};


class SpinachOrientation : public Orientation {
public:
};


class SpinachVector {
public:

};

class OrientationWrap {
 public:
  OrientationWrap();
  //I'm not sure what type function needs to return yet
  virtual void getAsMatrix() const = 0;

};

class EulerA : public OrientationWrap {
 public:
  EulerA(double alpha, double beta, double gamma);
  double getAlpha() const {return alpha;}
  double getBeta() const {return beta;}
  double getGamma() const {return gamma;}
 private:
  double alpha;
  double beta;
  double gamma;
};

class EulerB : public OrientationWrap {
 public:
  EulerB(double alpha, double beta, double gamma);
  double getAlpha() const {return alpha;}
  double getBeta() const {return beta;}
  double getGamma() const {return gamma;}
 private:
  double alpha;
  double beta;
  double gamma;
};

class QuaternionWrap : public OrientationWrap {
  QuaternionWrap(double Re, double i, double j, double k);
  double getRe() const {return mRe;}
  double getI()  const {return mI;}
  double getJ()  const {return mJ;}
  double getK()  const {return mK;}
 private:
  double mRe;
  double mI;
  double mJ;
  double mK;
};

