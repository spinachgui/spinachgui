
#ifndef __EASYSPIN__H__
#define __EASYSPIN__H__

#include <boost/optional.hpp>
#include <shared/orientation.hpp>

class ostream;

class EasySpin {
public:
    enum Mode {
        PARALLEL,
        PERPENDICULAR
    };
    enum SampleState {
        POWDER,
        CRYSTAL
    };
    enum Method {
        MATRIX,
        PERT1,
        PERT2
    };

    void generate(ostream& out);

private:
    //------------------------------------------------------------//
    // Experiment

    double mCentre;
    double mWidth;
    
    double mMWFreq;
    double mTemperature;

    unsigned long mNPoints;
    unsigned long mHarmonic;
    
    Mode mMode;
    SampleState mSampleState;

    //Should only be set iff mSampleState = Cystal
    boost::optional<unsigned long> mSpaceGroup;
    boost::optional<SpinXML::Orientation> mCystalOrient;
    boost::optional<SpinXML::Orientation> mCystalOrient2;

    //Valid range is 0 to 2Pi
    double mwPhase;

    //------------------------------------------------------------//
    // Options

    Method mMethod;

    //0 => unused/default
    unsigned long mNKnots;

    //0 => no interpolation
    unsigned long mInterpolate;

    enum Output {
        SUMMED,
        SEPERATE
    };
    Output mOutput;
};

#endif
