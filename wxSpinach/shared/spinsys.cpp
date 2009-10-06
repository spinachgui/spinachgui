#include <shared/spinsys.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <shared/nuclear_data.hpp>

using namespace std;
using namespace SpinXML;



//==============================================================================//
// Vector3

Vector3::Vector3() {
    
}

Vector3::Vector3(double _x,double _y,double _z) : x(_x),y(_y),z(_z) {

}

double Vector3::GetX() const {
  return x;
}
double Vector3::GetY() const {
  return y;
}

double Vector3::GetZ() const {
  return z;
}

void Vector3::SetX(double _x) {
  x=_x;
  return;
}

void Vector3::SetY(double _y) {
  y=_y;
  return;
}

void Vector3::SetZ(double _z) {
  z=_z;
  return;
}

void Vector3::GetCoordinates(double* _x,double* _y, double* _z) const {
  *_x=x;
  *_y=y;
  *_z=z;
  return;
}

void Vector3::SetCoordinates(double _x,double _y, double _z) {
  x=_x;
  y=_y;
  z=_z;
  return;
}


//==============================================================================//
// Matrix3
    
const double IDENTITY3[]={1.0,0.0,0.0,
	  		0.0,1.0,0.0,
			  0.0,0.0,1.0};


Matrix3::Matrix3() {
  memcpy(raw,IDENTITY3,9*sizeof(double));
}

Matrix3::Matrix3(double a00,double a01,double a02,double a10,double a11,double a12,double a20,double a21,double a22) {
  raw[0]=a00;   raw[1]=a01;   raw[2]=a02;
  raw[3]=a10;   raw[4]=a11;   raw[5]=a12;
  raw[6]=a20;   raw[7]=a21;   raw[8]=a22;
}

Matrix3::Matrix3(const Matrix3& m) {
  memcpy(raw,m.raw,9*sizeof(double));
}
    
const double* Matrix3::GetRaw() const {
  return raw;
}
    
double Matrix3::Get(long column, long row) const {
  return raw[3*column+row];
}

double Matrix3::operator() (long column, long row) const {
  return raw[3*column+row];
}

Matrix3 Matrix3::operator+ (const Matrix3& m) const {
  Matrix3 result;
  result.raw[0]=raw[0]+m.raw[0];
  result.raw[1]=raw[1]+m.raw[1];
  result.raw[2]=raw[2]+m.raw[2];
	      	     	       
  result.raw[3]=raw[3]+m.raw[3];
  result.raw[4]=raw[4]+m.raw[4];
  result.raw[5]=raw[5]+m.raw[5];
	      	     	       
  result.raw[6]=raw[6]+m.raw[6];
  result.raw[7]=raw[7]+m.raw[7];
  result.raw[8]=raw[8]+m.raw[8];
  return result;
}

Matrix3& Matrix3::operator+= (const Matrix3& m) {
  raw[0]=raw[0]+m.raw[0];
  raw[1]=raw[1]+m.raw[1];
  raw[2]=raw[2]+m.raw[2];
       	     	       
  raw[3]=raw[3]+m.raw[3];
  raw[4]=raw[4]+m.raw[4];
  raw[5]=raw[5]+m.raw[5];
       	     	       
  raw[6]=raw[6]+m.raw[6];
  raw[7]=raw[7]+m.raw[7];
  raw[8]=raw[8]+m.raw[8];
  return *this;
}

Matrix3& Matrix3::operator= (const Matrix3& m) {
  memcpy(raw,m.raw,9*sizeof(double));
}

void Matrix3::Set(long column,long row,double val) {
  raw[3*column+row]=val;
}

void Matrix3::Dump() const {
  cout << "Matrix3:" << endl;
  cout << " (" << raw[0] << " " <<  raw[1] << " " <<  raw[2] << endl;
  cout << "  " << raw[3] << " " <<  raw[4] << " " <<  raw[5] << endl;
  cout << "  " << raw[6] << " " <<  raw[7] << " " <<  raw[8] << ")" << endl;
}



//==============================================================================//
// SpinSystem

SpinSystem::SpinSystem() : mLabFrame(new ReferenceFrame(NULL,new Vector3(0,0,0),new Orientation())) {
}

