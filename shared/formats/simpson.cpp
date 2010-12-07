
#include <shared/formats/simpson.hpp>
#include <shared/nuclear_data.hpp>
#include <fstream>
#include <algorithm>

using namespace std;
using namespace SpinXML;

const char* example_par_section =
    "par {\n"
    "  proton_frequency 400e6\n"
    "  spin_rate 10000\n"
    "  sw spin_rate/2.0\n"
    "  np 32\n"
    "  crystal_file rep320\n"
    "  gamma_angles 18\n"
    "  start_operator I1x\n"
    "  detect_operator I1p\n"
    "  verbose 1101\n"
    "  variable rf 150000\n"
    "}";


void SIMPSONLoader::SaveFile(const SpinSystem* ss,const char* filename) const {
	ofstream fout(filename);
	if(!fout.is_open()) {
		throw runtime_error("Couldn't open file");
	}
	vector<Spin*> spins=ss->GetSpins();
	long count=spins.size();

	fout << "spinsys {" << endl;
	fout << "  channels ";
	vector<pair<long,long> > chanels;
	for(long i=0;i<count;i++){
		Spin* spin=spins[i];
		long element=spin->GetElement();
		long neutronN=spin->GetIsotope();

		if(getNuclearSpin(element,neutronN) ==0) {
			//We don't have an NMR active isotope, skip it
			continue;
		}
		pair<long,long> p(element,neutronN);
		if(find(chanels.begin(),chanels.end(),p)==chanels.end()) {
			chanels.push_back(p);
		}
	}
	for(unsigned long i=0;i<chanels.size();i++) {
		pair<long,long> p = chanels[i];
		fout << p.second+p.first << getElementSymbol(p.first)  << " ";
	}
	fout << endl;
	fout << "  nuclei ";
	for(long i=0;i<count;i++) {
		Spin* spin=spins[i];
		long element=spin->GetElement();
		long isotope=spin->GetIsotope()+element;
		if(getNuclearSpin(element,isotope-element) ==0) {
			//We don't have an NMR active isotope, skip it
			continue;
		}
		fout << isotope << getElementSymbol(element)  << " ";
	}
	fout << " shift" << endl;
	for(long i=0;i<count;i++) {

	}

	for(long i=0;i<count;i++) {
		//Obtain the dipole-dipole interactions
		Spin* spin=spins[i];
		vector<Interaction*> inters=ss->GetInteractionsBySpinOnce(spin,Interaction::DIPOLAR);
      
		for(vector<Interaction*>::iterator it=inters.begin(); it != inters.end();++it) {
			fout << "  dipole " 
				 << i << " " 
				 << ss->GetSpinNumber((*it)->GetOtherSpin(spin))
				 << " "<< endl;
		}
	}
	fout << "}" << endl;
  
	fout << example_par_section << endl;
}
