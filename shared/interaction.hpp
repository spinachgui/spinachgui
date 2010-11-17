
#ifndef INTERACTION_H
#define INTERACTION_H

#include <sigc++/sigc++.h>
#include <shared/unit.hpp>
#include <boost/variant.hpp>

#include <shared/spin.hpp>
#include <shared/orientation.hpp>


namespace SpinXML {
	class InteractionView;

    ///============================================================//
	template<typename T>
    struct Eigenvalues_T {
        Eigenvalues_T(const T _XX,const T _YY,const T _ZZ, const Orientation& o) 
            : xx(_XX), yy(_YY), zz(_ZZ), mOrient(o) {
        }
        T xx;
        T yy;
        T zz;
        Orientation mOrient;
    };
	typedef Eigenvalues_T<energy> Eigenvalues;
	typedef Eigenvalues_T<double> UEigenvalues;

    ///============================================================//
	template<typename T>
    struct AxRhom_T {
        AxRhom_T(const T _iso,const T _ax,const T _rh, const Orientation& o) 
            : iso(_iso), ax(_ax), rh(_rh), mOrient(o) {
        }
        T iso;
        T ax;
        T rh;
        Orientation mOrient;
    };
	typedef AxRhom_T<energy> AxRhom;
	typedef AxRhom_T<double> UAxRhom;
	

    ///============================================================//
	template<typename T>
    struct SpanSkew_T {
        SpanSkew_T(const T _iso,const T _span,const double _skew, const Orientation& o) 
            : iso(_iso), span(_span), skew(_skew), mOrient(o) {
        }
        T iso;
        T span;
        double skew;
        Orientation mOrient;
    };
	typedef SpanSkew_T<energy> SpanSkew;
	typedef SpanSkew_T<double> USpanSkew;

    //Interaction converstions
    energy ConvertToScalar(const energy& inter);
    energy ConvertToScalar(const Matrix3d& inter);
    energy ConvertToScalar(const Eigenvalues& inter);
    energy ConvertToScalar(const AxRhom& inter);
    energy ConvertToScalar(const SpanSkew& inter);

    Matrix3d ConvertToMatrix(const energy& inter);
    Matrix3d ConvertToMatrix(const Matrix3d& inter);
    Matrix3d ConvertToMatrix(const Eigenvalues& inter);
    Matrix3d ConvertToMatrix(const AxRhom& inter);
    Matrix3d ConvertToMatrix(const SpanSkew& inter);

    Eigenvalues ConvertToEigenvalues(const energy& inter);
    Eigenvalues ConvertToEigenvalues(const Matrix3d& inter);
    Eigenvalues ConvertToEigenvalues(const Eigenvalues& inter);
    Eigenvalues ConvertToEigenvalues(const AxRhom& inter);
    Eigenvalues ConvertToEigenvalues(const SpanSkew& inter);

    AxRhom ConvertToAxRhom(const energy& inter);
    AxRhom ConvertToAxRhom(const Matrix3d& inter);
    AxRhom ConvertToAxRhom(const Eigenvalues& inter);
    AxRhom ConvertToAxRhom(const AxRhom& inter);
    AxRhom ConvertToAxRhom(const SpanSkew& inter);

    SpanSkew ConvertToSpanSkew(const energy& inter);
    SpanSkew ConvertToSpanSkew(const Matrix3d& inter);
    SpanSkew ConvertToSpanSkew(const Eigenvalues& inter);
    SpanSkew ConvertToSpanSkew(const AxRhom& inter);
    SpanSkew ConvertToSpanSkew(const SpanSkew& inter);


    class Interaction : public SpinXMLBase<Interaction,InteractionView> {
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
            CUSTOM_QUADRATIC
        };

        ///Construct from a scalar
        Interaction(energy inter             ,Type t,Spin* spin1, Spin* spin2=NULL)
            : mData(inter),mType(t),mSpin1(spin1),mSpin2(spin2){
            valid_or_throw();
        }
        ///Construct from a matrix
        Interaction(const Matrix3d& inter    ,Type t,Spin* spin1, Spin* spin2=NULL)
            : mData(inter),mType(t),mSpin1(spin1),mSpin2(spin2){
            valid_or_throw();
        }
        ///Construct from a matrix
        Interaction(const Eigenvalues& inter ,Type t,Spin* spin1, Spin* spin2=NULL)
            : mData(inter),mType(t),mSpin1(spin1),mSpin2(spin2){
            valid_or_throw();
        }
        ///Construct from a matrix
        Interaction(const AxRhom& inter      ,Type t,Spin* spin1, Spin* spin2=NULL)
            : mData(inter),mType(t),mSpin1(spin1),mSpin2(spin2){
            valid_or_throw();
        }
        ///Construct from a matrix
        Interaction(const SpanSkew& inter    ,Type t,Spin* spin1, Spin* spin2=NULL)
            : mData(inter),mType(t),mSpin1(spin1),mSpin2(spin2){
            valid_or_throw();
        }
        ///Copy constructor
        Interaction(const Interaction& inter);
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
        void GetMatrix(Matrix3d* OutMatrix) const;
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
        void SetMatrix(const Matrix3d& InMatrix);
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
        Matrix3d    AsMatrix() const;
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