SpinSystem::SpinSystem(const SpinSystem& system) : mLabFrame(system.mLabFrame) {
  long spinCount=system.mSpins.size();
  long interCount=system.mInteractions.size();

  mSpins.resize(spinCount);
  mInteractions.resize(interCount);

  for(long i=0;i<spinCount;i++) {
    mSpins[i]=new Spin(*system.mSpins[i],this);
  }
  for(long i=0;i<interCount;i++) {
    mInteractions[i]=new Interaction(*system.mInteractions[i]);
    //Problem, mSpin1 and mSpin2 will still point to the spins in the
    //old SpinSystem
    Spin* Spin1=system.mInteractions[i]->mSpin1;
    Spin* Spin2=system.mInteractions[i]->mSpin2;
    int found=0;
    for(long j=0;j<spinCount;j++) {
      if(Spin1==system.mSpins[j]) {
	mInteractions[i]->mSpin1=mSpins[j];
	found++;
      }
      if(Spin2==system.mSpins[j]) {
	mInteractions[i]->mSpin2=mSpins[j];
	found++;
      }
      if(found==2) {
	break;
      }
    }
  }
  //TODO: Impliment proper copying of reference frames
  mLabFrame=new ReferenceFrame(NULL,new Vector3(0,0,0),new Orientation());
  mLabFrame->GetOrientation()->SetQuaternion(1.0,0.0,0.0,0.0);
}

SpinSystem::~SpinSystem() {
  Clear();
}

void SpinSystem::Clear() {
  for(long i=0;i<mSpins.size();i++) {
    delete mSpins[i];
  }
  for(long i=0;i<mInteractions.size();i++) {
    delete mInteractions[i];
  }
}

void SpinSystem::Dump() const {
  cout << "Dumping SpinSystem:" << endl;
  cout << " Spin count=" << mSpins.size() << endl;
  cout << " Interaction count=" << mInteractions.size() << endl;
  cout << endl;

  cout << "Dumping Spins:" << endl;
  for(long i=0;i<mSpins.size();i++) {
    mSpins[i]->Dump();
  }
}
    
long SpinSystem::GetSpinCount() const {
  return mSpins.size();
}

Spin* SpinSystem::GetSpin(long n) const {
  return mSpins[n];
}

long SpinSystem::GetSpinNumber(Spin* spin) const {
  for(long i=0;i<mSpins.size();i++) {
    if(mSpins[i]==spin) {
      return i;
    }
  }
  return -1;
}

vector<Spin*> SpinSystem::GetSpins() const {
  return mSpins;
}

void SpinSystem::InsertSpin(Spin* _Spin,long Position) {
  if(Position==END) {
    mSpins.push_back(_Spin);
  } else {
    mSpins.insert(mSpins.begin()+Position,_Spin);
  }
}

void SpinSystem::RemoveSpin(long Position) {
  mSpins.erase(mSpins.begin()+Position);
}

void SpinSystem::RemoveSpin(Spin* _Spin) {
  for(long i=0;i<mSpins.size();i++) {
    if(mSpins[i]==_Spin) {
      mSpins.erase(mSpins.begin()+i);
      return;
    }
  }
}

ReferenceFrame* SpinSystem::GetRootFrame() const {
  return mLabFrame;
}

void SpinSystem::LoadFromGamesFile(const char* filename) {
  
}

void SpinSystem::LoadFromG03File(const char* filename) throw(runtime_error){
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
    throw runtime_error("Couldn't Open File");
  }
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
	  if(mSpins.size() > nAtoms) {
	    mSpins[nAtoms]->SetPosition(new Vector3(x,y,z));
	  } else {
	    cerr << "Error reading standard orientation: the number of centres is not consistant amoung the standard orientation tables" << endl;
	    throw runtime_error("Error reading standard orientation: the number of centres is not consistant amoung the standard orientation tables");
	  }
	} else {
	  //Create a new spin
	  string isotopeSymbol(getElementSymbol(atomicNumber));
	  isotopeSymbol=isotopeSymbol; //We have no clue what the mass number is yet
	  Spin* s=new Spin(this,new Vector3(x,y,z),string("A Spin")+isotopeSymbol,GetRootFrame(),atomicNumber);
	  mSpins.push_back(s);
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
	  ostringstream eout;
	  eout << "Error reading GIAO Magnetic shielding tensor" << endl;
	  eout << "Expected 'Isotropic' got " << Isotropic << endl;
	  eout << "Expected 'Anisotropy' got " << Anisotropy << endl;
	  eout << "Expected 'Eigenvalues:' got " << Eigenvalues << endl;

	  eout << "XX=" << sXX;
	  eout << " XY=" << sXY;
	  eout << " XZ=" << sXZ << endl;

	  eout << "YX=" << sYX;
	  eout << " YY=" << sYY;
	  eout << " YZ=" << sYZ << endl;

	  eout << "ZX=" << sZX;
	  eout << " ZY=" << sZY;
	  eout << " ZZ=" << sZZ << endl;
	  cerr << eout.str();
	  throw runtime_error(eout.str());
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
	    ostringstream eout;
	    eout << "Error reading J couplings, found topCenterNumber=" << topCenterNumber << " expected " << i*5+k+1 << endl; 
	    cerr << eout.str();
	    throw runtime_error(eout.str());
	  }
	}

	for(long j=i*5;j<nAtoms;j++) {  //For each little row in the big row
	  long centerNumber;
	  fin >> centerNumber;
	  if(centerNumber != j+1) {
	    ostringstream eout;
	    eout << "Error reading J Couplings, found centerNumber=" << centerNumber << " expected " << j+1 << endl;
	    cerr << eout.str();
	    throw runtime_error(eout.str());
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
	      ostringstream eout;
	      eout << "Was expecting a J coupling value in the form +-0.00000D00 but there was no D. Got" << JCouplingStr << endl;
	      cerr << eout.str();
	      throw runtime_error(eout.str());
	    }
	    //Pack it back into a istream and parse
	    istringstream stream;
	    stream.str(JCouplingStr);
	    stream >> JCoupling;
	    Interaction* inter=new Interaction();
            inter->SetScalar(JCoupling);
	    inter->SetSubType(Interaction::ST_SCALAR);
	    inter->SetSpin1(GetSpin(j+1));
            inter->SetSpin2(GetSpin(i*5+k+1));
	    mInteractions.push_back(new Interaction());
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
        
        Interaction* inter=new Interaction();
        inter->SetScalar(isoCoupling);
	inter->SetSubType(Interaction::ST_HFC);
	inter->SetSpin1(GetSpin(i));
	mInteractions.push_back(inter);
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

	Orientation* o=new Orientation();
	o->SetEigenSystem(new Vector3(x1,y1,z1),new Vector3(x2,y2,z2),new Vector3(x3,y3,z3));

	Interaction* inter=new Interaction(); //Last paramiter is reference frame, which is always lab
	inter->SetEigenvalues(eigenvalue1*0.05,eigenvalue2*0.05,eigenvalue3*0.05,o);
	inter->SetSubType(Interaction::ST_HFC);
	inter->SetSpin1(GetSpin(i));
	
	mInteractions.push_back(inter);
      }
    }
  }
  cout << "Finished loading the g03 file, saving mSpins.size()=" << mSpins.size() << endl;
}



