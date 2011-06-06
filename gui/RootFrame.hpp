
#ifndef ROOT_FRAME_H
#define ROOT_FRAME_H

#include <auto/SpinachGUI.h>

#include <shared/unit.hpp>

#include <utility>
#include <vector>

class InterDisplaySettingsPanel;
class FrameTree;
class wxAuiManager;
class SpinGrid;
class Display3D;
class SpinInterEditPanel;

namespace SpinXML {
        class ISpinSystemLoader;
};

class RootFrame : public RootFrameBase {
public:
    RootFrame(wxWindow* parent) : RootFrameBase(parent) {
        SetSize(wxSize(1024,768));
        InitFrame();
    }
    
    ~RootFrame() {};

    void InitFrame();

    void LoadFromFile(const wxString& path, const wxString& dir, const wxString& filename);
    void SaveToFile(const wxString& filename,SpinXML::ISpinSystemLoader* saver);

    //Utility Functions
    void SaveAs();

    //File Menu event handlers
    void OnNew   (wxCommandEvent& e);
    void OnOpen  (wxCommandEvent& e);
    void OnSave  (wxCommandEvent& e);
    void OnSaveAs(wxCommandEvent& e);
    void OnExportToEasyspin(wxCommandEvent& e);
    void OnExit  (wxCommandEvent& e);

    //Edit Menu Event handlers
    void OnMakeIso(wxCommandEvent& e);
    void OnCalcDipoles(wxCommandEvent& e);

    //Selection Menu
    void OnElementSelect(wxCommandEvent& e);

    //View Menu Event Hanlders
    void OnNmrEpr(wxCommandEvent& e);
    void OnNmr(wxCommandEvent& e);
    void OnEpr(wxCommandEvent& e);
    void OnBondToggle(wxCommandEvent& e);

    void OnToggleGrid(wxCommandEvent& e);
    void OnToggleTensorVis(wxCommandEvent& e);
    void OnToggleInterEdit(wxCommandEvent& e);
    void OnToggleFrames(wxCommandEvent& e);

    void OnSupress(wxCommandEvent& e);
    void OnUnSupress(wxCommandEvent& e);

    void OnAxes(wxCommandEvent& e);
    void OnEllipsoid(wxCommandEvent& e);

    //Menu event, fired when a user opens a menu
    void OnMenu(wxMenuEvent& e);

    //To prevent flicker, overide this function to do nothing
    void OnEraseBackground(wxEraseEvent& e) {e.Skip(false);}

    //Resize Event
    void OnResize(wxSizeEvent& e);



    //Unit Menu
    void OnUnitChange(wxCommandEvent& e);

    DECLARE_EVENT_TABLE();

private:
    void UpdateTitle();

    wxAuiManager* mAuiManager;

    SpinGrid* mSpinGrid;
    InterDisplaySettingsPanel* mInterSizePanel;
    SpinInterEditPanel* mSpinInterEdit;
    Display3D* mDisplay3D;
    FrameTree* mFrameTree;

    SpinXML::ISpinSystemLoader* mSaver;

    ///Full path of the open file
    wxString mOpenPath;
    ///The directory containing the open file
    wxString mOpenDir;
    ///Just the name of the open file
    wxString mOpenFile;

    std::vector<unit> mIdToUnit;

    void AUIToggle(wxWindow* p);
};



#endif
