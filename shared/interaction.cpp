
#include <shared/interaction.hpp>
#include <Eigen/Eigenvalues>
#include <Eigen/Geometry>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <shared/basic_math.hpp>

#include <shared/panic.hpp>

using namespace SpinXML;
using namespace std;
using namespace boost;

energy SpinXML::ConvertToScalar(const energy&   I)    {return I;}
energy SpinXML::ConvertToScalar(const Matrix3d& I)    {return energy(I.trace()/3.0);}
energy SpinXML::ConvertToScalar(const Eigenvalues& I) {return energy((I.xx+I.yy+I.zz)/3.0);}
energy SpinXML::ConvertToScalar(const AxRhom& I)      {return energy(I.iso/3.0);}
energy SpinXML::ConvertToScalar(const SpanSkew& I)    {return energy(I.iso/3.0);}


Matrix3d SpinXML::ConvertToMatrix(const energy& I) {return MakeMatrix3d(I,0,0,
                                                                        0,I,0,
                                                                        0,0,I);}
Matrix3d SpinXML::ConvertToMatrix(const Matrix3d& I) {return I;}
Matrix3d SpinXML::ConvertToMatrix(const Eigenvalues& I) {
    //Undo the eigensystem decomposition
    Matrix3d in_eigen_frame(MakeMatrix3d(I.xx,0      ,0       ,
                                         0      ,I.yy,0       ,
                                         0      ,0      ,I.zz));
    Matrix3d rotMatrix=I.mOrient.GetAsMatrix();
    return rotMatrix*in_eigen_frame*rotMatrix.inverse();
}
Matrix3d SpinXML::ConvertToMatrix(const AxRhom& I)  {return ConvertToMatrix(ConvertToEigenvalues(I));}
Matrix3d SpinXML::ConvertToMatrix(const SpanSkew& I){return ConvertToMatrix(ConvertToEigenvalues(I));}

Eigenvalues SpinXML::ConvertToEigenvalues(const energy& I) {
    Orientation o(Quaterniond(1,0,0,0));
    return Eigenvalues(I,I,I,o);
}
Eigenvalues SpinXML::ConvertToEigenvalues(const Matrix3d& I) {
    EigenSolver<Matrix3d> solver(I, true); //true => compute eigenvectors
    Vector3d v1,v2,v3;//Eigenvectors

    double e1=real(solver.eigenvalues()[0]);
    double e2=real(solver.eigenvalues()[1]);
    double e3=real(solver.eigenvalues()[2]);
    return Eigenvalues(energy(e1),energy(e2),energy(e3),Orientation(solver.eigenvectors().real()));
}
Eigenvalues SpinXML::ConvertToEigenvalues(const Eigenvalues& I) {return I;}
Eigenvalues SpinXML::ConvertToEigenvalues(const AxRhom& I) {
    energy xx=I.iso - I.ax/6.0 + I.rh/2.0;
    energy yy=I.iso - I.ax/6.0 - I.rh/2.0;
    energy zz=I.iso + I.ax/3.0;
    return Eigenvalues(xx,yy,zz,I.mOrient);
}
Eigenvalues SpinXML::ConvertToEigenvalues(const SpanSkew& I) {
    energy xx=I.iso+I.span*I.skew/6.0-I.span/2.0;
    energy yy=I.iso-I.span*I.skew/3.0;
    energy zz=I.iso+I.span*I.skew/6.0+I.span/2.0;
    return Eigenvalues(xx,yy,zz,I.mOrient);
}

AxRhom SpinXML::ConvertToAxRhom(const energy& I) {
    Orientation o(Quaterniond(1,0,0,0));
    return AxRhom(I,0.0*Joules,0.0*Joules,o);
}
AxRhom SpinXML::ConvertToAxRhom(const Matrix3d& I) {return ConvertToAxRhom(ConvertToEigenvalues(I));}
AxRhom SpinXML::ConvertToAxRhom(const Eigenvalues& I) {
    energy xx=I.xx; energy yy=I.yy; energy zz=I.zz;
    //Sort so that xx <= yy <= zz
    energy tmp;
    if(zz < yy) tmp=zz; zz=yy; yy=tmp;
    if(zz < xx) tmp=zz; zz=xx; xx=tmp; //Now zz is definatly bigest
    if(yy < xx) tmp=xx; xx=yy; yy=tmp;

    energy iso = (xx+yy+zz)/3.0;
    energy ax=2*zz -  xx - yy;
    energy rh=xx - yy;
    return AxRhom(iso,ax,rh,I.mOrient);
}
AxRhom SpinXML::ConvertToAxRhom(const AxRhom& I) {return I;}
AxRhom SpinXML::ConvertToAxRhom(const SpanSkew& I) {return ConvertToAxRhom(ConvertToEigenvalues(I));}

