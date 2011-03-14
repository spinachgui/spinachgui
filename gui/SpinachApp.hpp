

#ifndef SPINACHAPP_H
#define SPINACHAPP_H

#include <wx/app.h>
#include <shared/spinsys.hpp>
#include <shared/frame.hpp>
#include <assert.h>

#include <gui/SpinSysManager.hpp>

#include <set>
#include <algorithm>

void PANIC(std::string s);


class SelectionManager;

class SpinachApp : public wxApp {
public:
    ~SpinachApp();
    virtual bool OnInit();
    SpinXML::SpinSystem* GetSpinSystem() const {return mSS;}
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

//================================================================================//
// Unit systems

const SpinXML::UnitSystem* GetUnitSystem();
extern sigc::signal<void> sigUnitSystemChange;

//================================================================================//
// Selection Manager functions

//readers
bool IsSelected(SpinXML::Spin* spin);
unsigned int GetSelectedCount();

extern sigc::signal<void,SpinXML::Spin*>            sigHover;
extern sigc::signal<void,std::set<SpinXML::Spin*> > sigSelectChange;

//Selection writers
void ClearSelection();
void SetHover(SpinXML::Spin* spin);
void SetSelection(SpinXML::Spin* spin);
void SetSelection(std::set<SpinXML::Spin*>& selection);

void AddSelection(SpinXML::Spin* spinToAdd);
void RemoveSelection(SpinXML::Spin* spin);
SpinXML::Spin* GetHover();
std::set<SpinXML::Spin*> GetSelection();

//Selection Actions
void DeleteSelectedSpins();



///Fakes the get app macro
SpinachApp& wxGetApp();

//Define macros for accessing the most up to date spin system
#define GetSS() (wxGetApp().GetSpinSystem()->GetView(NULL,GetUnitSystem()))
#define GetRawSS() (wxGetApp().GetSpinSystem())
#define Chkpoint(x) 

#endif
