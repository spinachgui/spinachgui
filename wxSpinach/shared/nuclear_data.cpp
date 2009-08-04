
#include "nuclear_data.hpp"
#include <cstring>

using namespace std;

//A very simple struct like datatype for storing data about nuclei
struct Isotope {
  const char* symbol;
  const char* name;
  long protonN; //Number of protons
  long massN;    //Number or protons+neutrons
  long multiplicity;
  double gamma;
  double red;
  double green;
  double blue;
};


long getIsotopeCount(){
  return gKnownIsotopeCount;
}

long getIsotopeBySymbol(const char* str) {
  for(long i=0;i<gKnownIsotopeCount;i++) {
    if(strcmp(str,gIsotopes[i].symbol)==0) {
      return i;
    }
  }
  return -1;
}

long getIsotope(long protonN,long massN){
  for(long i=0;i<gKnownIsotopeCount;i++) {
    if(gIsotopes[i].protonN==protonN && gIsotopes[i].massN==massN) {
      return i;
    }
  }
  return -1;
}
double getIsotopeColorR(long index) {
  return gIsotopes[index].red;
}
double getIsotopeColorG(long index) {
  return gIsotopes[index].green;
}
double getIsotopeColorB(long index) {
  return gIsotopes[index].blue;
}

const char* getIsotopeSymbol(long index) {
  return gIsotopes[index].symbol;
}

const long gKnownIsotopeCount=3;
const Isotope gIsotopes[]={
  {"E","Electron",-1,0,2,1.760859770e11,0.0,0.0,1.0},
  {"H","Hydrogen",1 ,0,2,1.760859770e11,1.0,0.0,0.0},
  {"C","Carbon"  ,6 ,6,2,1.760859770e11,0.6,0.6,0.6},
  {"N","Nitrogen",7 ,7,3,1.9337798e7   ,0.7,0.0,0.0}
};

