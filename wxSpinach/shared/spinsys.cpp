
#include "spinsys.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/algorithm/string/trim.hpp>
#include <shared/nuclear_data.hpp>
#include <cmath>
#include <complex>

using namespace std;

//============================================================//
// Utility Functions

typedef complex<double> cdouble;

void solveCubic(const cdouble a,const cdouble b,const cdouble c,cdouble& z1, cdouble& z2, cdouble& z3) {
  //Define some numerical constants

  static const cdouble i=cdouble(0.0,1.0);
  static const double root3(sqrt(3.0));
  static const double croot2(pow(2.0,1.0/3.0));
  static const double croot3(pow(3.0,1.0/3.0));

  static const double twoToTheFiveThirds=pow(2.0,5.0/3.0);
  static const double threeToTheFiveSixths=pow(3.0,5.0/6.0);
  
  static const double pi=3.141592654;

  //Equation is in the form z^3 + az^2 + bz + c = 0
  const cdouble n=-a*a/3.0 + b;
  const cdouble m=2.0*a*a*a/27.0 - a*b/3.0 + c;

  //Equation is now in the form x^3 + nx + m == 0 where x = z - a/3

  //And has a simpler solution (see the mathmatica notebook

  //Use vieta's substitution x=w-n/(3w) to get the the cubic equation into the form
  // w^6 - m w^3 + n^3/27, a quadratic, solve for w^3. Only the positive roots
  // are required as the negative roots give identical results.
  const cdouble w3= (-m + sqrt(m*m + 4.0*n*n*n/27.0))/2.0;

  cout << "Should be 0 = " << w3*w3 + m*w3 - n*n*n/27.0 << endl;
  
  //There are three roots of w3
  const cdouble r1 = polar(abs(w3),arg(w3)/3.0);
  const cdouble r2 = polar(abs(w3),arg(w3)/3.0+2.0*pi/3.0);
  const cdouble r3 = polar(abs(w3),arg(w3)/3.0+4.0*pi/3.0);


  const cdouble x1=r1-n/(3.0*r1);
  const cdouble x2=r2-n/(3.0*r2);
  const cdouble x3=r3-n/(3.0*r3);

  cout << "x1=" << x1 << " x2=" << x2 << " x3=" << x3 << endl;
  cout << "g(x1)=" << x1*x1*x1 + n*x1 + m << endl;
  cout << "g(x2)=" << x2*x2*x2 + n*x2 + m << endl;
  cout << "g(x3)=" << x3*x3*x3 + n*x3 + m << endl;


  //z1=x1num/x1denom - a/3.0;
  //z2=x2num/x2denom - a/3.0;
  //z3=x3num/x3denom - a/3.0;
  return;
}

//============================================================//
//

SpinsysXMLRoot::SpinsysXMLRoot() : mXMLSpinSys(new SpinachSpinsys()) {
  cout << "The size of a newly created set of spins is " << mXMLSpinSys->getSpin().size() << endl;
}

void SpinsysXMLRoot::loadFromFile(const char* filename) {
  clear();
  try {
    auto_ptr<Spin_system> tmpPtr(parseSpin_system(filename));
    mXMLSpinSys.reset(new SpinachSpinsys(*tmpPtr));
  } catch(const xml_schema::Exception& e) {
    cerr << e << endl;
    exit(1);
  }
}

void SpinsysXMLRoot::saveToFile(const char* filename) const {
  xml_schema::NamespaceInfomap map;
  map[""].name = "";
  map[""].schema = "../data/spinsys_spec.xsd";

  ofstream fout(filename);
  if(!fout.is_open()) {
    cerr << "Could not open " << filename << endl;
    return;
  }
  Spin_system ss(*mXMLSpinSys);
  cout << "About to save" << endl;
  try {
    serializeSpin_system(fout, ss, map);
  } catch (const xml_schema::Exception& e) {
    cerr << e << endl;
    exit(1);
  }
}

void SpinsysXMLRoot::loadFromG03File(const char* filename) {
  clear();
  mXMLSpinSys->loadFromG03File(filename);
}

