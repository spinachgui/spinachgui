
#ifndef _unit_h
#define _unit_h


/*************************************************************
Units API.



The general approch to units If a quantity is stored in a
double, then that quantity is in SI. Quanties stored in a ddouble or
dimenstion double can be stored as a user defined unit type. Assigning
or implicitly converting a ddouble to a double should result in an SI
quantity being assigned to the double (and visa versa with the custem
unit type being assigned to the ddouble)


*************************************************************/


#include <iostream>
#include <string>
#include <cmath>
#include <boost/static_assert.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/equal.hpp>

#include <boost/mpl/vector_c.hpp>

using namespace boost;


struct plus_f {
    template <class T1, class T2>
    struct apply : mpl::plus<T1,T2> {};
};
struct minus_f {
    template <class T1, class T2>
    struct apply : mpl::minus<T1,T2> {};
};


//Fundimental dimensions
typedef mpl::vector_c<int,0,0,0,0,0,0,0>  _unitless;
typedef mpl::vector_c<int,1,0,0,0,0,0,0>  _mass;
typedef mpl::vector_c<int,0,1,0,0,0,0,0>  _length;
typedef mpl::vector_c<int,0,0,1,0,0,0,0>  _time;
typedef mpl::vector_c<int,0,0,0,1,0,0,0>  _current;
typedef mpl::vector_c<int,0,0,0,0,1,0,0>  _temp;
typedef mpl::vector_c<int,0,0,0,0,0,1,0>  _lum_intensity;
typedef mpl::vector_c<int,0,0,0,0,0,0,1>  _mole;

//Derived dimensions
typedef mpl::vector_c<int,0,1,-1,0,0,0,0>  _speed;
typedef mpl::vector_c<int,0,1,-2,0,0,0,0>  _acceleration;
typedef mpl::vector_c<int,1,1,-2,0,0,0,0>  _force;
typedef mpl::vector_c<int,1,2,-2,0,0,0,0>  _energy;
typedef mpl::vector_c<int,0,0,-1,0,0,0,0>  _frequency;

typedef mpl::transform<_length, _length,plus_f>::type _length2;
typedef mpl::transform<_length2,_length,plus_f>::type _length3;
typedef mpl::transform<_length3,_length,plus_f>::type _length4;
typedef mpl::transform<_length4,_length,plus_f>::type _length5;


template <class D>
struct unit {
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
	inline double toSI(double nonSI) const {
	    return mToSI*nonSI;
	}
	inline double fromSI(double SI) const {
	    return mFromSI*SI;
	}
    private:
	std::string mName;
	double mToSI;
	double mFromSI;
};

//============================================================//
// dreal_helper

///This template is used to make cin >> x[metres]; y[MHz]=400; type
/// syntax work, it shouldn't be used explicitly
template<typename T,typename D>
struct dreal_helper {
    dreal_helper(T* _si,const unit<D>& _u) 
	: u(_u),si(_si) {
    }
    const dreal_helper& setup(double* _si,const unit<D>& _u) {
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
    unit<D> u;
    double* si;
};

template<typename T,typename D>
std::istream& operator >> (std::istream& in,const dreal_helper<T,D>& val) {
    double temp;
    in >> temp;
    (*val.si)=val.u.toSI(temp);
    return in;
}

template<typename T,typename D>
std::ostream& operator << (std::ostream& out,const dreal_helper<T,D>& val) {
    out << val.u.fromSI(*(val.si));
    return out;
}


//============================================================//
// dreal 
// definition and specialisation


template <class T,class D>
struct dreal {
    dreal() {}

    template <class OtherDimensions>
    dreal(dreal<T,OtherDimensions> const& rhs)
	: si(rhs.si) {
	BOOST_STATIC_ASSERT((mpl::equal<D,OtherDimensions>::type::value));
    }

    explicit dreal(T _si)
	: si(_si) {
    }
    const dreal_helper<T,D> operator[](const unit<D>& u) {
	return dreal_helper<T,D>(&si,u);
    }

    T si;
};

//Allow explicit conversion for unitless numbers
template <>
struct dreal<double,_unitless> {
    dreal() {}
    template <class OtherDimensions>
    dreal(dreal<double,OtherDimensions> const& rhs)
	: si(rhs.si) {
	BOOST_STATIC_ASSERT((mpl::equal<_unitless,OtherDimensions>::type::value));
    }

    dreal(double val)
	: si(val) {
    }
    operator double() {
	return si;
    }
    double si;
};


template <class T, class D>
dreal<T,D>
operator+(dreal<T,D> x, dreal<T,D> y) {
  return dreal<T,D>(x.si + y.si);
}

template <class T, class D>
dreal<T,D>
operator-(dreal<T,D> x, dreal<T,D> y) {
  return dreal<T,D>(x.si - y.si);
}

template <class T, class D1, class D2>
dreal<T,typename mpl::transform<D1,D2,plus_f>::type>
operator*(dreal<T,D1> x, dreal<T,D2> y) {
    typedef typename mpl::transform<D1,D2,plus_f>::type dim;
    return dreal<T,dim>(x.si * y.si);
}

template <class T, class D1, class D2>
dreal<T,typename mpl::transform<D1,D2,minus_f>::type>
operator/(dreal<T,D1> x, dreal<T,D2> y) {
    typedef typename mpl::transform<D1,D2,minus_f>::type dim;
    return dreal<T,dim>(x.si / y.si);
}



//============================================================//
// Opereations on units

///Alow expresions like length x=5.0/metres
template <class T, class D>
dreal<T,D>
operator/(T real, unit<D> u) {
    return dreal<T,D>(u.toSI(real));
}

///Alow expresions like area a=2/(metres * metres)
template <class D1, class D2>
unit<typename mpl::transform<D1,D2,plus_f> >
operator * (unit<D1> u1, unit<D2> u2) {
    return unit<typename mpl::transform<D1,D2,plus_f> >();
}

///Alow expresions like speed = 3*(metres/second)
template <class D1, class D2>
unit<typename mpl::transform<D1,D2,minus_f> >
operator / (unit<D1> u1, unit<D2> u2) {
    return unit<typename mpl::transform<D1,D2,minus_f> >();
}

//============================================================//
// Define speicific units

#define name_dimension(dim)			\
    typedef dreal<double,_##dim> dim;			\
    typedef unit<_##dim> dim##_unit;		

name_dimension(unitless);

name_dimension(length);
name_dimension(length2);
name_dimension(length3);
name_dimension(length4);
name_dimension(length5);

name_dimension(speed);


name_dimension(mass);
//Drat! Something the the standard library is already called time
typedef dreal<double,_time> time_double;
typedef unit<_time> time_unit;
name_dimension(current);
name_dimension(temp);
name_dimension(mole);

name_dimension(energy);


const energy_unit Joules("Joule",1.0);
const energy_unit Hz("Hz",6.626068e-34);
const energy_unit KHz("KHz",6.626068e-31);
const energy_unit MHz("MHz",6.626068e-28);
const energy_unit eV("eV",1.60217646e-19);

const time_unit seconds("Seconds",1.0);

const length_unit metres("metres",1.0);
const length_unit milimetre("milimetre",1e-3);
const length_unit micrometre("micrometre",1e-6);
const length_unit nanometre("nanometre",1e-9);
const length_unit Angstroms("Angstroms",1e-10);


void test() {
    length l1 = 1.0/metres;
    length l2 = 1.0/metres;

    length l3 = l1+l1;

    dreal<double,_time> t;// = 4/seconds;
    speed s = l1/t;

    std::cout <<l1[milimetre] << std::endl;
}


#endif

