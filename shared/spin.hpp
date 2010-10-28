
#ifndef SPIN_H
#define SPIN_H

#include <sigc++/sigc++.h>
#include <Eigen/Dense>
#include <Eigen/Geometry> 
#include <vector>

#include <shared/unit.hpp>
#include <shared/frame.hpp>

using namespace Eigen;


namespace SpinXML {

    ///A class representing a spin in a spin system
    class Spin : public sigc::trackable {
    public:  
        Spin(Vector3d mPosition,std::string mLabel,long atomicNumber=1,long isotope=1);
        Spin(const Spin& s);
        ~Spin();
  
        Vector3d& GetPosition();
        void SetPosition(Vector3d Position);
        void GetCoordinates(length* _x,length* _y, length* _z) const;
        void SetCoordinates(length _x,length _y, length _z);

        void SetLabel(std::string Label);
        const char* GetLabel() const;

        long GetElement() const;
        void SetElement(long element);
        long GetIsotope() const;
        void SetIsotope(long isotope);

        sigc::signal<void> sigChange;
        sigc::signal<void,Spin*> sigDying;
    private:
        Vector3d mPosition;
        std::string mLabel;
        long mElement;
        long mIsotope;
    };

    class SpinView : public ViewBase<SpinView> {
        SpinView(Spin* spin,Frame* frame, UnitSystem* unitSystem);

        ///When the SpinView detects the spin it is watching has been
        ///deleted it deletes itself and passes the signal upwards.
        void OnSpinDie();

        Vector3d& GetPosition() const;
        void SetPosition(Vector3d Position);
        void GetCoordinates(double* _x,double* _y, double* _z) const;
        void SetCoordinates(double  _x,double  _y, double  _z);

        void SetLabel(std::string Label);
        const char* GetLabel() const;

        long GetElement() const;
        void SetElement(long element);
        long GetIsotope() const;
        void SetIsotope(long isotope) const;

        sigc::signal<void> sigChange;
        sigc::signal<void,Spin*> sigDying;
    };

};



#endif
