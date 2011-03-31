
#ifndef SPIN_H
#define SPIN_H

#include <Eigen/Dense>
#include <Eigen/Geometry> 
#include <sigc++/sigc++.h>
#include <vector>

#include <shared/unit.hpp>
#include <shared/frame.hpp>
#include <shared/base.hpp>

namespace SpinXML {
	class SpinView;

    ///A class representing a spin in a spin system
    class Spin : public SpinXMLBase<Spin,SpinView> {
    public:  
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
        Spin(Eigen::Vector3d mPosition,std::string mLabel,long atomicNumber=1,long isotope=1);
        Spin(const Spin& s);
        ~Spin();
  
		Eigen::Vector3d& GetPosition();
        void SetPosition(Eigen::Vector3d Position);
        void GetCoordinates(length* _x,length* _y, length* _z) const;
        void SetCoordinates(length _x,length _y, length _z);

        void SetLabel(std::string Label);
        const char* GetLabel() const;

        long GetElement() const;
        void SetElement(long element);
        long GetIsotope() const;
        void SetIsotope(long isotope);
    private:
        Eigen::Vector3d mPosition;
        std::string mLabel;
        long mElement;
        long mIsotope;
    };

	extern sigc::signal<void,Spin*> sigAnySpinDying;

    class SpinView : public ViewBase<SpinView,Spin> {
    public:
        typedef ViewBase<SpinView,Spin> Base;

        SpinView(Spin* spin,const Frame* frame, const UnitSystem* unitSystem) 
            : Base(spin,frame,unitSystem) {
        }

        ///When the SpinView detects the spin it is watching has been
        ///deleted it deletes itself and passes the signal upwards.

		
        Eigen::Vector3d GetPosition() const;
        void SetPosition(Eigen::Vector3d Position);
        void GetCoordinates(double* _x,double* _y, double* _z) const;
        void SetCoordinates(double  _x,double  _y, double  _z);

        ///All these are compleatly trivial
        void SetLabel(std::string Label)    {mData->SetLabel(Label);}
        const char* GetLabel() const        {return mData->GetLabel();}

        long GetElement() const             {return mData->GetElement();}
        void SetElement(long element)       {mData->SetElement(element);}
        long GetIsotope() const             {return mData->GetIsotope();}
        void SetIsotope(long isotope) const {mData->SetIsotope(isotope);}
    };

};



#endif
