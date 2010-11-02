
#include <shared/frame.hpp>

using namespace SpinXML;
using namespace Eigen;

Frame::Frame(Vector3d translation, Orientation rotation, Frame* parent) 
    : mParent(parent),mTranslate(translation),mOrient(rotation) {
    updateAfline();
}

void Frame::SetTranslation(const Vector3d& vec) {
    mTranslate = vec;
    updateAfline();
}

void Frame::SetTranslation(const Orientation orient) {
    mOrient = orient;
    updateAfline();
}


void Frame::updateAfline() {

}


