

#ifndef SPINACHAPP_H
#define SPINACHAPP_H

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <wx/app.h>
#include <wx/grid.h>
#include <wx/aui/auibook.h>

#include <res/SpinachGUI.h>

#include <gui/SpinSysManager.hpp>
#include <gui/glDisplay.hpp>
#include <gui/SpinGrid.hpp>

#include <shared/spinsys.hpp>

using namespace SpinXML;

class RootFrame : public RootFrameBase {
public:
  RootFrame(wxWindow* parent) : RootFrameBase(parent) {
    SetSize(wxSize(1024,768));
    InitFrame();
  }
    
  ~RootFrame() {};

  void InitFrame() {
    mNotebook=new wxAuiNotebook(mAuiPanel);

    mSpinGrid=new SpinGrid(mNotebook);

    mGLDisplay=new glDisplay(mNotebook);

    // add the panes to the manager
    mNotebook->AddPage(mGLDisplay, wxT("3D View"));
    mNotebook->AddPage(mSpinGrid, wxT("Grid View"));

    mAuiPanel->GetSizer()->Add(mNotebook,1,wxEXPAND);
  }

  //Event handlers

  void OnUndo(wxCommandEvent& e);
  void OnRedo(wxCommandEvent& e);

  DECLARE_EVENT_TABLE();

private:
  wxAuiNotebook* mNotebook;
  SpinGrid* mSpinGrid;
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

#endif
