
#ifndef SPINSYS_H;
#define SPINSYS_H;

#include <vector>
#include <string>
#include <string.h>
#include <stdexcept>
#include <shared/mathtypes.hpp>
#include <sigc++/sigc++.h>
#include <shared/unit.hpp>



namespace SpinXML {
    typedef Vector3_t<length> Vector3l;

    void SetSchemaLocation(const char* loc);

    const long END=-1;

    class SpinSystem;
    class Spin;
    class Orientation;
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



    ///Class representing one of the shielding paramiters such as the
    ///chemical shift or J coupling
    class Interaction : public sigc::trackable {
        friend class SpinSystem;
    public:
        ///Constructs an undefined interaction. The type are returned by
        ///GetType() is null UNDEFINED.
        Interaction();
        ///Copy constructor
        Interaction(const Interaction& inter, SpinSystem* newSystem=NULL);
        ///Destructor
        ~Interaction();

        //TODO: This function should probably assert that it's a HFC, linear or quadratic
        void OnSpinDying(Spin*) {delete this;}
    
        ///Print the interaction to the strandard output in a human readable
        ///form.
        void Dump() const;

        ///Enumeration of the agebrake forms
        enum Form {
            LINEAR,
            BILINEAR,
            QUADRATIC,
            ANY_FORM
        };

        ///Enumeration of the storage conventions used by this interaction
        enum Type {
            UNDEFINED,
            SCALAR,
            MATRIX,
            EIGENVALUES,
            AXRHOM,
            SPANSKEW
        };

        ///Enumeration what the physical source of this interaction is. Can
        ///be used as a hint for simulation software or it might determine
        ///how a partcular interaction is visualised.
        enum SubType {
            ST_ANY,
            ST_NMR,
            ST_EPR,

            //EPR INTERACTIONS
            ST_HFC,
            ST_G_TENSER,
            ST_ZFS,
            ST_EXCHANGE,

            //NMR INTERACTIONS
            ST_SHIELDING,
            ST_SCALAR,   

            //Interactions relevent to both nmr and epr
            ST_QUADRUPOLAR,
            ST_DIPOLAR,
            ST_CUSTOM_LINEAR,	 
            ST_CUSTOM_BILINEAR,
            ST_CUSTOM_QUADRATIC
        };

        ///Get a human readable name for a member of the enum Type
        static const char* GetTypeName(Type t);
        ///Get a human readable name for a member of the enum SubType
        static const char* GetSubTypeName(SubType st);
        static Form GetFormFromSubType(SubType st);

        ///Get the storage convention being used
        Type GetType() const;
        ///Get the physical source of this interaction
        SubType GetSubType() const;
        ///Set a flag indicating the physical source of this interaction.
        void SetSubType(SubType st, Spin* spin1, Spin* spin2=NULL);
        ///Returns true if the physical source of this interaction is t. The
        ///members ST_NMR, ST_EPR and ST_ANY may be used here and will be
        ///interpreted coorectly. For example, if inter is of SubType
        ///ST_SCALAR then inter.IsSubType(ST_NMR) is true.
        bool IsSubType(SubType t) const;
    
        ///For a bilinear interaction, get the spin that is not spinA
        Spin* GetOtherSpin(Spin* spinA) {
            if(spinA==mSpin1) {
                return mSpin2;
            } else {
                return mSpin1;
            }
        }

        ///If the scalar sorage convention being used then this function will set
        ///the value of its argument to the appropreate value. Otherwise the
        ///result is undefined.
        void GetScalar(energy* Scalar) const;
        ///If the Matrix sorage convention being used then this function will set
        ///the value of its argument to the appropreate value. Otherwise the
        ///result is undefined.
        void GetMatrix(Matrix3* OutMatrix) const;
        ///If the Eigenvalues sorage convention being used then this function will set
        ///the value of its arguments to the appropreate value. Otherwise the
        ///result is undefined.
        void GetEigenvalues(energy* XX,energy* YY, energy* ZZ, Orientation* OrientOut) const;
        ///If the Axiality-Rhombicity sorage convention being used then this function will set
        ///the value of its arguments to the appropreate value. Otherwise the
        ///result is undefined.
        void GetAxRhom(energy* iso,energy* ax, energy* rh, Orientation* OrientOut) const;
        ///If the Span-Skew sorage convention being used then this function will set
        ///the value of its arguments to the appropreate value. Otherwise the
        ///result is undefined.
        void GetSpanSkew(energy* iso,energy* Span, double* Skew, Orientation* OrientOut) const;

        ///Set the value of the interaction using the scalar covention.
        void SetScalar(energy Scalar);
        ///Set the value of the interaction using the matrix covention.
        void SetMatrix(const Matrix3& InMatrix);
        ///Set the value of the interaction using the eigenvalue covention.
        void SetEigenvalues(energy XX,energy YY, energy ZZ, const Orientation& Orient);
        ///Set the value of the interaction using the axiality-rhombicity covention.
        void SetAxRhom(energy iso,energy ax, energy rh, const Orientation& Orient);
        ///Set the value of the interaction using the span-skew covention.
        void SetSpanSkew(energy iso,energy Span, double Skew, const Orientation& Orient);

