
#ifndef __EASYSPIN_CODEGN_H__
#define __EASYSPIN_CODEGN_H__

#include <string>
#include <map>
#include <vector>
#include <boost/optional.hpp>
#include <shared/orientation.hpp>

namespace SpinXML {
    class SpinSystem;
};

struct EasySpinInput {
    EasySpinInput() {
        mTCorrUseLog = false;
    }

    std::string generate(SpinXML::SpinSystem* spinsys) const;

    typedef boost::optional<double> maybeDouble;
    typedef boost::optional<long> maybeLong;
    typedef boost::optional<unsigned long> maybeULong;

    //================================================================================//
    // EasySpin function
    enum EasySpinFunc {
        GARLIC,
        CHILI,
        PEPPER
    };
    void setEasySpinFunction(EasySpinFunc easySpinFunc) {
        mEasySpinFunc = easySpinFunc;
    }
    EasySpinFunc mEasySpinFunc;

    //================================================================================//
    // Experiment
    void setCentreSweep(double centre,double sweep) {
        mCentre = centre;
        mSweep = sweep;
    }
    double mCentre;
    double mSweep;

    //----------------------------------------//
    void setMWFreq(double mwFreq) {
        mMWFreq = mwFreq;
    }
    double mMWFreq;

    //----------------------------------------//
    void setTemperature(double temperature) {
        mTemperature = temperature;
    }
    boost::optional<double> mTemperature;
    
    //----------------------------------------//
    void setModAmp(double modAmp) {
        mModAmp = modAmp;
    }
    boost::optional<double> mModAmp;


    //----------------------------------------//
    void setNPoints(maybeULong nPoints) {
        mNPoints = nPoints;
    }
    maybeULong mNPoints;
    //----------------------------------------//
    void setHarmonic(unsigned long harmonic) {
        mHarmonic = harmonic;
    }
    unsigned long mHarmonic;
    //----------------------------------------//
    enum Mode {
        PARALLEL,
        PERPENDICULAR
    };
    void setMode(Mode mode) {
        mMode = mode;
    }
    Mode mMode;
    //----------------------------------------//

    //Should only be set iff mSampleState = Cystal
    void setSpaceGroup(unsigned long spaceGroup) {
        mSpaceGroup = spaceGroup;
    }

    boost::optional<unsigned long> mSpaceGroup;
    std::vector<SpinXML::Orientation> mOrients;

    //----------------------------------------//
    //Valid range is 0 to 2Pi
    void setMWPhase(maybeDouble mwPhase) {
        mMWPhase = mwPhase;
    }
    maybeDouble mMWPhase;

    //================================================================================//
    // Options
    enum Method {
        MATRIX,
        EXACT,
        PERT1,
        PERT2,
        PERT3,
        PERT4,
        PERT5
    };
    Method mMethod;

    bool mMOND;

    void setNKnots(unsigned long nKnots) {
        mNKnots = nKnots;
    }
    //0 => unused/default
    unsigned long mNKnots;

    void setInterpolate(unsigned long interpolate) {
        mInterpolate = interpolate;
    }
    //0 => no interpolation
    unsigned long mInterpolate;

    enum Output {
        SUMMED,
        SEPERATE
    };
    void setOutput(Output output) {
        mOutput = output;
    }
    Output mOutput;

    long mEvenLMax,mOddLMax,mKMax,mMMax;

    //============================================================//
    // Broadernings
    maybeDouble mGaussianFWHM;
    maybeDouble mLorentFWHM;

    maybeDouble mHStrainX,mHStrainY,mHStrainZ;        
    maybeDouble mGStrainX,mGStrainY,mGStrainZ;        
    maybeDouble mAStrainX,mAStrainY,mAStrainZ;        
    maybeDouble mDStrainD,mDStrainE,mDStrainCorrCoeff;

    //============================================================//
    // Dynamics for chili

    maybeDouble mLambda20,mLambda22,mLambda40,mLambda42,mLambda44;

    bool mTCorrUseLog;
    
    maybeDouble mTCorr;
    maybeDouble mTCorrXY,mTCorrAxialZ;
    maybeDouble mTCorrX, mTCorrY,mTCorrZ;
    SpinXML::Orientation mDynamicsOrient;
    
    maybeDouble mExchange;

    //Static stuff
    static void staticCtor() {
        mModeNames[PARALLEL] = "parallel";
        mModeNames[PERPENDICULAR] = "perpendicular";

        mMethodNames[MATRIX] = "matrix?";
        mMethodNames[PERT1] = "pert1?";
        mMethodNames[PERT2] = "pert2?";

        mOutputNames[SUMMED] = "summed?";
        mOutputNames[SEPERATE] = "seperate?";

        mEasySpinNames[GARLIC] = "garlic";
        mEasySpinNames[CHILI] = "chili";
        mEasySpinNames[PEPPER] = "pepper";
    }
    struct __Init {
        __Init() {
            staticCtor();
        }
    };
    static std::map<Mode  ,std::string> mModeNames;
    static std::map<Method,std::string> mMethodNames;
    static std::map<Output,std::string> mOutputNames;
    static std::map<EasySpinFunc,std::string> mEasySpinNames;


private:
    static __Init __init;
};


#endif

