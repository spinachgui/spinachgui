

#ifndef FRAME_H
#define FRAME_H

#include <sigc++/sigc++.h>
#include <shared/orientation.hpp>
#include <vector>

using namespace Eigen;

namespace SpinXML {

    class UnitSystem {
	public:
        //SI by default
        UnitSystem() : energyUnit(Joules),lengthUnit(metres),timeUnit(seconds) {
        }
        energy_unit energyUnit;
        length_unit lengthUnit;
        time_unit timeUnit;
    };

    ///Class representing a frame in a hiraci of nested reference
    ///frames. This class will try to do a resonable amount of
    ///precomputation as soon as possible possible so that all queries
    ///involving transforming to and from the lab frame are as fast as
    ///possible.
    class Frame : public sigc::trackable {
    public:
        Frame(Vector3d translation, Orientation rotation, UnitSystem* unitSystem, Frame* parent=NULL);

        void SetTranslation(const Vector3d& vec);
        const Vector3d& GetTranslation() const {return mTranslate;}

        void SetOrientation(const Orientation orient);
        const Orientation& GetOrientation() const {return mOrient;}

        ///Return a 4x4 isometary matrix in openGL convention a
        ///transformation into this frame. In other words if v is a
        ///vector in the lab frame then f.getTransformFromLab() * v is a
        ///vector in the f frame. Units are SI
        Matrix4d getTransformFromLab() const {return mAffine;}
        ///Return a 4x4 isometary matrix expression a transformation
        ///into this frame. In other words if v is a vector in the f
        ///frame then: f.getTransformToLab() * v is a vector in the
        ///lab frame. Units are SI
        Matrix4d getTransformToLab() const {return mInvertedAffine;}

        ///Returns the parent frame of this frame
        Frame* upOne() const {return mParent;}

        ///Emited when Frame is about to be deleted
        sigc::signal<Frame*> sigDying;
        ///Emited when the Frame has been edited
        sigc::signal<Frame*> sigChange;
        ///Emited when a ancesstor of this frame is changed.
        sigc::signal<Frame*> sigAncestorChange;
    private:
        void updateAfine();

        Frame* mParent;
        Vector3d mTranslate;
        Orientation mOrient;
        Matrix4d mAffine;
        Matrix4d mInvertedAffine;
		UnitSystem* mUnitSystem;
    };

    template <typename Derived,typename T>
	class ViewBase : public sigc::trackable {
    public:
		typedef T ViewedType;

        ViewBase(T* data,const Frame* frame, const UnitSystem* unitSystem)
            : mData(data),mFrame(frame),mUnitSystem(unitSystem) {
			mData->sigChange.connect(sigChange);
            mData->sigDying.connect(sigDying);
            mData->sigDying.connect(mem_fun(this,&ViewBase::OnObjectDie));
        }
        void OnObjectDie(T*) {mData=NULL;}
        void SetFrame(Frame* frame) {mFrame=frame;}
        void SetUnitSystem(UnitSystem* unitSystem) {mUnitSystem=unitSystem;}

        T* operator ->() {return mData;}
		T* Get() {return mData;}
		const T* Get() const {return mData;}

        sigc::signal<void> sigChange;
        sigc::signal<void,T*> sigDying;
    protected:
		template <typename ViewType>
		std::vector<ViewType> MapVectorToViewVector(const std::vector<typename ViewType::ViewedType*>& v) const {
			std::vector<ViewType> ret;
			for(long i=0;i<v.size();i++) {
				ret.push_back(v[i]->GetView(mFrame,mUnitSystem));
			}
			return ret;
		}

        T* mData;
        const Frame* mFrame;
        const UnitSystem* mUnitSystem;
    };
};

#endif

