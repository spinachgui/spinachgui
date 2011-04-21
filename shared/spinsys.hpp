
#ifndef SPINSYS_H
#define SPINSYS_H

#include <shared/interaction.hpp>

#include <shared/unit.hpp>

#include <Eigen/Dense>
#include <sigc++/sigc++.h>
#include <boost/variant.hpp>

#include <vector>
#include <string>
#include <string.h>
#include <stdexcept>


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


    class SpinSystem : public SpinXMLBase<SpinSystem> {
    public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
        SpinSystem();
        SpinSystem(const SpinSystem& system);
        ~SpinSystem();

        void Clear();

        void Dump() const;

        long GetSpinCount() const;
        Spin* GetSpin(unsigned long n) const;
        long GetSpinNumber(Spin* spin) const;

        const std::vector<Spin*>& GetSpins() const;
        void InsertSpin(Spin* _Spin,long Position=END);

        ///Remove a spin from the spin system. If it's no longer required it still has to be deleted.
        Spin* RemoveSpin(long Position);
        ///Remove a spin from the spin system. If it's no longer required it still has to be deleted.
        Spin* RemoveSpin(Spin* _Spin);

        ///Remove a spin from the spin system.
		void DiscardSpin(long Position) {delete RemoveSpin(Position);}
        ///Remove a spin from the spin system.
		void DiscardSpin(Spin* _Spin)   {delete RemoveSpin(_Spin);}


        void OnSpinDeleted(Spin* spin);

        void InsertInteraction(Interaction* inter);
        Interaction* RemoveInteraction(Interaction* inter);
		void SlotInterDying(Interaction* inter) {RemoveInteraction(inter);}

		void DiscardInteraction(Interaction* inter) {delete RemoveInteraction(inter);}

        long GetInterCount() const {return mInteractions.size();}

        //Returns all interactions involving Spin spin
        std::vector<Interaction*> GetInteractionsBySpin(const Spin* spin,Interaction::Type t=Interaction::ANY) const;
        //Get all the interactions involving both spin1 and spin2
        std::vector<Interaction*> GetInteractionsBySpin(const Spin* spin1, Spin* spin2,Interaction::Type t=Interaction::ANY) const;
        //Returns all interactions involving Spin spin
        std::vector<Interaction*> GetInteractionsBySpinOnce(const Spin* spin,Interaction::Type t=Interaction::ANY) const;
		//Returns all interactions. Good for writing output filters
		std::vector<Interaction*> GetAllInteractions() const {return mInteractions;}

        void LoadFromFile(const char* filename,ISpinSystemLoader* loader);
        void SaveToFile(const char* filename,ISpinSystemLoader* saver) const;

        ///Return all spins withing distance of point pos. Do not return
        ///skip all spins below Ignore
        std::vector<Spin*> GetNearbySpins(Eigen::Vector3d pos,length distance,Spin* Ignore=NULL);

        sigc::signal<void,Spin*,long> sigNewSpin;
        sigc::signal<void,Interaction*> sigNewInter;
        sigc::signal<void> sigReloaded;

        ///Automacially calculate the nuclear dipole-dipole couplings
        ///from the positions off the nuclear spins
        void CalcNuclearDipoleDipole();

		///If two interactions act on the same spin or spin pair,
		///crush them into one, large, equivilent interaction. This
		///treatment is needed, for example, after loading from a
		///gaussian file (gaussian gives the isotropic and anisotropic
		///parts seperatly)
		void CompressDuplicateInteractions();

		///Get the lab frame, which is always an unrotated frame centred at the origin
		Frame* GetLabFrame() const {return mRootFrame;}

		///Delete all frames and create a single lab frame
		void SetupLabFrame();
    private:
        std::vector<Interaction*> mInteractions;
        std::vector<Spin*> mSpins;
		Frame* mRootFrame;
    };


}; //End Namespace


#endif
