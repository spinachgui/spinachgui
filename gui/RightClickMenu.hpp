#ifndef RIGHT_CLICK_MENU_H
#define RIGHT_CLICK_MENU_H

#include <wx/menu.h>
#include <shared/spinsys.hpp>
#include <boost/function.hpp>.
#include <gui/Action.hpp>
///Class for easily building the right click menu. Just construct it,
///and the required options, call build and show it. The menu should
///take care of ensuing the required actions are carried out.

class RightClickMenu : public wxMenu {
public:
    RightClickMenu(wxWindow* parent);
    ~RightClickMenu();

    //Build the right click menu with a default list of acitons (do
    //not use in new code)
    void Build();

    void Build(std::vector<Action*> actions);

    void OnSelect(wxCommandEvent& e);
private:
    wxWindow* mParent;
    std::vector<Action*> mActions;
};

//============================================================//
// Common Actions

///Quite often it's not worth the effort to derive an entire class
///just to add an action to a right click menu. This class uses
///boost::function
class RCActionLambda : public Action {
public:
    typedef boost::function<void(wxCommandEvent&)> CommandT;
    RCActionLambda(wxString str,CommandT command);
    virtual void Exec(wxCommandEvent& e);
    virtual bool Visible() const {return true;}
private:
    CommandT mCommand;
};

class RCActionDeleteHover : public Action {
public:
    RCActionDeleteHover();
    virtual void Exec(wxCommandEvent& e);
    virtual bool Enabled() const {return mHoverAtTimeOfOpening != NULL;}
private:
    SpinXML::Spin* mHoverAtTimeOfOpening;
};

class RCActionDeleteSelection : public Action {
public:
    RCActionDeleteSelection()
		: Action(wxT("Delete selected spins"),wxT("Delete selected spins")) {
	}
    virtual void Exec(wxCommandEvent&e);
    virtual bool Enabled() const;
};

#endif RIGHT_CLICK_MENU_H
