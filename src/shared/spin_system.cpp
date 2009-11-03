
#include "spin_system.h"

void SpinSystem::addSpin(SpinPnt S) {
    mSpins.push_back(S);
}

SpinPnt SpinSystem::GetSpin(long index) {
    return mSpins[index];
}
