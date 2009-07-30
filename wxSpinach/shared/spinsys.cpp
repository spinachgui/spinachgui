
#include "spinsys.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/algorithm/string/trim.hpp>

using namespace std;

Spinsys::Spinsys() : mXMLSpinSys(new Spin_system()) {

}

void Spinsys::createNew() {
  mXMLSpinSys.reset(new Spin_system());
  mSpins.resize(0);
}

void Spinsys::dump() const{
  cout << "Printing out a spin system" << endl;
  cout << "Spins:" << endl;
  for (Spin_system::SpinConstIterator i (mXMLSpinSys->getSpin().begin());
       i != mXMLSpinSys->getSpin().end();
       ++i)
    {
      cout << " Spin name=" << i->getLabel() << endl;
      cout << "      number=" << i->getNumber() << endl;
      cout << "      isotope=" << i->getIsotope() << endl;
      cout << "      coords=(" << i->getCoordinates().getX() << ",";
      cout << "              " << i->getCoordinates().getY() << ",";
      cout << "              " << i->getCoordinates().getZ() << ")" << endl;
    }


}

void Spinsys::loadFromFile(const char* filename) {
  createNew();
  try {
    mXMLSpinSys = parseSpin_system(filename);
  } catch(const xml_schema::Exception& e) {
    cerr << e << endl;
    exit(1);
  }

  //Load the spins
  for(Spin_system::SpinConstIterator i(mXMLSpinSys->getSpin().begin()); 
      i != mXMLSpinSys->getSpin().end();
      ++i) {
    cout << "Loading a spin" << endl;
    SpinachSpin thisSpin(*i);
    mSpins.push_back(thisSpin);
  }
  //Load the interactions
  for(Spin_system::InteractionConstIterator i(mXMLSpinSys->getInteraction().begin()); 
      i != mXMLSpinSys->getInteraction().end();
      ++i) {
    cout << "Loading an interaction" << endl;
    SpinachInteraction thisInt(*i);
    mInteractions.push_back(thisInt);
  }
  //Load the frames
  for(Spin_system::Reference_frameConstIterator i(mXMLSpinSys->getReference_frame().begin()); 
      i != mXMLSpinSys->getReference_frame().end();
      ++i) {
    cout << "Loading a frame" << endl;
    SpinachFrame thisFrame(*i);
    mFrames.push_back(thisFrame);
  }
  
}

void Spinsys::loadFromG03File(const char* filename) {
  /*
    This function really needs some work done on in, as it's not using C++
    streams properly. This is due to it being adapted from matlab code (which uses
    c style I/O
   */
  createNew();
  ifstream fin(filename);
  cout << "Opening a g03 file:" << filename << endl;
  if(!fin.is_open()) {
    //Throw some sort of error here
    cerr << "Couldn't open file" << endl;
    return;
  }
  long nAtoms=0;
  while(!fin.eof()) {
    string line;
    char buff[500];  //TODO buletproof this
    fin.getline(buff,500); line=buff; //Read a line
    boost::algorithm::trim(line); //Remove whitespace

    if (line=="Standard orientation:") {
      cout << "Standard orientation found." << endl;
      //Skip 4 lines
      fin.getline(buff,500); line=buff; //Read a line
      fin.getline(buff,500); line=buff; //Read a line
      fin.getline(buff,500); line=buff; //Read a line
      fin.getline(buff,500); line=buff; //Read a line
      istringstream stream;

      fin.getline(buff,500); line=buff; stream.str(line); //Read a line
      while(line.find("--------") == string::npos && !fin.eof()) {
	int dummy1,dummy2,dummy3;
	double x,y,z;
	stream >> dummy1 >> dummy2 >> dummy3 >> x >> y >> z;
	nAtoms++;
	stream.clear();
	fin.getline(buff,500); line=buff; stream.str(line); //Read a line
	Spin s(Vector(x,y,z),nAtoms,"H1",0);
	s.setLabel("A Spin");
	mSpins.push_back(s);
      }
    }
    if (line=="Isotropic Fermi Contact Couplings") {
      //Skip a line
      fin.getline(buff,500); line=buff; //Read a line
      for (long i=0;i<nAtoms;i++) {
	fin.getline(buff,500); line=buff; //Read a line
	cout << "This should be a fermi-contact line" << line << endl;
	//S=char(A(n+1+k)); S=eval(['[' S(20:end) ']']); iso_hfc(k)=S(3);
      }          
    }
    if(line=="Anisotropic Spin Dipole Couplings in Principal Axis System") {
      cout << "Anisotropic spin dipole couplings found." << endl; 
    
      for (long i=0;i>nAtoms;i++) {
	fin.getline(buff,500); line=buff; //Read a line
	cout << line << endl;
	fin.getline(buff,500); line=buff; //Read a line
	cout << line << endl;
	fin.getline(buff,500); line=buff; //Read a line
	cout << line << endl;
        //nbaa=A(n+4*k+1); baa=char(baa); baa=eval(['[' baa(4:end) ']']); Baa(k)=baa(3); aa_axis(k,:)=baa(5:7);
        //bbb=A(n+4*k+2); bbb=char(bbb); bbb=eval(['[' bbb(14:end) ']']); Bbb(k)=bbb(3); bb_axis(k,:)=bbb(5:7);
	//bcc=A(n+4*k+3); bcc=char(bcc); bcc=eval(['[' bcc(4:end) ']']); Bcc(k)=bcc(3); cc_axis(k,:)=bcc(5:7);
      }
    }
  }
  dump();
}

