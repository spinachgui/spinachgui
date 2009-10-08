#ifndef NUCLEAR_DATA_H
#define NUCLEAR_DATA_H

#include <string>

struct Isotope;

//Elemental functions
long getElementCount();
const char* getElementSymbol(long p);
const char* getElementName(long p);
double getElementR(long p);
double getElementG(long p);
double getElementB(long p);


//Isotope functions

///Call this somewhere in the initalisation to load the istope data
///from data/isotopes.dat
void LoadIsotopes();

///Get the number of known isotopes of a given element
long getIsotopeCount(long protonN);
///Get the index'th isotope of a given element where 0 <= index <
///getIsotopeCount()

//
long getNeutrons(long protonN,long index);

///
long getMultiplicity(long protonN,long index);

///
double getGamma(long protonN,long index);



#endif // __spinachcalcFrameBase__
