

#include <sigc++/sigc++.h>
#include <Eigen/Dense>
#include <Eigen/Geometry> 
#include <vector>

#include <shared/unit.hpp>

using namespace Eigen;


namespace SpinXML {

    ///A class representing a spin in a spin system
    class Spin : public sigc::trackable {
    public:  
        Spin(Vector3d mPosition,std::string mLabel,long atomicNumber=1);
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
        std::vector<long> GetIsotopes() const;
        void SetIsotopes(std::vector<long> isotopes) const;

        sigc::signal<void> sigChange;
        sigc::signal<void,Spin*> sigDying;
    private:
        Vector3d mPosition;
        std::string mLabel;
        long mElement;
        std::vector<long> mIsotopes;
    };


};
