
#include <shared/formats/easyspin.hpp>
#include <shared/nuclear_data.hpp>
#include <shared/unit.hpp>
#include <shared/foreach.hpp>
#include <fstream>
#include <sstream>

#include <vector>

#include <boost/filesystem.hpp>

using namespace std;
using namespace SpinXML;
using namespace Eigen;
using namespace boost::filesystem;

string MatlabMatrix(Matrix3d mat) {
	ostringstream out;	

	out << "[";

	out << mat(0,0) << ",";
	out << mat(0,1) << ",";
	out << mat(0,2) << ";";

	out << mat(1,0) << ",";
	out << mat(1,1) << ",";
	out << mat(1,2) << ";";

	out << mat(2,0) << ",";
	out << mat(2,1) << ",";
	out << mat(2,2);

	out << "]";

	return out.str();
}

struct Appender {
	Appender(string& str_,const string& delim_ = ",")
		: first(true),delim(delim_),str(str_) {
	}
	void operator()(string toAppend) {
		if(!first) 
			str += delim;
		first = false;
		str += toAppend;
	}
	string& Get() {return str;}
	bool first;
	string delim;
	string& str;
};

Matrix3d Crush(const vector<Interaction*>& inters,unit u) {
	Matrix3d total;
	total <<
		0,0,0,
		0,0,0,
		0,0,0;
	for(unsigned long j = 0;j<inters.size();j++) {
		Interaction* inter = inters[j];
		total += inter->AsMatrix()*(1/u);
	}
	return total;
}

void monoTensor(const SpinSystem* ss,ostream& out,string varname,vector<Spin*> spins,Interaction::Type t,unit u) {
	string line;
	Appender appender(line);
	foreach(Spin* spin,spins) {
		vector<Interaction*> tensors = ss->GetInteractionsBySpin(spin,t);
		appender(MatlabMatrix(Crush(tensors,u)));
	}
	out << "sys." << varname << " = " << line << ";" << endl;
}

void EasySpinLoader::SaveFile(const SpinSystem* ss,const char* filename) const {
	ofstream fout(filename);
	if(!fout.is_open()) {
		throw runtime_error("Couldn't open file");
	}

	//The name of the matlab function is the name of the file
	path filepath(filename);
	string functionName = filepath.stem();

	fout << "function " << functionName << "()" << endl;

	ostringstream NucsStream;
	long count=ss->GetSpinCount();

	string electronLine;
	Appender electronAppender(electronLine);
	bool first = true;
	vector<Spin*> electronSpins;
	vector<Spin*> nucSpins;

	for(long i=0;i<count;i++) {
		Spin* spin = ss->GetSpin(i);
		if(spin->GetElement() == 0) {
			//We have an electron
			electronSpins.push_back(spin);
			electronAppender("1/2");
		} else {
			//We have a nucleus
			nucSpins.push_back(spin);
			if(!first) NucsStream << ",";
			NucsStream << spin->GetIsotope() << getElementSymbol(spin->GetElement());
			first = false;
		}
	}

	fout << "sys.S = [" << electronLine << "];" << endl;
	fout << "sys.Nucs = [" << NucsStream.str() << "];" << endl;

	
	//G tensors
	monoTensor(ss,fout,"g",electronSpins,Interaction::G_TENSER,Joules);
	//Nuclear Quadrupole
	monoTensor(ss,fout,"Q",nucSpins,Interaction::QUADRUPOLAR,MHz);
	//ZFS
	monoTensor(ss,fout,"D",electronSpins,Interaction::ZFS,MHz);

	/*
	  From the easyspin documentation:

	  It is possible to specify full A matrices in A. The 3x3 matrices
	  have to be combined like the 1x3 vectors used when only
	  principal values are defined: For different electrons, put the
	  3x3 matrices side by side, for different nuclei, on top of each
	  other. If full matrices are given in A, Apa is ignored.

	  
	*/
	string ATensorLine;
	Appender ATensorAppender(ATensorLine);
	foreach(Spin* nuc,nucSpins) {
		string NuclearALine;  //The A line of a singular nucleous
		Appender nucAppender(NuclearALine,";");

		foreach(Spin* electron,electronSpins) {
			vector<Interaction*> a_tensor = ss->GetInteractionsBySpin(electron,nuc,Interaction::HFC);
			//If, for some reason, more than one g_tensor is specified,
			//crush them all together

			nucAppender(MatlabMatrix(Crush(a_tensor,MHz)));
		}
		ATensorAppender(NuclearALine);
	}
	fout << "sys.A = [" << ATensorLine << "];" << endl;
	
	/*
	  From the easyspin documentation:
	  
	  Principal value of the electron-electron interaction
	  matrices. Each row corresponds to the diagonal of an interaction
	  matrix (in its eigenframe). They are lexicographically ordered
	  according to the indices of the electrons involved. If n is the
	  number of electrons, there are N = n(n-1)/2 rows. E.g. for four
	  electrons there have to be six rows with the principal values
	  for the interaction of electrons 1-2, 1-3, 1-4, 2-3, 2-4, and
	  3-4, in this order. For two electrons, ee contains one row only.
	*/
	string eeTensorLine;
	Appender eeTensorAppender(eeTensorLine);
	for(vector<Spin*>::iterator i = electronSpins.begin();i != electronSpins.end();++i) {

		for(vector<Spin*>::iterator j = i+1;j != electronSpins.end();++j) {
			vector<Interaction*> tensors = ss->GetInteractionsBySpin(*i,*j,Interaction::DIPOLAR);
			//If, for some reason, more than one g_tensor is specified,
			//crush them all together
			eeTensorAppender(MatlabMatrix(Crush(tensors,MHz)));
		}

	}
	fout << "sys.ee = [" << eeTensorLine << "];" << endl;
	

	fout << "end" << endl;
}


