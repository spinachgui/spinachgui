#ifndef NUCLEAR_DATA_H
#define NUCLEAR_DATA_H

#include <string>
#include <vector>

using namespace std;

struct Isotope;

extern  const long gKnownIsotopeCount;
extern  const Isotope gIsotopes[];

long getIsotopeCount();
long getIsotope(long protonN,long massN);
long getIsotopeBySymbol(const char* str);
double getIsotopeColorR(long index);
double getIsotopeColorG(long index);
double getIsotopeColorB(long index);
const char* getIsotopeSymbol(long index);


#endif // __spinachcalcFrameBase__