void SpinsysXMLRoot::loadFromXYZFile(const char* filename) {
  clear();
  mXMLSpinSys->loadFromXYZFile(filename);
}

void SpinsysXMLRoot::clear() {
  mXMLSpinSys.reset(new SpinachSpinsys());
}


//============================================================//
//

void SpinachSpinsys::dump() const {
  cout << "Printing out a spin system" << endl;
  cout << "Spins:" << endl;
  for (SpinConstIterator i(getSpin().begin()); i != getSpin().end(); ++i) {
    cout << " Spin name=" << i->getLabel() << endl;
    cout << "      number=" << i->getNumber() << endl;
    cout << "      isotope=" << i->getIsotope() << endl;
    cout << "      coords=(" << i->getCoordinates().getX() << ",";
    cout << "              " << i->getCoordinates().getY() << ",";
    cout << "              " << i->getCoordinates().getZ() << ")" << endl;
  }
  for (InteractionConstIterator i(getInteraction().begin()); i != getInteraction().end();  ++i) {
    SpinachInteraction so(*i);
    cout << " Interaction type=" << so.getFormAsString() << endl;
    cout << "      Spin_1=" << so.getSpin_1() << endl;
  }
}

void SpinachSpinsys::loadFromXYZFile(const char* filename) {
  ifstream fin(filename);
  cout << "Opening an xyz file " << filename << endl;
  if(!fin.is_open()) {
    //Throw some sort of error here
    cerr << "Couhn't open the file" << endl;
  }
  SpinSequence Spins;
  string element;
  double x,y,z;
  long nAtoms=0;
  while(!fin.eof()) {
    fin >> element >> x >> y >> z >> ws;
    cout << element << " " << x << "  " << y << " " << z << endl;
    Spin s(Vector(x,y,z),nAtoms,element,0);  //Last paramiter is the reference frame
    s.setLabel("Spin");
    Spins.push_back(s);
    nAtoms++;
  } 
  setSpin(Spins);
}


