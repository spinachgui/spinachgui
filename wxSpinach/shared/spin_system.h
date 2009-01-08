
#ifndef SPIN_SYSTEM_H
#define SPIN_SYSTEM_H

#include <string>
#include <vector>

#include "../shared/spin.h"

using namespace std;

class SpinSystem {
public:
    SpinSystem(){};

    void addSpin(Spin S);
    vector<Spin>* GetSpins();

protected:
    vector<Spin> mSpins;
};


#endif
