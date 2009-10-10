

#ifndef SPINACHAPP_H
#define SPINACHAPP_H

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <wx/app.h>
#include <wx/grid.h>
#include <wx/aui/auibook.h>

#include <res/SpinachGUI.h>

#include <gui/SpinSysManager.hpp>
#include <gui/EventSystem.hpp>
#include <gui/glDisplay.hpp>
#include <gui/SpinGrid.hpp>

#include <shared/spinsys.hpp>

using namespace SpinXML;

class RootFrame : public RootFrameBase,public IEventListener {
public:
  RootFrame(wxWindow* parent) : RootFrameBase(parent) {
    SetSize(wxSize(1024,768));
    InitFrame();
  }
    
  ~RootFrame() {};

  void InitFrame();
  //Event handlers

  void OnUndo(wxCommandEvent& e);
  void OnRedo(wxCommandEvent& e);

  virtual bool HandleEvent(CEvent const& event);

  DECLARE_EVENT_TABLE();

private:
  wxAuiNotebook* mNotebook;
  SpinGridPanel* mSpinGridPanel;
  glDisplay* mGLDisplay;
};


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
