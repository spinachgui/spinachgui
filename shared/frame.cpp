
#include <shared/frame.hpp>
#include <Eigen/Geometry>

using namespace SpinXML;
using namespace Eigen;

Frame::Frame(Vector3d translation, Orientation rotation, Frame* parent) 
    : mParent(parent),mTranslate(translation),mOrient(rotation) {
    updateAfine();
}

void Frame::SetTranslation(const Vector3d& vec) {
    mTranslate = vec;
    updateAfine();
}

void Frame::SetOrientation(const Orientation orient) {
    mOrient = orient;
    updateAfine();
}


void Frame::updateAfine() {
    Affine3d T=Translation3d(mTranslate)*Affine3d(mOrient.GetAsQuaternion());
    Matrix4d mat=T.matrix();
    mAffine = (mParent==NULL) ? mat : mParent->mAffine*mat;
}