//==============================================================================//
// Spin

Spin::Spin(SpinSystem* Parent,Vector3* Position,string Label,ReferenceFrame* mFrame,long atomicNumber) 
  : mParent(Parent),mPosition(Position),mLabel(Label),mElement(atomicNumber) {
  
}

Spin::Spin(const Spin& s,SpinSystem* ss) :
  mParent(ss),
  mPosition(new Vector3(*s.mPosition)),
  mLabel(s.mLabel),
  mFrame(NULL),
  mElement(s.mElement),
  mIsotopes(s.mIsotopes){
  
}

Spin::~Spin() {
  if(mPosition != NULL) {
    delete mPosition;
  }
}

void Spin::Dump() const {
  cout << "  Dumping class Spin" << endl;
  cout << "    Label=" << mLabel << endl;
  cout << "    Position=(" << mPosition->GetX() << "," << mPosition->GetY() << "," << mPosition->GetZ() << ")" << endl;
  cout << "    Interaction Count=" << GetInteractionCount() << endl;
}

Vector3* Spin::GetPosition() const {
  return mPosition;
}

void Spin::SetPosition(Vector3* Position) {
  if(mPosition != NULL) {
    delete mPosition;
  }
  mPosition=Position;
}

void Spin::GetCoordinates(double* _x,double* _y, double* _z) const {
  mPosition->GetCoordinates(_x,_y,_z);
}


void Spin::SetCoordinates(double _x,double _y, double _z) {
  mPosition->SetCoordinates(_x,_y,_z);
}

void Spin::SetLabel(string Label) {
  mLabel=Label;
}

const char* Spin::GetLabel() const {
  return mLabel.c_str();
}
    
long Spin::GetInteractionCount() const {
  long count=0;
  for(long i=0;i<mParent->mInteractions.size();i++) {
    if(mParent->mInteractions[i]->GetSpin1()==this || mParent->mInteractions[i]->GetSpin2()==this) {
      count++;
    }
  }
  return count;
}

Interaction* Spin::GetInteraction(long n) const {
  long count=0;
  for(long i=0;i<mParent->mInteractions.size();i++) {
    if(mParent->mInteractions[i]->GetSpin1()==this || mParent->mInteractions[i]->GetSpin2()==this) {
      if(count==n) {
	return mParent->mInteractions[i];
      }
      count++;
    }
  }
  return NULL;
}

vector<Interaction*> Spin::GetInteractions() const {
  vector<Interaction*> InterPtrs;
  for(long i=0;i<mParent->mInteractions.size();i++) {
    if(mParent->mInteractions[i]->GetSpin1()==this || mParent->mInteractions[i]->GetSpin2()==this) {
      InterPtrs.push_back(mParent->mInteractions[i]);
    }
  }
  return InterPtrs;
}

