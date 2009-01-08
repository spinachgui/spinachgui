
#include "spin_system.h"

void SpinSystem::addSpin(Spin S) {
    mSpins.push_back(S);
}

vector<Spin>* SpinSystem::GetSpins() {
    return &mSpins;
}