void SpinachSpinsys::loadFromG03File(const char* filename) {
  /*
    This function really needs some work done on in, as it's not using C++
    streams properly. This is due to it being adapted from matlab code (which uses
    c style I/O
  */
  ifstream fin(filename);
  cout << "Opening a g03 file:" << filename << endl;
  if(!fin.is_open()) {
    //Throw some sort of error here
    cerr << "Couldn't open file" << endl;
    return;
  }
  SpinSequence Spins;
  InteractionSequence Interactions;
  long nAtoms=0;
  bool standardOrientFound=false; //Some files seem to have many standard orientation sections.
  while(!fin.eof()) {
    string line;
    char buff[500];  //TODO buletproof this
    fin.getline(buff,500); line=buff; //Read a line
    boost::algorithm::trim(line); //Remove whitespace

    if (line=="Standard orientation:") {
      cout << "Standard orientation found." << endl;
      nAtoms=0;
      //Skip 4 lines
      fin.getline(buff,500); line=buff; //Read a line
      fin.getline(buff,500); line=buff; //Read a line
      fin.getline(buff,500); line=buff; //Read a line
      fin.getline(buff,500); line=buff; //Read a line
      istringstream stream;

      fin.getline(buff,500); line=buff; stream.str(line); //Read a line
      while(line.find("--------") == string::npos && !fin.eof()) {
	int dummy1,atomicNumber,dummy3;
	double x,y,z;
	stream >> dummy1 >> atomicNumber >> dummy3 >> x >> y >> z;
	stream.clear();
	fin.getline(buff,500); line=buff; stream.str(line); //Read a line
	if(standardOrientFound) {
	  //Update an existing spin
	  if(Spins.size() > nAtoms) {
	    Spins[nAtoms].setCoordinates(Vector(x,y,z));
	  } else {
	    cerr << "Error reading standard orientation: the number of centres is not consistant amoung the standard orientation tables" << endl;
	  }
	} else {
	  //Create a new spin
	  string isotopeSymbol(getElementSymbol(atomicNumber));
	  isotopeSymbol=isotopeSymbol+"1"; //We have no clue what the mass number is yet
	  Spin s(Vector(x,y,z),nAtoms,isotopeSymbol,0);
	  s.setLabel("A Spin");
	  Spins.push_back(s);
	}
	nAtoms++;
      }
      standardOrientFound=true;

    } else if(line=="g tensor (ppm):") {
      
    } else if(line=="g tensor [g = g_e + g_RMC + g_DC + g_OZ/SOC]:") {
    } else if(line=="SCF GIAO Magnetic shielding tensor (ppm):") {
      for(long i=0;i<nAtoms;i++) {
	long centerNumber; 
	string element;
	string Isotropic,Anisotropy,Eigenvalues; //Should always end up with the value "Isotropic" or "Anisotropy" etc. or file is corrupt
	string equils,equils2;
        double isotropicValue,anisotropicValue;
	string sXX,sYX,sZX, //Should end up "<variable name>="
	  sXY,sYY,sZY,
	  sXZ,sYZ,sZZ;
	double XX,YX,ZX, //Should end up "<variable name>="
	  XY,YY,ZY,
	  XZ,YZ,ZZ;
	double ev1,ev2,ev3;

	fin >> centerNumber >> element >> Isotropic >> equils >> isotropicValue >> Anisotropy >> equils2 >> anisotropicValue;
	fin >> sXX >> XX >> sYX >> YX >> sZX >> ZX;
	fin >> sXY >> XY >> sYY >> YY >> sZY >> ZY;
	fin >> sXZ >> XZ >> sYZ >> YZ >> sZZ >> ZZ;
	fin >> Eigenvalues >> ev1 >> ev2 >> ev3;

	//Check we got the input we expected
	if(Isotropic!="Isotropic" || Anisotropy!="Anisotropy" || Eigenvalues!="Eigenvalues:"
	   ||sXX!="XX="||sYX!="YX="||sZX!="ZX=" 
	   ||sXY!="XY="||sYY!="YY="||sZY!="ZY="
	   ||sXZ!="XZ="||sYZ!="YZ="||sZZ!="ZZ=") {
	  cerr << "Error reading GIAO Magnetic shielding tensor" << endl;
	  cerr << "Expected 'Isotropic' got " << Isotropic << endl;
	  cerr << "Expected 'Anisotropy' got " << Anisotropy << endl;
	  cerr << "Expected 'Eigenvalues:' got " << Eigenvalues << endl;

	  cerr << "XX=" << sXX;
	  cerr << " XY=" << sXY;
	  cerr << " XZ=" << sXZ << endl;

	  cerr << "YX=" << sYX;
	  cerr << " YY=" << sYY;
	  cerr << " YZ=" << sYZ << endl;

	  cerr << "ZX=" << sZX;
	  cerr << " ZY=" << sZY;
	  cerr << " ZZ=" << sZZ << endl;

	} else {
	  //Store the interaction
	}
      }
    } else if(line=="Total nuclear spin-spin coupling J (Hz):") {
      long rows = nAtoms/5; //The number of rows the matrix is split over with the full 5 colums.
                            //I'm relying on the fact that division is rounded down
      long columsOnLastRow = nAtoms % 5;
      for(long i=0;i<rows;i++) {  //For each big row
	//Skip the center number labels on the top row
	for(long k=0;k<5;k++) {
	  long topCenterNumber;
	  fin >> topCenterNumber;
	  if(i*5+k+1 != topCenterNumber) {
	    cerr << "Error reading J couplings, found topCenterNumber=" << topCenterNumber << " expected " << i*5+k+1 << endl; 
	  }
	}

	for(long j=i*5;j<nAtoms;j++) {  //For each little row in the big row
	  long centerNumber;
	  fin >> centerNumber;
	  if(centerNumber != j+1) {
	    cerr << "Error reading J Couplings, found centerNumber=" << centerNumber << " expected " << j+1 << endl;
	  }
	  for(long k=0;k<5 && k < j+1-i*5;k++) {
	    string JCouplingStr;
	    double JCoupling;
	    fin >> JCouplingStr;
	    //Fotran double precision output isn't compatable with C++ iostreams, so we have to tweek it
	    //Replace the D with an E
	    long Dpos=JCouplingStr.find('D');
	    if(Dpos >= 0) {
	      JCouplingStr.replace(Dpos,1,"e");
	    } else {
	      cerr << "Was expecting a J coupling value in the form +-0.00000D00 but there was no D. Got" << JCouplingStr << endl;
	    }
	    //Pack it back into a istream and parse
	    istringstream stream;
	    stream.str(JCouplingStr);
	    stream >> JCoupling;
	    //cout << "J(" << j+1 << "," << i*5+k+1 << ")=" << JCoupling << endl;
	    Interaction1 inter("J","MHz",j+1,0);
	    inter.setScalar(JCoupling);
	    inter.setSpin_2(i*5+k+1);
	    Interactions.push_back(inter);
	  }
	}
      }
    } else if(line=="Isotropic Fermi Contact Couplings") {
      cout << "Isotropic couplings found" << endl;
      //Skip a line
      fin.getline(buff,500); line=buff; //Read a line
      istringstream stream;
      for (long i=0;i<nAtoms;i++) {
	string dummy1,dummy2,dummy3;
	double isoCoupling;
	fin.getline(buff,500); line=buff; //Read a line
	stream.clear(); stream.str(line);
	//Read the coupling strength (in megaherz)
	stream >> dummy1 >> dummy2 >> dummy3 >> isoCoupling;
	Interaction1 inter("Isotropic","MHz",i,0); //Last paramiter is reference frame, which is always lab
	inter.setScalar(isoCoupling);
	Interactions.push_back(inter);
      }          
    }
    if(line=="Anisotropic Spin Dipole Couplings in Principal Axis System") {
      cout << "Anisotropic couplings found" << endl;
      //Skip 4 lines
      fin.getline(buff,500); line=buff; //Read a line
      fin.getline(buff,500); line=buff; //Read a line
      fin.getline(buff,500); line=buff; //Read a line
      fin.getline(buff,500); line=buff; //Read a line
      istringstream stream;
      for (long i=0;i<nAtoms;i++) {
	string dummy1,dummy2,dummy3,dummy4,dummy5,isotope;
	double eigenvalue1,eigenvalue2,eigenvalue3;
	double x1,y1,z1,x2,y2,z2,x3,y3,z3;
	//Read the rotation matrix and eigenvalues (in megaherz)
	fin.getline(buff,500); line=buff; stream.clear(); stream.str(line); //Read a line
	stream                      >> dummy1 >> dummy2 >> eigenvalue1 >> dummy3 >> dummy4 >> x1 >> y1 >> z1;

	fin.getline(buff,500); line=buff; stream.clear(); stream.str(line); //Read a line
	stream >> dummy5 >> isotope >> dummy1 >> dummy2 >> eigenvalue2 >> dummy3 >> dummy4 >> x2 >> y2 >> z2;

	fin.getline(buff,500); line=buff; stream.clear(); stream.str(line); //Read a line
	stream                      >> dummy1 >> dummy2 >> eigenvalue3 >> dummy3 >> dummy4 >> x3 >> y3 >> z3;
	//Skip a line
	fin.getline(buff,500); line=buff; //Read a line

	Interaction1 inter("Isotropic","MHz",i,0); //Last paramiter is reference frame, which is always lab
	inter.setEigenvalues(Eigenvalues(eigenvalue1*0.05,eigenvalue2*0.05,eigenvalue3*0.05));
	Orientation o;
	o.setEigensystem(Eigensystem(Vector(x1,y1,z1),Vector(x2,y2,z2),Vector(x3,y3,z3)));
	inter.setOrientation(o);
	Interactions.push_back(inter);

	//Now process the isotope
	long leftBracket=isotope.find("(");
	long rightBracket=isotope.find(")");
	string element=isotope.substr(0,leftBracket);
	string massnum=isotope.substr(leftBracket+1,rightBracket-leftBracket-1);
	isotope=element+massnum;
	Spins[i].setIsotope(isotope);
      }
    }
  }
  cout << "Finished loading the g03 file, saving spins.size()=" << Spins.size() << endl;
  setSpin(Spins);
  cout << " and getSpin().size()=" << getSpin().size() << endl;
  setInteraction(Interactions);
}

