
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
    double x,y,z;
    ss->GetSpins()[i]->GetCoordinates(&x,&y,&z);
    fout << getElementSymbol(ss->GetSpins()[i]->GetElement()) << " " 
	 << x << " "<< y << " " << z << " " << endl;
  }
}


void XYZLoader::LoadFile(SpinSystem* ss,const char* filename) const {
#ifdef SPINXML_EVENTS
  PushEventLock();
#endif
  ss->Clear();
  ifstream fin(filename);
  if(!fin.is_open()) {
    throw runtime_error("Couldn't open file");
  }
  long size;
  fin >> size;
  for(long i=0;i<size;i++) {
    if(fin.eof()) {
#ifdef SPINXML_EVENTS
      PopEventLock();
      ss->GetNode()->Change(IEventListener::CHANGE);
#endif
      throw runtime_error("Unexpected end of file");
    }
    string el;
    double x,y,z;
    fin >> el >> x >> y >> z;
    long atomicN=getElementBySymbol(el.c_str());
    if(atomicN==-1) {
      throw runtime_error("Unknown Element");
    }
    ss->InsertSpin(new Spin(ss,Vector3(x,y,z),el,0,atomicN));
  }

#ifdef SPINXML_EVENTS
  PopEventLock();
  ss->GetNode()->Change(IEventListener::CHANGE);
#endif
}

