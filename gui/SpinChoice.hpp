
#ifndef SPIN_CHOICE_H__
#define SPIN_CHOICE_H__

#include <wx/choice.h>
#include <sigc++/sigc++.h>

/*
  A simple widget that displays all spins in the currently loaded spin
  system and emits a signal when the user selects one of them
 */

namespace SpinXML {
	class Spin;
};

class SpinChoice : public wxChoice,public sigc::trackable {
public:
	SpinChoice(wxWindow* parent,wxWindowID id,
			   const wxPoint& pos,
			   const wxSize& size,
			   const wxArrayString& choices,
			   long style = 0,
			   const wxValidator& validator = wxDefaultValidator,
			   const wxString& name = wxChoiceNameStr);
	SpinXML::Spin* GetSpin() const;
	
    sigc::signal<void,SpinXML::Spin*> sigChange;

	void SetSpin(SpinXML::Spin* spin);
	void OnChange(wxCommandEvent& e);

	void SlotAnySpinDying (SpinXML::Spin* spin);
	void SlotAnySpinChange(SpinXML::Spin* spin);
	void SlotNewSpin(SpinXML::Spin* spin,long);

    DECLARE_EVENT_TABLE();
private:
};

#endif



