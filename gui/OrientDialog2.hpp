
#ifndef __ORIENT_DIALOG_2__H__
#define __ORIENT_DIALOG_2__H__

#include <auto/orientdialog.h>
#include <vector>

class OrientDialog2 : public OrientDialog2Base {
public:
    OrientDialog2(wxWindow* parent);
private:
    std::vector<TextCtrlFocus*> mEulerAngleCtrls;
    std::vector<TextCtrlFocus*> mMatrixCtrls;
    std::vector<TextCtrlFocus*> mAngleAxisCtrls;
    std::vector<TextCtrlFocus*> mQuaternionCtrls;
};

#endif