void Spin::InsertInteraction(Interaction* _Interaction,long Position) {
  if(Position==END) {
    mParent->mInteractions.push_back(_Interaction);
  } else {
    long count=0;
    for(long i=0;i<mParent->mInteractions.size();i++) {
      if(mParent->mInteractions[i]->GetSpin1()==this || mParent->mInteractions[i]->GetSpin2()==this) {
	if(count==Position) {
	  mParent->mInteractions.insert(mParent->mInteractions.begin()+i,_Interaction);
	  return;
	}
	count++;
      }
    }
  }
}

void Spin::RemoveInteraction(long Position) {
  long count=0;
  for(long i=0;i<mParent->mInteractions.size();i++) {
    if(mParent->mInteractions[i]->GetSpin1()==this || mParent->mInteractions[i]->GetSpin2()==this) {
      if(count==Position) {
	cout << "Errasing an interaction" << endl;
	mParent->mInteractions.erase(mParent->mInteractions.begin()+i);
      }
      count++;
    }
  }
}

void Spin::RemoveInteraction(Interaction* _Interaction) {
  if(_Interaction == NULL) {
    return;
  }
  for(long i=0;i<mParent->mInteractions.size();i++) {
    if(mParent->mInteractions[i]==_Interaction) {
      mParent->mInteractions.erase(mParent->mInteractions.begin()+i);
    }
  }
}

double Spin::GetLinearInteractionAsScalar(Interaction::SubType t) const {
  double total=0.0;
  long interCount=mParent->mInteractions.size();
  for(long i=0;i < interCount;++i) {
    Interaction* inter=mParent->mInteractions[i];
    if(inter->GetSpin2()!=NULL) {
      continue;
    }
    if(inter->GetSpin1()==this) {
      //Interaction is relevent
      if(inter->IsSubType(t)) {
	total+=inter->GetAsScalar();
      }
    }
  }
  return total;
}

double Spin::GetBilinearInteractionAsScalar(Spin* OtherSpin,Interaction::SubType t) const {
  return 0;
}

double Spin::GetQuadrapolarInteractionAsScalar(Interaction::SubType t) const {
  return 0;
}

Matrix3 Spin::GetLinearInteractionAsMatrix(Interaction::SubType t) const {
  Matrix3 total=Matrix3(0,0,0,
			0,0,0,
			0,0,0);
  long interCount=mParent->mInteractions.size();
  for(long i=0;i < interCount;++i) {
    Interaction* inter=mParent->mInteractions[i];
    if(inter->GetSpin2()!=NULL) {
      continue;
    }
    if(inter->GetSpin1()==this) {
      //Interaction is relevent
      if(inter->IsSubType(t)) {
	total+=inter->GetAsMatrix();
      }
    }
  }
  return total;
}

Matrix3 Spin::GetBilinearInteractionAsMatrix(Spin* OtherSpin,Interaction::SubType t) const {
  return Matrix3(1,0,0,0,1,0,0,0,1);
}

Matrix3 Spin::GetQuadrapolarInteractionAsMatrix(Interaction::SubType t) const {
  return Matrix3(1,0,0,0,1,0,0,0,1);
}


Interaction::SubType Interaction::GetSubType() const {
  return mSubType;
}

void Interaction::SetSubType(SubType st) {
  mSubType=st;
}

bool Interaction::IsSubType(SubType t) const {
  if(t==ST_ANY) {
    return true;
  }
  if(t==ST_EPR) {
    switch(t) {
    case ST_HFC:
    case ST_G_TENSER:
    case ST_ZFS: 
    case ST_EXCHANGE:
    case ST_QUADRUPOLAR:
    case ST_DIPOLAR:
    case ST_CUSTOM:
      return true;
    default:
      return false;
    }
  }
  if(t==ST_NMR) {
    switch(t) {
    case ST_SHIELDING:
    case ST_SCALAR:
    case ST_QUADRUPOLAR:
    case ST_DIPOLAR:
    case ST_CUSTOM:
      return true;
    default:
      return false;
    }
  }
  if(t==mSubType) {
    return true;
  }
  return false;
}

    
ReferenceFrame* Spin::GetFrame() const {
  return mFrame;
}

void Spin::SetFrame(ReferenceFrame* Frame) {
  mFrame=Frame;
}


std::vector<Spin*> Spin::GetNearbySpins(double distance) {
  std::vector<Spin*> result;

  double dist2=distance*distance;
  double x1,y1,z1;
  GetCoordinates(&x1,&y1,&z1);

  long spinCount=mParent->mSpins.size();
  //Skip spins up to this spin in mParent->mSpins
  long pos;
  for(pos=0;mParent->mSpins[pos]!=this && pos<spinCount;pos++){}

  
  for(long i=pos+1;i<spinCount;i++) {
    double x2,y2,z2;
    mParent->mSpins[i]->GetCoordinates(&x2,&y2,&z2);
    double deltaX=(x1-x2);
    double deltaY=(y1-y2);
    double deltaZ=(z1-z2);
    if(deltaX*deltaX+deltaY*deltaY+deltaZ*deltaZ < dist2) {
      result.push_back(mParent->mSpins[i]);
    }
  }
  return result;
}

