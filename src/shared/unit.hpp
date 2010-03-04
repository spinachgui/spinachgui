
#ifndef _unit_h
#define _unit_h

#include <iostream>
#include <istream>
#include <string>
#include <vector>
#include <cmath>

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
    const char* get_name_c() const {
        return mName.c_str();
    }
    const std::string& get_name() const {
        return mName;
    }
    operator const char* () {
        return mName.c_str();
    }
    double toSI(double nonSI) const {
        return mToSI*nonSI;
    }
    double fromSI(double SI) const {
        return mFromSI*SI;
    }
private:
    std::string mName;
    double mToSI;
    double mFromSI;
};

///This template is used to make cin >> x[metres]; y[MHz]=400; type
/// syntax work, it shouldn't be used explicitly
template<int energy,int length>
struct ddouble_helper {
    ddouble_helper(double* _si,const unit<energy,length>& _u) 
        : u(_u),si(_si) {
    }
    const ddouble_helper& setup(double* _si,const unit<energy,length>& _u) {
        si=_si;
        u=_u;
        return *this;
    }
    operator double() const {
        return u.fromSI(*si);
    }

    double operator = (double val) const {
        *si=u.toSI(val);
        return val;
    }
    unit<energy,length> u;
    double* si;
};

template<int energy,int length>
std::istream& operator >> (std::istream& in,const ddouble_helper<energy,length>& val) {
    double temp;
    in >> temp;
    (*val.si)=val.u.toSI(temp);
    return in;
}

template<int energy,int length>
std::ostream& operator << (std::ostream& out,const ddouble_helper<energy,length>& val) {
    out << val.u.fromSI(*(val.si));
    return out;
}

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
    //static ddouble_helper<energy,length> helper;
    ///Get the quantity in a particular unit example usage: double
    ///gap=energy_gap[MHz]; Not thread safe, I'm not sure how to
    ///achive thread safeness right now, and it probably doesn't
    ///matter.
    const ddouble_helper<energy,length> operator[](const unit<energy,length>& u) {
        return ddouble_helper<energy,length>(&si,u);
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

///This template enables expressions like length x=2/len; 
template<int E,int L>
ddouble<E,L> operator/(double no_unit,ddouble<E,L> with_unit) {
    return ddouble<E,L>(with_unit.si/no_unit);
}

///This template enables expressions like length x=len*2; 
template<int E,int L>
ddouble<E,L> operator*(ddouble<E,L> with_unit,double no_unit) {
    return ddouble<E,L>(with_unit.si*no_unit);
}

///This template enables expressions like length x=len/2; 
template<int E,int L>
ddouble<E,L> operator/(ddouble<E,L> with_unit,double no_unit) {
    return ddouble<E,L>(with_unit.si/no_unit);
}


template<int E1,int E2,int L1,int L2>
ddouble<E1+E2,L1+L2> operator*(ddouble<E1,L1> lhs,ddouble<E2,L2> rhs) {
    return ddouble<E1+E2,L1+L2>(lhs.si*rhs.si);
}

template<int E1,int E2,int L1,int L2>
ddouble<E1-E2,L1-L2> operator/(ddouble<E1,L1> lhs,ddouble<E2,L2> rhs) {
    return ddouble<E1-E2,L1-L2>(lhs.si/rhs.si);
}

///This template enables expressions like x=length1+length2
template<int E,int L>
ddouble<E,L> operator+(ddouble<E,L> lhs,ddouble<E,L> rhs) {
    return ddouble<E,L>(lhs.si+rhs.si);
}

///This template enables expressions like x=length1+length2
template<int E,int L>
ddouble<E,L> operator-(const ddouble<E,L>& lhs,const ddouble<E,L>& rhs) {
    return ddouble<E,L>(lhs.si-rhs.si);
}

///This template enables expressions like length1 > length2
template<int E,int L>
bool operator>(const ddouble<E,L>& lhs,const ddouble<E,L>& rhs) {
    return lhs.si>rhs.si;
}

///This template enables expressions like length1 < length2
template<int E,int L>
bool operator<(ddouble<E,L> lhs,ddouble<E,L> rhs) {
    return lhs.si<rhs.si;
}

///This template enables expressions like sqrt(legnth * length)
template<int E,int L>
ddouble<E/2,L/2> sqrt(ddouble<E,L> lhs) {
    return ddouble<E/2,L/2>(std::sqrt(lhs.si));
}




typedef ddouble<1,0> energy;
typedef ddouble<0,1> length;
typedef ddouble<0,2> length2;
typedef ddouble<0,3> length3;
typedef ddouble<0,4> length4;
typedef ddouble<0,5> length5;

typedef unit<1,0> energy_unit;
typedef unit<0,1> length_unit;

const energy_unit Joules("Joule",1.0);
const energy_unit Hz("Hz",6.626068e-34);
const energy_unit KHz("KHz",6.626068e-31);
const energy_unit MHz("MHz",6.626068e-28);
const energy_unit eV("eV",1.60217646e-19);

const length_unit metres("metres",1.0);
const length_unit milimetre("milimetre",1e-3);
const length_unit micrometre("micrometre",1e-6);
const length_unit nanometre("nanometre",1e-9);
const length_unit Angstroms("Angstroms",1e-10);

#endif

