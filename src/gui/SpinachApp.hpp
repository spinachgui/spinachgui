

#ifndef SPINACHAPP_H
#define SPINACHAPP_H

#include <wx/app.h>
#include <gui/SpinSysManager.hpp>
#include <shared/spinsys.hpp>

class SpinachApp : public wxApp {
public:
    ~SpinachApp();
    virtual bool OnInit();
    SpinXML::SpinSystem* GetSS() const {return mSS;}
    const std::vector<SpinXML::ISpinSystemLoader*>& GetIOFilters() {return mIOFilters;}
private:
    SpinXML::SpinSystem* mSS;
    std::vector<SpinXML::ISpinSystemLoader*> mIOFilters;
};



DECLARE_APP(SpinachApp);

//Define macros for accessing the most up to date spin system
#define GetSS() (wxGetApp().GetSS())
#define Chkpoint(x) 

#endif
