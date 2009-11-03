

#ifndef SPINACHAPP_H
#define SPINACHAPP_H

#include <wx/app.h>
#include <gui/SpinSysManager.hpp>

class SpinachApp : public wxApp {
public:
  virtual bool OnInit();
  SpinSysManager* GetSpinSysManager() const {return mSSMgr;}
private:
  SpinSysManager* mSSMgr;
};


DECLARE_APP(SpinachApp);

//Define macros for accessing the most up to date spin system
#define GetSSMgr() (*(wxGetApp().GetSpinSysManager()))
#define GetSS() (*(wxGetApp().GetSpinSysManager()->Get()))
#define Chkpoint(x) wxGetApp().GetSpinSysManager()->Checkpoint((x));

#endif
