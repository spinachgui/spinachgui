
#ifndef _unit_h
#define _unit_h

#include <iostream>
#include <string>
#include <vector>

/*************************************************************
Units API.



The general approch to units If a quantity is stored in a
double, then that quantity is in SI. Quanties stored in a ddouble or
dimenstion double can be stored as a user defined unit type. Assigning
or implicitly converting a ddouble to a double should result in an SI
quantity being assigned to the double (and visa versa with the custem
unit type being assigned to the ddouble)


*************************************************************/


template<int energy,int length>
class unit {
public:
    unit(const std::string& name,double toSIf) : mName(name) {
        mToSI=toSIf;
        mFromSI=1/mToSI;
    }
    const char* get_name_c() {
        return mName.c_str();
    }
    const std::string& get_name() {
        return mName;
    }
    operator const char* () {
        return mName.c_str();
    }
    double toSI(double nonSI) {
        return mToSI*nonSI;
    }
    double fromSI(double SI) {
        return mFromSI*SI;
    }
private:
    std::string mName;
    double mToSI;
    double mFromSI;
};


template<int energy,int length>
struct ddouble {
    typedef ddouble<energy,length> self_t;
    ///An uninitalised ddouble works just like an uninitalised double
    ddouble() {
    }
    ///Trying to construct a ddouble without specifying a unit doesn't
    ///work unless you explicitly say you know what you're doing.
    explicit ddouble(double _si) : si(_si) {
    }
    ///Construct a 
    ddouble(double quantity,const unit<energy,length>& u) 
        : si(u.toSI(quantity)){
    }
    ///Get the quantity in a particular unit example usage:
    ///double gap=energy_gap[MHz];
    double operator[](const unit<energy,length>& u) {
        return u.fromSI(si);
    }
    //The physical quatity in SI units
    double si;
};

///This template enables expressions like api_call(400*MHz); where
///otherwise api_call(energy(400,MHz)) would have been needed
template<int E,int L>
ddouble<E,L> operator*(double quantity,unit<E,L> u) {
    return ddouble<E,L>(quantity,u);
}

///This template enables expressions like api_call(MHz*400); where
///otherwise api_call(energy(400,MHz)) would have been needed
template<int E,int L>
ddouble<E,L> operator*(unit<E,L> u,double quantity) {
    return ddouble<E,L>(quantity,u);
}

///This template enables expressions like length x=2*len; 
template<int E,int L>
ddouble<E,L> operator*(double no_unit,ddouble<E,L> with_unit) {
    return ddouble<E,L>(with_unit.si*no_unit);
}

///This template enables expressions like length x=len*2; 
template<int E,int L>
ddouble<E,L> operator*(ddouble<E,L> with_unit,double no_unit) {
    return ddouble<E,L>(with_unit.si*no_unit);
}


template<int E1,int E2,int L1,int L2>
ddouble<E1+E2,L1+L2> operator*(ddouble<E1,L1> lhs,ddouble<E2,L2> rhs) {
    return ddouble<E1+E2,L1+L2>(lhs.si*rhs.si);
}

template<int E1,int E2,int L1,int L2>
ddouble<E1-E2,L1-L2> operator/(ddouble<E1,L1> lhs,ddouble<E2,L2> rhs) {
    return ddouble<E1-E2,L1-L2>(lhs.si/rhs.si);
}



typedef ddouble<1,0> energy;
typedef ddouble<0,1> length;
typedef ddouble<0,2> length2;

typedef unit<1,0> energy_unit;
typedef unit<0,1> length_unit;

const energy_unit Joule("Joule",1.0);

const length_unit metre("metre",1.0);
const length_unit milimetre("milimetre",1e-3);
const length_unit micrometre("micrometre",1e-6);
const length_unit nanometre("nanometre",1e-9);
const length_unit angstrom("Angstrom",1e-10);

#endif

