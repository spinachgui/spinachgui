
#ifndef SPINDIALOG_H
#define SPINDIALOG_H

#include <shared/spinsys.hpp>
#include <res/SpinachGUI.h>
#include <gui/SpinInteractionEdit.hpp>


using namespace std;

class SpinDialog : public SpinDialogBase {
public:
  SpinDialog(wxWindow* parent,SpinXML::Spin* spin,wxWindowID id= -1); 

  void OnApply(wxCommandEvent& e);

  int ShowModal();

  void SaveToSpin();
  void LoadFromSpin();

  void UpdateIsotopeDropDown();

  //Event Handlers
  void OnElementChange(wxCommandEvent& e);

protected:
  DECLARE_EVENT_TABLE();

private:
  SpinXML::Spin* mSpin;
  SpinInterEditPanel* mInterEdit;
};



#endif

