
#include <shared/formats/simpson.hpp>
#include <shared/nuclear_data.hpp>
#include <fstream>

using namespace std;
using namespace SpinXML;

const char* example_par_section =
    "par {"
    "  proton_frequency 400e6"
    "  spin_rate 10000"
    "  sw spin_rate/2.0"
    "  np 32"
    "  crystal_file rep320"
    "  gamma_angles 18"
    "  start_operator I1x"
    "  detect_operator I1p"
    "  verbose 1101"
    "  variable rf 150000"
    "}";


void SIMPSONLoader::SaveFile(const SpinSystem* ss,const char* filename) const {
  ofstream fout(filename);
  if(!fout.is_open()) {
    throw runtime_error("Couldn't open file");
  }

  long count=ss->GetSpinCount();
  fout << "spinsys {" << endl;
  fout << "  channels 13C 1H" << endl;
  fout << "  nuclei";
  for(long i=0;i<count;i++) {
    Spin* spin=ss->GetSpins()[i];
    long element=spin->GetElement();
    long isotope=spin->GetIsotopes()[0];
    fout << isotope << getElementSymbol(element)  << " ";
  }
  fout << endl;
  fout << " dipole" << endl;
  fout << " shift" << endl;
  fout << "}" << endl;
}
