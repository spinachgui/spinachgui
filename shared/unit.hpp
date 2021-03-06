
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
#include <Eigen/Dense>

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
    inline double get_to_si() const {
        return mToSI;
    }
    inline double get_from_si() const {
        return mFromSI;
    }
private:
    std::string mName;
    double mToSI;
    double mFromSI;
};

double operator*(double number,const unit& u);
double operator/(double number,const unit& u);

Eigen::Vector3d operator*(const Eigen::Vector3d& v,const unit& u);
Eigen::Vector3d operator/(const Eigen::Vector3d& v,const unit& u);

Eigen::Matrix3d operator*(const Eigen::Matrix3d& m,const unit& u);
Eigen::Matrix3d operator/(const Eigen::Matrix3d& m,const unit& u);

//============================================================//
// Define speicific units

typedef double length;
typedef double length2;

typedef double mass;

typedef double energy;

const unit Unitless("Unitless",1.0);

const unit Joules("Joule",1.0);
const unit Hz("Hz",6.626068e-34);
const unit KHz("KHz",6.626068e-31);
const unit MHz("MHz",6.626068e-28);
const unit eV("eV",1.60217646e-19);

const unit seconds("Seconds",1.0);

const unit metres("Metres",1.0);
const unit milimetre("Milimetre",1e-3);
const unit micrometre("Micrometre",1e-6);
const unit nanometre("Nanometre",1e-9);
const unit Angstroms("Angstroms",1e-10);
const unit BohrRadius("Bohr Radius",52.9177e-12);

#endif

