
#include <gui/RightClickMenu.hpp>
#include <gui/SpinDialog.hpp>
#include <gui/SpinachApp.hpp>

using namespace SpinXML;
using namespace std;

//============================================================//
// PopupMenu

enum {
  MENU_SPIN_PROP,
  MENU_SPIN_DELETE
};

RightClickMenu::RightClickMenu(wxWindow* parent) : wxMenu(),
						   mParent(parent){
}

void RightClickMenu::Build() {
	unsigned int count = GetSelectedCount();
	if(count > 0) {
		wxString str = count == 1 ? wxT("Delete Spin"):wxT("Delete Spins");
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

void RightClickMenu::OnDeleteSpin(wxCommandEvent& e) {
  Chkpoint(wxT("Delete Spin"));
  DeleteSelectedSpins();
}


BEGIN_EVENT_TABLE(RightClickMenu,wxMenu)

EVT_MENU  (  MENU_SPIN_PROP,          RightClickMenu::OnShowSpinProperties)
EVT_MENU  (  MENU_SPIN_DELETE,        RightClickMenu::OnDeleteSpin)

END_EVENT_TABLE()
