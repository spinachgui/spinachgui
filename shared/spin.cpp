
#include <shared/spin.hpp>
#include <shared/nuclear_data.hpp>

using namespace std;
using namespace SpinXML;
using namespace sigc;

//============================================================//
// Spin-view

Vector3d SpinView::GetPosition() const {
    return Vector3d(0,0,0);
}

void SpinView::SetPosition(Vector3d Position) {

}

void SpinView::GetCoordinates(double* _x,double* _y, double* _z) const {
    length ux,uy,uz;
    mData->GetCoordinates(&ux,&uy,&uz);

}

void SpinView::SetCoordinates(double  _x,double  _y, double  _z) {

}


//==============================================================================//
// Spin

Spin::Spin(Vector3d Position,string Label,long atomicNumber,long isotope) 
    : mPosition(Position),
      mLabel(Label),
      mElement(atomicNumber),
      mIsotope(isotope) {
}

Spin::Spin(const Spin& s) :
    mPosition(s.mPosition),
    mLabel(s.mLabel),
    mElement(s.mElement),
    mIsotope(s.mIsotope){
}

Spin::~Spin() {
    sigDying(this);
}

Vector3d& Spin::GetPosition() {
    return mPosition;
}

void Spin::SetPosition(Vector3d Position) {
    sigChange();
    mPosition=Position;
}

void Spin::GetCoordinates(length* _x,length* _y, length* _z) const {
	_x->si=mPosition.x();
	_y->si=mPosition.y();
	_z->si=mPosition.z();
}


void Spin::SetCoordinates(length _x,length _y, length _z) {
    sigChange();
	mPosition[0]=_x.si;
	mPosition[1]=_y.si;
	mPosition[2]=_z.si;
}

void Spin::SetLabel(string Label) {
    sigChange();
    mLabel=Label;
}

const char* Spin::GetLabel() const {
    return mLabel.c_str();
}

long Spin::GetElement() const {
    return mElement;
}

void Spin::SetElement(long element) {
    sigChange();
    mElement=element;
}

void Spin::SetIsotope(long isotope) {
    mIsotope=isotope;
}

long Spin::GetIsotope() const {
    return mIsotope;
}


