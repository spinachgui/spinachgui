
#ifndef SPINSYS_H;
#define SPINSYS_H;

#include <vector>
#include <string>
#include <string.h>
#include <stdexcept>
#include <sigc++/sigc++.h>
#include <shared/unit.hpp>
#include <boost/variant.hpp>
#include <Eigen/Dense>
#include <shared/interaction.hpp>

using namespace Eigen;

namespace SpinXML {

    //============================================================//
    // 3D vector type



    void SetSchemaLocation(const char* loc);

    const long END=-1;


    class SpinSystem;
    class Spin;
    class Interaction;

    class ISpinSystemLoader {
    public:
        enum FilterType {
            LOAD,
            SAVE,
            LOADSAVE
        };
        virtual FilterType GetFilterType() const=0;
        virtual const char* GetFilter() const=0;
        virtual const char* GetFormatName() const=0;
        virtual void LoadFile(SpinSystem* ss,const char* filename) const=0;
        virtual void SaveFile(const SpinSystem* ss,const char* filename) const=0;
    };


    class SpinSystem : public sigc::trackable {
    public:
        SpinSystem();
        SpinSystem(const SpinSystem& system);
        ~SpinSystem();

        void Clear();
    
        void Dump() const;

        long GetSpinCount() const;
        Spin* GetSpin(long n) const;
        long GetSpinNumber(Spin* spin) const;

        const std::vector<Spin*>& GetSpins() const;
        void InsertSpin(Spin* _Spin,long Position=END);

        ///Remove a spin from the spin system. If it's no longer required it still has to be deleted.
        Spin* RemoveSpin(long Position);
        ///Remove a spin from the spin system. If it's no longer required it still has to be deleted.
        Spin* RemoveSpin(Spin* _Spin);

        void OnSpinDeleted(Spin* spin);

        void InsertInteraction(Interaction* inter);
		
        //Returns all interactions involving Spin spin
        std::vector<Interaction*> GetInteractionBySpin(Spin* spin,Interaction::Type t=Interaction::ANY); 
        //Get all the interactions involving both spin1 and spin2
        std::vector<Interaction*> GetInteractionBySpin(Spin* spin1, Spin* spin2,Interaction::Type t=Interaction::ANY);
        //Returns all interactions involving Spin spin
        std::vector<Interaction*> GetInteractionBySpinOnce(Spin* spin,Interaction::Type t=Interaction::ANY); 
		

        void LoadFromFile(const char* filename,ISpinSystemLoader* loader);
        void SaveToFile(const char* filename,ISpinSystemLoader* saver) const;

        ///Return all spins withing distance of point pos. Do not return
        ///skip all spins below Ignore
        std::vector<Spin*> GetNearbySpins(Vector3d pos,length distance,Spin* Ignore=NULL);

        sigc::signal<void,Spin*,long> sigNewSpin;
        sigc::signal<void,Interaction*> sigNewInter;
        sigc::signal<void> sigReloaded;
        sigc::signal<void> sigDying;

        ///Automacially calculate the nuclear dipole-dipole couplings
        ///from the positions off the nuclear spins
        void CalcNuclearDipoleDipole();
  
    private:
        std::vector<Interaction*> mInteractions;
        std::vector<Spin*> mSpins;
        Spin* mIgnoreSpinKill;
    };

}; //End Namespace


#endif
