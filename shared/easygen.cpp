
#include <shared/easygen.hpp>

#include <string>
#include <vector>
#include <Eigen/Dense>
#include <shared/spinsys.hpp>
#include <shared/interaction.hpp>
#include <shared/foreach.hpp>
#include <shared/nuclear_data.hpp>
#include <boost/optional.hpp>

using namespace std;
using namespace Eigen;
using namespace SpinXML;
using namespace boost;

template<typename T,typename T2>
T orDefault(optional<T> o,T2 d) {
    return o ? o.get() : T(d);
}

string MatlabMatrix_(Matrix3d mat) {
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

struct Appender_ {
	Appender_(string& str_,const string& delim_ = ",")
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

Matrix3d Crush_(const vector<Interaction*>& inters,unit u) {
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

void monoTensor_(const SpinSystem* ss,ostream& out,string varname,vector<Spin*> spins,Interaction::Type t,unit u) {
	string line;
	Appender_ appender(line);
	foreach(Spin* spin,spins) {
		vector<Interaction*> tensors = ss->GetInteractionsBySpin(spin,t);
		appender(MatlabMatrix_(Crush_(tensors,u)));
	}
	out << "sys." << varname << " = " << line << ";" << endl;
}


string EasySpinInput::generate(SpinSystem* spinsys) const {
    ostringstream oss;

    ostringstream NucsStream;
    long count=spinsys->GetSpinCount();

    string electronLine;
    Appender_ electronAppender(electronLine);
    bool first = true;
    vector<Spin*> electronSpins;
    vector<Spin*> nucSpins;

    for(long i=0;i<count;i++) {
        Spin* spin = spinsys->GetSpin(i);
        if(spin->GetElement() == 0) {
            //We have an electron
            electronSpins.push_back(spin);
            electronAppender("1/2");
        } else {
            //We have a nucleus
            nucSpins.push_back(spin);

            //Format is:
            //NewSys = nucspinadd(Sys,Nuc,A,Apa,Q,Qpa)
            NucsStream << "Sys = nucspinadd(Sys,"
                       << (spin->GetIsotope()+spin->GetElement()) 
                       << getElementSymbol(spin->GetElement()) << "," 
                       << "[Ax Ay Az]" << "," << "[alpha,beta,gamma]" << ","
                       << "[Qx Qy Qz]" << "," << "[alpha,beta,gamma]" << ");" << endl;
            first = false;
        }
    }

    oss << "sys.S = [" << electronLine << "];" << endl;
    oss << NucsStream.str() << endl;


    //G tensors
    monoTensor_(spinsys,oss,"g",electronSpins,Interaction::G_TENSER,Joules);
    //ZFS
    monoTensor_(spinsys,oss,"D",electronSpins,Interaction::ZFS,MHz);


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
    Appender_ eeTensorAppender(eeTensorLine);
    for(vector<Spin*>::iterator i = electronSpins.begin();i != electronSpins.end();++i) {

        for(vector<Spin*>::iterator j = i+1;j != electronSpins.end();++j) {
            vector<Interaction*> tensors = spinsys->GetInteractionsBySpin(*i,*j,Interaction::DIPOLAR);
            //If, for some reason, more than one g_tensor is specified,
            //crush them all together
            eeTensorAppender(MatlabMatrix_(Crush_(tensors,MHz)));
        }

    }
    oss << "sys.ee = [" << eeTensorLine << "];" << endl;

    oss << endl;

    oss << "Exp.mwFreq = " << mMWFreq << ";" << endl;
    oss << "Exp.CenterSweep = [" << mCentre << " " << mSweep << "];" << endl;
    oss << endl;
    oss << "Exp.Harmonic = " << mHarmonic << ";" << endl;
    oss << "Exp.nPoints = "  << mNPoints << ";" << endl;
    oss << "Exp.Mode = '"     << mModeNames[mMode] << "';" << endl;
    oss << "Exp.mwPhase = "  << mMWPhase << ";" << endl;

    oss << endl;
    if(mModAmp) {
        oss << "Exp.ModAmp = " << mModAmp.get() << ";" << endl;}
    if(mTemperature) {
        oss << "Exp.Temperature = " << mTemperature.get() << ";" << endl;}

    //Brodernings
    oss << "Sys.lw = [" << mGaussianFWHM << "," << mLorentFWHM << "]" << endl;

    if(mEasySpinFunc == PEPPER) {
        //For each of these, if any of x,y and z are instansiated we print out the row.
        if(mHStrainX || mHStrainY || mHStrainZ) {
            oss << "Sys.HStrain = ["
                << orDefault(mHStrainX,0) << ","
                << orDefault(mHStrainY,0) << ","
                << orDefault(mHStrainZ,0) << "];" << endl;
        }
        if(mGStrainX || mGStrainY || mGStrainZ) {
            oss << "Sys.HStrain = ["
                << orDefault(mGStrainX,0) << ","
                << orDefault(mGStrainY,0) << ","
                << orDefault(mGStrainZ,0) << "];" << endl;
        }
        if(mAStrainX || mAStrainY || mAStrainZ) {
            oss << "Sys.HStrain = ["
                << orDefault(mAStrainX,0) << ","
                << orDefault(mAStrainY,0) << ","
                << orDefault(mAStrainZ,0) << "];" << endl;
        }
        if(mDStrainD || mDStrainE || mDStrainCorrCoeff) {
            oss << "Sys.HStrain = ["
                << orDefault(mDStrainD,0) << ","
                << orDefault(mDStrainE,0) << ","
                << orDefault(mDStrainCorrCoeff,0) << "];" << endl;
        }
    }
    oss << endl;

    oss << mEasySpinNames[mEasySpinFunc] << "(Sys,Exp)" << endl;

    return oss.str();
}


// NB Make sure the maps are defined before __init or the static
// constructor called by __init will try to use them before they are
// constructed.
map<EasySpinInput::Mode        ,string> EasySpinInput::mModeNames;
map<EasySpinInput::Method      ,string> EasySpinInput::mMethodNames;
map<EasySpinInput::Output      ,string> EasySpinInput::mOutputNames;
map<EasySpinInput::EasySpinFunc,string> EasySpinInput::mEasySpinNames;
EasySpinInput::__Init EasySpinInput::__init;

