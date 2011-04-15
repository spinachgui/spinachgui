
#ifndef ROOT_FRAME_H
#define ROOT_FRAME_H

#include <shared/unit.hpp>

#include <utility>
#include <vector>
#include <wx/event.h>
#include <wx/frame.h>

#include <map>

class InterDisplaySettingsPanel;
class FrameTree;
class wxAuiManager;
class SpinGrid;
class Display3D;
class SpinInterEditPanel;

class wxMenuBar;
class Menu;
class Action;

namespace SpinXML {
	class ISpinSystemLoader;
};

class MenuFrame : public wxFrame {
	/*
	  The entire purpose of this class is to work around a bug in
	  wxWidgets 2.8 that causes classes derived from wxMenu to be
	  unable to recieve events
	 */
public:
	MenuFrame(wxWindow* parent, wxWindowID id, const wxString& title);
	//Once an action is registed, when this class hears an event with
	//the specified id will be sent to the specified menu
	void RegisterAddActionWorkaround(Menu* menu,wxWindowID id);
	void OnClick(wxCommandEvent& e);
    DECLARE_EVENT_TABLE();
private:
	std::map<long,Menu*> mIdToMenu;
};

class RootFrame : public MenuFrame {
public:
    RootFrame(wxWindow* parent)
		: MenuFrame(parent, wxID_ANY, wxT("Spinach")) {
        SetSize(wxSize(1024,768));
        InitFrame();
    }
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

    void OnMakeIso(wxCommandEvent& e);
	void OnCalcDipoles(wxCommandEvent& e);
    //Selection Menu
    void OnCarbon(wxCommandEvent& e);
    void OnHydrogen(wxCommandEvent& e);

    //View Menu Event Hanlders
    void OnNmrEpr(wxCommandEvent& e);
    void OnNmr(wxCommandEvent& e);
    void OnEpr(wxCommandEvent& e);
    void OnBondToggle(wxCommandEvent& e);

	void OnToggle3D(wxCommandEvent& e);
	void OnToggleGrid(wxCommandEvent& e);
	void OnToggleTensorVis(wxCommandEvent& e);
	void OnToggleInterEdit(wxCommandEvent& e);
	void OnToggleFrames(wxCommandEvent& e);

	void OnSupress(wxCommandEvent& e);
	void OnUnSupress(wxCommandEvent& e);

	void OnAxes(wxCommandEvent& e);
	void OnEllipsoid(wxCommandEvent& e);


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

	std::vector<std::pair<PhysDimension,unit> > mIdToUnit;

	void AUIToggle(wxWindow* p);
};



#endif
