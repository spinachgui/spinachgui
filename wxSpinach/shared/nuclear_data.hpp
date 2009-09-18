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

long getIsotopeCount();
long getIsotope(long protonN,long massN);
long getIsotopeBySymbol(const char* str);
double getIsotopeColorR(long index);
double getIsotopeColorG(long index);
double getIsotopeColorB(long index);
const char* getIsotopeSymbol(long index);


#endif // __spinachcalcFrameBase__
