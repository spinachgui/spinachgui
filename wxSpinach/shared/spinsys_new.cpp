
#include <shared/spinsys_new.hpp>

//==============================================================================//
// Vector3

Vector3::Vector3() {
    
}

Vector3::Vector3(double x,double y,double z) {

}

double Vector3::GetX() {

}
double Vector3::GetY() {

}

double Vector3::GetZ() {

}

//==============================================================================//
// Matrix3
    

Matrix3::Matrix3() {

}

Matrix3::Matrix3(double a00,double a01,double a02,double a10,double a11,double a12,double a20,double a21,double a22) {

}
    
const double* Matrix3::GetRaw() {

}
    
double Matrix3::Get(long i, long j) {

}

double Matrix3::operator() (long i,long j) {

}

void Matrix3::Set(long i,long j,double val) {

}



//==============================================================================//
// SpinSystem

SpinSystem::SpinSystem() {

}

SpinSystem::~SpinSystem() {

}
    
long SpinSystem::GetSpinCount() {

}

Spin* SpinSystem::GetSpin(long n) {

}

vector<Spin*> SpinSystem::GetSpins() {

}

void SpinSystem::InsertSpin(Spin* _Spin,long Position) {

}

void SpinSystem::RemoveSpin(long Position) {

}

void SpinSystem::RemoveSpin(Spin* _Spin) {

}

ReferenceFrame* SpinSystem::GetRootFrame() {

}

void SpinSystem::SetRootFrame(ReferenceFrame* Frame) {

}
    
void SpinSystem::LoadFromGamesFile(const char* filename) {

}

void SpinSystem::LoadFromG03File(const char* filename) {

}

void SpinSystem::LoadFromXMLFile(const char* filename) {

}


//==============================================================================//
// Spin

Spin::Spin(SpinSystem* Parent) {

}

Spin::~Spin() {

}

Vector3 GetPosition() {

}

void SetPosition(Vector3 Position) {

}

void SetLabel(string Label) {

}

const char* GetLabel() {

}
    
long GetInteractionCount() {

}

Interaction* GetInteraction(long n) {

}

vector<Interaction*> GetInteractions() {

}

void InsertInteraction(Interaction* _Interaction,long Position) {

}

void RemoveInteraction(long Position) {

}

void RemoveInteraction(Interaction* _Interaction) {

}
    
ReferenceFrame* GetFrame() {

}

void SetFrame(ReferenceFrame* Frame) {

}

//==============================================================================//
// Orientation

Orientation::Orientation() {

}

Orientation::~Orientation() {

}
 
Orientation::Type Orientation::GetType() {

}

Matrix3 Orientation::GetAsMatrix3() {

}

void Orientation::GetEuler(double* alpha,double* beta,double* gamma) {

}

void Orientation::GetAngleAxis(double* angle,Vector3* axis) {

}

void Orientation::GetQuaternion(double* real, double* i, double* j, double* k) {

}

void Orientation::GetEigenSystem(Vector3* XAxis,Vector3* YAxis, Vector3* ZAxis) {

}

void Orientation::SetEuler(double alpha,double beta,double gamma) {

}

void Orientation::SetAngleAxis(double angle,Vector3 axis) {

}

void Orientation::SetQuaternion(double real, double i, double j, double k) {

}

void Orientation::SetEigenSystem(Vector3* XAxis,Vector3* YAxis, Vector3* ZAxis) {

}


//==============================================================================//
// Interaction

Interaction::Interaction() {

}

Interaction::~Interaction() {

}
  
Interaction::Type Interaction::GetType() {

}

Matrix3 Interaction::GetAsMatrix3() {

}
    
void Interaction::GetScalar(double* Scalar) {

}

void Interaction::GetMatrix(Matrix3* Matrix) {

}

void Interaction::GetEigenvalues(double* XX,double* YY, double* ZZ, Orientation* Orient) {

}

void Interaction::GetAxRhom(double* iso,double* ax, double* rh, Orientation* Orient) {

}

void Interaction::GetSpanSkew(double* iso,double* Span, double* Skew, Orientation* Orient) {

}

void Interaction::SetScalar(double Scalar) {

}

void Interaction::SetMatrix(Matrix3* Matrix) {

}

void Interaction::SetEigenvalues(double XX,double YY, double ZZ, Orientation* Orient) {

}

void Interaction::SetAxRhom(double iso,double ax, double rh, Orientation* Orient) {

}

void Interaction::SetSpanSkew(double iso,double Span, double Skew, Orientation* Orient) {

}

//==============================================================================//
// Reference Frame

ReferenceFrame::ReferenceFrame() {

}

ReferenceFrame::ReferenceFrame(ReferenceFrame* Parent) {

}

ReferenceFrame::~ReferenceFrame() {

}
    
long ReferenceFrame::GetChildCount() {

}

ReferenceFrame* ReferenceFrame::GetChildFrame(long n) {

}

vector<ReferenceFrame*> ReferenceFrame::GetChildFrames() {

}

void ReferenceFrame::InsertChild(ReferenceFrame* Frame,long Position) {

}

void ReferenceFrame::RemoveChild(long Position) {

}

void ReferenceFrame::RemoveChild(ReferenceFrame* Child) {

}
   
Vector3 ReferenceFrame::GetPosition() {

}

Orientation* ReferenceFrame::GetOrientation() {

}

    

