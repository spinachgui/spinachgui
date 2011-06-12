
#include <shared/easygen.hpp>

#include <string>
#include <vector>
#include <Eigen/Dense>
#include <shared/spinsys.hpp>
#include <shared/interaction.hpp>
#include <shared/foreach.hpp>
#include <shared/nuclear_data.hpp>
#include <boost/optional.hpp>
#include <shared/orientation.hpp>

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

struct _Appender {
	_Appender(string& str_,const string& delim_ = ",")
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

Eigenvalues Crush_2(const vector<Interaction*>& inters,unit u) {
	Matrix3d total;
	total <<
		0,0,0,
		0,0,0,
		0,0,0;
	for(unsigned long j = 0;j<inters.size();j++) {
		Interaction* inter = inters[j];
		total += inter->AsMatrix()*(1/u);
	}
    return ConvertToEigenvalues(total);
}

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
	_Appender appender(line);
	foreach(Spin* spin,spins) {
		vector<Interaction*> tensors = ss->GetInteractionsBySpin(spin,t);
		appender(MatlabMatrix_(Crush_(tensors,u)));
	}
	out << "Sys." << varname << " = " << line << ";" << endl;
}


string EasySpinInput::generate(SpinSystem* spinsys) const {
    ostringstream oss;

    if(spinsys != NULL) {
        string electronLine;
        vector<Spin*> electronSpins;
        vector<Spin*> nucSpins;

        //Step 1, divide the spin system into nuclear an electron spins
        foreach(Spin* spin,spinsys->GetSpins()) {
            if(spin->GetElement() == 0) {
                electronSpins.push_back(spin);
            } else {
                nucSpins.push_back(spin);
            }
        }

        oss << "Sys.S = ";
        if(electronSpins.size() != 1) oss << "[";
        for(unsigned long i = 0;i<electronSpins.size();i++) {
            oss << "1/2 ";
        }
        if(electronSpins.size() != 1) oss << "];" << endl;

        if(electronSpins.size() == 0) {
            oss << "%Warning, no electron spins in spin system" << endl;
        }
        foreach(Spin* nucSpin,nucSpins) {
            oss << "Sys = nucspinadd(Sys,'"
                << (nucSpin->GetIsotope()+nucSpin->GetElement()) 
                << getElementSymbol(nucSpin->GetElement()) << "',";

            ostringstream APart; //In the form [Axx,Ayy,Azz],[alpha,beta,gamma]
            ostringstream QPart; //In the form [Qxx,Qyy,Qzz],[alpha,beta,gamma]

            bool useQ = false;

            foreach(Spin* eSpin,electronSpins) {
                Eigenvalues a_tensor = Crush_2(spinsys->GetInteractionsBySpin(nucSpin,eSpin,Interaction::HFC),MHz);
                EulerAngles ea = a_tensor.mOrient.GetAsEuler();

                Eigenvalues q_tensor = Crush_2(spinsys->GetInteractionsBySpin(nucSpin,eSpin,Interaction::QUADRUPOLAR),MHz);
                EulerAngles ea_q = q_tensor.mOrient.GetAsEuler();
            
                APart << "[" << a_tensor.xx << "," << a_tensor.yy << "," << a_tensor.zz << "]" << "," 
                      << "[" << ea.alpha/PI*180 << "," << ea.beta/PI*180 << "," << ea.gamma/PI*180 << "]*PI/180";
                QPart << "[" << q_tensor.xx << "," << q_tensor.yy << "," << q_tensor.zz << "]" << "," 
                      << "[" << ea_q.alpha/PI*180 << "," << ea_q.beta/PI*180 << "," << ea_q.gamma/PI*180 << "]*PI/180";

                if(q_tensor.xx != 0 || q_tensor.yy != 0 || q_tensor.zz != 0) {
                    useQ = true;
                }
            }
            string APartStr = APart.str();
            string QPartStr = QPart.str();

            if(useQ) {
                oss << APartStr << "," << QPartStr << ")" << endl;
            } else {
                oss << APartStr << ")" << endl;
            }
        }

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
        /*
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
          oss << "Sys.ee = [" << eeTensorLine << "];" << endl;
        */
        oss << endl;
    }
    oss << "Exp.mwFreq = " << mMWFreq << ";" << endl;
    oss << "Exp.CenterSweep = [" << mCentre << " " << mSweep << "];" << endl;
    oss << endl;
    oss << "Exp.Harmonic = " << mHarmonic << ";" << endl;
    if(mNPoints) {
        oss << "Exp.nPoints = "  << mNPoints.get() << ";" << endl;
    }
    oss << "Exp.Mode = '"     << mModeNames[mMode] << "';" << endl;
    if(mMWPhase) {
        oss << "Exp.mwPhase = "  << mMWPhase.get() << ";" << endl;
    }

    oss << endl;
    if(mModAmp) {
        oss << "Exp.ModAmp = " << mModAmp.get() << ";" << endl;}
    if(mTemperature) {
        oss << "Exp.Temperature = " << mTemperature.get() << ";" << endl;}

    //Brodernings
    if(mGaussianFWHM && mLorentFWHM) {
        oss << "Sys.lw = [" << mGaussianFWHM.get() << "," << mLorentFWHM.get() << "]" << endl;
    } else if(mGaussianFWHM) {
        oss << "Sys.lw = " << mGaussianFWHM.get() << ";" << endl;
    } else if(mLorentFWHM) {
        oss << "Sys.lw = [0," << mLorentFWHM.get() << "]" << endl;
    }

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
    //To does make sense to use MOND if none of the Lambdas are set
    if(mMOND && (mLambda20 || mLambda22 || mLambda40 || mLambda42 || mLambda44)) {
        oss << "Exp.MOND = 1;" << endl;
    }

    if(mEasySpinFunc == CHILI) {
        if(mLambda20 || mLambda22 || mLambda40 || mLambda42 || mLambda44) {
            oss << "Sys.lambda = ["
                << orDefault(mLambda20,0) << ","
                << orDefault(mLambda22,0) << ","
                << orDefault(mLambda40,0) << ","
                << orDefault(mLambda42,0) << ","
                << orDefault(mLambda44,0)
                << "]" << endl;
        }

        if(!mTCorrUseLog) {
            if(mTCorr) {
                oss << "Sys.tcorr = " << mTCorr.get() <<  ";" << endl;
            } else  if(mTCorrXY || mTCorrAxialZ) {
                oss << "Sys.tcorr = [" << orDefault(mTCorrXY,0) << ","
                    << orDefault(mTCorrAxialZ,0) << "];" << endl;
            } else if(mTCorrX || mTCorrY || mTCorrZ) {
                oss << "Sys.tcorr = [" << orDefault(mTCorrX,0) << ","
                    << orDefault(mTCorrY,0) << ","
                    << orDefault(mTCorrZ,0) << "];" << endl;
            }
        } else {
            if(mTCorr) {
                oss << "Sys.logtcorr = " << mTCorr.get() <<  ";" << endl;
            } else if(mTCorrXY || mTCorrAxialZ) {
                oss << "Sys.logtcorr = [" << orDefault(mTCorrXY,0) << ","
                    << orDefault(mTCorrAxialZ,0) <<  "];" << endl;
            } else if(mTCorrX || mTCorrY || mTCorrZ) {
                oss << "Sys.logtcorr = [" << orDefault(mTCorrX,0) << ","
                    << orDefault(mTCorrY,0) << ","
                    << orDefault(mTCorrZ,0) << "];" << endl;
            }
            if(mExchange) {
                oss << "Sys.Exchange = " << mExchange.get() << ";" << endl;
            }
        }
        if(mTCorrXY || mTCorrAxialZ || mTCorrX || mTCorrY || mTCorrZ) {
            //If we ouputed principle components of the rotatinal
            //correlation potential we need to output the orientation
            //too
            EulerAngles ea = mDynamicsOrient.GetAsEuler();
            oss << "Diffpa = [" << ea.alpha/PI*180 << ", " << ea.beta/PI/180 << ", " << ea.gamma/PI/180 << "]/180*pi;" << endl;
        }
        if(mExchange) {
            oss << "Sys.Exchange = " << mExchange.get() << ";" << endl;
        }

    }
    
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

