
#include <shared/frame.hpp>
#include <Eigen/Geometry>

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
}

void Frame::SetOrientation(const Orientation orient) {
    mOrient = orient;
    updateAfine();
	sigChange(this);
}


void Frame::updateAfine() {
    Affine3d T=Translation3d(mTranslate)*Affine3d(mOrient.GetAsQuaternion());
	Matrix4d mat = T.matrix();
    mAffine = (mParent==NULL) ? mat : mParent->mAffine*mat;
}

void Frame::AddChild(Frame* frame) {
	mChildren.push_back(frame);
	frame->mParent = frame;
	sigChange(this);
	frame->sigChange(frame);
}


Matrix4d Frame::getTransformFromLab() const {
	return mAffine;
}

Matrix4d Frame::getTransformToLab() const {
	return mInvertedAffine;
}


//Initalise the static default UnitSystem
UnitSystem UnitSystem::singleton;

