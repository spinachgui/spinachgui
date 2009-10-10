
#ifndef ROOT_FRAME_H
#define ROOT_FRAME_H

#include <res/SpinachGUI.h>
#include <gui/glDisplay.hpp>
#include <gui/SpinGrid.hpp>
#include <wx/aui/auibook.h>


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



#endif
