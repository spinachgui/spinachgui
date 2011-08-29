
#include "nuclear_data.hpp"
#include <cstring>

#include <vector>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <shared/log.hpp>

using namespace std;

//A simple struct like datatype for storing data about nuclei
struct Isotope {
    unsigned long neutron;    //Number or protons+neutrons
    unsigned long twiceSpin;
    double gyro;
    Isotope() {}
    Isotope(long n,long twospin,double g) : neutron(n),twiceSpin(twospin),gyro(g) {}
};


struct Element {
    Element(const string& name_,const string& symbol_,long massN_,double r,double g,double b)
        : name(name_),
          symbol(symbol_),
          massN(massN_),
          red(r),
          green(g),
          blue(b) {
    }
    string name;
    string symbol;
    long massN;
    double red;
    double green;
    double blue;
};

//An array of all the elements
vector<Element> gElements;

//An array of arrays of isotopes.
vector<map<long,Isotope> > gIsotopes;


//Elemental functions

long getCommonIsotope(long p) {
    if(p>=long(gElements.size())) {
        WARN("getCommonIsotope called on unknown element " << p);
        return -1;
    }
    return gElements[p].massN;
}

long getElementCount() {
    return gElements.size();
}

const char* getElementSymbol(long p) {
    if(p>=long(gElements.size())) {
        WARN("getElementSymbol called on unknown element " << p);
        return "Unknown";
    }
    return gElements[p].symbol.c_str();
}

const char* getElementName(long p) {
    if(p>=long(gElements.size())) {
        WARN("getElementName called on unknown element " << p);
        return "Unknown";
    }
    return gElements[p].name.c_str();
}

long getElementBySymbol(const char* symb) {
    for(unsigned long i=0;i<gElements.size();i++) {
        if(strcmp(symb,gElements[i].symbol.c_str())==0) {
            return i;
        }
    }
    WARN("getElementBySymbol called on unknown symbol " << symb);
    return -1;
}

long getIsotopeBySymbol(const char* symb) {
    for(unsigned long i=0;i<gElements.size();i++) {
        if(strcmp(symb,gElements[i].symbol.c_str())==0) {
            return gElements[i].massN;
        }
    }
    return -1;
}


double getElementR(long p) {
    if(p>=long(gElements.size())) {
        WARN("getElementR called on " << p << " and unknown element");
        return 1.0;
    }
    return gElements[p].red;
}

double getElementG(long p) {
    if(p>=long(gElements.size())) {
        WARN("getElementG called on " << p << " and unknown element");
        return 1.0;
    }
    return gElements[p].green;
}

double getElementB(long p) {
    if(p>=long(gElements.size())) {
        WARN("getElementB called on " << p << " and unknown element");
        return 1.0;
    }
    return gElements[p].blue;
}

void LoadElements() {
    TRACE("LoadElements");
    fstream fin("data/elements.dat",ios::in);
  
    if(!fin.is_open()) {
        throw runtime_error("Could not load elements.dat");
    }
    TRACE("Elements file opened");

    string line;
    gElements.clear();

    while(getline(fin,line)) {
        TRACE("Reading line \"" << line << "\"");
        string beforeComment=line.substr(0,line.find("#"));
        istringstream stream;
        stream.str(beforeComment);


        string name;
        string symbol;
        unsigned long m;  //Mass number
        double red;
        double green;
        double blue;

        stream >> name;
        if(!stream.eof()) { //We have a blank or commented line
            stream >> symbol;
            if(stream.eof()) {throw runtime_error("Corupt file when reading symbol");}
            stream >> m;
            if(stream.eof()) {throw runtime_error("Corupt file when reading mass number");}
            stream >> red;
            if(stream.eof()) {throw runtime_error("Corupt file when reading red");}
            stream >> green;
            if(stream.eof()) {throw runtime_error("Corupt file when reading green");}
            stream >> blue;
            if(!stream.eof()) {throw runtime_error("Corupt file when reading blue");}

            gElements.push_back(Element(name,symbol,m,red,green,blue));
        }
    }

}

//Isotope functions

void LoadIsotopes() {
    fstream fin("data/isotopes.dat",ios::in);
  
    if(!fin.is_open()) {
        throw runtime_error("Could not load isotopes.data");
    }

    string line;
    gIsotopes.clear();
    gIsotopes.resize(gElements.size());
    while(getline(fin,line)) {
        string beforeComment=line.substr(0,line.find("#"));
        istringstream stream;
        stream.str(beforeComment);

        unsigned long p;  //Proton number
        unsigned long m;  //Mass number
        long multiplicity;
        double gyro; 
        stream >> p;
        if(!stream.eof()) { //We have a blank or commented line
            stream >> m;
            if(stream.eof()) {throw runtime_error("Corupt file");}
            stream >> multiplicity;
            if(stream.eof()) {throw runtime_error("Corupt file");}
            stream >> gyro;
            if(!stream.eof()) {throw runtime_error("Corupt file");}

            if(p >= gIsotopes.size()) {
                gIsotopes.resize(p+1);
            }
			gyro = gyro * 1e6  / (2*M_PI);
            gIsotopes[p][m-p]=Isotope(m-p,multiplicity-1,gyro);
        }
    }
}

vector<long> getKnownIsotopes(long protonN) {
    vector<long> isotopes;
    for(map<long,Isotope>::const_iterator i=gIsotopes[protonN].begin();
        i!=gIsotopes[protonN].end();++i) {
        isotopes.push_back((*i).second.neutron+protonN);
    }
    return isotopes;
}

long getIsotopeCount(long protonN){
    return gIsotopes[protonN].size();
}

long getNeutrons(long protonN,long neutronN) {
    return gIsotopes[protonN][neutronN].neutron;
}

///Get the nuclear spin in units of hbar/2
long getNuclearSpin(long protonN,long neutronN) {
    return gIsotopes[protonN][neutronN].twiceSpin;
}

double getGyromagneticRatio(long protonN,long neutronN) {
    return gIsotopes[protonN][neutronN].gyro;
}




