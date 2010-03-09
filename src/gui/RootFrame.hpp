
#ifndef ROOT_FRAME_H
#define ROOT_FRAME_H

#include <auto/SpinachGUI.h>
#include <gui/Display3D.hpp>
#include <gui/SpinGrid.hpp>
#include <wx/aui/aui.h>
#include <wx/splitter.h>

class RootFrame : public RootFrameBase {
public:
    RootFrame(wxWindow* parent) : RootFrameBase(parent) {
        SetSize(wxSize(1024,768));
        InitFrame();
    }
    
    ~RootFrame() {};

    void InitFrame();

    void LoadFromFile(const wxString& path, const wxString& dir, const wxString& filename);
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
    void OnBondToggle(wxCommandEvent& e);

    //Resize Event
    void OnResize(wxSizeEvent& e);

    //Debug Menu
    void OnGLReset(wxCommandEvent& e);

    //Signals
    sigc::signal<void,bool> sigSetShowBonds;

    DECLARE_EVENT_TABLE();

private:
    void UpdateTitle();

    wxAuiManager* mAuiManager;
    SpinGrid* mSpinGrid;
    wxPanel* mInterSizePanel;
    SpinInterEditPanel* mSpinInterEdit;
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
