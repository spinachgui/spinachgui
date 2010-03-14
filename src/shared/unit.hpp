
#ifndef _unit_h
#define _unit_h

#include <iostream>
#include <string>
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


#define unit_template_dec1 int E1,int L1
#define unit_template_dec2 int E2,int L2

#define unit_template_inst1 E1,L1
#define unit_template_inst2 E2,L2

#define unit_template_combine(utc_op) E1 utc_op E2,L1 utc_op L2
#define unit_template_inst_with_metafunc1(metafuc) E1 metafuc,L1 metafuc

template<unit_template_dec1>
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
template<unit_template_dec1>
struct ddouble_helper {
    ddouble_helper(double* _si,const unit<unit_template_inst1>& _u) 
        : u(_u),si(_si) {
    }
    const ddouble_helper& setup(double* _si,const unit<unit_template_inst1>& _u) {
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
    unit<unit_template_inst1> u;
    double* si;
};

template<unit_template_dec1>
std::istream& operator >> (std::istream& in,const ddouble_helper<unit_template_inst1>& val) {
    double temp;
    in >> temp;
    (*val.si)=val.u.toSI(temp);
    return in;
}

template<unit_template_dec1>
std::ostream& operator << (std::ostream& out,const ddouble_helper<unit_template_inst1>& val) {
    out << val.u.fromSI(*(val.si));
    return out;
}

template<unit_template_dec1>
struct ddouble {
    typedef ddouble<unit_template_inst1> self_t;
    ///An uninitalised ddouble works just like an uninitalised double
    ddouble() {
    }
    ///Trying to construct a ddouble without specifying a unit doesn't
    ///work unless you explicitly say you know what you're doing.
    explicit ddouble(double _si) : si(_si) {
    }
    ///Construct a 
    ddouble(double quantity,const unit<unit_template_inst1>& u) 
        : si(u.toSI(quantity)){
    }
    //static ddouble_helper<unit_template_inst1> helper;
    ///Get the quantity in a particular unit example usage: double
    ///gap=energy_gap[MHz]; Not thread safe, I'm not sure how to
    ///achive thread safeness right now, and it probably doesn't
    ///matter.
    const ddouble_helper<unit_template_inst1> operator[](const unit<unit_template_inst1>& u) {
        return ddouble_helper<unit_template_inst1>(&si,u);
    }
    //The physical quatity in SI units
    double si;
};

///Special case of the unitless ddouble, which can be convered to and
///from a regular double implicityly
template<>
struct ddouble<0,0> {
    typedef ddouble<0,0> self_t;
    ///An uninitalised ddouble works just like an uninitalised double
    ddouble() {
    }
    //Allow implicit conversiont to double
    operator double() {
	return si;
    }
    ///Trying to construct a ddouble without specifying a unit doesn't
    ///work unless you explicitly say you know what you're doing.
    ddouble(double _si) : si(_si) {
    }
    ///Construct a 
    ddouble(double quantity,const unit<0,0>& u) 
        : si(u.toSI(quantity)){
    }
    //static ddouble_helper<0,0> helper;
    ///Get the quantity in a particular unit example usage: double
    ///gap=energy_gap[MHz]; Not thread safe, I'm not sure how to
    ///achive thread safeness right now, and it probably doesn't
    ///matter.
    const ddouble_helper<0,0> operator[](const unit<0,0>& u) {
        return ddouble_helper<0,0>(&si,u);
    }
    //The physical quatity in SI units
    double si;
};


///These templates enable basic expressions that do not change the
///unit (are closed in the unit)
#define declare_op_closed(op)						\
    template<unit_template_dec1>					\
    ddouble<unit_template_inst1> operator op(ddouble<unit_template_inst1> lhs, \
					     ddouble<unit_template_inst1> rhs) { \
	return ddouble<unit_template_inst1>(lhs.si op rhs.si);		\
    }
declare_op_closed(+);
declare_op_closed(+=);
declare_op_closed(-);
declare_op_closed(-=);



//Declare an operator that does not change the dimension
#define declare_op_open(op, meta_op)					\
    template<unit_template_dec1,unit_template_dec2>			\
    ddouble<unit_template_combine(meta_op)> operator op (ddouble<unit_template_inst1> lhs, \
							 ddouble<unit_template_inst2> rhs) { \
	return ddouble<unit_template_combine(meta_op)>(lhs.si op rhs.si); \
    }
declare_op_open(*,+);
declare_op_open(*=,+);
declare_op_open(/,-);
declare_op_open(/=,-);

//Delcare an operator the maps two ddoubles onto a bool
#define declare_predicate(op)						\
    template<unit_template_dec1>					\
    bool operator op (ddouble<unit_template_inst1> lhs,			\
		      ddouble<unit_template_inst1> rhs) {		\
	return (lhs.si op rhs.si);					\
    }

declare_predicate(>);
declare_predicate(<);

///This template enables expressions like sqrt(legnth * length)
template<unit_template_dec1>
ddouble<unit_template_inst_with_metafunc1(/2)> sqrt(ddouble<unit_template_inst1> lhs) {
    return ddouble<unit_template_inst_with_metafunc1(/2)>(std::sqrt(lhs.si));
}

///This template enables expressions like length x=2/len; 
#define declate_half_unitless_op(op)				\
    template<unit_template_dec1>					\
    ddouble<unit_template_inst1> operator op (double no_unit,		\
					      ddouble<unit_template_inst1> with_unit) { \
	return ddouble<unit_template_inst1>(no_unit op with_unit.si);	\
    }									\
    template<unit_template_dec1>					\
    ddouble<unit_template_inst1> operator op (ddouble<unit_template_inst1> with_unit, \
					      double no_unit) {		\
	return ddouble<unit_template_inst1>(with_unit.si op no_unit);	\
    }
declate_half_unitless_op(*);
declate_half_unitless_op(/);

///This template enables expressions like like api_call(400*MHz);
/// where otherwise api_call(energy(400,MHz)) would have been needed
#define declate_literal_helper(op)					\
    template<unit_template_dec1>					\
    ddouble<unit_template_inst1> operator op (double no_unit,		\
					      unit<unit_template_inst1> u) { \
	return ddouble<unit_template_inst1>(no_unit,u);			\
    }									\
    template<unit_template_dec1>					\
    ddouble<unit_template_inst1> operator op (unit<unit_template_inst1> u, \
					      double no_unit) {		\
	return ddouble<unit_template_inst1>(no_unit,u);			\
    }
declate_literal_helper(*);

///This template enables expressions like length x=-x; 
template<unit_template_dec1>
ddouble<unit_template_inst1> operator-(ddouble<unit_template_inst1> with_unit) {
    return ddouble<unit_template_inst1>(-with_unit);
}



typedef ddouble<1,0> energy;
typedef ddouble<0,1> length;
typedef ddouble<0,2> length2;
typedef ddouble<0,3> length3;
typedef ddouble<0,4> length4;
typedef ddouble<0,5> length5;

typedef unit<1,0> energy_unit;
typedef unit<0,1> length_unit;
typedef unit<0,0> no_unit;

const no_unit     unitless("Unitless",1.0);

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

