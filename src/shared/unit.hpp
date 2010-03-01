
#ifndef _unit_h
#define _unit_h

#include <iostream>
#include <string>
#include <vector>

enum dimension_t {
    ENERGY_HFC,
    ENERGY_G_TENSER,
    ENERGY_ZFS,
    ENERGY_EXCHANGE,
    ENERGY_SHIELDING,
    ENERGY_SCALAR,
    ENERGY_QUADRUPOLAR,
    ENERGY_DIPOLAR,
    ENERGY_CUSTOM,
    //Generic dimensions
    ENERGY,
    LENGTH
};

class UnitSystem {
public:
    UnitSystem() {}
    UnitSystem& add(std::string,double factor) {
        return *this;
    }
    const char* get_symbol(dimension_t dim) const {
        return "MHz";
    }
private:
    std::vector<double> ToSIFactors;
};
void PushUnitSystem(const UnitSystem* us);
void PopUnitSystem();


template<dimension_t D>
struct withUnit {
    typedef withUnit<D> self_t;
    withUnit() {
    }
    withUnit(double v) : val(v) {
    }
    self_t operator*(double o) {
        return val*o;
    }
    self_t operator+(self_t o) {
        return withUnit(val+o.val);
    }
    self_t operator-(self_t o) {
        return withUnit(val-o.val);
    }
    self_t operator/(double o) {
        return val/o;
    }
    //Get the actual quantity stored with no automatic conversion
    double get_double() {
        return val;
    }
    //Get the quantity in SI units
    double operator()();
    //Set the quantity based on an SI unit
    void operator()(double si);
private:
    
    double val;
};

typedef withUnit<ENERGY> energy;
typedef withUnit<LENGTH> length;

#endif