        ///Cache the form of the interaction
        bool SetLinear();   
        bool SetBilinear(); 
        bool SetQuadratic();

        bool GetIsLinear();   
        bool GetIsBilinear(); 
        bool GetIsQuadratic();

        ///Get the isotropic value of the interaction
        double GetAsScalar() const;
        ///Get the interaction as a full matrix
        Matrix3 GetAsMatrix() const /*throw(logic_error)*/;
        sigc::signal<void> sigChange;
        sigc::signal<void,Interaction*> sigDying;
        ///This signal is emited whenever one of the spins this interaction
        ///referes to changes. The first argument is a pointer to the old
        ///spin
        sigc::signal<void,Interaction*,Spin*> sigRemoveSpin;

    private:
        union  {
            double mScalar;
            Matrix3* mMatrix;
            struct {
                double XX;
                double YY;
                double ZZ;
            } mEigenvalues;
            struct {
                double iso;
                double ax;
                double rh;
            } mAxRhom;
            struct {
                double iso;
                double span;
                double skew;
            } mSpanSkew;
        } mData;

        Orientation mOrient;

        Type mType;
        SubType mSubType;

    public:
        Spin* GetSpin1() const {return mSpin1;}
        Spin* GetSpin2() const {return mSpin2;}
        Spin* GetOtherSpin(const Spin* spin) const {return (mSpin1==spin ? mSpin2 : 
                                                            (mSpin2==spin ? mSpin1 : NULL));}
    private:
        Spin* mSpin1;
        sigc::connection mConnect1;
        sigc::connection mDyingConnect1;
        Spin* mSpin2;
        sigc::connection mConnect2;
        sigc::connection mDyingConnect2;
    private:
    };

    ///A class representing a spin in a spin system
    class Spin : public sigc::trackable {
    private:
    public:  
        Spin(Vector3l mPosition,std::string mLabel,long atomicNumber=1);
        Spin(const Spin& s);
        ~Spin();
  
        Vector3l& GetPosition();
        void SetPosition(Vector3l Position);
        void GetCoordinates(length* _x,length* _y, length* _z) const;
        void SetCoordinates(length _x,length _y, length _z);

        void SetLabel(std::string Label);
        const char* GetLabel() const;

        const std::vector<Interaction*>& GetInteractions() const {return mInter;}
        ///Returns a vector of all linear, quadratic interactions and some
        ///of the bilinear interactions. The bilinear interactions
        ///included will be chosen so that if GetInteractionsOnce is
        ///called on both spins involved in the interaction, the bilinear
        ///interaction will show up exactly once
        std::vector<Interaction*> GetInteractionsOnce() const;

        void InsertInteraction(Interaction* _Interaction,long Position=END);
        void RemoveInteraction(Interaction* _Interaction);
        void OnRemoveInteraction(Interaction* inter,Spin* spin) {
            if(spin==this){
                RemoveInteraction(inter);
            }
        }
        void OnInteractionChange() {sigChange();}

        double GetLinearInteractionAsScalar(Interaction::SubType t=Interaction::ST_ANY) const;
        double GetQuadrapolarInteractionAsScalar(Interaction::SubType t=Interaction::ST_ANY) const;

        Matrix3 GetLinearInteractionAsMatrix(Interaction::SubType t=Interaction::ST_ANY) const;
        Matrix3 GetQuadrapolarInteractionAsMatrix(Interaction::SubType t=Interaction::ST_ANY) const;

        bool GetHasInteractionOfType(Interaction::SubType t) const;
        Matrix3 GetTotalInteraction(Interaction::SubType t) const;

        long GetElement() const;
        void SetElement(long element);
        std::vector<long> GetIsotopes() const;
        void SetIsotopes(std::vector<long> isotopes) const;

        sigc::signal<void,Interaction*> sigNewInteraction;
        sigc::signal<void> sigChange;
        sigc::signal<void,Spin*> sigDying;

    private:
        std::vector<Interaction*> mInter;
        Vector3l mPosition;
        std::string mLabel;
        long mElement;
        std::vector<long> mIsotopes;
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
        void RemoveSpin(long Position);
        void RemoveSpin(Spin* _Spin);

        void LoadFromFile(const char* filename,ISpinSystemLoader* loader);
        void SaveToFile(const char* filename,ISpinSystemLoader* saver) const;

        ///Return all spins withing distance of point pos. Do not return
        ///skip all spins below Ignore
        std::vector<Spin*> GetNearbySpins(Vector3l pos,length distance,Spin* Ignore=NULL);

        //Event Handlers
        void OnSpinDeleted(Spin* spin){RemoveSpin(spin);}

        sigc::signal<void,Spin*,long> sigNewSpin;
        sigc::signal<void> sigReloaded;
        sigc::signal<void> sigDying;

        ///Automacially calculate the nuclear dipole-dipole couplings
        ///from the positions off the nuclear spins
        void CalcNuclearDipoleDipole();

  
    private:
        friend class Spin;

        //Set just before deleting a spin
        Spin* mIgnoreSpinKill;
        std::vector<Spin*> mSpins;
    };

}; //End Namespace


#endif
