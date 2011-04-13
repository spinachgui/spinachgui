
#include <gui/RightClickMenu.hpp>
#include <gui/SpinachApp.hpp>
#include <shared/foreach.hpp>

using namespace SpinXML;
using namespace std;

//============================================================//
// RightClickAction

//-- Lambda

RCActionLambda::RCActionLambda(wxString str,CommandT command)
	: Action(str,str),mCommand(command) {
}

void RCActionLambda::Exec(wxCommandEvent& e) {
    mCommand(e);
}

//-- Delete Selection

void RCActionDeleteSelection::Exec(wxCommandEvent&e) {
    DeleteSelectedSpins();
}

bool RCActionDeleteSelection::Enabled() const {
    return GetSelectedCount() >=0;
}


//-- Delete Hover

RCActionDeleteHover::RCActionDeleteHover()
    : Action(wxT("Delete Spin (under mouse)"),wxT("Delete Spin (under mouse)")),
      mHoverAtTimeOfOpening(GetHover()) {
}

void RCActionDeleteHover::Exec(wxCommandEvent& e) {
    delete mHoverAtTimeOfOpening;
}


//============================================================//
// PopupMenu


RightClickMenu::RightClickMenu(wxWindow* parent)
    : wxMenu(),mParent(parent) {
    Connect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(RightClickMenu::OnSelect));
}

RightClickMenu::~RightClickMenu() {
    for(std::vector<Action*>::iterator i = mActions.begin();i!=mActions.end();++i) {
		delete *i;
    }
}

void RightClickMenu::Build(std::vector<Action*> actions) {
    mActions = actions;
    int id = 0;
    foreach(Action* action,actions) {
		Append(id, action->GetText());
		id++;
    }
}

void RightClickMenu::OnSelect(wxCommandEvent& e) {
    mActions[e.GetId()]->Exec(e);
}


void RightClickMenu::Build() {
    vector<Action*> actions;
    actions.push_back(new RCActionDeleteHover                );
    actions.push_back(new RCActionDeleteSelection	     );
    Build(actions);
}


