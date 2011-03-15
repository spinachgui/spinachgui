
#ifndef ROOT_FRAME_H
#define ROOT_FRAME_H

#include <auto/SpinachGUI.h>
#include <gui/Display3D.hpp>
#include <gui/SpinGrid.hpp>
#include <wx/aui/aui.h>
#include <wx/splitter.h>

#include <shared/unit.hpp>
#include <gui/EventTranslator.hpp>

class RootFrame : public RootFrameBase, public EventTranslator<RootFrame> {
	EventTranslator<RootFrame> mTranslator;
public:
    RootFrame(wxWindow* parent);
    
    ~RootFrame() {};

    void InitFrame();

    void LoadFromFile(const wxString& path, const wxString& dir, const wxString& filename);
    void SaveToFile(const wxString& filename,SpinXML::ISpinSystemLoader* saver=NULL);

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

	//Unit Menu Event Handlers
	void OnUnitChange(PhysDimenstion d,unit u,wxCommandEvent& e);

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
    SpinXML::ISpinSystemLoader* mSaver;

    ///Full path of the open file
    wxString mOpenPath;
    ///The directory containing the open file
    wxString mOpenDir;
    ///Just the name of the open file
    wxString mOpenFile;

	///translate wxEventID into a physical dimension + unit. This
	///seems to be the best way to get around the wxWidgets event
	///system without defining loads of OnUnitChange functions, one
	///for each unit
	//std::vector<std::pair<PhysDimenstion,unit> > mIdToUnit;

};



#endif
