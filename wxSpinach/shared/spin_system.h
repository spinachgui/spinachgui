
#ifndef SPIN_SYSTEM_H
#define SPIN_SYSTEM_H

#include <string>
#include <vector>

#include "../shared/spin.h"

using namespace std;

class SpinSystem {
public:
    SpinSystem(){};

    void addSpin(SpinPnt S);
    SpinPnt GetSpin(long index);
    inline long GetSpinCount(){return mSpins.size();}

protected:
    vector<SpinPnt> mSpins;
};


#endif
