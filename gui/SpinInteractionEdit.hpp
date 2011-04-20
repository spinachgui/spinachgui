
#ifndef SPIN_INTERACTION_EDIT_H
#define SPIN_INTERACTION_EDIT_H

#include <gui/InteractionEdit.hpp>
#include <shared/spinsys.hpp>
#include <auto/SpinachGUI.h>


class SpinInterEditPanel : public SpinInterEditPanelBase, public sigc::trackable {
public:
	SpinInterEditPanel(wxWindow* parent,wxWindowID id= -1);
	~SpinInterEditPanel();

	void SetSpin(SpinXML::Spin* spin);

	//wx events originating from within
	void OnNewButton(wxCommandEvent& e);
	void OnDeleteButton(wxCommandEvent& e);
	void OnInteractionChange(wxCommandEvent& e);

	void LoadFromSpin();

	void UpdateListBox();
	void InteractionChange();

	enum EDIT_MODE {
		EDIT_ALL,
		EDIT_LINEAR,
		EDIT_BILINEAR,
		EDIT_QUAD
	};
	void SetEditMode(EDIT_MODE mEditMode);


protected:
	void Clear();
	static wxString NameInteraction(SpinXML::Interaction* inter);
  
	DECLARE_EVENT_TABLE();

private:
	//GUI Stuff
	InterEditPanel* mInterEdit;
	///Are we editing linear, bilinear, quadrapolar or all the
	///interactions which apply to this spin
	EDIT_MODE mEditMode;

	//Data
	SpinXML::Spin* mSpin;
  
	bool mUpdatingListBox;

};

#endif