void SpinachSpinsys::addSpin() {
  //  Spin s();
  //  mXMLSpinSys->getSpin().push_back(s);
}

long SpinachSpinsys::getSpinCount() const {
  return getSpin().size();
}



SpinachSpin SpinachSpinsys::getSpinByIndex(long n) {
  if(n>=0 && n<getSpin().size()) {
    return getSpin()[n];
  }
  SpinachSpin s;
  return s;
}

SpinachInteraction SpinachSpinsys::getInteractionByIndex(long n) {
  if(n>=0 && n<getInteraction().size()) {
    return getInteraction()[n];
  }
  SpinachInteraction inter;
  return inter;
}

long SpinachSpinsys::getInteractionCount() const {
  return getInteraction().size();
}

std::vector<long> SpinachSpinsys::getNearbySpins(long spinNumber,double distance) {
  SpinSequence Spins(getSpin());
  std::vector<long> result;
  double dist2=distance*distance;
  Vector coords1=Spins[spinNumber].getCoordinates();
  double x1=coords1.getX();
  double y1=coords1.getY();
  double z1=coords1.getZ();

  for(long i=spinNumber+1;i<getSpin().size();i++) {
    Vector coords2=Spins[i].getCoordinates();
    double x2=coords2.getX();
    double y2=coords2.getY();
    double z2=coords2.getZ();
    double deltaX=(x1-x2);
    double deltaY=(y1-y2);
    double deltaZ=(z1-z2);
    if(deltaX*deltaX+deltaY*deltaY+deltaZ*deltaZ < dist2) {
      result.push_back(i);
    }
  }
  return result;
}

