
#include "nuclear_data.hpp"
#include <cstring>

#include <vector>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

//A simple struct like datatype for storing data about nuclei
struct Isotope {
  long neutron;    //Number or protons+neutrons
  long twiceSpin;
  double gyro;
  Isotope(long n,long twospin,double g) : neutron(n),twiceSpin(twospin),gyro(g) {}
};


struct Element {
  const char* symbol;
  const char* name;
  double red;
  double green;
  double blue;
  long common;
};


//An array of arrays of isotopes.
vector<vector<Isotope> > gIsotopes;



//List of elements indexed by proton number
const long gKnownElements=9;
const Element gElements[]={
    {"E","Electron",   0.0,0.0,1.0,  0},
    {"H","Hydrogen",   0.5,0.5,0.5,  0},
    {"He","Helium",    1.0,1.0,1.0,  2},
    {"Li","Lithium",   1.0,1.0,1.0,  4},
    {"Be","Beryllium", 1.0,1.0,1.0,  5},
    {"B","Boron",      1.0,1.0,1.0,  6},
    {"C","Carbon",     0.2,0.2,0.2,  6},
    {"N","Nitrogen",   0.0,0.0,0.3,  7},
    {"O","Oxygen",     0.3,0.0,0.0,  8},
    {"F","Fluorine",   1.0,1.0,1.0,  10},
    {"Ne","Neon",      1.0,1.0,1.0,  10},
};


//Elemental functions

long getCommonIsotope(long p) {
  if(p>=gKnownElements) {
    return 1.0;
  }
  return gElements[p].common;
}

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

long getElementBySymbol(const char* symb) {
  for(long i=0;i<gKnownElements;i++) {
    if(strcmp(symb,gElements[i].symbol)==0) {
      return i;
    }
  }
  return -1;
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

void LoadIsotopes() {
  fstream fin("data/isotopes.dat",ios::in);
  
  if(!fin.is_open()) {
    throw runtime_error("Could not load isotopes.data");
  }

  string line;
  while(getline(fin,line)) {
    string beforeComment=line.substr(0,line.find("#"));
    istringstream stream;
    stream.str(beforeComment);

    unsigned long p;  //Proton number
    unsigned long m;  //Mass number
    long twicespin;
    double gyro; 
    stream >> p;
    if(!stream.eof()) { //We have a blank or commented line
      stream >> m;
      if(stream.eof()) {throw runtime_error("Corupt file");}
      stream >> twicespin;
      if(stream.eof()) {throw runtime_error("Corupt file");}
      stream >> gyro;
      if(!stream.eof()) {throw runtime_error("Corupt file");}

      if(p >= gIsotopes.size()) {
	gIsotopes.resize(p+1);
      }

      gIsotopes[p].push_back(Isotope(m-p,twicespin,gyro));
    }
  }
}

long getIsotopeCount(long protonN){
  return gIsotopes[protonN].size();
}

long getNeutrons(long protonN,long index) {
  return gIsotopes[protonN][index].neutron;
}

///Get the nuclear spin in units of hbar/2
long getNuclearSpin(long protonN,long index) {
  return gIsotopes[protonN][index].twiceSpin;
}

double getGyromagneticRatio(long protonN,long index) {
  return gIsotopes[protonN][index].gyro;
}




