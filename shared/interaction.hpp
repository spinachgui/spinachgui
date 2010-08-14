
#include <shared/mathtypes.hpp>
#include <sigc++/sigc++.h>
#include <shared/unit.hpp>
#include <boost/variant.hpp>

namespace SpinXML {
    class Interaction : public sigc::trackable {
    public:
        ///Enumeration of the storage conventions used by this interaction
        enum Storage {
            SCALAR,
            MATRIX,
            EIGENVALUES,
            AXRHOM,
            SPANSKEW
        };

        ///Construct from a scalar
        Interaction(energy inter)          
            : mData(inter){}
        ///Construct from a matrix
        Interaction(const Matrix3d& inter)
            : mData(inter){}
        ///Construct from a matrix
        Interaction(const Eigenvalues& inter)
            : mData(inter){}
        ///Construct from a matrix
        Interaction(const AxRhom& inter)
            : mData(inter){}
        ///Construct from a matrix
        Interaction(const SpanSkew& inter)
            : mData(inter){}
        ///Copy constructor
        Interaction(const Interaction& inter, SpinSystem* newSystem=NULL);
        ///Destructor
        ~Interaction();

        //TODO: This function should probably assert that it's a HFC, linear or quadratic
        void OnSpinDying(Spin*) {delete this;}
    
        ///Print the interaction to the strandard output in a human readable
        ///form.
        void Dump() const;

        ///Enumeration of the algebraic forms
        enum Form {
            LINEAR,
            BILINEAR,
            QUADRATIC,
            ANY_FORM
        };

        ///Enumeration what the physical source of this interaction is. Can
        ///be used as a hint for simulation software or it might determine
        ///how a partcular interaction is visualised.
        enum Type {
            ANY,
            NMR,
            EPR,

            //EPR INTERACTIONS
            HFC,
            G_TENSER,
            ZFS,
            EXCHANGE,

            //NMR INTERACTIONS
            SHIELDING,
            SCALAR,   

            //Interactions relevent to both nmr and epr
            QUADRUPOLAR,
            DIPOLAR,
            CUSTOM_LINEAR,	 
            CUSTOM_BILINEAR,
            CUSTOM_QUADRATIC
        };

        ///Get a human readable name for a member of the enum Type
        static const char* GetStorageName(Storage t);
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
        void GetMatrix(Matrix3e* OutMatrix) const;
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
        void SetMatrix(const Matrix3e& InMatrix);
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

        sigc::signal<void> sigChange;
        sigc::signal<void,Interaction*> sigDying;
        ///This signal is emited whenever one of the spins this interaction
        ///referes to changes. The first argument is a pointer to the old
        ///spin
        sigc::signal<void,Interaction*,Spin*> sigRemoveSpin;

        void ToScalar();
        void ToMatrix();
        void ToEigenvalues();
        void ToAxRhom();
        void ToSpanSkew();

        Interaction AsScalar() const;
        Interaction AsMatrix() const;
        Interaction AsEigenvalues() const;
        Interaction AsAxRhom() const;
        Interaction AsSpanSkew() const;

    private:

	typedef boost::variant<energy,Matrix3e,eigenvalues_t,ax_rhom_t,span_skew_t> var_t;
	var_t mData;
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
};

