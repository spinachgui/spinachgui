
#include <shared/spinsys_new.hpp>

#include <stdio.h>

//==============================================================================//
// Vector3

Vector3::Vector3() {
    
}

Vector3::Vector3(double _x,double _y,double _z) : x(_x),y(_y),z(_Z) {

}

double Vector3::GetX() {
  return _x;
}
double Vector3::GetY() {
  return _y;
}

double Vector3::GetZ() {
  return _z;
}

//==============================================================================//
// Matrix3
    
const double* IDENTITY3={1.0,0.0,0.0,
			0.0,1.0,0.0,
			0.0,0.0,1.0}


Matrix3::Matrix3() {
  memcpy(raw,IDENTITY3,9*sizeof(double));
}

Matrix3::Matrix3(double a00,double a01,double a02,double a10,double a11,double a12,double a20,double a21,double a22) {
  raw[0]=a00;   raw[1]=a01   raw[2]=a02;
  raw[3]=a10;   raw[4]=a11   raw[5]=a12;
  raw[6]=a20;   raw[7]=a21   raw[8]=a22;
}
    
const double* Matrix3::GetRaw() {
  return raw;
}
    
double Matrix3::Get(long column, long row) {
  return raw[3*column+row];
}

double Matrix3::operator() (long column, long row) {
  return raw[3*column+row];
}

void Matrix3::Set(long i,long j,double val) {
  raw[3*column+row]=val;
}



//==============================================================================//
// SpinSystem

SpinSystem::SpinSystem() : mLabFrame(ReferenceFrame()) {
  mLabFrame.SetPosition(Vector3(0,0,0));
}

SpinSystem::~SpinSystem() {
  for(long i=0;i<mSpins.size();i++) {
    delete mSpins[i];
  }
  for(long i=0;i<mInteractions.size();i++) {
    delete mSpins[i];
  }
}
    
long SpinSystem::GetSpinCount() {
  return mSpins.size();
}

Spin* SpinSystem::GetSpin(long n) {
  return mSpins[i];
}

vector<Spin*> SpinSystem::GetSpins() {
  return mSpins;
}

void SpinSystem::InsertSpin(Spin* _Spin,long Position) {
  if(Position==END) {
    mSpins.push_back(_Spin);
  } else {
    mSpins.insert(Position,_Spin);
  }
}

void SpinSystem::RemoveSpin(long Position) {
  mSpins.erase(Position);
}

void SpinSystem::RemoveSpin(Spin* _Spin) {
  for(long i=0;i<mSpins.size();i++) {
    if(mSpins[i]==_Spin) {
      mSpins[i].erase(i);
      return;
    }
  }
}

const ReferenceFrame* SpinSystem::GetRootFrame() {
  return &mLabFrame;
}

void SpinSystem::LoadFromGamesFile(const char* filename) {
  
}

void SpinSystem::LoadFromG03File(const char* filename) {

}

void SpinSystem::LoadFromXMLFile(const char* filename) {

}


//==============================================================================//
// Spin

Spin::Spin(SpinSystem* Parent) mParent(Parent) {
  
}

Spin::~Spin() {

}

Vector3 GetPosition() {
  return mPosition;
}

void SetPosition(Vector3 Position) {
  mPosition=Position;
}

void SetLabel(string Label) {
  mLabel=Label;
}

const char* GetLabel() {
  return mLabel.c_str();
}
    
long GetInteractionCount() {
  long count=0;
  for(long i=0;i<mParent->mInteractions.size();i++) {
    if(mParent->mInteractions[i].GetSpin1()==this || mParent->mInteractions[i].GetSpin2()==this) {
      count++;
    }
  }
  return count;
}

Interaction* GetInteraction(long n) {
  long count=0;
  for(long i=0;i<mParent->mInteractions.size();i++) {
    if(mParent->mInteractions[i].GetSpin1()==this || mParent->mInteractions[i].GetSpin2()==this) {
      if(count==n) {
	return mParent->mInteractions[i];
      }
      count++;
    }
  }
  return NULL;
}