long Spin::GetElement() const {
  return mElement;
}

void Spin::SetElement(long element) {
  mElement=element;
}

void Spin::SetIsotopes(std::vector<long> isotopes) const {

}
 
vector<long> Spin::GetIsotopes() const {
  vector<long> retVal;
  retVal.push_back(1);
  return retVal;
}


//==============================================================================//
// Orientation

Orientation::Orientation() : mType(UNDEFINED) {
  
}

Orientation::Orientation(const Orientation& orient) :
  mType(orient.mType) {
  if(mType==ANGLE_AXIS) {
    mData.mAngleAxis.axis = new Vector3(*orient.mData.mAngleAxis.axis);
  } else if(mType==EIGENSYSTEM) {
    mData.mEigenSystem.XAxis = new Vector3(*orient.mData.mEigenSystem.XAxis);
    mData.mEigenSystem.YAxis = new Vector3(*orient.mData.mEigenSystem.YAxis);
    mData.mEigenSystem.ZAxis = new Vector3(*orient.mData.mEigenSystem.ZAxis);
  } else if(mType==QUATERNION) {
    mData.mQuaternion.real = orient.mData.mQuaternion.real;
    mData.mQuaternion.i    = orient.mData.mQuaternion.i;
    mData.mQuaternion.j    = orient.mData.mQuaternion.j;
    mData.mQuaternion.k    = orient.mData.mQuaternion.k;
  } else if(mType==EULER) {
    mData.mEuler.alpha =     orient.mData.mEuler.alpha;
    mData.mEuler.beta  =     orient.mData.mEuler.beta;
    mData.mEuler.gamma =     orient.mData.mEuler.gamma;
  }
}

Orientation::~Orientation() {
  if(mType==ANGLE_AXIS && mData.mAngleAxis.axis != NULL) {
    delete mData.mAngleAxis.axis;
  } else if(mType==EIGENSYSTEM) {
    if(mData.mEigenSystem.XAxis != NULL) {
      delete mData.mEigenSystem.XAxis;
    }
    if(mData.mEigenSystem.YAxis != NULL) {
      delete mData.mEigenSystem.YAxis;
    }
    if(mData.mEigenSystem.ZAxis != NULL) {
      delete mData.mEigenSystem.ZAxis;
    }
  }
}
 
Orientation::Type Orientation::GetType() const {
  return mType;
}

Matrix3 Orientation::GetAsMatrix() const throw(logic_error) {
  if(mType==EIGENSYSTEM) {
    const Vector3* xa=mData.mEigenSystem.XAxis;
    const Vector3* ya=mData.mEigenSystem.YAxis;
    const Vector3* za=mData.mEigenSystem.ZAxis;

    return Matrix3(xa->GetX(),ya->GetX(),za->GetX(),
		   xa->GetY(),ya->GetY(),za->GetY(),
		   xa->GetZ(),ya->GetZ(),za->GetZ());
  }
  return Matrix3(1,0,0,0,1,0,0,0,1);
}

void Orientation::GetEuler(double* alpha,double* beta,double* gamma) const {
  *alpha = mData.mEuler.alpha;
  *beta = mData.mEuler.beta;
  *gamma = mData.mEuler.gamma;
  return;
}

void Orientation::GetAngleAxis(double* angle,Vector3* axis) const {
  *angle = mData.mAngleAxis.angle;
  *axis = *mData.mAngleAxis.axis;
  return;
}

void Orientation::GetQuaternion(double* real, double* i, double* j, double* k) const {
  *real = mData.mQuaternion.real;
  *i = mData.mQuaternion.i;
  *j = mData.mQuaternion.j;
  *k = mData.mQuaternion.k;
  return;
}

void Orientation::GetEigenSystem(Vector3* XAxis,Vector3* YAxis, Vector3* ZAxis) const {
  *XAxis = *mData.mEigenSystem.XAxis;
  *YAxis = *mData.mEigenSystem.YAxis;
  *ZAxis = *mData.mEigenSystem.ZAxis;
  return;
}

void Orientation::SetEuler(double alpha,double beta,double gamma) {
  mType=EULER;
  mData.mEuler.alpha=alpha;
  mData.mEuler.beta=beta;
  mData.mEuler.gamma=gamma;
  mType=EULER;
  return;
}

void Orientation::SetAngleAxis(double angle,Vector3* axis) {
  mType=ANGLE_AXIS;
  mData.mAngleAxis.angle=angle;
  mData.mAngleAxis.axis=axis;
  mType=ANGLE_AXIS;
  return;

}

void Orientation::SetQuaternion(double real, double i, double j, double k) {
  mType=QUATERNION;
  mData.mQuaternion.real=real;
  mData.mQuaternion.i=i;
  mData.mQuaternion.j=j;
  mData.mQuaternion.k=k;
  mType=QUATERNION;
  return;
}