void Spinsys::addSpin() {
  //  Spin s();
  //  mXMLSpinSys->getSpin().push_back(s);
}

void Spinsys::saveToFile(const char* filename) {
  xml_schema::NamespaceInfomap map;
  map[""].name = "";
  map[""].schema = "../data/spinsys_spec.xsd";

  ofstream fout(filename);
  if(!fout.is_open()) {
    cerr << "Could not open " << filename << endl;
    return;
  }
  serializeSpin_system(fout, *mXMLSpinSys, map);
}

long Spinsys::getSpinCount() const {
  return mSpins.size();
}

SpinachSpin Spinsys::getSpin(long n) {
  if(n>=0 && n<mSpins.size()) {
    return mSpins[n];
  }
  SpinachSpin s;
  return s;
}

SpinachInteraction Spinsys::getInteraction(long n) {
  if(n>=0 && n<mInteractions.size()) {
    return mInteractions[n];
  }
  SpinachInteraction inter;
  return inter;
}

long Spinsys::getInteractionCount() const {
  return mInteractions.size();
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

Matrix3 SpinachOrientation::getAsMatrix() const {
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
  } else if(getForm()==INTER_EIGENVALUES) {
    //Convert to a matrix

    const Eigenvalues ev=getEigenvalues().get();

    double xx=ev.getXX();
    double yy=ev.getYY();
    double zz=ev.getZZ();

    const SpinachOrientation o=getOrientation().get();
    Matrix3 intMatrix=o.getAsMatrix();

    intMatrix.set(0,0,intMatrix.get(0,0)*xx);
    intMatrix.set(0,1,intMatrix.get(0,1)*xx);
    intMatrix.set(0,2,intMatrix.get(0,2)*xx);

    intMatrix.set(1,0,intMatrix.get(1,0)*yy);
    intMatrix.set(1,1,intMatrix.get(1,1)*yy);
    intMatrix.set(1,2,intMatrix.get(1,2)*yy);

    intMatrix.set(2,0,intMatrix.get(2,0)*zz);
    intMatrix.set(2,1,intMatrix.get(2,1)*zz);
    intMatrix.set(2,2,intMatrix.get(2,2)*zz);
    return intMatrix;
  } else {
    cerr << "Interaction type not suported in getAsMatrix()" << endl;
  }
  //Return the identity
  Matrix3 identity(1,0,0,0,1,0,0,0,1);
  return identity;
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
		 double a20,double a21,double a22) {
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
