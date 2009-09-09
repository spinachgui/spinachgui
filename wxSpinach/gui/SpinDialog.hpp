
#ifndef SPINDIALOG_H
#define SPINDIALOG_H

#include <shared/spinsys.hpp>
#include <res/SpinachGUI.h>
#include <gui/InteractionEdit.hpp>


using namespace std;

class SpinDialog : public SpinDialogBase {
public:
  SpinDialog(wxWindow* parent,SpinXML::Spin* spin,wxWindowID id= -1); 

  void OnApply(wxCommandEvent& e);

  int ShowModal();

  void SaveToSpin();
  void LoadFromSpin();

private:
  SpinXML::Spin* mSpin;
  SpinInterEditPanel* mInterEdit;
};



#endif