Matrix3 SpinachSpinsys::GetTotalInteractionOnSpinAsMatrix(long n) {
  Matrix3 totalMatrix(0,0,0,0,0,0,0,0,0);
  for(long i=0;i < getInteraction().size();++i) {
    if(getInteraction()[i].getSpin_1()==n && !getInteraction()[i].getSpin_2().present()) {
      totalMatrix=totalMatrix+SpinachInteraction(getInteraction()[i]).getAsMatrix();
    }
  }
  return totalMatrix;
}

double SpinachSpinsys::GetTotalIsotropicInteractionOnSpinPair(long n,long m) {
  double totalScalar=0.0;
  for(long i=0;i < getInteraction().size();++i) {
    if(getInteraction()[i].getSpin_1()==n && getInteraction()[i].getSpin_2().present()) {
      totalScalar+=SpinachInteraction(getInteraction()[i]).getAsScalar();
    }
  }
  return totalScalar;
}


//============================================================//
// SpinachSpin

Vector SpinachSpin::getCoords() {
  return getCoordinates();
}

void SpinachSpin::dump() {
  cout << "Spin name=" << getLabel() << endl;
  //const Spin::CoordinatesType c = getCoordinates();
  //cout << c.getX() << endl;
  //cout << c.getY() << endl;
  //cout << c.getZ() << endl;
  //cout << "IsotopeE = " << getIsotope() << endl;
}


//============================================================//
// SpinachOrientation

long SpinachOrientation::getForm() const {
  if(getEuler_angles().present()) {
    return ORIENT_EULER;
  } else if(getAngle_axis().present()) {
    return ORIENT_ANGLE_AXIS;
  } else if(getQuaternion().present()) {
    return ORIENT_QUATERNION;
  } else if(getEigensystem().present()) {
    return ORIENT_EIGENSYSTEM;
  } else {
    //TODO Throw some sort of error here
  }
}


Matrix3 SpinachOrientation::getAsMatrix() const {
  if(getForm()==ORIENT_EIGENSYSTEM) {
    Eigensystem sys=getEigensystem().get();
    Vector xa=sys.getX_axis();
    Vector ya=sys.getY_axis();
    Vector za=sys.getZ_axis();

    return Matrix3(xa.getX(),ya.getX(),za.getX(),
		   xa.getY(),ya.getY(),za.getY(),
		   xa.getZ(),ya.getZ(),za.getZ());
  }
  return Matrix3(1,0,0,0,1,0,0,0,1);
}



