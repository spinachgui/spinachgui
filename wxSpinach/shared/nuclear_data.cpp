
#include "nuclear_data.h"


Isotope::Isotope(string _name,string _shortName,long _multiplicity,double _gamma)
: name(_name),shortName(_shortName),multiplicity(_multiplicity),gamma(_gamma) {
}


void LoadIsotopes() {
/*
This function should probably graduate to loading the data from a file
*/
    KnownIsotopes.resize(6);
    KnownIsotopes[0]=Isotope("Electron","E"   ,2,  1.760859770e11);
    KnownIsotopes[1]=Isotope("Hydrogen","1H"  ,2,  26.7522205e7);
    KnownIsotopes[2]=Isotope("Hydrogen","2H"  ,3,  4.10662914e7);
    KnownIsotopes[3]=Isotope("Hydrogen","13C" ,2,  6.728286e7);
    KnownIsotopes[4]=Isotope("Hydrogen","14N" ,3,  1.9337798e7);
    KnownIsotopes[5]=Isotope("Hydrogen","15N" ,2,  -2.7126188e7);
}


