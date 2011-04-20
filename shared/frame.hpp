

#ifndef FRAME_H
#define FRAME_H

#include <Eigen/Geometry>
#include <sigc++/sigc++.h>
#include <shared/orientation.hpp>
#include <vector>
#include <shared/objcounter.hpp>

namespace SpinXML {

    class UnitSystem : private Counter<UnitSystem> {
    public:
        using Counter<UnitSystem>::objCount;
        //SI by default
        UnitSystem();
        unit energyUnit;
        unit lengthUnit;
        unit timeUnit;
        static const UnitSystem* GetDefault() {return &singleton;}
    private:
        static UnitSystem singleton;

    };
    ///Class representing a frame in a hiraci of nested reference
    ///frames. This class will try to do a resonable amount of
    ///precomputation as soon as possible possible so that all queries
    ///involving transforming to and from the lab frame are as fast as
    ///possible.
    class Frame : public sigc::trackable {
    public:
        Frame(Eigen::Vector3d translation, Orientation rotation, const UnitSystem* unitSystem);
        ~Frame();

        void SetTranslation(const Eigen::Vector3d& vec);
        const Eigen::Vector3d& GetTranslation() const {return mTranslate;}

        void SetOrientation(const Orientation orient);
        const Orientation& GetOrientation() const {return mOrient;}

        ///Return a 4x4 isometary matrix in openGL convention a
        ///transformation into this frame. In other words if v is a
        ///vector in the lab frame then f.getTransformFromLab() * v is a
        ///vector in the f frame. Units are SI
        Eigen::Matrix4d getTransformFromLab() const;
        ///Return a 4x4 isometary matrix expression a transformation
        ///into this frame. In other words if v is a vector in the f
        ///frame then: f.getTransformToLab() * v is a vector in the
        ///lab frame. Units are SI
        Eigen::Matrix4d getTransformToLab() const;

        ///Returns the parent frame of this frame
        Frame* upOne() const {return mParent;}

        void AddChild(Frame* frame);
        const std::vector<Frame*>& GetChildren() const {return mChildren;}

        ///Emited when Frame is about to be deleted
        sigc::signal<void,Frame*> sigDying;
        ///Emited when the Frame has been edited
        sigc::signal<void,Frame*> sigChange;
        ///Emited when a ancesstor of this frame is changed.
        sigc::signal<void,Frame*> sigAncestorChange;
    private:
        void Invariant() const;
        void updateAfine();

        Frame* mParent;
        std::vector<Frame*> mChildren;

        Eigen::Vector3d mTranslate;
        Orientation mOrient;
        Eigen::Matrix4d mAffine;
        Eigen::Matrix4d mInvertedAffine;
        const UnitSystem* mUnitSystem;
    };
    Eigen::Vector3d ToLabVec3d(Frame* frame,const Eigen::Vector3d& v);
    Eigen::Vector3d FromLabVec3d(Frame* frame,const Eigen::Vector3d& v);

    Eigen::Vector3d ToLabMatrix3d(Frame* frame,const Eigen::Matrix3d& m);
    Eigen::Vector3d FromLabMatrix3d(Frame* frame,const Eigen::Matrix3d& m);

};

#endif

