
#include <shared/spin.hpp>
#include <shared/nuclear_data.hpp>

using namespace std;
using namespace SpinXML;
using namespace sigc;
using namespace Eigen;

//============================================================//
// Gobal Singals

sigc::signal<void,Spin*> SpinXML::sigAnySpinDying;

//============================================================//
// Spin-view

Vector3d SpinView::GetPosition() const {
	Vector3d pos=mData->GetPosition();
	Vector4d v(mUnitSystem->lengthUnit.toSI(pos.x()),
			   mUnitSystem->lengthUnit.toSI(pos.y()),
			   mUnitSystem->lengthUnit.toSI(pos.z()),1);
	v=mFrame->getTransformFromLab()*v;
    return Vector3d(v.x(),v.y(),v.z());
}

void SpinView::SetPosition(Vector3d Position) {
	Vector4d v(Position.x(),Position.y(),Position.z(),1);
	v=mFrame->getTransformToLab()*v;
	mData->SetCoordinates(v.x()*mUnitSystem->lengthUnit,
						  v.y()*mUnitSystem->lengthUnit,
						  v.z()*mUnitSystem->lengthUnit);
}

void SpinView::GetCoordinates(double* _x,double* _y, double* _z) const {
    length ux,uy,uz;
    mData->GetCoordinates(&ux,&uy,&uz);
	Vector4d v=mFrame->getTransformFromLab()*Vector4d(ux/mUnitSystem->lengthUnit,
													  uy/mUnitSystem->lengthUnit,
													  uz/mUnitSystem->lengthUnit,1);
	*_x=v.x();
	*_y=v.y();
	*_z=v.z();
	
}

void SpinView::SetCoordinates(double  _x,double  _y, double  _z) {
	Vector4d v=mFrame->getTransformToLab()*Vector4d(_x,_y,_z,1);
	mData->SetCoordinates(v.x()*mUnitSystem->lengthUnit,
						  v.y()*mUnitSystem->lengthUnit,
						  v.z()*mUnitSystem->lengthUnit);
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
	sigAnySpinDying(this);
}

Vector3d& Spin::GetPosition() {
    return mPosition;
}

void Spin::SetPosition(Vector3d Position) {
    mPosition=Position;
    sigChange();
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
}

void Spin::SetLabel(string Label) {
    mLabel=Label;
    sigChange();
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
}

void Spin::SetIsotope(long isotope) {
    mIsotope=isotope;
    sigChange();
}

long Spin::GetIsotope() const {
    return mIsotope;
}


