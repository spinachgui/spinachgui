

#ifndef SPINACHAPP_H
#define SPINACHAPP_H

#include <wx/app.h>
#include <shared/spinsys.hpp>
#include <assert.h>

#include <gui/SpinSysManager.hpp>

#include <set>

class SelectionManager;

class SpinachApp : public wxApp {
public:
    ~SpinachApp();
    virtual bool OnInit();
    SpinXML::SpinSystem* GetSS() const {return mSS;}
    SelectionManager* GetSelectionManager() const {return mSelectionManager;}
    const std::vector<SpinXML::ISpinSystemLoader*>& GetIOFilters() {return mIOFilters;}

    sigc::signal<void> sigDying;
private:
    SpinXML::SpinSystem* mSS;
    std::vector<SpinXML::ISpinSystemLoader*> mIOFilters;
    SelectionManager* mSelectionManager;
};

#include <map>
#include <sigc++/sigc++.h>

///A singleton class which keeps track of which objects are currently
///selected or are being hovered over. (At the moment only spins can
///be selected)
class SelectionManager : public sigc::trackable {
public:
    SelectionManager();
    ~SelectionManager();

    void SetHover(SpinXML::Spin* spin);
    void SetSelection(SpinXML::Spin* spin);
    void SetSelection(const std::set<SpinXML::Spin*>& selection);
	void AddSelection(SpinXML::Spin* spinToAdd);
	bool IsSelected(SpinXML::Spin* spin);
	void RemoveSelection(SpinXML::Spin* spin);

    SpinXML::Spin* GetHover() const {return mHover;}
    const std::set<SpinXML::Spin*>& GetSelection() const {return mSelection;}

    static SelectionManager* Instance();

    sigc::signal<void,SpinXML::Spin*>               sigHover;
    sigc::signal<void,std::set<SpinXML::Spin*> > sigSelectChange;
private:
    SpinXML::Spin* mHover;
    std::set<SpinXML::Spin*> mSelection;
    static SelectionManager* static_this;
};

///Fakes the get app macro
SpinachApp& wxGetApp();

//Define macros for accessing the most up to date spin system
#define GetSelMgr() (wxGetApp().GetSelectionManager())
#define GetSS() (wxGetApp().GetSS())
#define Chkpoint(x) 

#endif