void Orientation::SetEigenSystem(Vector3* XAxis,Vector3* YAxis, Vector3* ZAxis) {
  mType=EIGENSYSTEM;
  mData.mEigenSystem.XAxis=XAxis;
  mData.mEigenSystem.YAxis=YAxis;
  mData.mEigenSystem.ZAxis=ZAxis;
  return;
}

string Orientation::ToString() const {
  ostringstream oss;
  switch(mType) {
  case EULER:
    oss << "eu:" << mData.mEuler.alpha << "," << mData.mEuler.beta << "," << mData.mEuler.gamma;
    break;
  case ANGLE_AXIS:
    oss << "aa:"
	<< mData.mAngleAxis.angle << ","
	<< mData.mAngleAxis.axis->GetX() << ","
	<< mData.mAngleAxis.axis->GetY() << ","
	<< mData.mAngleAxis.axis->GetZ();
    break;
  case QUATERNION:
    oss << "q:"
	<< mData.mQuaternion.real << ","
	<< mData.mQuaternion.i    << ","
	<< mData.mQuaternion.j    << ","
	<< mData.mQuaternion.k;
    break;
  case EIGENSYSTEM:
    oss << "es:"
	<< mData.mEigenSystem.XAxis->GetX() << "," << mData.mEigenSystem.XAxis->GetY() << "," <<  mData.mEigenSystem.XAxis->GetZ() 
	<< mData.mEigenSystem.YAxis->GetX() << "," << mData.mEigenSystem.YAxis->GetY() << "," <<  mData.mEigenSystem.YAxis->GetZ() 
	<< mData.mEigenSystem.ZAxis->GetX() << "," << mData.mEigenSystem.ZAxis->GetY() << "," <<  mData.mEigenSystem.ZAxis->GetZ();
    break;
  case UNDEFINED:
    oss << "undefined";
    break;
  }
  return oss.str();
}

void Orientation::FromString(std::string string) {
  cerr << "Not Implimented" << endl;
}

//==============================================================================//
// Interaction

Interaction::Interaction() : mType(UNDEFINED),mSubType(ST_ANY),mSpin1(NULL),mSpin2(NULL) {
  
}

Interaction::Interaction(const Interaction& inter,SpinSystem* system) :
  mType(inter.mType),
  mSubType(inter.mSubType),
  mSpin1(inter.mSpin1),
  mSpin2(inter.mSpin2) {

  if(mType==SCALAR) {
    mData.mScalar=inter.mData.mScalar;
  } else if(mType==MATRIX) {
    mData.mMatrix=new Matrix3(*inter.mData.mMatrix);
  } else if(mType==EIGENVALUES) {
    mData.mEigenvalues.XX=inter.mData.mEigenvalues.XX;
    mData.mEigenvalues.YY=inter.mData.mEigenvalues.YY;
    mData.mEigenvalues.ZZ=inter.mData.mEigenvalues.ZZ;
    mData.mEigenvalues.Orient=new Orientation(*inter.mData.mEigenvalues.Orient);
  } else if(mType==AXRHOM) {
    mData.mAxRhom.ax=inter.mData.mAxRhom.ax;
    mData.mAxRhom.rh=inter.mData.mAxRhom.rh;
    mData.mAxRhom.iso=inter.mData.mAxRhom.iso;
    mData.mAxRhom.Orient=new Orientation(*inter.mData.mEigenvalues.Orient);
  } else if(mType==SPANSKEW) {
    mData.mSpanSkew.span=inter.mData.mSpanSkew.span;
    mData.mSpanSkew.skew=inter.mData.mSpanSkew.skew;
    mData.mSpanSkew.iso=inter.mData.mSpanSkew.iso;
    mData.mSpanSkew.Orient=new Orientation(*inter.mData.mEigenvalues.Orient);
  }
}

Interaction::~Interaction() {
  if(mType==EIGENVALUES && mData.mEigenvalues.Orient != NULL) {
    delete mData.mEigenvalues.Orient;
  } else if(mType==AXRHOM && mData.mAxRhom.Orient != NULL) {
    delete mData.mEigenvalues.Orient;
  } else if(mType==SPANSKEW && mData.mSpanSkew.Orient != NULL) {
    delete mData.mSpanSkew.Orient;
  }
}

void Interaction::Dump() const {
  cout << "  Dumping an Interaction" << endl;
  switch (GetType()) {
  case UNDEFINED:
    cout << "    Type=Undefined" <<  endl;
  case SCALAR:
    cout << "    Type=Scalar" <<  endl;
    cout << "    Value=" << mData.mScalar << endl;
  case MATRIX:
    cout << "    Type=Matrix" <<  endl;
    cout << "    Value=" << endl;
  case EIGENVALUES:
    cout << "    Type=Eigenvalues" <<  endl;
    cout << "    Value=" << endl;
  case AXRHOM:
    cout << "    Type=Axiality Rhombicity" <<  endl;
    cout << "    Value=" << endl; 
  case SPANSKEW:
    cout << "    Type=Span-Skew" <<  endl;
    cout << "    Value=" << endl;
  }
}

