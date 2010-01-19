
#ifndef ROOT_FRAME_H
#define ROOT_FRAME_H

#include <auto/SpinachGUI.h>
#include <gui/3DDisplay.hpp>
#include <gui/SpinGrid.hpp>
#include <wx/aui/auibook.h>


class RootFrame : public RootFrameBase {
public:
  RootFrame(wxWindow* parent) : RootFrameBase(parent) {
    SetSize(wxSize(1024,768));
    InitFrame();
  }
    
  ~RootFrame() {};

  void InitFrame();

  void SaveToFile(const wxString& filename,ISpinSystemLoader* saver=NULL);

  //Utility Functions
  void SaveAs();

  //File Menu event handlers
  void OnNew   (wxCommandEvent& e);
  void OnOpen  (wxCommandEvent& e);
  void OnSave  (wxCommandEvent& e);
  void OnSaveAs(wxCommandEvent& e);
  void OnExit  (wxCommandEvent& e);

  //Edit Menu Event handlers
  void OnUndo(wxCommandEvent& e);
  void OnRedo(wxCommandEvent& e);

  //View Menu Event Hanlders
  void OnNmrEpr(wxCommandEvent& e);
  void OnNmr(wxCommandEvent& e);
  void OnEpr(wxCommandEvent& e);


  DECLARE_EVENT_TABLE();

private:
  void UpdateTitle();

  wxAuiNotebook* mNotebook;
  SpinGridPanel* mSpinGridPanel;
  Display3D* mDisplay3D;
  ISpinSystemLoader* mSaver;

  ///Full path of the open file
  wxString mOpenPath;
  ///The directory containing the open file
  wxString mOpenDir;
  ///Just the name of the open file
  wxString mOpenFile;
};



#endif
