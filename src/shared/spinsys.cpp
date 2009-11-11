#include <shared/spinsys.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <shared/nuclear_data.hpp>

using namespace std;
using namespace SpinXML;



//==============================================================================//
// SpinSystem

SpinSystem::SpinSystem() : mLabFrame(NULL) {
#ifdef SPINXML_EVENTS
  mNode=new EventNode(PART_SYSTEM,0,wxString(wxT("RootNode")));
#endif
}


SpinSystem::SpinSystem(const SpinSystem& system) : mLabFrame(NULL) {
#ifdef SPINXML_EVENTS
  mNode=system.mNode;
#endif

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
  mLabFrame=NULL;//new ReferenceFrame(NULL,Vector3(0,0,0),Orientation());
  //mLabFrame->GetOrientation().SetQuaternion(1.0,0.0,0.0,0.0);
}

SpinSystem::~SpinSystem() {
  Clear();
#ifdef SPINXML_EVENTS
  delete mNode;
#endif
}

void SpinSystem::Clear() {
  //Currently we need to delete the interactions Before the spins as
  //the spins have interactions as their children. 
#ifdef SPINXML_EVENTS
  //NB: This lock is important for more than just efficency. this
  //operation must be atomic as mSpins and mInteractions end up
  //holding invalid values. during execution.
  PushEventLock();
#endif
  for(long i=0;i<mInteractions.size();i++) {
    delete mInteractions[i];
  }
  for(long i=0;i<mSpins.size();i++) {
    delete mSpins[i];
  }
  mSpins.resize(0);
  mInteractions.resize(0);
#ifdef SPINXML_EVENTS
  PopEventLock();
  mNode->Change(IEventListener::CHANGE);
#endif
}

void SpinSystem::Dump() const {
#ifdef SPINXML_EVENTS
  mNode->Dump();
#endif
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
#ifdef SPINXML_EVENTS
    _Spin->mNode->SetPartAndHint(PART_SPIN,mSpins.size()-1);
    cout << "Set Hint to " << mSpins.size()-1 <<  endl;
#endif
  } else {
    mSpins.insert(mSpins.begin()+Position,_Spin);
#ifdef SPINXML_EVENTS
    _Spin->mNode->SetPartAndHint(PART_SPIN,Position);
    cout << "'Set Hint to " << Position <<  endl;
#endif
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
#ifdef SPINXML_EVENTS
  mNode->Change(IEventListener::CHANGE);
#endif
}

void SpinSystem::LoadFromG03File(const char* filename) {
  /*
    This function really needs some work done on in, as it's not using C++
    streams properly. This is due to it being adapted from matlab code (which uses
    c style I/O
  */
#ifdef SPINXML_EVENTS
  PushEventLock();
#endif
  Clear();
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
	    mSpins[nAtoms]->SetPosition(Vector3(x,y,z));
	  } else {
	    cerr << "Error reading standard orientation: the number of centres is not consistant amoung the standard orientation tables" << endl;
	    throw runtime_error("Error reading standard orientation: the number of centres is not consistant amoung the standard orientation tables");
	  }
	} else {
	  //Create a new spin
	  string isotopeSymbol(getElementSymbol(atomicNumber));
	  isotopeSymbol=isotopeSymbol; //We have no clue what the mass number is yet
	  Spin* s=new Spin(this,Vector3(x,y,z),string("A Spin")+isotopeSymbol,GetRootFrame(),atomicNumber);
	  InsertSpin(s);
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

	Orientation o;
	o.SetEigenSystem(Vector3(x1,y1,z1),Vector3(x2,y2,z2),Vector3(x3,y3,z3));

	Interaction* inter=new Interaction(); //Last paramiter is reference frame, which is always lab
	inter->SetEigenvalues(eigenvalue1*0.05,eigenvalue2*0.05,eigenvalue3*0.05,o);
	inter->SetSubType(Interaction::ST_HFC);
	inter->SetSpin1(GetSpin(i));
	
	mInteractions.push_back(inter);
      }
    }
  }
  cout << "Finished loading the g03 file, saving mSpins.size()=" << mSpins.size() << endl;
#ifdef SPINXML_EVENTS
  PopEventLock();
  mNode->Change(IEventListener::CHANGE);
#endif
}



//==============================================================================//
// Spin

Spin::Spin(SpinSystem* Parent,Vector3 Position,string Label,ReferenceFrame* mFrame,long atomicNumber) 
  : mParent(Parent),mPosition(Position),mLabel(Label),mElement(atomicNumber) {

#ifdef SPINXML_EVENTS
  mNode=new EventNode(PART_SPIN,0,wxString(wxT("Spin: ")) << wxString(Label.c_str(),wxConvUTF8));
  mParent->GetNode()->AddChild(mNode);
#endif
}

