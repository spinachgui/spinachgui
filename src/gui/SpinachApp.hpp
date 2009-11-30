

#ifndef SPINACHAPP_H
#define SPINACHAPP_H

#include <wx/app.h>
#include <gui/SpinSysManager.hpp>
#include <shared/spinsys.hpp>

class SpinachApp : public wxApp {
public:
  ~SpinachApp();
  virtual bool OnInit();
  SpinSysManager* GetSpinSysManager() const {return mSSMgr;}
  const std::vector<SpinXML::ISpinSystemLoader*>& GetIOFilters() {return mIOFilters;}
private:
  SpinSysManager* mSSMgr;
  std::vector<SpinXML::ISpinSystemLoader*> mIOFilters;
};



DECLARE_APP(SpinachApp);

//Define macros for accessing the most up to date spin system
#define GetSSMgr() (*(wxGetApp().GetSpinSysManager()))
#define GetSS() (*(wxGetApp().GetSpinSysManager()->Get()))
#define Chkpoint(x) wxGetApp().GetSpinSysManager()->Checkpoint((x));

#endif
