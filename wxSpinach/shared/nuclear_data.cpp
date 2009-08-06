
#include "nuclear_data.hpp"
#include <cstring>

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

struct Element {
  const char* symbol;
  const char* name;
  double red;
  double green;
  double blue;
};


const long gKnownIsotopeCount=3;
const Isotope gIsotopes[]={
  {"E","Electron",-1,0,2,1.760859770e11,0.0,0.0,1.0},
  {"H","Hydrogen",1 ,0,2,1.760859770e11,1.0,0.0,0.0},
  {"C","Carbon"  ,6 ,6,2,1.760859770e11,0.6,0.6,0.6},
  {"N","Nitrogen",7 ,7,3,1.9337798e7   ,0.7,0.0,0.0}
};



//List of elements indexed by proton number
const long gKnownElements=9;
const Element gElements[]={
  {"E","Electron",   0.0,0.0,1.0},
  {"H","Hydrogen",   0.5,0.5,0.5},
  {"He","Helium",    1.0,1.0,1.0},
  {"Li","Lithium",   1.0,1.0,1.0},
  {"Be","Beryllium", 1.0,1.0,1.0},
  {"B","Boron",      1.0,1.0,1.0},
  {"C","Carbon",     0.2,0.2,0.2},
  {"N","Nitrogen",   0.0,0.0,0.3},
  {"O","Oxygen",     0.3,0.0,0.0},
  {"F","Fluorine",   1.0,1.0,1.0},
  {"Ne","Neon",      1.0,1.0,1.0},
};


//Elemental functions

long getElementCount() {
  return gKnownElements;
}

const char* getElementSymbol(long p) {
  if(p>=gKnownElements) {
    return "Unknown";
  }
  return gElements[p].symbol;
}

const char* getElementName(long p) {
  if(p>=gKnownElements) {
    return "Unknown";
  }
  return gElements[p].name;
}

double getElementR(long p) {
  if(p>=gKnownElements) {
    return 1.0;
  }
  return gElements[p].red;
}

double getElementG(long p) {
  if(p>=gKnownElements) {
    return 1.0;
  }
  return gElements[p].green;
}

double getElementB(long p) {
  if(p>=gKnownElements) {
    return 1.0;
  }
  return gElements[p].blue;
}


//Isotope functions

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