const char* Interaction::GetTypeName(Type t) {
  switch(t) {
  case UNDEFINED:
    return "Undefined";
  case SCALAR:
    return "Scalar";      
  case MATRIX:
    return "Matrix";     
  case EIGENVALUES:
    return "Eigenvalues";
  case AXRHOM:
    return "Axiality-Rhombicty";
  case SPANSKEW:
    return "Span-Skew";
  default:
    throw std::runtime_error("Unknow subtype submited to Interaction::GetTypeName()");
    return "Error in Interaction::GetTypeName()";
  };         
};


const char* Interaction::GetSubTypeName(SubType st) {
  switch(st) {
  case ST_ANY:         return "Any";
  case ST_EPR:         return "EPR";
  case ST_NMR:         return "NMR";
  case ST_HFC:         return "HFC";
  case ST_G_TENSER:    return "G Tensor";
  case ST_ZFS: 	       return "Zero Field Splitting";
  case ST_EXCHANGE:    return "Exchange";
  case ST_QUADRUPOLAR: return "Quadrupolar";
  case ST_DIPOLAR:     return "Dipolar";
  case ST_SHIELDING:   return "Shielding";
  case ST_SCALAR:      return "Scalar";
  case ST_CUSTOM:      return "Custom";
  default:
    throw std::runtime_error("Unknow subtype submited to Interaction::GetSubTypeName()");
    return "Error in Interaction::GetSubTypeName()";
  }
}
  
Interaction::Type Interaction::GetType() const {
  return mType;
}


void Interaction::SetSpin1(Spin* Spin1) {
  mSpin1=Spin1;
}

void Interaction::SetSpin2(Spin* Spin2) {
  mSpin2=Spin2;
}

Spin* Interaction::GetSpin1() const {
  return mSpin1;
}

Spin* Interaction::GetSpin2() const {
  return mSpin2;
}
    
void Interaction::GetScalar(double* Scalar) const {
  *Scalar=mData.mScalar;
}

void Interaction::GetMatrix(Matrix3* OutMatrix) const {
  *OutMatrix=*mData.mMatrix;
  return;
}

void Interaction::GetEigenvalues(double* XX,double* YY, double* ZZ, Orientation** OrientOut) const {
  *XX=mData.mEigenvalues.XX;
  *YY=mData.mEigenvalues.YY;
  *ZZ=mData.mEigenvalues.ZZ;
  *OrientOut=mData.mEigenvalues.Orient;
  return;
}

void Interaction::GetAxRhom(double* iso,double* ax, double* rh, Orientation** OrientOut) const {
  *iso=mData.mAxRhom.iso;
  *ax=mData.mAxRhom.ax;
  *rh=mData.mAxRhom.rh;
  *OrientOut=mData.mAxRhom.Orient;
  return;
}

void Interaction::GetSpanSkew(double* iso,double* Span, double* Skew, Orientation** OrientOut) const {
  *iso=mData.mSpanSkew.iso;
  *Span=mData.mSpanSkew.span;
  *Skew=mData.mSpanSkew.skew;
  *OrientOut=mData.mSpanSkew.Orient;
  return;
}

void Interaction::SetScalar(double Scalar) {
  mType=SCALAR;
  mData.mScalar=Scalar;
}

void Interaction::SetMatrix(Matrix3* Matrix) {
  mType=MATRIX;
  mData.mMatrix=Matrix;
}

void Interaction::SetEigenvalues(double XX,double YY, double ZZ, Orientation* Orient) {
  mType=EIGENVALUES;
  mData.mEigenvalues.XX=XX;
  mData.mEigenvalues.YY=YY;
  mData.mEigenvalues.ZZ=ZZ;
  mData.mEigenvalues.Orient=Orient;
  return;
}

void Interaction::SetAxRhom(double iso,double ax, double rh, Orientation* Orient) {
  mType=AXRHOM;
  iso=mData.mAxRhom.iso;
  ax=mData.mAxRhom.ax;
  rh=mData.mAxRhom.rh;
  Orient=mData.mAxRhom.Orient;
  return;
}

void Interaction::SetSpanSkew(double iso,double Span, double Skew, Orientation* Orient) {
  mType=SPANSKEW;
  mData.mSpanSkew.iso=iso;
  mData.mSpanSkew.span= Span;
  mData.mSpanSkew.skew=Skew;
  mData.mSpanSkew.Orient=Orient;
  return;
}

double Interaction::GetAsScalar() const {
  return 0;
}

