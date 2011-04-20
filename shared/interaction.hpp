
#ifndef INTERACTION_H
#define INTERACTION_H

#include <sigc++/sigc++.h>
#include <shared/unit.hpp>
#include <boost/variant.hpp>

#include <shared/spin.hpp>
#include <shared/orientation.hpp>

namespace SpinXML {
    ///============================================================//
    struct Eigenvalues {
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
        Eigenvalues(const double _XX,const double _YY,const double _ZZ, const Orientation& o) 
            : xx(_XX), yy(_YY), zz(_ZZ), mOrient(o) {
        }
        double xx;
        double yy;
        double zz;
        Orientation mOrient;
    };
	typedef Eigenvalues UEigenvalues;

    ///============================================================//
    struct AxRhom {
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
        AxRhom(const double _iso,const double _ax,const double _rh, const Orientation& o) 
            : iso(_iso), ax(_ax), rh(_rh), mOrient(o) {
        }
        double iso;
        double ax;
        double rh;
        Orientation mOrient;
    };
	typedef AxRhom UAxRhom;
	

    ///============================================================//
    struct SpanSkew {
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
        SpanSkew(const double _iso,const double _span,const double _skew, const Orientation& o) 
            : iso(_iso), span(_span), skew(_skew), mOrient(o) {
        }
        double iso;
        double span;
        double skew;
        Orientation mOrient;
    };
	typedef SpanSkew USpanSkew;

    //Interaction converstions
    energy ConvertToScalar(const energy& inter);
    energy ConvertToScalar(const Eigen::Matrix3d& inter);
    energy ConvertToScalar(const Eigenvalues& inter);
    energy ConvertToScalar(const AxRhom& inter);
    energy ConvertToScalar(const SpanSkew& inter);

    Eigen::Matrix3d ConvertToMatrix(const energy& inter);
	Eigen::Matrix3d ConvertToMatrix(const Eigen::Matrix3d& inter);
    Eigen::Matrix3d ConvertToMatrix(const Eigenvalues& inter);
    Eigen::Matrix3d ConvertToMatrix(const AxRhom& inter);
    Eigen::Matrix3d ConvertToMatrix(const SpanSkew& inter);

    Eigenvalues ConvertToEigenvalues(const energy& inter);
    Eigenvalues ConvertToEigenvalues(const Eigen::Matrix3d& inter);
    Eigenvalues ConvertToEigenvalues(const Eigenvalues& inter);
    Eigenvalues ConvertToEigenvalues(const AxRhom& inter);
    Eigenvalues ConvertToEigenvalues(const SpanSkew& inter);

    AxRhom ConvertToAxRhom(const energy& inter);
    AxRhom ConvertToAxRhom(const Eigen::Matrix3d& inter);
    AxRhom ConvertToAxRhom(const Eigenvalues& inter);
    AxRhom ConvertToAxRhom(const AxRhom& inter);
    AxRhom ConvertToAxRhom(const SpanSkew& inter);

    SpanSkew ConvertToSpanSkew(const energy& inter);
    SpanSkew ConvertToSpanSkew(const Eigen::Matrix3d& inter);
    SpanSkew ConvertToSpanSkew(const Eigenvalues& inter);
    SpanSkew ConvertToSpanSkew(const AxRhom& inter);
    SpanSkew ConvertToSpanSkew(const SpanSkew& inter);


    class Interaction : public SpinXMLBase<Interaction> {
    public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
        ///Enumeration of the storage conventions used by this interaction
        enum Storage {
            STORAGE_SCALAR,
            MATRIX,
            EIGENVALUES,
            AXRHOM,
            SPANSKEW
        };

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
            CUSTOM_QUADRATIC,
			
			TYPE_END
        };
		

        ///Construct from a scalar
        Interaction(energy inter             ,Type t,Spin* spin1, Spin* spin2=NULL)
            : mData(inter), mSpin1(NULL), mSpin2(NULL) {
			InitalSetType(t,spin1,spin2);
			Invariant();
        }
        ///Construct from a matrix
        Interaction(const Eigen::Matrix3d& inter    ,Type t,Spin* spin1, Spin* spin2=NULL)
            : mData(inter), mSpin1(NULL), mSpin2(NULL) {
			InitalSetType(t,spin1,spin2);
			Invariant();
        }
        ///Construct from a matrix
        Interaction(const Eigenvalues& inter ,Type t,Spin* spin1, Spin* spin2=NULL)
            : mData(inter), mSpin1(NULL), mSpin2(NULL) {
			InitalSetType(t,spin1,spin2);
			Invariant();
        }
        ///Construct from a matrix
        Interaction(const AxRhom& inter      ,Type t,Spin* spin1, Spin* spin2=NULL)
            : mData(inter), mSpin1(NULL), mSpin2(NULL) {
			InitalSetType(t,spin1,spin2);
			Invariant();
        }
        ///Construct from a matrix
        Interaction(const SpanSkew& inter    ,Type t,Spin* spin1, Spin* spin2=NULL)
            : mData(inter), mSpin1(NULL), mSpin2(NULL)  {
			InitalSetType(t,spin1,spin2);
			Invariant();
        }
	protected:
		void InitalSetType(Type t,Spin* spin1, Spin* spin2);
	public:
        ///Destructor
        ~Interaction();

