#ifndef RIGHT_CLICK_MENU_H
#define RIGHT_CLICK_MENU_H

#include <wx/menu.h>
#include <shared/spinsys.hpp>

///Class for easily building the right click menu. Just construct it,
///and the required options, call build and show it. The menu should
///take care of ensuing the required actions are carried out.
class RightClickMenu : public wxMenu {
public:
	RightClickMenu(wxWindow* parent);

	//Event Handlers
	void OnDeleteHover(wxCommandEvent& e);
	void OnShowSpinProperties(wxCommandEvent& e);
	void OnDeleteSelection(wxCommandEvent& e);

	///Call to build the menu. inserts the menu items in a sensible
	///order.
	void Build();
protected:
	DECLARE_EVENT_TABLE();
	wxWindow* mParent;
	SpinXML::Spin* mHoverAtTimeOfOpening;
};


#endif RIGHT_CLICK_MENU_H
