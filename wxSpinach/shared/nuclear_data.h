#ifndef NUCLEAR_DATA_H
#define NUCLEAR_DATA_H

#include <string>
#include <vector>

using namespace std;

class Isotope {
/*
    A very simple struct like datatype for storing data about nuclei
*/
public:
    Isotope(){};
    Isotope(string _name,string _shortName,long _multiplicity,double _gamma);
    string name;
    string shortName;
    long multiplicity;
    double gamma;
};

//A global Variable. Access with extern
vector<Isotope> KnownIsotopes;

void LoadIsotopes();


#endif // __spinachcalcFrameBase__
