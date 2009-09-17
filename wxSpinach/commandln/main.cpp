
#include <iostream>
#include <fstream>

//#include <flens/flens.h>

#include <string.h>
#include <shared/spinsys_spec.hpp>



//using namespace flens;
using namespace std;

//typedef GeMatrix<FullStorage<double, ColMajor> > MyMatrix;

int main(int argc,char** argv) {
  bool bFailed = false;

  // initialize the XML library.
    
  try {

    std::auto_ptr<Spin_system> ss (parseSpin_system("../data/spinsys.xml"));
    for (Spin_system::SpinConstIterator i (ss->getSpin().begin());
	 i != ss->getSpin().end();
	 ++i)
      {
	cout << "x=" << i->getLabel() << "!" << endl;
      }

    xml_schema::NamespaceInfomap map;
    map[""].name = "";
    map[""].schema = "../data/spinsys_spec.xsd";

    serializeSpin_system(cout, *ss, map);

  } catch (const xml_schema::Exception& e) {
    cerr << e << endl;
    return 1;
  }
  /*
    MyMatrix A(4,4);

    A = 1, 2, 3, 4,
    5, 6, 7, 8,
    9, 10,11,12,
    13,14,15,16;

    cout << A << endl;
  */
	

  return 0;
}

