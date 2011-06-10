
#include <gui/OrientDialog2.hpp>
#include <iostream>

using namespace sigc;
using namespace std;

OrientDialog2::OrientDialog2(wxWindow* parent)
    : OrientDialog2Base(parent,wxID_ANY,wxT("Orientation"),wxDefaultPosition, wxSize( 600,350 )) {
    mOrientDisplay3D->sigRotate.connect(mem_fun(this,&OrientDialog2::SlotRotate));
}

void OrientDialog2::SlotRotate(Eigen::Matrix3f rotation) {
    cout << rotation << endl;
}

