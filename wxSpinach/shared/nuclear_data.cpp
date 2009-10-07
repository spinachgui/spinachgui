
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
  long multiplicity;
  double gamma;
  Isotope(long n,long mult,double g) : neutron(n),multiplicity(mult),gamma(g) {}
};


struct Element {
  const char* symbol;
  const char* name;
  double red;
  double green;
  double blue;
};


//An array of arrays of isotopes.
vector<vector<Isotope> > gIsotopes;



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

    long p;  //Proton number
    long m;  //Mass number
    long mult;
    double gamma; //Gamma
    stream >> p;
    if(!stream.eof()) { //We have a blank or commented line
      stream >> m;
      if(stream.eof()) {throw runtime_error("Corupt file");}
      stream >> mult;
      if(stream.eof()) {throw runtime_error("Corupt file");}
      stream >> gamma;
      if(!stream.eof()) {throw runtime_error("Corupt file");}

      if(p >= gIsotopes.size()) {
	gIsotopes.resize(p+1);
      }

      gIsotopes[p].push_back(Isotope(m-p,mult,gamma));
    }
  }
}

long getIsotopeCount(long protonN){
  return gIsotopes[protonN].size();
}

long getNeutrons(long protonN,long index) {
  return gIsotopes[protonN][index].neutron;
}

long getMultiplicity(long protonN,long index) {
  return gIsotopes[protonN][index].multiplicity;
}

double getGamma(long protonN,long index) {
  return gIsotopes[protonN][index].gamma;
}




