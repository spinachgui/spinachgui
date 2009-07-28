
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

  //Load the spins
  for(Spin_system::SpinConstIterator i(mXMLSpinSys->getSpin().begin()); 
      i != mXMLSpinSys->getSpin().end();
      ++i) {
    cout << "Loading a spin" << endl;
    SpinachSpin thisSpin(*i);
    mSpins.push_back(thisSpin);
  }
  //Load the interactions
  for(Spin_system::InteractionConstIterator i(mXMLSpinSys->getInteraction().begin()); 
      i != mXMLSpinSys->getInteraction().end();
      ++i) {
    cout << "Loading an interaction" << endl;
    SpinachInteraction thisInt(*i);
    mInteractions.push_back(thisInt);
  }
  //Load the frames
  for(Spin_system::Reference_frameConstIterator i(mXMLSpinSys->getReference_frame().begin()); 
      i != mXMLSpinSys->getReference_frame().end();
      ++i) {
    cout << "Loading a frame" << endl;
    SpinachFrame thisFrame(*i);
    mFrames.push_back(thisFrame);
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


SpinachSpin Spinsys::getSpin(long n) {
  if(n>=0 && n<mSpins.size()) {
    return mSpins[n];
  }
  SpinachSpin s;
  return s;
}

Vector SpinachSpin::getCoords() {
  cout << "About to return a vector " << endl;
  return getCoordinates();
}

void SpinachSpin::dump() {
  cout << "Spin name=" << getLabel() << endl;
  //const Spin::CoordinatesType c = getCoordinates();
  //cout << c.getX() << endl;
  //cout << c.getY() << endl;
  //cout << c.getZ() << endl;
  //cout << "IsotopeE = " << getIsotope() << endl;
}