SpanSkew SpinXML::ConvertToSpanSkew(const energy& I) {
    Orientation o(Quaterniond(1,0,0,0));
    return SpanSkew(I,0.0*Joules,0.0,o);
}
SpanSkew SpinXML::ConvertToSpanSkew(const Matrix3d& I) {return ConvertToSpanSkew(ConvertToEigenvalues(I));}
SpanSkew SpinXML::ConvertToSpanSkew(const Eigenvalues& I) {
    energy xx=I.xx; energy yy=I.yy; energy zz=I.zz;
    //Sort so that xx <= yy <= zz
    energy tmp;
    if(zz < yy) tmp=zz; zz=yy; yy=tmp;
    if(zz < xx) tmp=zz; zz=xx; xx=tmp; //Now zz is the bigest
    if(yy < xx) tmp=xx; xx=yy; yy=tmp;

    energy iso = (xx+yy+zz)/3.0;
    energy span=zz - xx;
    double skew=span == energy(0.0) && span == energy(-0.0) ? 0.5 : ((3.0/2.0)*((iso-yy)/span));
    return SpanSkew(iso,span,skew,I.mOrient);
}
SpanSkew SpinXML::ConvertToSpanSkew(const AxRhom& I) {return ConvertToSpanSkew(ConvertToEigenvalues(I));}
SpanSkew SpinXML::ConvertToSpanSkew(const SpanSkew& I) {return I;}



//==============================================================================//
// Interaction

Interaction::~Interaction() {
    cout << "Calling the destructor" << endl;
}

class thisVisitor : public static_visitor<> {
public:
    void operator()(const energy& dat) const {
        cout << "    Type=Scalar" <<  endl;
        cout << "    Value=" << dat * MHz << "MHz" << endl;
    }
    void operator()(const Matrix3d& dat) const {
        cout << "    Type=Matrix" <<  endl;
        cout << "    Value=" << endl;
    }
    void operator()(const Eigenvalues& dat) const {
        cout << "    Type=Eigenvalues" <<  endl;
        cout << "    Value=(XX="
             << dat.xx * MHz << "MHz ,YY="
             << dat.yy * MHz << "MHz ,ZZ="
             << dat.zz * MHz << "MHz)"
             << endl;
        cout << "    Orient=";
    }
    void operator()(const AxRhom& dat) const {
        cout << "    Type=Axiality Rhombicity" <<  endl;
        cout << "    Value=" << endl;
        cout << "    Orient=";
        cout << dat.mOrient.ToString() << endl;
    }
    void operator()(const SpanSkew& dat) const {
        cout << "    Type=Span-Skew" <<  endl;
        cout << "    Value=" << endl;
        cout << "    Orient=";
        cout << dat.mOrient.ToString() << endl;
    }
};
void Interaction::Dump() const {
    cout << "  Dumping an Interaction" << endl;
    apply_visitor(thisVisitor(),mData);
}

const char* Interaction::GetStorageName(Storage t) {
    switch(t) {
    case STORAGE_SCALAR:  return "Scalar";
    case MATRIX:          return "Matrix";
    case EIGENVALUES:     return "Eigenvalues";
    case AXRHOM:          return "Axiality-Rhombicty";
    case SPANSKEW:        return "Span-Skew";
    default:
        ostringstream stream;
        stream << "Unknow type submited to Interaction::GetStorageName() (t=";
        stream << t << ")" << endl;
        throw std::runtime_error(stream.str());
        return "Error in Interaction::GetStorageName()";
    };
};