Spin::Spin(const Spin& s,SpinSystem* ss) :
  mParent(ss),
  mPosition(s.mPosition),
  mLabel(s.mLabel),
  mFrame(NULL),
  mElement(s.mElement),
  mIsotopes(s.mIsotopes){
}

Spin::~Spin() {
#ifdef SPINXML_EVENTS
  delete mNode;
#endif
}

void Spin::Dump() const {
  cout << "  Dumping class Spin" << endl;
  cout << "    Label=" << mLabel << endl;
  cout << "    Position=(" << mPosition.GetX() << "," << mPosition.GetY() << "," << mPosition.GetZ() << ")" << endl;
  cout << "    Interaction Count=" << GetInteractionCount() << endl;
}

Vector3& Spin::GetPosition() {
  return mPosition;
}

void Spin::SetPosition(Vector3 Position) {
#ifdef SPINXML_EVENTS
  if(mParent != NULL) {
    //If the spin has no parent, we probably don't want to emit events anyway.
    mNode->Change(IEventListener::CHANGE);
  }
#endif
  mPosition=Position;
}

void Spin::GetCoordinates(double* _x,double* _y, double* _z) const {
  mPosition.GetCoordinates(_x,_y,_z);
}


void Spin::SetCoordinates(double _x,double _y, double _z) {
#ifdef SPINXML_EVENTS
  mNode->Change(IEventListener::CHANGE);
#endif
  mPosition.SetCoordinates(_x,_y,_z);
}