        void valid_or_throw() const;

		typedef boost::variant<energy,Matrix3d,Eigenvalues,AxRhom,SpanSkew> var_t;
		var_t mData;
        Type mType;

        Spin* mSpin1;
        sigc::connection mDyingConnect1;
        Spin* mSpin2;
        sigc::connection mDyingConnect2;
    };





    class InteractionView : public ViewBase<InteractionView,Interaction> {
    public:
        typedef ViewBase<InteractionView,Interaction> Base;

        ///Construct from a scalar
        InteractionView(Interaction* inter   ,const Frame* frame, const UnitSystem* unitSystem)
			: Base(inter,frame,unitSystem) {
			mData->sigRemoveSpin.connect(sigRemoveSpin);
        }

        ///Get the storage convention being used
        Interaction::Storage GetStorage() const                   {return mData->GetStorage();}
        ///Get the physical source of this interaction
        Interaction::Type GetType() const                         {return mData->GetType();}
        ///Get the algebrake form of the interaction
        Interaction::Form GetForm() const                         {return mData->GetForm();}

        ///Set a flag indicating the physical source of this interaction.
        void SetType(Interaction::Type st, Spin* spin1, Spin* spin2=NULL) {return mData->SetType(st,spin1,spin2);}
        ///Returns true if the physical source of this interaction is t. The
        ///members ST_NMR, ST_EPR and ST_ANY may be used here and will be
        ///interpreted coorectly. For example, if inter is of SubType
        ///ST_SCALAR then inter.IsSubType(ST_NMR) is true.
        bool IsType(Interaction::Type t) const {return mData->IsType(t);}
    
        ///For a bilinear interaction, get the spin that is not spinA
        SpinView GetOtherSpin(SpinView spinA) {return mData->GetOtherSpin(spinA.Get())->GetView(mFrame,mUnitSystem);}

        ///Set the value of the interaction using the scalar covention.
        void SetScalar(double Scalar);
        ///Set the value of the interaction using the matrix covention.
        void SetMatrix(const Matrix3d& InMatrix);
        ///Set the value of the interaction using the eigenvalue covention.
        void SetEigenvalues(double XX,double YY,   double ZZ,  const Orientation& Orient);
        ///Set the value of the interaction using the axiality-rhombicity covention.
        void SetAxRhom(double iso,    double ax,   double rh,  const Orientation& Orient);
        ///Set the value of the interaction using the span-skew covention.
        void SetSpanSkew(double iso,  double Span, double Skew,const Orientation& Orient);

        bool GetIsLinear() const    {return mData->GetIsLinear();}
        bool GetIsBilinear() const  {return mData->GetIsBilinear();}
        bool GetIsQuadratic() const {return mData->GetIsQuadratic();}

        void ToScalar()      {mData->ToScalar();}
        void ToMatrix()      {mData->ToMatrix();}
        void ToEigenvalues() {mData->ToEigenvalues();}
        void ToAxRhom()      {mData->ToAxRhom();}
        void ToSpanSkew()    {mData->ToSpanSkew();}

        double       AsScalar() const;
        Matrix3d     AsMatrix() const;
        UEigenvalues AsEigenvalues() const;
        UAxRhom      AsAxRhom() const;
        USpanSkew    AsSpanSkew() const;


        SpinView GetSpin1() const {return mData->GetSpin1()->GetView(mFrame,mUnitSystem);}
        SpinView GetSpin2() const {return mData->GetSpin2()->GetView(mFrame,mUnitSystem);}
        ///If the interaction involves two spins and this function is
        ///given a pointer to one of those spins, return a pointer to
        ///the other spin. In all other cases return NULL
        SpinView GetOtherSpin(const SpinView spin) const {return mData->GetOtherSpin(spin.Get())->GetView(mFrame,mUnitSystem);}

        sigc::signal<void,Interaction*,Spin*> sigRemoveSpin;
    };

};




#endif