//============================================================//
// SpinachInteraction



SpinachOrientation SpinachInteraction::getSpinachOrientation() {
  OrientationOptional orentOpt=Interaction1::getOrientation();
  if(orentOpt.present()) {
    SpinachOrientation orient(orentOpt.get());
    return orient;
  } else {
    //TODO Return some sort of exception to the calling code at this point
    cerr << "Trying to get the orientation when this type of interaction doesn't need it" << endl;
    const SpinachOrientation so;
    return so;
  }
};

Matrix3 SpinachInteraction::getAsMatrix() const {
  if(getForm()==INTER_SCALER) {
    //Return the identity multipled by the scalar
    double s=getScalar().get();
    Matrix3 m(s,0,0,0,s,0,0,0,s);
    return m;
  } else if(getForm()==INTER_MATRIX) {
    //Just return the matrix
    Matrix3 m(getMatrix().get());
    return m;
  } else if(getForm()==INTER_EIGENVALUES || getForm() == INTER_AXIALITY_RHOMBICITY) {
    //Convert to a matrix
    double xx,yy,zz;

    if(getForm()==INTER_EIGENVALUES) {
      const Eigenvalues ev=getEigenvalues().get();
    
      xx=ev.getXX();
      yy=ev.getYY();
      zz=ev.getZZ();
    } else if(getForm()==INTER_AXIALITY_RHOMBICITY) {
      const Axiality_rhombicity ar=getAxiality_rhombicity().get();
      double a=ar.getAx();
      double r=ar.getRh();
      double iso=ar.getIso();
      xx=a/3+iso/3;
      yy=-r/2-a/6+iso/3;
      zz= r/2-a/6+iso/3;
    } else if(getForm()==INTER_SPAN_SKEW) {
      const Span_skew ss=getSpan_skew().get();
      double span=ss.getSpan();
      double skew=ss.getSkew();
      double iso=ss.getIso();
      xx=span*skew/6-span/2;
      yy=iso-span*skew/3;
      zz=span*skew/6+span/2;
    }
    const SpinachOrientation o=getOrientation().get();
    Matrix3 intMatrix=o.getAsMatrix();

    intMatrix.set(0,0,intMatrix.get(0,0)*xx);
    intMatrix.set(0,1,intMatrix.get(0,1)*yy);
    intMatrix.set(0,2,intMatrix.get(0,2)*zz);

    intMatrix.set(1,0,intMatrix.get(1,0)*xx);
    intMatrix.set(1,1,intMatrix.get(1,1)*yy);
    intMatrix.set(1,2,intMatrix.get(1,2)*zz);

    intMatrix.set(2,0,intMatrix.get(2,0)*xx);
    intMatrix.set(2,1,intMatrix.get(2,1)*yy);
    intMatrix.set(2,2,intMatrix.get(2,2)*zz);
    return intMatrix;
  } else {
    cerr << "Unknown interaction type returned by SpinachInteraction::getForm(). This is a serious error" << endl;
  }
  //Return the zero matrix identity
  Matrix3 zero(0,0,0,0,0,0,0,0,0);
  return zero;
}

double SpinachInteraction::getAsScalar() const {
  if(getForm()==INTER_SCALER) {
    //Return the identity multipled by the scalar
    double s=getScalar().get();
    return s;
  } else {
    return getAsMatrix().trace();
  }
}

long SpinachInteraction::getForm() const {
  if(getScalar().present()) {
    return INTER_SCALER;
  } else if(getMatrix().present()) {
    return INTER_MATRIX;
  } else if(getEigenvalues().present()) {
    return INTER_EIGENVALUES;
  } else if(getAxiality_rhombicity().present()) {
    return INTER_AXIALITY_RHOMBICITY;
  } else if(getSpan_skew().present()) {
    return INTER_SPAN_SKEW;
  } else {
    //TODO Throw some sort of error here
  }
}

