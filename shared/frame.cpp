
#include <shared/frame.hpp>
#include <Eigen/Geometry>
#include <shared/panic.hpp>

using namespace SpinXML;
using namespace Eigen;

UnitSystem::UnitSystem()
	: energyUnit(unit("Joules",1.0)),lengthUnit(unit("Metres",1.0)),timeUnit(unit("Seconds",1.0)) {
}

//============================================================//

Frame::Frame(Vector3d translation, Orientation rotation, const UnitSystem* unitSystem) 
    : mParent(NULL),
	  mTranslate(translation * (unitSystem->lengthUnit.get_to_si())),
	  mOrient(rotation),mUnitSystem(unitSystem) {
    updateAfine();
	Invariant();
}

Frame::~Frame() {
	for(unsigned long i=0;i<mChildren.size();i++) {
		delete mChildren[i];
	}
	sigDying(this);
}

void Frame::SetTranslation(const Vector3d& vec) {
    mTranslate = vec;
    updateAfine();
	sigChange(this);
	Invariant();
}

void Frame::SetOrientation(const Orientation orient) {
    mOrient = orient;
    updateAfine();
	sigChange(this);
	Invariant();
}


void Frame::updateAfine() {
    Affine3d T=Translation3d(mTranslate)*Affine3d(mOrient.GetAsQuaternion());
	Matrix4d mat = T.matrix();
	Matrix4d imat = T.inverse().matrix();
    mAffine =         (mParent==NULL) ? mat : mParent->mAffine*mat;
    mInvertedAffine = (mParent==NULL) ? mat : mat * mParent->mInvertedAffine;
}

void Frame::AddChild(Frame* frame) {
	mChildren.push_back(frame);
	frame->mParent = frame;
	sigChange(this);
	frame->sigChange(frame);
	Invariant();
}


Matrix4d Frame::getTransformFromLab() const {
	return mAffine;
}

Matrix4d Frame::getTransformToLab() const {
	return mInvertedAffine;
}


void Frame::Invariant() const {
	NaNPANIC(mTranslate.x(),"mTranslate.x is NaN");
	NaNPANIC(mTranslate.y(),"mTranslate.x is NaN");
	NaNPANIC(mTranslate.z(),"mTranslate.x is NaN");

	NaNPANIC(mAffine(0,0),"mAffine containts a NaN element");
	NaNPANIC(mAffine(0,1),"mAffine containts a NaN element");
	NaNPANIC(mAffine(0,2),"mAffine containts a NaN element");

	NaNPANIC(mAffine(1,0),"mAffine containts a NaN element");
	NaNPANIC(mAffine(1,1),"mAffine containts a NaN element");
	NaNPANIC(mAffine(1,2),"mAffine containts a NaN element");

	NaNPANIC(mAffine(2,0),"mAffine containts a NaN element");
	NaNPANIC(mAffine(2,1),"mAffine containts a NaN element");
	NaNPANIC(mAffine(2,2),"mAffine containts a NaN element");

	NaNPANIC(mInvertedAffine(0,0),"mInvertedAffine containts a NaN element");
	NaNPANIC(mInvertedAffine(0,1),"mInvertedAffine containts a NaN element");
	NaNPANIC(mInvertedAffine(0,2),"mInvertedAffine containts a NaN element");

	NaNPANIC(mInvertedAffine(1,0),"mInvertedAffine containts a NaN element");
	NaNPANIC(mInvertedAffine(1,1),"mInvertedAffine containts a NaN element");
	NaNPANIC(mInvertedAffine(1,2),"mInvertedAffine containts a NaN element");

	NaNPANIC(mInvertedAffine(2,0),"mInvertedAffine containts a NaN element");
	NaNPANIC(mInvertedAffine(2,1),"mInvertedAffine containts a NaN element");
	NaNPANIC(mInvertedAffine(2,2),"mInvertedAffine containts a NaN element");
}


//Initalise the static default UnitSystem
UnitSystem UnitSystem::singleton;