const char* Interaction::GetFormName(Form t) {
    switch(t) {
    case LINEAR:      return "Linear";
    case BILINEAR:    return "Bilinear";
    case QUADRATIC:   return "Quadratic";
    default:
        ostringstream stream;
        stream << "Unknow type submited to Interaction::GetFormName() (t=";
        stream << t << ")" << endl;
        throw std::runtime_error(stream.str());
        return "Error in Interaction::GetFormName()";
    };
}


const char* Interaction::GetTypeName(Type t) {
    switch(t) {
    case ANY:         return "Any";
    case EPR:         return "EPR";
    case NMR:         return "NMR";
    case HFC:         return "HFC";
    case G_TENSER:    return "G Tensor";
    case ZFS:          return "Zero Field Splitting";
    case EXCHANGE:    return "Exchange";
    case QUADRUPOLAR: return "Quadrupolar";
    case DIPOLAR:     return "Dipolar";
    case SHIELDING:   return "Shielding";
    case SCALAR:      return "Scalar";
    case CUSTOM_LINEAR:     return "Custom (Linear)";
    case CUSTOM_BILINEAR:   return "Custom (Bilinear)";
    case CUSTOM_QUADRATIC:  return "Custom (Quadratic)";
    default:
        ostringstream stream;
        stream << "Unknow type submited to Interaction::GetTypeName() (st=";
        stream << t << ")" << endl;
        throw std::runtime_error(stream.str());
        return "Error in Interaction::GetTypeName()";
    }
}

Interaction::Storage Interaction::GetStorage() const {
    if(get<energy>(&mData)!=NULL) {
        return STORAGE_SCALAR;
    } else if(get<Matrix3d>(&mData)!=NULL) {
        return MATRIX;
    } else if(get<Eigenvalues>(&mData)!=NULL) {
        return EIGENVALUES;
    } else if(get<AxRhom>(&mData)!=NULL) {
        return AXRHOM;
    } else  {
        return SPANSKEW;
    }
}



void Interaction::GetScalar(energy* Scalar) const {
    *Scalar=get<energy>(mData);
}

void Interaction::GetMatrix(Matrix3d* OutMatrix) const {
    *OutMatrix=get<Matrix3d>(mData);
    return;
}

void Interaction::GetEigenvalues(energy* XX,energy* YY, energy* ZZ, Orientation* OrientOut) const {
    *XX=get<Eigenvalues>(mData).xx;
    *YY=get<Eigenvalues>(mData).yy;
    *ZZ=get<Eigenvalues>(mData).zz;
    *OrientOut=get<Eigenvalues>(mData).mOrient;
    return;
}

void Interaction::GetAxRhom(energy* iso,energy* ax, energy* rh, Orientation* OrientOut) const {
    *iso=get<AxRhom>(mData).iso;
    *ax= get<AxRhom>(mData).ax;
    *rh= get<AxRhom>(mData).rh;
    *OrientOut=get<AxRhom>(mData).mOrient;
    return;
}

void Interaction::GetSpanSkew(energy* iso,energy* Span, double* Skew, Orientation* OrientOut) const {
    *iso= get<SpanSkew> (mData).iso;
    *Span=get<SpanSkew> (mData).span;
    *Skew=get<SpanSkew> (mData).skew;
    *OrientOut=get<SpanSkew>(mData).mOrient;
    return;
}

void Interaction::SetScalar(energy Scalar) {
    mData=Scalar;
    sigChange();
    Invariant();
}

void Interaction::SetMatrix(const Matrix3d& Matrix) {
    mData=Matrix3d(Matrix);
        Invariant();
    sigChange();
}

void Interaction::SetEigenvalues(energy XX,energy YY, energy ZZ, const Orientation& Orient) {
    mData=Eigenvalues(XX,YY,ZZ,Orient);
    Invariant();
    sigChange();
}

void Interaction::SetAxRhom(energy iso,energy ax, energy rh, const Orientation& Orient) {
    mData=AxRhom(iso,ax,rh,Orient);
    Invariant();
    sigChange();
}

void Interaction::SetSpanSkew(energy iso,energy Span, double Skew, const Orientation& Orient) {
    sigChange();
    mData=SpanSkew(iso,Span,Skew,Orient);
    Invariant();
}


Interaction::Type Interaction::GetType() const {
    return mType;
}