vector<Interaction*> GetInteractions() {
  vector<Interaction*> InterPtrs;
  for(long i=0;i<mParent->mInteractions.size();i++) {
    if(mParent->mInteractions[i].GetSpin1()==this || mParent->mInteractions[i].GetSpin2()==this) {
      InterPtrs.push_back(mParent->mInteractions[i]);
    }
  }
  return InterPtrs;
}

void InsertInteraction(Interaction* _Interaction,long Position) {
  if(Position==END) {
    mParent->mInteractions.push_back(_Interaction);
  } else {
    long count=0;
    for(long i=0;i<mParent->mInteractions.size();i++) {
      if(mParent->mInteractions[i].GetSpin1()==this || mParent->mInteractions[i].GetSpin2()==this) {
	if(count==Position) {
	  return mParent->mInteractions.insert(i,_Interaction);
	}
	count++;
      }
    }
  }
}

void RemoveInteraction(long Position) {
  if(Position==END) {
    mParent->mInteractions.push_back(_Interaction);
  } else {
    long count=0;
    for(long i=0;i<mParent->mInteractions.size();i++) {
      if(mParent->mInteractions[i].GetSpin1()==this || mParent->mInteractions[i].GetSpin2()==this) {
	if(count==Position) {
	  mParent->mInteractions.erase(i);
	}
	count++;
      }
    }
  }
}

void RemoveInteraction(Interaction* _Interaction) {
  if(_Interaction == NULL) {
    return;
  }
  for(long i=0;i<mParent->mInteractions.size();i++) {
    if(mParent->mInteractions[i]==_Interaction) {
      mParent->mInteractions.erase(i);
    }
  }
}
    
ReferenceFrame* GetFrame() {
  return mFrame;
}

void SetFrame(ReferenceFrame* Frame) {
  mFrame=Frame;
}

//==============================================================================//
// Orientation

Orientation::Orientation() : mType(UNDEFINED) {
  
}

