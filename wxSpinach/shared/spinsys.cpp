
#include "spinsys.hpp"
#include <iostream>
#include <fstream>

using namespace std;

Spinsys::Spinsys() : mXMLSpinSys(new Spin_system()) {

}

void Spinsys::createNew() {
  mXMLSpinSys.reset(new Spin_system());
}

void Spinsys::dump() const{
  cout << "Printing out a spin system" << endl;
  cout << "Spins:" << endl;
  for (Spin_system::SpinConstIterator i (mXMLSpinSys->getSpin().begin());
       i != mXMLSpinSys->getSpin().end();
       ++i)
    {
      cout << " Spin name=" << i->getLabel() << endl;
      cout << "      number=" << i->getNumber() << endl;
      cout << "      isotope=" << i->getIsotope() << endl;
    }


}

void Spinsys::loadFromFile(const char* filename) {
  try {
    mXMLSpinSys = parseSpin_system(filename);
  } catch(const xml_schema::Exception& e) {
    cerr << e << endl;
    exit(1);
  }

  for(Spin_system::SpinConstIterator i(mXMLSpinSys->getSpin().begin()); 
      i != mXMLSpinSys->getSpin().end();
      ++i) {
    cout << "Loading a spin" << endl;
  }
}

void Spinsys::addSpin() {
  //  Spin s();
  //  mXMLSpinSys->getSpin().push_back(s);
}

void Spinsys::saveToFile(const char* filename) {
  xml_schema::NamespaceInfomap map;
  map[""].name = "";
  map[""].schema = "../data/spinsys_spec.xsd";

  ofstream fout(filename);
  if(!fout.is_open()) {
    cerr << "Could not open " << filename << endl;
    return;
  }
  serializeSpin_system(fout, *mXMLSpinSys, map);
}



void SpinachSpin::dump() {
  cout << "Spin name=" << getLabel() << endl;
  /*  cout << getCoordinates().getX() << endl;
  cout << getCoordinates().getY() << endl;
  cout << getCoordinates().getZ() << endl;*/
  cout << "IsotopeE = " << getIsotope() << endl;
}
