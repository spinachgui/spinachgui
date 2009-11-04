
#include <iostream>
#include <fstream>

#include <shared/nuclear_data.hpp>
#include <shared/spinsys.hpp>

using namespace SpinXML;
using namespace std;

int main(int argc,char** argv) {
  try {
    LoadIsotopes();
  } catch(runtime_error e) {
    cerr << "Error loading data/isotopes.dat. Is the file present and not corrupt?" << endl << 
      "Message was:" << e.what() << endl;

    return 1;
  }
	
  SpinSystem* SS = new SpinSystem;
  SS->LoadFromXMLFile("tryo.xml");

  vector<Interaction*> inters=SS->GetInteractions();

  for(long i=0;i<inters.size();i++) {
    inters[i]->Dump();
  }

  SS->LoadFromXMLFile("tryo.xml");
  delete SS;

  return 0;
}