        //TODO: This function should probably assert that it's a HFC, linear or quadratic
        void OnSpinDying(Spin*) {delete this;}
    
        ///Print the interaction to the strandard output in a human readable
        ///form.
        void Dump() const;



        ///Get a human readable name for a member of the enum Storage
        static const char* GetStorageName(Storage t);
        ///Get a human readable name for a member of the enum Type
        static const char* GetTypeName(Type t);
        ///Get a human readable name for a member of the enum Form
        static const char* GetFormName(Form t);


        ///Each Type implies a form (for example, HFC is bilinear)
        ///which can be retrieved via this function
        static Form GetFormFromType(Type st);

        ///Get the storage convention being used
        Storage GetStorage() const;
        ///Get the physical source of this interaction
        Type GetType() const;
        ///Get the algebrake form of the interaction
        Form GetForm() const;

        ///Set a flag indicating the physical source of this interaction.
        void SetType(Type st, Spin* spin1, Spin* spin2=NULL);
        ///Returns true if the physical source of this interaction is t. The
        ///members ST_NMR, ST_EPR and ST_ANY may be used here and will be
        ///interpreted coorectly. For example, if inter is of SubType
        ///ST_SCALAR then inter.IsSubType(ST_NMR) is true.
        bool IsType(Type t) const;
    
        ///If the scalar sorage convention being used then this function will set
        ///the value of its argument to the appropreate value. Otherwise the
        ///result is undefined.
        void GetScalar(energy* Scalar) const;
        ///If the Matrix sorage convention being used then this function will set
        ///the value of its argument to the appropreate value. Otherwise the
        ///result is undefined.
        void GetMatrix(Eigen::Matrix3d* OutMatrix) const;
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
        void SetMatrix(const Eigen::Matrix3d& InMatrix);
        ///Set the value of the interaction using the eigenvalue covention.
        void SetEigenvalues(energy XX,energy YY, energy ZZ, const Orientation& Orient);
        ///Set the value of the interaction using the axiality-rhombicity covention.
        void SetAxRhom(energy iso,energy ax, energy rh, const Orientation& Orient);
        ///Set the value of the interaction using the span-skew covention.
        void SetSpanSkew(energy iso,energy Span, double Skew, const Orientation& Orient);

        bool GetIsLinear() const;   
        bool GetIsBilinear() const; 
        bool GetIsQuadratic() const;

        ///This signal is emited whenever one of the spins this interaction
        ///referes to changes. The first argument is a pointer to the old
        ///spin
        sigc::signal<void,Interaction*,Spin*> sigRemoveSpin;

        void ToScalar();
        void ToMatrix();
        void ToEigenvalues();
        void ToAxRhom();
        void ToSpanSkew();

        energy      AsScalar() const;
        Eigen::Matrix3d    AsMatrix() const;
        Eigenvalues AsEigenvalues() const;
        AxRhom      AsAxRhom() const;
        SpanSkew    AsSpanSkew() const;


        Spin* GetSpin1() const {return mSpin1;}
        Spin* GetSpin2() const {return mSpin2;}
        ///If the interaction involves two spins and this function is
        ///given a pointer to one of those spins, return a pointer to
        ///the other spin. In all other cases return NULL
        Spin* GetOtherSpin(const Spin* spin) const {return (mSpin1==spin ? mSpin2 : 
                                                            (mSpin2==spin ? mSpin1 : NULL));}
		///Returns true if the interaction involves Spin spin
		bool GetHasSpin(const Spin* spin) const {return mSpin1 == spin || mSpin2 == spin;}
    private:
		void Invariant() const;
        void valid_or_throw() const;

		typedef boost::variant<energy,Eigen::Matrix3d,Eigenvalues,AxRhom,SpanSkew> var_t;
		var_t mData;
        Type mType;

        Spin* mSpin1;
        sigc::connection mDyingConnect1;
        Spin* mSpin2;
        sigc::connection mDyingConnect2;
    };

	extern std::vector<Interaction::Type> Types;
	extern std::vector<Interaction::Type> MonoTypes;
	extern std::vector<Interaction::Type> BinaryTypes;

};

//Private

struct __InterInit {
	__InterInit();
};

#endif
