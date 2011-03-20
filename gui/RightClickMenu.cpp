
#include <gui/RightClickMenu.hpp>
#include <gui/SpinDialog.hpp>
#include <gui/SpinachApp.hpp>

using namespace SpinXML;
using namespace std;

//============================================================//
// PopupMenu

enum {
  MENU_SPIN_PROP,
  MENU_SPIN_DELETE,
  MENU_DELETE_HOVER,
};

RightClickMenu::RightClickMenu(wxWindow* parent) : wxMenu(),
						   mParent(parent){
}

void RightClickMenu::Build() {
	unsigned int count = GetSelectedCount();
	mHoverAtTimeOfOpening = GetHover();
	if(mHoverAtTimeOfOpening != NULL) {
		Append(MENU_DELETE_HOVER, wxT("Delete Spin (Under Mouse)"));    
	}
	if(count > 0) {
		wxString str = count == 1 ? wxT("Delete Spin (Selected)"):wxT("Delete Spins (Selected)");
		Append(MENU_SPIN_DELETE, str);    
	}
	if (count == 1) {
		Append(MENU_SPIN_PROP, wxT("Spin Properties..."));
	}
}

void RightClickMenu::OnShowSpinProperties(wxCommandEvent& e) {
	const set<Spin*>& selection = GetSelection();
	if(selection.begin() != selection.end()) {
		SpinDialog* dialog=new SpinDialog(mParent,*(selection.begin()));
		dialog->ShowModal(); 
	}
}

void RightClickMenu::OnDeleteHover(wxCommandEvent& e) {
	delete mHoverAtTimeOfOpening;
}

void RightClickMenu::OnDeleteSpin(wxCommandEvent& e) {
  Chkpoint(wxT("Delete Spin"));
  DeleteSelectedSpins();
}



BEGIN_EVENT_TABLE(RightClickMenu,wxMenu)

EVT_MENU  (  MENU_SPIN_PROP,          RightClickMenu::OnShowSpinProperties)
EVT_MENU  (  MENU_SPIN_DELETE,        RightClickMenu::OnDeleteSpin)
EVT_MENU  (  MENU_DELETE_HOVER,       RightClickMenu::OnDeleteHover)

END_EVENT_TABLE()