Orientation::~Orientation() {
  if(mType==ANGLE_AXIS && mData.mAngleAxis.Vector3 != NULL) {
    delete mData.mAngleAxis.Vector3;
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
 
Orientation::Type Orientation::GetType() {
  return mType;
}

Matrix3 Orientation::GetAsMatrix3() {
  return Matrix3(1,0,0,0,1,0,0,0,1);
}

void Orientation::GetEuler(double* alpha,double* beta,double* gamma) {
  *alpha = mData.mEuler.alpha;
  *beta = mData.mEuler.beta;
  *gamma = mData.mEuler.gamma;
  return;
}

void Orientation::GetAngleAxis(double* angle,Vector3* axis) {
  *angle = mData.mAngleAxis.angle;
  *axis = *mData.mAngleAxis.axis;
  return;
}

void Orientation::GetQuaternion(double* real, double* i, double* j, double* k) {
  *real = mData.mQuaternion.real;
  *i = mData.mQuaternion.i;
  *j = mData.mQuaternion.j;
  *k = mData.mQuaternion.k;
  return;
}

void Orientation::GetEigenSystem(Vector3* XAxis,Vector3* YAxis, Vector3* ZAxis) {
  *XAxis = *mData.mEigenSystem.XAxis;
  *YAxis = *mData.mEigenSystem.YAxis;
  *ZAxis = *mData.mEigenSystem.ZAxis;
  return;
}

void Orientation::SetEuler(double alpha,double beta,double gamma) {
  mData.mEuler.alpha=alpha;
  mData.mEuler.beta=beta;
  mData.mEuler.gamma=gamma;
  mType=EULER;
  return;
}

void Orientation::SetAngleAxis(double angle,Vector3 axis) {
  mData.mAngleAxis.angle=angle;
  mData.mAngleAxis.axis=axis;
  mType=ANGLE_AXIS;
  return;

}

void Orientation::SetQuaternion(double real, double i, double j, double k) {
  mData.mQuaternion.real=real;
  mData.mQuaternion.i=i;
  mData.mQuaternion.j=j;
  mData.mQuaternion.k=k;
  mType=QUATERNION;
  return;
}

void Orientation::SetEigenSystem(Vector3* XAxis,Vector3* YAxis, Vector3* ZAxis) {
  mData.mEigenSystem.XAxis=XAxis;
  mData.mEigenSystem.YAxis=YAxis;
  mData.mEigenSystem.ZAxis=ZAxis;
  return;
}


//==============================================================================//
// Interaction

Interaction::Interaction() : mType(UNDEFINED) {
  
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
  
Interaction::Type Interaction::GetType() {
  return mType;
}

Matrix3 Interaction::GetAsMatrix3() {
  return Matrix3(1,0,0,0,1,0,0,0,1);
}
    
void Interaction::GetScalar(double* Scalar) {
  *Scalar=mData.mScalar;
}

void Interaction::GetMatrix(Matrix3* Matrix) {
  *Matrix=*mData.mMatrix;
  return;
}

void Interaction::GetEigenvalues(double* XX,double* YY, double* ZZ, Orientation* Orient) {
  *XX=mData.Eigenvalues.XX;
  *YY=mData.Eigenvalues.YY;
  *ZZ=mData.Eigenvalues.ZZ;
  *Orient=*mData.Eigenvalues.Orient;
  return;
}

void Interaction::GetAxRhom(double* iso,double* ax, double* rh, Orientation* Orient) {
  *iso=mData.mAxRhom.iso;
  *ax=mData.mAxRhom.ax;
  *rho=mData.mAxRhom.rho;
  *Orient=*mData.Eigenvalues.Orient;
  return;
}

void Interaction::GetSpanSkew(double* iso,double* Span, double* Skew, Orientation* Orient) {
  *iso=mData.mAxRhom.iso;
  *Span=mData.mAxRhom.Span;
  *Skew=mData.mAxRhom.Skew;
  *Orient=*mData.Eigenvalues.Orient;
  return;
}

void Interaction::SetScalar(double Scalar) {
  mData.mScalar=Scalar;
}

void Interaction::SetMatrix(Matrix3* Matrix) {
  mData.mMatrix=Matrix;
}

void Interaction::SetEigenvalues(double XX,double YY, double ZZ, Orientation* Orient) {
  mData.Eigenvalues.XX=XX;
  mData.Eigenvalues.YY=YY;
  mData.Eigenvalues.ZZ=ZZ;
  mData.Eigenvalues.Orient=Orient;
  return;
}

void Interaction::SetAxRhom(double iso,double ax, double rh, Orientation* Orient) {
  *iso=mData.mAxRhom.iso;
  *ax=mData.mAxRhom.ax;
  *rho=mData.mAxRhom.rho;
  *Orient=*mData.Eigenvalues.Orient;
  return;
}

void Interaction::SetSpanSkew(double iso,double Span, double Skew, Orientation* Orient) {
  mData.mAxRhom.iso=iso;
  mData.mAxRhom.Span= Span;
  mData.mAxRhom.Skew=Skew;
  mData.Eigenvalues.Orient=Orient;
  return;
}

//==============================================================================//
// Reference Frame

ReferenceFrame::ReferenceFrame() : mParent(NULL),mPosition(0,0,0){
  
}

ReferenceFrame::ReferenceFrame(ReferenceFrame* Parent) : mParent(Parent) {

}

ReferenceFrame::~ReferenceFrame() {

}
    
long ReferenceFrame::GetChildCount() {
  return mChildren.size();
}

ReferenceFrame* ReferenceFrame::GetChildFrame(long n) {
  return mChildren[n];
}

vector<ReferenceFrame*> ReferenceFrame::GetChildFrames() {
  return mChildren;
}

void ReferenceFrame::InsertChild(ReferenceFrame* Frame,long Position) {
  if(Position != END) {
    mChildren.push_back(Frame);
  } else {
    mChildren.insert(Position,Frame);
  }
}

void ReferenceFrame::RemoveChild(long Position) {
  mChildren.erase(Position);
}

void ReferenceFrame::RemoveChild(ReferenceFrame* Child) {
  if(child==NULL) {
    return;
  }
  for(long i=0;i<mChildren.size();i++) {
    if(mChildren[i]==child) {
      mChildren.erase(i);
      return;
    }    
  }
}
   
Vector3 ReferenceFrame::GetPosition() {
  return mPosition;
}

Orientation* ReferenceFrame::GetOrientation() {
  return &mOrient;
}

    

