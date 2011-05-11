

#ifndef SPINACHAPP_H
#define SPINACHAPP_H

#include <wx/app.h>
#include <assert.h>

#include <sigc++/sigc++.h>

#include <shared/unit.hpp>
#include <shared/frame.hpp>

#include <set>
#include <map>
#include <vector>
#include <algorithm>


namespace SpinXML {
	class Spin;
	class SpinSystem;
	class ISpinSystemLoader;
};


class SpinachApp : public wxApp {
public:
    ~SpinachApp();
    virtual bool OnInit();
    SpinXML::SpinSystem* GetSpinSystem() const {return mSS;}


    sigc::signal<void> sigDying;
private:
    SpinXML::SpinSystem* mSS;
};

//================================================================================//
// IOFilters

const std::vector<SpinXML::ISpinSystemLoader*>& GetIOFilters();

//Given a string containing a file extension, such as "xml" return a
//suitable filter
SpinXML::ISpinSystemLoader* GetLoaderFromExtension(const std::string& ext);

//================================================================================//
// Unit systems

void SetUnit(PhysDimension d,unit u);
unit GetUnit(PhysDimension d);

const SpinXML::UnitSystem* GetUnitSystem();

extern sigc::signal<void> sigUnitSystemChange;
extern sigc::signal<void,PhysDimension,unit> sigUnitChange;

//================================================================================//
// Reference frames, there is exactly on active reference frame at a time

void SetFrame(SpinXML::Frame* frame);
SpinXML::Frame* GetFrame();

extern sigc::signal<void,SpinXML::Frame*> sigFrameChange;


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
const std::set<SpinXML::Spin*>& GetSelection();

//Selection Actions
void DeleteSelectedSpins();



///Fakes the get app macro
SpinachApp& wxGetApp();

//Define macros for accessing the most up to date spin system
//#define GetSS() (wxGetApp().GetSpinSystem()->GetView(GetFrame(),GetUnitSystem()))
#define GetRawSS() (wxGetApp().GetSpinSystem())
#define Chkpoint(x) 

//Private

struct __GUI__Init {
	__GUI__Init();
	~__GUI__Init();
};

#endif