Interaction::Form Interaction::GetForm() const {
    if(GetIsLinear())
        return LINEAR;
    if(GetIsBilinear())
        return BILINEAR;
    if(GetIsQuadratic())
        return QUADRATIC;
    throw runtime_error("GetForm failed");
}

void Interaction::InitalSetType(Type t,Spin* spin1,Spin* spin2) {
    mType=t;
    mSpin1 = spin1;
    mSpin2 = spin2;
    if(mSpin1) {
        mDyingConnect1=mSpin1->sigDying.connect(mem_fun(this,&Interaction::OnSpinDying));
    }
    if(mSpin2) {
        mDyingConnect2=mSpin2->sigDying.connect(mem_fun(this,&Interaction::OnSpinDying));
    }
    Invariant();
}

void Interaction::SetType(Type t,Spin* spin1,Spin* spin2) {
    mType=t;
    //true if mSpin1 is not mentioned and thus will not be kept.
    bool loseSpin1=mSpin1!=spin1 && mSpin1!=spin2;
    //ture if mSpin2 is not mentioned and thus will not be kept.
    bool loseSpin2=mSpin2!=spin1 && mSpin2!=spin2;

    if((loseSpin1 && loseSpin2) || (mSpin1==mSpin2)) {
        //Easyest case, we're replacing both spins
        sigRemoveSpin(this,mSpin1);
        mDyingConnect1.disconnect();

        sigRemoveSpin(this,mSpin2);
        mDyingConnect2.disconnect();

        mSpin1=spin1;
        if(spin1) {
            mDyingConnect1=mSpin1->sigDying.connect(mem_fun(this,&Interaction::OnSpinDying));
        }
        mSpin2=spin2;
        if(spin2){
            mDyingConnect2=mSpin2->sigDying.connect(mem_fun(this,&Interaction::OnSpinDying));
        }
    } else  if(loseSpin1) {
        sigRemoveSpin(this,mSpin1);
        mDyingConnect1.disconnect();

        //We're keeping mSpin2. But is it spin1 or spin2 that replaces mSpin1
        if(spin1==mSpin2) {
            //Replace with spin2
            mSpin1=spin2;
        } else {
            //Replace with spin1
            mSpin1=spin1;
        }
        if(mSpin1!=NULL){
            mDyingConnect1=mSpin1->sigDying.connect(mem_fun(this,&Interaction::OnSpinDying));
        }
    } else if (loseSpin2) {
        //We're keeping mSpin1. But is it spin1 or spin2 that replaces mSpin2
        sigRemoveSpin(this,mSpin2);
        mDyingConnect1.disconnect();

        if(spin1==mSpin1) {
            //Replace with spin2
            mSpin2=spin2;
        } else {
            //Replace with spin1
            mSpin2=spin1;
        }
        if(mSpin2!=NULL) {
            mDyingConnect2=mSpin2->sigDying.connect(mem_fun(this,&Interaction::OnSpinDying));
        }
    }
    sigChange();
}

Interaction::Form Interaction::GetFormFromType(Type st) {
    switch(st) {
    case CUSTOM_LINEAR:
    case SHIELDING:
    case G_TENSER:
        return LINEAR;

    case CUSTOM_BILINEAR:
    case SCALAR:
    case DIPOLAR:
    case HFC:
    case EXCHANGE:
        return BILINEAR;

    case ZFS:
    case QUADRUPOLAR:
    case CUSTOM_QUADRATIC:
        return QUADRATIC;
    case ANY:
    case NMR:
    case EPR:
        throw logic_error("Can't pass ST_ANY,ST_NMR,ST_EPR to GetFromFromType");
    default:
        throw logic_error("Unknown type in GetFromFromType");
    }

}

bool Interaction::IsType(Type t) const {
    if(t==ANY) {
        return true;
    }
    if(t==EPR) {
        switch(mType) {
        case HFC:
        case G_TENSER:
        case ZFS:
        case EXCHANGE:
        case QUADRUPOLAR:
        case DIPOLAR:
        case CUSTOM_LINEAR:
        case CUSTOM_BILINEAR:
        case CUSTOM_QUADRATIC:
            return true;
        default:
            return false;
        }
    }
    if(t==NMR) {
        switch(mType) {
        case SHIELDING:
        case SCALAR:
        case QUADRUPOLAR:
        case DIPOLAR:
        case CUSTOM_LINEAR:
        case CUSTOM_BILINEAR:
        case CUSTOM_QUADRATIC:
            return true;
        default:
            return false;
        }
    }
    if(t==mType) {
        return true;
    }
    return false;
}


