#include <shared/spinsys.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <shared/nuclear_data.hpp>


using namespace std;
using namespace SpinXML;


//==============================================================================//
// SpinSystem

SpinSystem::SpinSystem()  {
}


SpinSystem::SpinSystem(const SpinSystem& system)  {
  long spinCount=system.mSpins.size();
  long interCount=system.mInteractions.size();

  mSpins.resize(spinCount);
  mInteractions.resize(interCount);

  for(long i=0;i<spinCount;i++) {
    mSpins[i]=new Spin(*system.mSpins[i]);
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
}

SpinSystem::~SpinSystem() {
  Clear();
  sigDying();
}

void SpinSystem::Clear() {
  //Currently we need to delete the interactions Before the spins as
  //the spins have interactions as their children. 
  sigReloading();

  for(long i=0;i<mInteractions.size();i++) {
    delete mInteractions[i];
  }
  for(long i=0;i<mSpins.size();i++) {
    delete mSpins[i];
  }
  mSpins.resize(0);
  mInteractions.resize(0);
 
  sigReloaded();
}

void SpinSystem::Dump() const {
  cout << "Dumping SpinSystem:" << endl;
  cout << " Spin count=" << mSpins.size() << endl;
  cout << " Interaction count=" << mInteractions.size() << endl;
  cout << endl;
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

long SpinSystem::GetInteractionCount() const {
  return mInteractions.size();
}

Interaction* SpinSystem::GetInteraction(long n) const {
  return mInteractions[n];
}

vector<Spin*> SpinSystem::GetSpins() const {
  return mSpins;
}

void SpinSystem::InsertSpin(Spin* _Spin,long Position) {
  if(Position==END) {
    mSpins.push_back(_Spin);
    sigNewSpin(_Spin,mSpins.size()-1);
  } else {
    mSpins.insert(mSpins.begin()+Position,_Spin);
    sigNewSpin(_Spin,Position);
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


void SpinSystem::LoadFromFile(const char* filename,ISpinSystemLoader* loader) {
  sigReloading();
  try {
    loader->LoadFile(this,filename);
  } catch(const runtime_error& e) {
    sigReloaded();
    throw e;
  }
  sigReloaded();
  return;
}

void SpinSystem::SaveToFile(const char* filename,ISpinSystemLoader* saver) const 
{
  saver->SaveFile(this,filename);
  return;
}



//==============================================================================//
// Spin

Spin::Spin(Vector3 Position,string Label,long atomicNumber) 
  : mPosition(Position),mLabel(Label),mElement(atomicNumber) {
}

Spin::Spin(const Spin& s) :
  mPosition(s.mPosition),
  mLabel(s.mLabel),
  mElement(s.mElement),
  mIsotopes(s.mIsotopes){
}

Spin::~Spin() {
  sigDying();
}


Vector3& Spin::GetPosition() {
  return mPosition;
}

void Spin::SetPosition(Vector3 Position) {
  sigChange();
  mPosition=Position;
}

void Spin::GetCoordinates(double* _x,double* _y, double* _z) const {
  mPosition.GetCoordinates(_x,_y,_z);
}


void Spin::SetCoordinates(double _x,double _y, double _z) {
  sigChange();
  mPosition.SetCoordinates(_x,_y,_z);
}

void Spin::SetLabel(string Label) {
  sigChange();
  mLabel=Label;
}

const char* Spin::GetLabel() const {
  return mLabel.c_str();
}

void Spin::InsertInteraction(Interaction* _Interaction,long Position) {
  sigChange();
  mInteractions.push_back(_Interaction);
}

void Spin::RemoveInteraction(long Position) {
  mInteractions.erase(mInteractions.begin()+Position);
}

void Spin::RemoveInteraction(Interaction* _Interaction) {
  if(_Interaction == NULL) {
    return;
  }
  for(long i=0;i<mInteractions.size();i++) {
    if(mInteractions[i]==_Interaction) {
      mInteractions.erase(mInteractions.begin()+i);
    }
  }
}

double Spin::GetLinearInteractionAsScalar(Interaction::SubType t) const {
  double total=0.0;
  long interCount=mInteractions.size();
  for(long i=0;i < interCount;++i) {
    Interaction* inter=mInteractions[i];
    if(!inter->GetIsLinear()) {
      continue;
    }
    if(inter->IsSubType(t)) {
      total+=inter->GetAsScalar();
    }
  }
  return total;
}


double Spin::GetQuadrapolarInteractionAsScalar(Interaction::SubType t) const {
  double total=0.0;
  long interCount=mInteractions.size();
  for(long i=0;i < interCount;++i) {
    Interaction* inter=mInteractions[i];
    if(!inter->GetIsQuadratic()) {
      continue;
    }
    if(inter->IsSubType(t)) {
      total+=inter->GetAsScalar();
    }
  }
  return total;
}

Matrix3 Spin::GetLinearInteractionAsMatrix(Interaction::SubType t) const {
  Matrix3 total=Matrix3(0,0,0,
			0,0,0,
			0,0,0);
  long interCount=mInteractions.size();
  for(long i=0;i < interCount;++i) {
    Interaction* inter=mInteractions[i];
    if(!inter->GetIsLinear()) {
      continue;
    }
    if(inter->IsSubType(t)) {
      total+=inter->GetAsMatrix();
    }
  }
  return total;
}


Matrix3 Spin::GetQuadrapolarInteractionAsMatrix(Interaction::SubType t) const {
  Matrix3 total=Matrix3(0,0,0,
			0,0,0,
			0,0,0);
  long interCount=mInteractions.size();
  for(long i=0;i < interCount;++i) {
    Interaction* inter=mInteractions[i];
    if(!inter->GetIsQuadratic()) {
      continue;
    }
    if(inter->IsSubType(t)) {
      total+=inter->GetAsMatrix();
    }
  }
  return total;
}


Interaction::SubType Interaction::GetSubType() const {
  return mSubType;
}

void Interaction::SetSubType(SubType st) {
  sigChange();
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



long Spin::GetElement() const {
  return mElement;
}

void Spin::SetElement(long element) {
  sigChange();
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
  sigDying();
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
  if(Spin1==mSpin1) {
    //Nothing happens, return imediately.
    return;
  }
  sigChange();
  mSpin1=Spin1;
}

void Interaction::SetSpin2(Spin* Spin2) {
  if(Spin2==mSpin2) {
    //Nothing happens, return imediately.
    return;
  }
  sigChange();
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
  sigChange();
  mType=SCALAR;
  mData.mScalar=Scalar;
}

void Interaction::SetMatrix(const Matrix3& Matrix) {
  sigChange();
  mType=MATRIX;
  mData.mMatrix=new Matrix3(Matrix);
}

void Interaction::SetEigenvalues(double XX,double YY, double ZZ, const Orientation& Orient) {
  sigChange();
  mType=EIGENVALUES;
  mData.mEigenvalues.XX=XX;
  mData.mEigenvalues.YY=YY;
  mData.mEigenvalues.ZZ=ZZ;
  mOrient=Orient;
  return;
}

void Interaction::SetAxRhom(double iso,double ax, double rh, const Orientation& Orient) {
  sigChange();
  mType=AXRHOM;
  mData.mAxRhom.iso=iso;
  mData.mAxRhom.ax=ax;
  mData.mAxRhom.rh=rh;
  mOrient=Orient;
  return;
}

void Interaction::SetSpanSkew(double iso,double Span, double Skew, const Orientation& Orient) {
  sigChange();
  mType=SPANSKEW;
  mData.mSpanSkew.iso=iso;
  mData.mSpanSkew.span= Span;
  mData.mSpanSkew.skew=Skew;
  mOrient=Orient;
  return;
}

double Interaction::GetAsScalar() const {
  if(mType==SCALAR) {
    //Return the identity multipled by the scalar
    double s=mData.mScalar;
    return s;
  } else if(mType==MATRIX) {
    return mData.mMatrix->Trace();
  } else if(mType==EIGENVALUES) {
    return mData.mEigenvalues.XX+mData.mEigenvalues.YY+mData.mEigenvalues.ZZ;
  } else if(mType==AXRHOM) {
    double a=mData.mAxRhom.ax;
    double r=mData.mAxRhom.rh;
    double iso=mData.mAxRhom.iso;
    double xx=a/3+iso/3;
    double yy=-r/2-a/6+iso/3;
    double zz= r/2-a/6+iso/3;
    return xx+yy+zz;
  } else if(mType==SPANSKEW) {
      double span=mData.mSpanSkew.span;
      double skew=mData.mSpanSkew.skew;
      double iso=mData.mSpanSkew.iso;
      double xx=span*skew/6-span/2;
      double yy=iso-span*skew/3;
      double zz=span*skew/6+span/2;
      return xx+yy+zz;
  }
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
      yy=mData.mEigenvalues.YY;
      zz=mData.mEigenvalues.ZZ;
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

    Matrix3 result=intMatrix*Matrix3(xx, 0.0,0.0,
				     0.0,yy, 0.0,
				     0.0,0.0,zz)*intMatrix.Transpose();
    return result;

  } else {
    cerr << "Interaction::mType is set to an invalid type! This is a serious programing error.\n" << endl;
    throw logic_error("Interaction::mType is set to an invalid type! This is a serious programing error.\n");
  }
  //Return the zero matrix identity
  Matrix3 zero(0,0,0,0,0,0,0,0,0);
  return zero;
}

void Interaction::SetQuadratic() {
  if(mSpin1==mSpin2) {
    return;
  }
  sigChange();
  mSpin2=mSpin1;
}

void Interaction::SetLinear() {
  if(mSpin2 == NULL) {
    return;
  }
  sigChange();
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

