
#include <shared/spin.hpp>
#include <shared/nuclear_data.hpp>

using namespace std;
using namespace SpinXML;
using namespace sigc;
using namespace Eigen;

//============================================================//
// Gobal Singals

sigc::signal<void,Spin*> SpinXML::sigAnySpinDying;
sigc::signal<void,Spin*> SpinXML::sigAnySpinChange;

//==============================================================================//
// Spin

Spin::Spin(Vector3d Position,string Label,long atomicNumber,long isotope) 
    : mPosition(Position),
      mLabel(Label),
      mElement(atomicNumber),
      mIsotope(isotope) {
    if(mIsotope == -1) {
        mIsotope = getCommonIsotope(mElement);
    }
}

Spin::Spin(const Spin& s) :
    mPosition(s.mPosition),
    mLabel(s.mLabel),
    mElement(s.mElement),
    mIsotope(s.mIsotope){
}

Spin::~Spin() {
    sigAnySpinDying(this);
}

Vector3d& Spin::GetPosition() {
    return mPosition;
}

void Spin::SetPosition(Vector3d Position) {
    mPosition=Position;
    sigChange();
	sigAnySpinChange(this);
}

void Spin::GetCoordinates(length* _x,length* _y, length* _z) const {
    *_x=mPosition.x();
    *_y=mPosition.y();
    *_z=mPosition.z();
}


void Spin::SetCoordinates(length _x,length _y, length _z) {
    mPosition[0]=_x;
    mPosition[1]=_y;
    mPosition[2]=_z;
    sigChange();
	sigAnySpinChange(this);
}

void Spin::SetLabel(string Label) {
    mLabel=Label;
    sigChange();
	sigAnySpinChange(this);
}

const char* Spin::GetLabel() const {
    return mLabel.c_str();
}

long Spin::GetElement() const {
    return mElement;
}

void Spin::SetElement(long element) {
    mElement=element;
    sigChange();
	sigAnySpinChange(this);
}

void Spin::SetIsotope(long isotope) {
    mIsotope=isotope;
    sigChange();
	sigAnySpinChange(this);
}

long Spin::GetIsotope() const {
    return mIsotope;
}


