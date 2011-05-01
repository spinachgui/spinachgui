
#include <shared/frame.hpp>
#include <Eigen/Geometry>
#include <shared/panic.hpp>

#include <boost/variant/static_visitor.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <iostream>

using namespace SpinXML;
using namespace Eigen;
using namespace boost;
using namespace std;

UnitSystem::UnitSystem()
    : energyUnit(unit("Joules",1.0)),lengthUnit(unit("Metres",1.0)),timeUnit(unit("Seconds",1.0)) {
}

//============================================================//

Frame::Frame(Vector3d translation, Orientation rotation) 
    : mParent(NULL),
      mTranslate(translation),
      mOrient(rotation) {

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
    Affine3d T=Translation3d(mTranslate)*Affine3d(mOrient.GetAsDCM());
    Matrix4d mat = T.matrix();
    Matrix4d imat = T.inverse().matrix();
    mAffine =         (mParent==NULL) ? mat : mParent->mAffine*mat;
    mInvertedAffine = (mParent==NULL) ? imat : imat * mParent->mInvertedAffine;
}

void Frame::AddChild(Frame* frame) {
    mChildren.push_back(frame);
    frame->mParent = frame;
	frame->updateAfine();
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

Vector3d SpinXML::ToLabVec3d(const Frame* frame,const Vector3d& v) {
    Vector3d vprime = Affine3d(frame->getTransformToLab()) * v;
    NaNPANIC(vprime,"ToLabVec3D result is NaN");
    return vprime;
}
Vector3d SpinXML::FromLabVec3d(const Frame* frame,const Vector3d& v) {
    Vector3d vprime = Affine3d(frame->getTransformFromLab()) * v;
    NaNPANIC(vprime,"ToLabVec3D result is NaN");
    return vprime;
}

Matrix3d SpinXML::ToLabMatrix3d(const Frame* frame,const Matrix3d& m) {
    Matrix3d RFromLab = Affine3d(frame->getTransformFromLab()).rotation();
    Matrix3d RToLab   = Affine3d(frame->getTransformToLab()).rotation();
    Matrix3d mprime   = RFromLab * m * RToLab;
    NaNPANIC(mprime,"ToLabMatrix3d result is NaN");
    return mprime;
}
Matrix3d SpinXML::FromLabMatrix3d(const Frame* frame,const Matrix3d& m) {
    Matrix3d RFromLab = Affine3d(frame->getTransformFromLab()).rotation();
    Matrix3d RToLab   = Affine3d(frame->getTransformToLab()).rotation();
    Matrix3d mprime   =  RToLab * m * RFromLab;
    NaNPANIC(mprime,"ToLabMatrix3d result is NaN");
    return mprime;
}


struct toLabVisitor : public static_visitor<Orientation> {
    toLabVisitor(const Frame* frame_)
        : frame(frame_) {
    }
    Orientation operator()(const EulerAngles& dat) const {
        Matrix3d mat3 = Affine3d(frame->getTransformToLab()).rotation();
        Quaterniond q = ConvertToQuaternion(mat3);
        Quaterniond qdat = ConvertToQuaternion(dat);
        return Orientation(ConvertToEuler(q*qdat*q.conjugate()));
    }
    Orientation operator()(const Matrix3d& dat) const    {
        return Orientation(ToLabMatrix3d(frame,dat));
    }
    Orientation operator()(const Quaterniond& dat) const {
        Matrix3d mat3 = Affine3d(frame->getTransformToLab()).rotation();
        Quaterniond q = ConvertToQuaternion(mat3);
        return Orientation(q*dat*q.conjugate());
    }
    Orientation operator()(const AngleAxisd& dat) const  {
        Matrix3d mat3 = Affine3d(frame->getTransformToLab()).rotation();
        Quaterniond q = ConvertToQuaternion(mat3);
        Quaterniond qDat = ConvertToQuaternion(dat);
        return Orientation(ConvertToAngleAxis(q*qDat*q.conjugate()));
    }

    const Frame* frame;
};
Orientation SpinXML::ToLabOrient(const Frame* frame,const Orientation& o) {
    Orientation normalized=o.Normalized();
    return apply_visitor(toLabVisitor(frame),normalized.__get_variant());
}


struct fromLabVisitor : public static_visitor<Orientation> {
    fromLabVisitor(const Frame* frame_)
        : frame(frame_) {
    }
    Orientation operator()(const EulerAngles& dat) const {
        Matrix3d mat3 = Affine3d(frame->getTransformFromLab()).rotation();
        Quaterniond q = ConvertToQuaternion(mat3);
        Quaterniond qdat = ConvertToQuaternion(dat);
        return Orientation(ConvertToEuler(q*qdat*q.conjugate()));
    }
    Orientation operator()(const Matrix3d& dat) const    {
        return Orientation(FromLabMatrix3d(frame,dat));
    }
    Orientation operator()(const Quaterniond& dat) const {
        Matrix3d mat3 = Affine3d(frame->getTransformFromLab()).rotation();
        Quaterniond q = ConvertToQuaternion(mat3);
        return Orientation(q*dat*q.conjugate());
    }
    Orientation operator()(const AngleAxisd& dat) const  {
        Matrix3d mat3 = Affine3d(frame->getTransformFromLab()).rotation();
        Quaterniond q = ConvertToQuaternion(mat3);
        Quaterniond qDat = ConvertToQuaternion(dat);
        return Orientation(ConvertToAngleAxis(q*qDat*q.conjugate()));
    }

    const Frame* frame;
};
Orientation SpinXML::FromLabOrient(const Frame* frame,const Orientation& o) {
    Orientation normalized=o.Normalized();
    return apply_visitor(fromLabVisitor(frame),normalized.__get_variant());
}



//================================================================================//


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

