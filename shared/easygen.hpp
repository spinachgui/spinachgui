
#ifndef __EASYSPIN_CODEGN_H__
#define __EASYSPIN_CODEGN_H__

#include <string>
#include <map>
#include <boost/optional.hpp>

namespace SpinXML {
    class SpinSystem;
};

struct EasySpinInput {
    EasySpinInput() {
    }

    std::string generate(SpinXML::SpinSystem* spinsys) const;

    //================================================================================//
    // EasySpin function
public:
    enum EasySpinFunc {
        GARLIC,
        CHILI,
        PEPPER
    };
    void setEasySpinFunction(EasySpinFunc easySpinFunc) {
        mEasySpinFunc = easySpinFunc;
    }
private:
    EasySpinFunc mEasySpinFunc;

    //================================================================================//
    // Experiment

public:
    void setCentreSweep(double centre,double sweep) {
        mCentre = centre;
        mSweep = sweep;
    }
private:
    double mCentre;
    double mSweep;

    //----------------------------------------//

public:
    void setMWFreq(double mwFreq) {
        mMWFreq = mwFreq;
    }
private:
    double mMWFreq;

    //----------------------------------------//

public:
    void setTemperature(double temperature) {
        mTemperature = temperature;
    }
private:
    boost::optional<double> mTemperature;
    
    //----------------------------------------//

public:
    void setModAmp(double modAmp) {
        mModAmp = modAmp;
    }
private:
    boost::optional<double> mModAmp;


    //----------------------------------------//
public:
    void setNPoints(unsigned long nPoints) {
        mNPoints = nPoints;
    }
private:
    unsigned long mNPoints;
    //----------------------------------------//
public:
    void setHarmonic(unsigned long harmonic) {
        mHarmonic = harmonic;
    }
private:
    unsigned long mHarmonic;
    //----------------------------------------//
public:
    enum Mode {
        PARALLEL,
        PERPENDICULAR
    };
    void setMode(Mode mode) {
        mMode = mode;
    }
private:
    Mode mMode;
    //----------------------------------------//

    //Should only be set iff mSampleState = Cystal
public:
    void setSpaceGroup(unsigned long spaceGroup) {
        mSpaceGroup = spaceGroup;
    }
private:
    boost::optional<unsigned long> mSpaceGroup;
    //----------------------------------------//
public:
    //TODO: Crystal Orientations
private:
    //----------------------------------------//
    //Valid range is 0 to 2Pi
public:
    void setMWPhase(unsigned long mwPhase) {
        mMWPhase = mwPhase;
    }
private:
    double mMWPhase;

    //================================================================================//
    // Options
public:
    enum Method {
        MATRIX,
        PERT1,
        PERT2
    };
    void setMethod(Method method) {
        mMethod = method;
    }
private:
    Method mMethod;

public:
    void setNKnots(unsigned long nKnots) {
        mNKnots = nKnots;
    }
private:
    //0 => unused/default
    unsigned long mNKnots;

public:
    void setInterpolate(unsigned long interpolate) {
        mInterpolate = interpolate;
    }
private:
    //0 => no interpolation
    unsigned long mInterpolate;

public:
    enum Output {
        SUMMED,
        SEPERATE
    };
    void setOutput(Output output) {
        mOutput = output;
    }
private:
    Output mOutput;

public:
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