void Spin::SetLabel(string Label) {
#ifdef SPINXML_EVENTS
  mNode->Change(IEventListener::CHANGE);
#endif
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
    if(inter->GetIsLinear()) {
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
  double total=0.0;
  long interCount=mParent->mInteractions.size();
  for(long i=0;i < interCount;++i) {
    Interaction* inter=mParent->mInteractions[i];
    if(inter->GetIsBilinear()) {
      continue;
    }
    if(inter->GetSpin1()==this && inter->GetSpin2() == OtherSpin) {
      //Interaction is relevent
      if(inter->IsSubType(t)) {
	total+=inter->GetAsScalar();
      }
    }
  }
  return total;
}

double Spin::GetQuadrapolarInteractionAsScalar(Interaction::SubType t) const {
  double total=0.0;
  long interCount=mParent->mInteractions.size();
  for(long i=0;i < interCount;++i) {
    Interaction* inter=mParent->mInteractions[i];
    if(inter->GetIsQuadratic()) {
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

Matrix3 Spin::GetLinearInteractionAsMatrix(Interaction::SubType t) const {
  Matrix3 total=Matrix3(0,0,0,
			0,0,0,
			0,0,0);
  long interCount=mParent->mInteractions.size();
  for(long i=0;i < interCount;++i) {
    Interaction* inter=mParent->mInteractions[i];
    if(!inter->GetIsLinear()) {
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
  Matrix3 total=Matrix3(0,0,0,
			0,0,0,
			0,0,0);
  long interCount=mParent->mInteractions.size();
  for(long i=0;i < interCount;++i) {
    Interaction* inter=mParent->mInteractions[i];
    if(!inter->GetIsBilinear()) {
      continue;
    }
    if(inter->GetSpin1()==this && inter->GetSpin2()==OtherSpin) {
      //Interaction is relevent
      if(inter->IsSubType(t)) {
	total+=inter->GetAsMatrix();
      }
    }
  }
  return total;
}

Matrix3 Spin::GetQuadrapolarInteractionAsMatrix(Interaction::SubType t) const {
  Matrix3 total=Matrix3(0,0,0,
			0,0,0,
			0,0,0);
  long interCount=mParent->mInteractions.size();
  for(long i=0;i < interCount;++i) {
    Interaction* inter=mParent->mInteractions[i];
    if(!inter->GetIsQuadratic()) {
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


Interaction::SubType Interaction::GetSubType() const {
  return mSubType;
}

void Interaction::SetSubType(SubType st) {
#ifdef SPINXML_EVENTS
  mNode->Change(IEventListener::CHANGE);
#endif
  mSubType=st;
}

bool Interaction::IsSubType(SubType t) const {
  if(t==ST_ANY) {
    return true;
  }
  if(t==ST_EPR) {
    switch(mSubType) {
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
    switch(mSubType) {
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
#ifdef SPINXML_EVENTS
  mNode->Change(IEventListener::CHANGE);
#endif
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
// Interaction

Interaction::Interaction() : mType(UNDEFINED),mSubType(ST_ANY),mSpin1(NULL),mSpin2(NULL) {
#ifdef SPINXML_EVENTS
  #warning "Interactions do not know their index, so all report as index 0."
  //Which could cause gui elements to needlessly refresh themselves.
  mNode=new EventNode(PART_INTERACTION,0,wxString(wxT("Interaction")));
#endif
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
    mOrient=mOrient;
  } else if(mType==AXRHOM) {
    mData.mAxRhom.ax=inter.mData.mAxRhom.ax;
    mData.mAxRhom.rh=inter.mData.mAxRhom.rh;
    mData.mAxRhom.iso=inter.mData.mAxRhom.iso;
    mOrient=mOrient;
  } else if(mType==SPANSKEW) {
    mData.mSpanSkew.span=inter.mData.mSpanSkew.span;
    mData.mSpanSkew.skew=inter.mData.mSpanSkew.skew;
    mData.mSpanSkew.iso=inter.mData.mSpanSkew.iso;
    mOrient=mOrient;
  }
}

Interaction::~Interaction() {
#ifdef SPINXML_EVENTS
  delete mNode;
#endif
}

void Interaction::Dump() const {
  cout << "  Dumping an Interaction" << endl;
  switch (GetType()) {
  case UNDEFINED:
    cout << "    Type=Undefined" <<  endl;
    break;
  case SCALAR:
    cout << "    Type=Scalar" <<  endl;
    cout << "    Value=" << mData.mScalar << endl;
    break;
  case MATRIX:
    cout << "    Type=Matrix" <<  endl;
    cout << "    Value=" << endl;
    mData.mMatrix->Dump();
    break;
  case EIGENVALUES:
    cout << "    Type=Eigenvalues" <<  endl;
    cout << "    Value=(XX=" 
	 << mData.mEigenvalues.XX << ",YY="
	 << mData.mEigenvalues.YY << ",ZZ="
	 << mData.mEigenvalues.ZZ << ")"
	 << endl;
    cout << "    Orient=";
    cout << mOrient.ToString() << endl;
    break;
  case AXRHOM:
    cout << "    Type=Axiality Rhombicity" <<  endl;
    cout << "    Value=" << endl; 
    cout << "    Orient=";
    cout << mOrient.ToString() << endl;
    break;
  case SPANSKEW:
    cout << "    Type=Span-Skew" <<  endl;
    cout << "    Value=" << endl;
    cout << "    Orient=";
    cout << mOrient.ToString() << endl;
    break;
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
    ostringstream stream;
    stream << "Unknow type submited to Interaction::GetTypeName() (t=";
    stream << t << ")" << endl;
    throw std::runtime_error(stream.str());
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
    ostringstream stream;
    stream << "Unknow subtype submited to Interaction::GetSubTypeName() (st=";
    stream << st << ")" << endl;
    throw std::runtime_error(stream.str());
    return "Error in Interaction::GetSubTypeName()";
  }
}
  
Interaction::Type Interaction::GetType() const {
  return mType;
}


void Interaction::SetSpin1(Spin* Spin1) {
#ifdef SPINXML_EVENTS
  if(Spin1==mSpin1) {
    //Nothing happens, return imediately.
    return;
  }
  if(mSpin1 != NULL && mSpin1 != mSpin2) {
    //If we have a bilinear inital state, we need to remove mSpin1 as
    //a parent
    mNode->RemoveParent(mSpin1->GetNode());
  }
  if(Spin1 != NULL && mSpin2 != Spin1 || (mSpin2 == Spin1 && mSpin2 == NULL)) {
    //If we have a bilinear final state or we're comming from a blank
    //inital state, we need to add Spin1 as a parent
    mNode->AddParent(Spin1->GetNode());
  }
#endif
  mSpin1=Spin1;
}

void Interaction::SetSpin2(Spin* Spin2) {
#ifdef SPINXML_EVENTS
  if(Spin2==mSpin2) {
    //Nothing happens, return imediately.
    return;
  }
  if(mSpin2 != NULL && mSpin2 != mSpin1) {
    //If we have a bilinear inital state, we need to remove mSpin2 as
    //a parent
    mNode->RemoveParent(mSpin2->GetNode());
  }
  if(Spin2 != NULL && Spin2 != mSpin1 || (mSpin2 == Spin2 && mSpin2 == NULL)) {
    //If we have a bilinear final state or we're comming from a blank
    //inital state, we need to add Spin2 as a parent
    mNode->AddParent(Spin2->GetNode());
  }
#endif
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

void Interaction::GetEigenvalues(double* XX,double* YY, double* ZZ, Orientation* OrientOut) const {
  *XX=mData.mEigenvalues.XX;
  *YY=mData.mEigenvalues.YY;
  *ZZ=mData.mEigenvalues.ZZ;
  *OrientOut=mOrient;
  return;
}

void Interaction::GetAxRhom(double* iso,double* ax, double* rh, Orientation* OrientOut) const {
  *iso=mData.mAxRhom.iso;
  *ax=mData.mAxRhom.ax;
  *rh=mData.mAxRhom.rh;
  *OrientOut=mOrient;
  return;
}

void Interaction::GetSpanSkew(double* iso,double* Span, double* Skew, Orientation* OrientOut) const {
  *iso=mData.mSpanSkew.iso;
  *Span=mData.mSpanSkew.span;
  *Skew=mData.mSpanSkew.skew;
  *OrientOut=mOrient;
  return;
}

void Interaction::SetScalar(double Scalar) {
#ifdef SPINXML_EVENTS
  mNode->Change(IEventListener::CHANGE);
#endif
  mType=SCALAR;
  mData.mScalar=Scalar;
}

void Interaction::SetMatrix(const Matrix3& Matrix) {
#ifdef SPINXML_EVENTS
  mNode->Change(IEventListener::CHANGE);
#endif
  mType=MATRIX;
  mData.mMatrix=new Matrix3(Matrix);
}

void Interaction::SetEigenvalues(double XX,double YY, double ZZ, const Orientation& Orient) {
#ifdef SPINXML_EVENTS
  mNode->Change(IEventListener::CHANGE);
#endif
  mType=EIGENVALUES;
  mData.mEigenvalues.XX=XX;
  mData.mEigenvalues.YY=YY;
  mData.mEigenvalues.ZZ=ZZ;
  mOrient=Orient;
  return;
}

void Interaction::SetAxRhom(double iso,double ax, double rh, const Orientation& Orient) {
#ifdef SPINXML_EVENTS
  mNode->Change(IEventListener::CHANGE);
#endif
  mType=AXRHOM;
  mData.mAxRhom.iso=iso;
  mData.mAxRhom.ax=ax;
  mData.mAxRhom.rh=rh;
  mOrient=Orient;
  return;
}

void Interaction::SetSpanSkew(double iso,double Span, double Skew, const Orientation& Orient) {
#ifdef SPINXML_EVENTS
  mNode->Change(IEventListener::CHANGE);
#endif
  mType=SPANSKEW;
  mData.mSpanSkew.iso=iso;
  mData.mSpanSkew.span= Span;
  mData.mSpanSkew.skew=Skew;
  mOrient=Orient;
  return;
}

double Interaction::GetAsScalar() const {
  return 0;
}

Matrix3 Interaction::GetAsMatrix() const {
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

    if(mType==EIGENVALUES) {
      xx=mData.mEigenvalues.XX;
      xx=mData.mEigenvalues.YY;
      xx=mData.mEigenvalues.ZZ;
    } else if(mType==AXRHOM) {
      double a=mData.mAxRhom.ax;
      double r=mData.mAxRhom.rh;
      double iso=mData.mAxRhom.iso;
      xx=a/3+iso/3;
      yy=-r/2-a/6+iso/3;
      zz= r/2-a/6+iso/3;
    } else if(mType==SPANSKEW) {
      double span=mData.mSpanSkew.span;
      double skew=mData.mSpanSkew.skew;
      double iso=mData.mSpanSkew.iso;
      xx=span*skew/6-span/2;
      yy=iso-span*skew/3;
      zz=span*skew/6+span/2;
    }

    Matrix3 intMatrix=mOrient.GetAsMatrix();

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
#ifdef SPINXML_EVENTS
  if(mSpin2 != NULL && mSpin2 != mSpin1) {
    mNode->RemoveParent(mSpin2->GetNode());
  }
  mNode->Change(IEventListener::CHANGE);
#endif
  mSpin2=mSpin1;
}

void Interaction::SetLinear() {
#ifdef SPINXML_EVENTS
  if(mSpin2 != NULL) {
    mNode->RemoveParent(mSpin2->GetNode());
    mNode->Change(IEventListener::CHANGE);
  }
#endif
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

ReferenceFrame::ReferenceFrame() : mParent(),mPosition(){
  
}

ReferenceFrame::ReferenceFrame(ReferenceFrame* Parent,const Vector3& Position,const Orientation& Orient) :
  mParent(Parent),mPosition(Position),mOrient(Orient) {

}

ReferenceFrame::~ReferenceFrame() {
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

void ReferenceFrame::SetPosition(const Vector3& Position) {
  mPosition=Position;
}

Vector3& ReferenceFrame::GetPosition() {
  return mPosition;
}

Orientation& ReferenceFrame::GetOrientation() {
  return mOrient;
}

    