const char* SpinachInteraction::getFormAsString() const {
  if(getScalar().present()) {
    return "scalar";
  } else if(getMatrix().present()) {
    return "matrix";
  } else if(getEigenvalues().present()) {
    return "eigenvalues";
  } else if(getAxiality_rhombicity().present()) {
    return "axiality_rhombicity";
  } else if(getSpan_skew().present()) {
    return "span_skew";
  } else {
    //TODO Throw some sort of error here
  }
}

//============================================================//
// SpinachFrame

SpinachFrame::SpinachFrame(const Reference_frame& _rf) : Reference_frame(_rf) {
  
}


  //============================================================//

Matrix3::Matrix3(double a00,double a01,double a02,
		 double a10,double a11,double a12,
		 double a20,double a21,double a22) : foundEigenVals(false) {
  ElementSequence elements;
  elements.resize(9);
  elements[0]=a00;
  elements[1]=a01;
  elements[2]=a02;

  elements[3]=a10;
  elements[4]=a11;
  elements[5]=a12;

  elements[6]=a20;
  elements[7]=a21;
  elements[8]=a22;
  setElement(elements);
}

double Matrix3::det() const {
  return
    get(0,0)  *  (get(1,1)*get(2,2)-get(1,2)*get(2,1))+
    get(1,0)  *  (get(0,1)*get(2,2)-get(0,2)*get(2,1))-
    get(2,0)  *  (get(0,1)*get(1,2)-get(1,1)*get(0,1));
}

void Matrix3::calcEigenvalues() const {
  //Find the 3 coefficents of the characteristic polynomial
  //This formulate was taken from the wikipedia page, the correctness should
  //be evident when it is used for arrow drawing


  //Fun fact, a,b,c are coefficents of the characteristic polynomial. They are also the three
  //invarients of the matrix. Coincidence? I think not.
  double a=-(get(0,0)+get(1,1)+get(2,2));
  double b=-(get(1,0)*get(0,1) + get(2,0)*get(0,2) + get(1,2)*get(2,1) -
	     get(0,0)*get(1,1) - get(0,0)*get(2,2) - get(1,1)*get(2,2))/2.0;
  double c=-det();

  cout << "a=" << a << " b=" << b << " c=" << c << endl;

  solveCubic(a,b,c,eigx,eigy,eigz);

  cdouble x1=eigx,x2=eigy,x3=eigz;
  cout << "x1=" << x1 << " x2=" << x2 << " x3=" << x3 << endl;
  cout << "f(x1)=" << x1*x1*x1 + a*x1*x1 + b*x1 + c << endl;
  cout << "f(x2)=" << x2*x2*x2 + a*x2*x2 + b*x2 + c << endl;
  cout << "f(x3)=" << x3*x3*x3 + a*x3*x3 + b*x3 + c << endl;


  cout << "eigx=" << eigx << " eigy=" << eigy << " eigz=" << eigz << endl;
}

void Matrix3::dump() const {
  const ElementSequence elements=getElement();
  double a00=elements[0];
  double a01=elements[1];
  double a02=elements[2];

  double a10=elements[3];
  double a11=elements[4];
  double a12=elements[5];

  double a20=elements[6];
  double a21=elements[7];
  double a22=elements[8];

  cout << "Matrix3:" << endl;
  cout << " (" << a00 << " " << a01 << " " << a02 << endl;
  cout << " (" << a10 << " " << a11 << " " << a12 << endl;
  cout << " (" << a20 << " " << a21 << " " << a22 << ")" << endl;
}

Matrix3 Matrix3::operator+(const Matrix3& m) const {
  //Well unlike with a serious matrix class, this doesn't have to be efficent
  Matrix3 retVal(get(0,0)+m.get(0,0),
		 get(0,1)+m.get(0,1),
		 get(0,2)+m.get(0,2),
		 get(1,0)+m.get(1,0),
		 get(1,1)+m.get(1,1),
		 get(1,2)+m.get(1,2),
		 get(2,0)+m.get(2,0),
		 get(2,1)+m.get(2,1),
		 get(2,2)+m.get(2,2));
  return retVal;
}

