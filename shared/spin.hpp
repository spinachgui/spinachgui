
#ifndef SPIN_H
#define SPIN_H

#include <Eigen/Geometry> 
#include <sigc++/sigc++.h>
#include <vector>

#include <shared/unit.hpp>
#include <shared/frame.hpp>
#include <shared/base.hpp>

namespace SpinXML {

    ///A class representing a spin in a spin system
    class Spin : public SpinXMLBase<Spin>, private Counter<Spin> {
    public:  
		using Counter<Spin>::objCount;
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
        Spin(Eigen::Vector3d mPosition,std::string mLabel,long atomicNumber=1,long isotope=-1);
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


};



#endif
