#ifndef NUCLEAR_DATA_H
#define NUCLEAR_DATA_H

#include <string>
#include <vector>
#include <shared/unit.hpp>

struct Isotope;

//Elemental functions
long getElementCount();
const char* getElementSymbol(long p);
const char* getElementName(long p);
double getElementR(long p);
double getElementG(long p);
double getElementB(long p);

///Given a particular nucleous, get a sensible default isotope
///(eg. p=6 should return 6, -> carbon is usually carbon 12)
long getCommonIsotope(long p);

long getElementBySymbol(const char* symb);
long getIsotopeBySymbol(const char* symb);


//Isotope functions

///Call this somewhere in the initalisation to load the istope data
///from data/isotopes.dat
void LoadIsotopes();

///Get the number of known isotopes of a given element
long getIsotopeCount(long protonN);
///Get the index'th isotope of a given element where 0 <= index <
///getIsotopeCount()

///Return a vector of known mass numbers for a given proton number
std::vector<long> getKnownIsotopes(long protonN);

///Get the number of neutrons given an isotobe index
long getNeutrons(long protonN,long neutronN);

///Get the nuclear spin in units of hbar/2
long getNuclearSpin(long protonN,long neutronN);

///
double getGyromagneticRatio(long protonN,long neutronN);
#endif // __spinachcalcFrameBase__