bool Interaction::GetIsLinear() const {
    return
        mType==EXCHANGE    ||
        mType==SHIELDING   ||
        mType==G_TENSER    ||
        mType==CUSTOM_LINEAR;
}

bool Interaction::GetIsBilinear() const {
    return
        mType==HFC            ||
        mType==DIPOLAR        ||
        mType==EXCHANGE       ||
        mType==SCALAR         ||
        mType==CUSTOM_BILINEAR;

}
bool Interaction::GetIsQuadratic() const {
    return
        mType==ZFS             ||
        mType==QUADRUPOLAR     ||
        mType==CUSTOM_QUADRATIC;

}


#define DEFINE_CONVERTER(name,return_type,function)                     \
    struct name : public static_visitor<return_type> {                  \
        return_type operator()(const energy& dat)      const {return function(dat);} \
        return_type operator()(const Matrix3d& dat)    const {return function(dat);} \
        return_type operator()(const Eigenvalues& dat) const {return function(dat);} \
        return_type operator()(const AxRhom& dat)      const {return function(dat);} \
        return_type operator()(const SpanSkew& dat)    const {return function(dat);} \
    };

DEFINE_CONVERTER(getAsScalarVisitor,energy,ConvertToScalar);
void Interaction::ToScalar() {
    mData=apply_visitor(getAsScalarVisitor(),mData);
    Invariant();
}
energy Interaction::AsScalar() const {
    return apply_visitor(getAsScalarVisitor(),mData);
}

DEFINE_CONVERTER(getAsMatrixVisitor,Matrix3d,ConvertToMatrix);
void Interaction::ToMatrix() {
    mData=apply_visitor(getAsMatrixVisitor(),mData);
    Invariant();
}
Matrix3d Interaction::AsMatrix() const {
    return apply_visitor(getAsMatrixVisitor(),mData);
}

DEFINE_CONVERTER(getAsEigenvaluesVisitor,Eigenvalues,ConvertToEigenvalues);
void Interaction::ToEigenvalues() {
    mData=apply_visitor(getAsEigenvaluesVisitor(),mData);
    Invariant();
}
Eigenvalues Interaction::AsEigenvalues() const {
    return apply_visitor(getAsEigenvaluesVisitor(),mData);
}

DEFINE_CONVERTER(getAsAxRhomVisitor,AxRhom,ConvertToAxRhom);
void Interaction::ToAxRhom() {
    cout << "ToAxRhom" << endl;
    mData=apply_visitor(getAsAxRhomVisitor(),mData);
    Invariant();
}
AxRhom Interaction::AsAxRhom() const {
    cout << "AsAxRhom" << endl;
    return apply_visitor(getAsAxRhomVisitor(),mData);
}

DEFINE_CONVERTER(getAsSpanSkewVisitor,SpanSkew,ConvertToSpanSkew);
void Interaction::ToSpanSkew() {
    mData=apply_visitor(getAsSpanSkewVisitor(),mData);
    Invariant();
}
SpanSkew Interaction::AsSpanSkew() const {
    return apply_visitor(getAsSpanSkewVisitor(),mData);
}

void Interaction::valid_or_throw() const {
    switch(mType) {
    case ANY:
    case NMR:
    case EPR:
        throw runtime_error("Invalid type in Interaction");
        break;
    case SCALAR:
    case EXCHANGE:
    case HFC:
    case CUSTOM_BILINEAR:
    case DIPOLAR:
        if(mSpin1 == NULL || mSpin2 == NULL)
            throw runtime_error("Bilinar Interactions but one of mSpin1 or mSpin2 is null!");
        break;
    case CUSTOM_LINEAR:
    case SHIELDING:
    case G_TENSER:
    case ZFS:
    case QUADRUPOLAR:
    case CUSTOM_QUADRATIC:
        if(mSpin1 != NULL && mSpin2 != NULL)
            throw runtime_error("Linear or quadratic interaction but both mSpin1 and mSpin2 are not null");
        break;
    default:
        throw runtime_error("Unknown, invalid type in Interaction");
    }
}

