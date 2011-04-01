
#ifndef SPINDIALOG_H
#define SPINDIALOG_H

#include <auto/SpinachGUI.h>

class SpinInterEditPanel;
namespace SpinXML {
	class Spin;
	class SpinSystem;
};

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
	SpinXML::SpinSystem* mSpinSystem;
	SpinInterEditPanel* mInterEdit;
};



#endif

