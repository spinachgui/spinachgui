
#ifndef SPINDIALOG_H
#define SPINDIALOG_H

#include <auto/SpinachGUI.h>
#include <shared/spin.hpp>

class SpinInterEditPanel;

using namespace std;

class SpinDialog : public SpinDialogBase {
public:
	SpinDialog(wxWindow* parent,SpinXML::SpinView spin,wxWindowID id= -1); 

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
	SpinXML::SpinView mSpin;
	SpinInterEditPanel* mInterEdit;
};



#endif

