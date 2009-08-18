
#include <shared/spinsys_new.hpp>


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
    
const double* Matrix3::GetRaw() const {
  return raw;
}
    
double Matrix3::Get(long column, long row) const {
  return raw[3*column+row];
}

double Matrix3::operator() (long column, long row) const {
  return raw[3*column+row];
}

void Matrix3::Set(long column,long row,double val) {
  raw[3*column+row]=val;
}



//==============================================================================//
// SpinSystem

SpinSystem::SpinSystem() {
  mLabFrame.SetPosition(new Vector3(0,0,0));
}

SpinSystem::~SpinSystem() {
  for(long i=0;i<mSpins.size();i++) {
    delete mSpins[i];
  }
  for(long i=0;i<mInteractions.size();i++) {
    delete mSpins[i];
  }
}
    
long SpinSystem::GetSpinCount() const {
  return mSpins.size();
}

Spin* SpinSystem::GetSpin(long n) const {
  return mSpins[n];
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

const ReferenceFrame* SpinSystem::GetRootFrame() const {
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

Spin::Spin(SpinSystem* Parent,Vector3* Position,string Label,ReferenceFrame* mFrame) : mParent(Parent),mPosition(Position),mLabel(Label) {
  
}

Spin::~Spin() {
  if(mPosition != NULL) {
    delete mPosition;
  }
}

Vector3* Spin::GetPosition() const {
  return mPosition;
}

void Spin::SetPosition(Vector3* Position) {
  mPosition=Position;
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
    
ReferenceFrame* Spin::GetFrame() const {
  return mFrame;
}

void Spin::SetFrame(ReferenceFrame* Frame) {
  mFrame=Frame;
}

//==============================================================================//
// Orientation

Orientation::Orientation() : mType(UNDEFINED) {
  
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

Matrix3 Orientation::GetAsMatrix3() const {
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
  mData.mEuler.alpha=alpha;
  mData.mEuler.beta=beta;
  mData.mEuler.gamma=gamma;
  mType=EULER;
  return;
}

void Orientation::SetAngleAxis(double angle,Vector3* axis) {
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
  
Interaction::Type Interaction::GetType() const {
  return mType;
}

Matrix3 Interaction::GetAsMatrix3() const {
  return Matrix3(1,0,0,0,1,0,0,0,1);
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

void Interaction::GetMatrix(Matrix3* Matrix) const {
  *Matrix=*mData.mMatrix;
  return;
}

void Interaction::GetEigenvalues(double* XX,double* YY, double* ZZ, Orientation* Orient) const {
  *XX=mData.mEigenvalues.XX;
  *YY=mData.mEigenvalues.YY;
  *ZZ=mData.mEigenvalues.ZZ;
  *Orient=*mData.mEigenvalues.Orient;
  return;
}

void Interaction::GetAxRhom(double* iso,double* ax, double* rh, Orientation* Orient) const {
  *iso=mData.mAxRhom.iso;
  *ax=mData.mAxRhom.ax;
  *rh=mData.mAxRhom.rh;
  *Orient=*mData.mAxRhom.Orient;
  return;
}

void Interaction::GetSpanSkew(double* iso,double* Span, double* Skew, Orientation* Orient) const {
  *iso=mData.mSpanSkew.iso;
  *Span=mData.mSpanSkew.span;
  *Skew=mData.mSpanSkew.skew;
  *Orient=*mData.mSpanSkew.Orient;
  return;
}

void Interaction::SetScalar(double Scalar) {
  mData.mScalar=Scalar;
}

void Interaction::SetMatrix(Matrix3* Matrix) {
  mData.mMatrix=Matrix;
}

void Interaction::SetEigenvalues(double XX,double YY, double ZZ, Orientation* Orient) {
  mData.mEigenvalues.XX=XX;
  mData.mEigenvalues.YY=YY;
  mData.mEigenvalues.ZZ=ZZ;
  mData.mEigenvalues.Orient=Orient;
  return;
}

void Interaction::SetAxRhom(double iso,double ax, double rh, Orientation* Orient) {
  iso=mData.mAxRhom.iso;
  ax=mData.mAxRhom.ax;
  rh=mData.mAxRhom.rh;
  Orient=mData.mAxRhom.Orient;
  return;
}

void Interaction::SetSpanSkew(double iso,double Span, double Skew, Orientation* Orient) {
  mData.mSpanSkew.iso=iso;
  mData.mSpanSkew.span= Span;
  mData.mSpanSkew.skew=Skew;
  mData.mSpanSkew.Orient=Orient;
  return;
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

    

