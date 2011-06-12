
#ifndef __ORIENT_DIALOG_2__H__
#define __ORIENT_DIALOG_2__H__

#include <auto/orientdialog.h>
#include <vector>
#include <boost/optional.hpp>

typedef boost::optional<SpinXML::Orientation::Type> maybeOrientType;

class OrientDialog2 : public OrientDialog2Base {
public:
    //Events
    OrientDialog2(wxWindow* parent);
    void SlotRotate(Eigen::Matrix3f rotation);
    
    //Other
    void SetOrient(const SpinXML::Orientation& orient,maybeOrientType typeToKeep = maybeOrientType());

protected:
    DECLARE_EVENT_TABLE();
    void OnEulerEdit(wxCommandEvent& e);
    void OnAngleAxisEdit(wxCommandEvent& e);
    void OnQuaternionEdit(wxCommandEvent& e);
    void OnDCMEdit(wxCommandEvent& e);
private:
    std::vector<TextCtrlFocus*> mEulerAngleCtrls;
    std::vector<TextCtrlFocus*> mMatrixCtrls;
    std::vector<TextCtrlFocus*> mAngleAxisCtrls;
    std::vector<TextCtrlFocus*> mQuaternionCtrls;
};

#endif