Vector Matrix3::getEigenvectorX() const {
  if(!foundEigenVals) {
    calcEigenvalues();
  }
  //Chose a value for x1, in this case, let x1=1
  //Then solve the two remaining simultanious equations
  cdouble y1=-get(0,1);
  cdouble y2=-get(0,2);

  //Diagonalise the 2 by 2 submatrix, watching out for any division by zero
  cdouble j,k,w1,w2;
  if(get(1,2)!=0) {
    j=get(2,1)/(get(1,1)-eigx);
    w1=y1/(get(1,1)-eigx);

    k=(get(2,2)-eigx)/get(1,2) - j;
    w2=y2/get(1,2)-w1;
  } else {
    cout << "zero found " << endl;
    j=get(2,1)/(get(1,1)-eigx);
    w1=y1/(get(1,1)-eigx);

    k=(get(2,2)-eigx);
    w2=y2;
    cout << " j=" << j << " w1=" << w1 << endl;
  }


  cdouble v2=w2/k;
  cdouble v1=w1-j*v2;

  cout << " unnormaized=(1.0," << v1 << ","  << v2 << ")" << endl;

  //Normalise
  cout << " w1=" << w1 << " v2=" << v2 << endl;
  cdouble norm=sqrt(1.0+w1*w1+v2*v2);

  Vector eig((1.0/norm).real(),(w1/norm).real(),(v2/norm).real());

  return eig;
}

Vector Matrix3::getEigenvectorY() const {
  if(!foundEigenVals) {
    calcEigenvalues();
  }
  /*n  //Chose a value for x1, in this case, let x1=1
  //Then solve the two remaining simultanious equations
  double y1=eigx-get(0,0);
  double y2=eigx-get(0,2);

  double j=get(2,0)/get(0,0);
  double k=get(2,2)/get(0,2) - j;

  double w1=y1/get(0,0);
  double w2=y2/get(0,2)-y2/get(0,0);

  double v3=w2/k;

  //Normalise
  double norm=sqrt(1+w1*w1+v3*v3);

  Vector eig(1.0/norm,w1/norm,v3/norm);
  */
  return Vector(1.0,1.0,1.0);
}

Vector Matrix3::getEigenvectorZ() const {
  if(!foundEigenVals) {
    calcEigenvalues();
  }
  //Chose a value for x1, in this case, let x1=1
  //Then solve the two remaining simultanious equations
  /*double y1=eigx-get(0,0);
  double y2=eigx-get(0,1);

  double j=get(1,0)/get(0,0);
  double k=get(1,1)/get(0,1) - j;

  double w1=y1/get(0,0);
  double w2=y2/get(0,1)-y2/get(0,0);

  double v3=w2/k;

  //Normalise
  double norm=sqrt(1+w1*w1+v3*v3);

  Vector eig(1/norm,w1/norm,v3/norm);
  */
  return Vector(1.0,1.0,1.0);
}

Vector Matrix3::solveSimEq(double y1,double y2,double y3) const {
  //This function may not actually be needed
  //First iteration, make every value on the top first collum zero except the first
  //Which will be one.

  //Start with the following matrix:
  // (a,b,c,
  //  d,e,f
  //  g,h,i) where b=get(1,0) for example

  double j = get(1,0)/get(0,0);
  double k = get(2,0)/get(0,0);
  double l = get(1,1)/get(0,1)-j;
  double m = get(2,1)/get(0,1)-k;
  double n = get(1,2)/get(0,2)-j;
  double p = get(2,2)/get(0,2)-k;;

  //Now we have
  // (1,j,k,
  //  0,l,m,
  //  0,n,p)
  //Repeate the process with the lower 2x2 matrix

  double q=m/l;
  double r=p/n-q;

  //Matrix is in the form
  // (1,j,k,
  //  0,1,q,
  //  0,0,r)

  double w1=y1/get(0,0);
  double w2=y2/get(0,1)-w1;
  double w3=y2/get(0,2)-w1;

  //z1=w1
  double z2=w2/l;
  double z3=(w3/n-w2/l)/r;

  return Vector(w1,z2,z3);
}


