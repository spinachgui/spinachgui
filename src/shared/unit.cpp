
#include <shared/unit.hpp>

using namespace std;

//============================================================//
// Globals

std::vector<const UnitSystem*> UnitSystemStack;

void PushUnitSystem(const UnitSystem* us) {
    UnitSystemStack.push_back(us);
}

void PopUnitSystem() {
    UnitSystemStack.pop_back();
}


    //Get the quantity in SI units
template<dimension_t D>
double withUnit<D>::operator()() {
    return val;
}
    //Set the quantity based on an SI unit
template<dimension_t D>
void withUnit<D>::operator()(double si) {
    val=si;
}
