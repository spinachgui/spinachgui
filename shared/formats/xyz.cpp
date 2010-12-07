
#include <shared/formats/xyz.hpp>
#include <shared/nuclear_data.hpp>
#include <fstream>

using namespace std;
using namespace SpinXML;

void XYZLoader::SaveFile(const SpinSystem* ss,const char* filename) const {
  ofstream fout(filename);
  if(!fout.is_open()) {
    throw runtime_error("Couldn't open file");
  }

  long count=ss->GetSpinCount();
  for(long i=0;i<count;i++) {
    length x,y,z;
    ss->GetSpins()[i]->GetCoordinates(&x,&y,&z);
    fout << getElementSymbol(ss->GetSpins()[i]->GetElement()) << " " 
		 << (x * Angstroms) << " " 
         << (y * Angstroms) << " " 
		 << (z * Angstroms) << " " << endl;
  }
}


void XYZLoader::LoadFile(SpinSystem* ss,const char* filename) const {

  ss->Clear();
  ifstream fin(filename);
  if(!fin.is_open()) {
    throw runtime_error("Couldn't open file");
  }
  long size;
  fin >> size;
  for(long i=0;i<size;i++) {
    if(fin.eof()) {
      ss->sigReloaded();
      throw runtime_error("Unexpected end of file");
    }
    string el;
    length x,y,z;
    fin >> el >> x >> y >> z;
	x=x/Angstroms;
	y=y/Angstroms;
	z=z/Angstroms;
    long atomicN=getElementBySymbol(el.c_str());
    if(atomicN==-1) {
      throw runtime_error("Unknown Element");
    }
    ss->InsertSpin(new Spin(Vector3d(x,y,z),el,atomicN));
  }
  ss->sigReloaded();
}