class InvariantVisitor : public static_visitor<> {
public:
    void operator()(const energy& dat) const {
        NaNPANIC(dat,"Interaction(Energy) is NaN");
    }
    void operator()(const Matrix3d& dat) const {
        NaNPANIC(dat,"Interaction(Matrix3d) has a NaN");
    }
    void operator()(const Eigenvalues& dat) const {
        NaNPANIC(dat.xx,"Interaction(Eigenvalues).xx has a NaN");
        NaNPANIC(dat.yy,"Interaction(Eigenvalues).yy has a NaN");
        NaNPANIC(dat.zz,"Interaction(Eigenvalues).zz has a NaN");
    }
    void operator()(const AxRhom& dat) const {
        NaNPANIC(dat.ax ,"Interaction(AxRhom).ax  has a NaN");
        NaNPANIC(dat.rh ,"Interaction(AxRhom).rh  has a NaN");
        NaNPANIC(dat.iso,"Interaction(AxRhom).iso has a NaN");
    }
    void operator()(const SpanSkew& dat) const {
        NaNPANIC(dat.span ,"Interaction(SpanSkew).span  has a NaN");
        NaNPANIC(dat.skew ,"Interaction(SpanSkew).skew  has a NaN");
        NaNPANIC(dat.iso,  "Interaction(SpanSkew).iso   has a NaN");
    }
};
void Interaction::Invariant() const {
    //Check the data for NaNs
    apply_visitor(InvariantVisitor(),mData);
    //Make sure the type makes sense
    switch(mType) {
    case ANY:
    case NMR:
    case EPR:
        PANIC("Interaction has type ANY,NMR or EPR");
        break;
    case SCALAR:
    case EXCHANGE:
    case HFC:
    case CUSTOM_BILINEAR:
    case DIPOLAR:
        if(mSpin1 == NULL || mSpin2 == NULL)
            PANIC("Bilinar Interactions but one of mSpin1 or mSpin2 is null!");
        break;
    case CUSTOM_LINEAR:
    case SHIELDING:
    case G_TENSER:
    case ZFS:
    case QUADRUPOLAR:
    case CUSTOM_QUADRATIC:
        if(mSpin1 != NULL && mSpin2 != NULL)
            PANIC("Linear or quadratic interaction but both mSpin1 and mSpin2 are not null");
        break;
    default:
        PANIC("Unknown, invalid type in Interaction");
    }
}


std::vector<Interaction::Type> SpinXML::Types;
std::vector<Interaction::Type> SpinXML::MonoTypes;
std::vector<Interaction::Type> SpinXML::BinaryTypes;

__InterInit::__InterInit() {
    cout << "Init interaction" << endl;
    Types.push_back(Interaction::HFC);
    Types.push_back(Interaction::G_TENSER);
    Types.push_back(Interaction::ZFS);
    Types.push_back(Interaction::EXCHANGE);
    Types.push_back(Interaction::SHIELDING);
    Types.push_back(Interaction::SCALAR);
    Types.push_back(Interaction::QUADRUPOLAR);
    Types.push_back(Interaction::DIPOLAR);
    Types.push_back(Interaction::CUSTOM_LINEAR);
    Types.push_back(Interaction::CUSTOM_BILINEAR);
    Types.push_back(Interaction::CUSTOM_QUADRATIC);

    MonoTypes.push_back(Interaction::G_TENSER);
    MonoTypes.push_back(Interaction::ZFS);
    MonoTypes.push_back(Interaction::SHIELDING);
    MonoTypes.push_back(Interaction::QUADRUPOLAR);
    MonoTypes.push_back(Interaction::CUSTOM_LINEAR);
    MonoTypes.push_back(Interaction::CUSTOM_QUADRATIC);

    BinaryTypes.push_back(Interaction::HFC);
    BinaryTypes.push_back(Interaction::EXCHANGE);
    BinaryTypes.push_back(Interaction::SCALAR);
    BinaryTypes.push_back(Interaction::DIPOLAR);
    BinaryTypes.push_back(Interaction::CUSTOM_BILINEAR);
}

static __InterInit __initInit;
