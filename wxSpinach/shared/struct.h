#ifndef STRUCT_H_INCLUDED
#define STRUCT_H_INCLUDED

#include <string>
#include <vector>

using namespace std;

class Spin {
public:
    Spin();
private:
};

class SpinSystem {
public:
    SpinSystem(){};

    void addSpin(Spin S);

protected:
    vector<Spin> Spins;
};



#endif // STRUCT_H_INCLUDED