Matrix3 Interaction::GetAsMatrix() const throw(logic_error) {
  if(mType==SCALAR) {
    //Return the identity multipled by the scalar
    double s=mData.mScalar;
    Matrix3 m(s,0,0,0,s,0,0,0,s);
    return m;
  } else if(mType==MATRIX) {
    //Just return the matrix
    Matrix3 m=*mData.mMatrix;
    return m;
  } else if(mType==EIGENVALUES || mType == AXRHOM || mType == SPANSKEW) {
    //Convert to a matrix
    double xx,yy,zz;

    Orientation* Orient;
    if(mType==EIGENVALUES) {
      xx=mData.mEigenvalues.XX;
      xx=mData.mEigenvalues.YY;
      xx=mData.mEigenvalues.ZZ;
      Orient=mData.mEigenvalues.Orient;
    } else if(mType==AXRHOM) {
      double a=mData.mAxRhom.ax;
      double r=mData.mAxRhom.rh;
      double iso=mData.mAxRhom.iso;
      Orient=mData.mAxRhom.Orient;
      xx=a/3+iso/3;
      yy=-r/2-a/6+iso/3;
      zz= r/2-a/6+iso/3;
    } else if(mType==SPANSKEW) {
      double span=mData.mSpanSkew.span;
      double skew=mData.mSpanSkew.skew;
      double iso=mData.mSpanSkew.iso;
      Orient=mData.mSpanSkew.Orient;
      xx=span*skew/6-span/2;
      yy=iso-span*skew/3;
      zz=span*skew/6+span/2;
    }
    Orient=mData.mSpanSkew.Orient;
    Matrix3 intMatrix=Orient->GetAsMatrix();

    intMatrix.Set(0,0,intMatrix.Get(0,0)*xx);
    intMatrix.Set(0,1,intMatrix.Get(0,1)*yy);
    intMatrix.Set(0,2,intMatrix.Get(0,2)*zz);

    intMatrix.Set(1,0,intMatrix.Get(1,0)*xx);
    intMatrix.Set(1,1,intMatrix.Get(1,1)*yy);
    intMatrix.Set(1,2,intMatrix.Get(1,2)*zz);

    intMatrix.Set(2,0,intMatrix.Get(2,0)*xx);
    intMatrix.Set(2,1,intMatrix.Get(2,1)*yy);
    intMatrix.Set(2,2,intMatrix.Get(2,2)*zz);
    return intMatrix;
  } else {
    cerr << "Interaction::mType is set to an invalid type! This is a serious programing error.\n" << endl;
    throw logic_error("Interaction::mType is set to an invalid type! This is a serious programing error.\n");
  }
  //Return the zero matrix identity
  Matrix3 zero(0,0,0,0,0,0,0,0,0);
  return zero;
}

void Interaction::SetQuadratic() {
  mSpin2=mSpin1;
}

void Interaction::SetLinear() {
  mSpin2=NULL;
}



bool Interaction::GetIsLinear() const {
  return mSpin2==NULL;
}

bool Interaction::GetIsBilinear() const {
  return mSpin2!=NULL && mSpin1 != mSpin2;
} 

bool Interaction::GetIsQuadratic() const {
  return mSpin2==mSpin1;
}


//==============================================================================//
// Reference Frame

ReferenceFrame::ReferenceFrame() : mParent(NULL),mPosition(NULL){
  
}

ReferenceFrame::ReferenceFrame(ReferenceFrame* Parent,Vector3* Position,Orientation* Orient) : mParent(Parent),mPosition(Position),mOrient(Orient) {

}

ReferenceFrame::~ReferenceFrame() {
  if(mPosition != NULL) {
    delete mPosition;
  }
  if(mOrient != NULL) {
    delete mOrient;
  }
}
    
long ReferenceFrame::GetChildCount() const {
  return mChildren.size();
}

ReferenceFrame* ReferenceFrame::GetChildFrame(long n) const {
  return mChildren[n];
}

vector<ReferenceFrame*> ReferenceFrame::GetChildFrames() const {
  return mChildren;
}

void ReferenceFrame::InsertChild(ReferenceFrame* Frame,long Position) {
  if(Position != END) {
    mChildren.push_back(Frame);
  } else {
    mChildren.insert(mChildren.begin()+Position,Frame);
  }
}

void ReferenceFrame::RemoveChild(long Position) {
  mChildren.erase(mChildren.begin()+Position);
}

void ReferenceFrame::RemoveChild(ReferenceFrame* Child) {
  if(Child==NULL) {
    return;
  }
  for(long i=0;i<mChildren.size();i++) {
    if(mChildren[i]==Child) {
      mChildren.erase(mChildren.begin()+i);
      return;
    }    
  }
}

void ReferenceFrame::SetPosition(Vector3* Position) {
  mPosition=Position;
}

Vector3* ReferenceFrame::GetPosition() const {
  return mPosition;
}

Orientation* ReferenceFrame::GetOrientation() const {
  return mOrient;
}

    

