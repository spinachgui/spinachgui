#include <gui/RootFrame.hpp>



#include <gui/InterDisplaySettings.hpp>
#include <gui/RightClickMenu.hpp>
#include <gui/SpinachApp.hpp>
#include <gui/SpinGrid.hpp>
#include <gui/SpinInteractionEdit.hpp>

#include <gui/SpinsysDisplay3D.hpp>

#include <shared/spinsys.hpp>

#include <stdexcept>
#include <wx/log.h>
#include <wx/statusbr.h>
#include <wx/treectrl.h>
#include <wx/aui/aui.h>
#include <shared/foreach.hpp>
#include <shared/nuclear_data.hpp>

#include <gui/EasySpinFrame.hpp>
#include <shared/log.hpp>

//Input and output filters
#define ID_UNIT_START 12345
#define ID_ELEMENT_START 20000
#define ID_ELEMENT_END   20500   //A periodic table up to element 500
                                                                 //should be good enough for anyone

using namespace std;
using namespace SpinXML;
using sigc::bind;
using sigc::mem_fun;
//============================================================//
// Utility Functions

wxString GetExtension(const wxString& filename) {
    int dot=filename.find(wxT("."));
    wxString ext=(dot != wxNOT_FOUND ? filename.Mid(dot+1) : wxT(""));
    return ext;
}

//============================================================//
// Reference frame tree view

class RCActionActivateFrame : public RightClickAction {
public:
    RCActionActivateFrame(Frame* frame) 
        : RightClickAction(wxT("Activate Frame")), mFrame(frame) {
    }
    bool Visible() const {return true;}
    void Exec(wxCommandEvent& e) {
        SetFrame(mFrame);
    }
private:
    Frame* mFrame;
};

//Quick class working with the wxWidgets clientData system
struct FramePointer : public wxTreeItemData {
    FramePointer(Frame* frame)
        : frame(frame) {
    }
    Frame* frame;
};

//Class for drawing the tree of reference frames.
class FrameTree : public wxTreeCtrl , public sigc::trackable {
public:

    FrameTree(wxWindow* parent) : wxTreeCtrl(parent) {
        TRACE("Creating the FrameTree");
        mRoot = AddRoot(wxT("Molecular Frame"),-1,-1,new FramePointer(GetRawSS()->GetLabFrame()));


        RefreshFromSpinSystem();
        sigFrameChange.connect(mem_fun(this,&FrameTree::SlotFrameChange));
        TRACE("Finished creating the frame tree");
    }
        
    void RefreshFromSpinSystem() {
        mapFrameToId.clear();
        mapFrameToId[GetRawSS()->GetLabFrame()] = mRoot;

        RefreshFromSpinSystemRecursive(mRoot,GetRawSS()->GetLabFrame());

        mActive = mapFrameToId[GetFrame()];
        SetItemBold(mActive);
        Refresh(); //Seems like we need to explicitly ask for a
        //repaint
    }

    void SlotFrameChange(Frame* frame) {
        SetItemBold(mActive,false);
        mActive = mapFrameToId[frame];
        SetItemBold(mActive);
        Refresh();
    }
private:
    void RefreshFromSpinSystemRecursive(wxTreeItemId itemId,Frame* frame) {
        mapFrameToId[frame] = itemId;
        vector<Frame*> children = frame->GetChildren();
        for(vector<Frame*>::iterator i = children.begin();i != children.end();++i) {
            wxTreeItemId nextItemId = AppendItem(itemId,wxT("Frame"),-1,-1,new FramePointer(*i));
            RefreshFromSpinSystemRecursive(nextItemId,*i);
        }
    }
    
    void OnRightClick(wxTreeEvent& e) {
        FramePointer* fp = (FramePointer*) GetItemData(e.GetItem());
        RightClickMenu* menu = new RightClickMenu(this);

        vector<RightClickAction*> actions;
        actions.push_back(new RCActionActivateFrame(fp->frame));

        menu->Build(actions);
        PopupMenu(menu);
        delete menu;
    }
    DECLARE_EVENT_TABLE();
    wxTreeItemId mRoot;
    wxTreeItemId mActive;
    map<Frame*,wxTreeItemId> mapFrameToId;
};


BEGIN_EVENT_TABLE(FrameTree,wxTreeCtrl)

EVT_TREE_ITEM_RIGHT_CLICK(wxID_ANY, FrameTree::OnRightClick)

END_EVENT_TABLE()


//============================================================//
// Custom Status Bar

class StatusBar : public wxStatusBar,public sigc::trackable {
public:
    StatusBar(wxWindow* parent) : wxStatusBar(parent) {
        TRACE("StatusBar::StatusBar");
        int widths_field[] = {-1,80,80};
        SetFieldsCount(3,widths_field);
        SlotLengthUnitChange(GetLengthUnit());
        TRACE("Done StatusBar::StatusBar");
    }

    void SlotLengthUnitChange(unit u) {
        wxString str = wxString(u.get_name().c_str(),wxConvUTF8);
        SetStatusText(str,1);
    }

    void SlotInterFocusChange(Interaction* inter) {
        if(inter == NULL) {
            SetStatusText(wxT(""),2);
            return;
        }
        Interaction::Type t = inter->GetType();
        wxString str = wxString(GetInterUnit(t).get_name().c_str(),wxConvUTF8);
        SetStatusText(str,2);
    }
    void SlotSizeChange(Interaction::Type t,double s) {

    }
};

//============================================================//

class InterDisplaySettingsPanel : public wxPanel,public sigc::trackable {
public:
    InterDisplaySettingsPanel(wxWindow* parent) : wxPanel(parent) {
        wxBoxSizer* bs=new wxBoxSizer(wxVERTICAL);

        //HACK: Quick hack to iterate though an enum
        for(int i = Interaction::HFC;i != Interaction::TYPE_END;++i) {
            Interaction::Type type = (Interaction::Type)i;
            InterDisplaySettings* widget = new InterDisplaySettings(this,type);
            bs->Add(widget,1,wxEXPAND);
        }

        this->SetSizer(bs);
    }
};


//============================================================//
// RootFrame

void RootFrame::InitFrame() {
    TRACE("RootFrame::InitFrame");
    //Setup the status bar
    StatusBar* statusBar = new StatusBar(this);
    SetStatusBar(statusBar);

    //We start with bonds shown
    mRootToolbar->ToggleTool(ID_BOND_TOGGLE,true);

    //We need to use Connect to set up an event handler for the
    //selection->element trigger expicitly
    wxObjectEventFunction afterCastElement = 
        (wxObjectEventFunction)(wxEventFunction)(&RootFrame::OnElementSelect);
    Connect(ID_ELEMENT_START,ID_ELEMENT_END,wxEVT_COMMAND_MENU_SELECTED,afterCastElement);

    //Set up the AUI, including the view menu function
    TRACE("Creating the panes/panels");
    mAuiManager=new wxAuiManager(this);

    mInterSizePanel= new InterDisplaySettingsPanel(this);
    mSpinGrid      = new SpinGrid(this);
    mSpinInterEdit = new SpinInterEditPanel(this);
    mDisplay3D     = new SpinsysDisplay3D(this);
    mFrameTree     = new FrameTree(this);
    TRACE("Done creating the panels/panes");

    // add the panes to the manager
    wxAuiPaneInfo display3dinfo;
    display3dinfo.Center();
    display3dinfo.CaptionVisible(false);
    display3dinfo.CloseButton(false);
    display3dinfo.Movable(false);
    display3dinfo.FloatingSize(wxSize(600,600)); //Workaround for http://trac.wxwidgets.org/ticket/12490
    mAuiManager->AddPane(mDisplay3D,display3dinfo);

    wxAuiPaneInfo spinGridInfo;
    spinGridInfo.Bottom();
    spinGridInfo.Caption(wxT("Spins"));
    spinGridInfo.FloatingSize(wxSize(600,600));//Workaround for http://trac.wxwidgets.org/ticket/12409
    mAuiManager->AddPane(mSpinGrid,spinGridInfo);

    wxAuiPaneInfo frameInfo;
    frameInfo.Right();
    frameInfo.Hide();
    frameInfo.Caption(wxT("Reference Frames"));
    frameInfo.FloatingSize(wxSize(300,400));
    mAuiManager->AddPane(mFrameTree,frameInfo);

    wxAuiPaneInfo tensorVisinfo;
    tensorVisinfo.Caption(wxT("Tensor Visualisation"));
	tensorVisinfo.BestSize(wxSize(300,600));
    tensorVisinfo.FloatingSize(wxSize(300,600)); //Workaround for http://trac.wxwidgets.org/ticket/12490
    mAuiManager->AddPane(mInterSizePanel,tensorVisinfo);

    wxAuiPaneInfo spinInterEditInfo;
    spinInterEditInfo.Bottom();
    spinInterEditInfo.Caption(wxT("Interactions"));
    spinInterEditInfo.FloatingSize(wxSize(600,400));//Workaround for http://trac.wxwidgets.org/ticket/12490
    mAuiManager->AddPane(mSpinInterEdit,spinInterEditInfo);

    //Setup the menu checks to reflect the default
    mMenuItemGrid->Check(true);
    mMenuItemTensorVis->Check(false);
    mMenuItemIntEdit->Check(true);
    mMenuItemFrames->Check(true);


    //Connect up the signals
    mSpinGrid->sigSelect.connect(sigc::mem_fun(mSpinInterEdit,&SpinInterEditPanel::SetSpin));
    sigLengthUnitChange.connect(sigc::mem_fun(statusBar,&StatusBar::SlotLengthUnitChange));

    mSpinInterEdit->GetInterEdit()->sigInterSet.connect(sigc::mem_fun(statusBar,&StatusBar::SlotInterFocusChange));

    //Units menu. To avoid writing an On* function for every unit
    //(which would make making units configurable impossible) we
    //connect them all to the same handler and setup a lookup for
    //translating the id into a unit and physical dimension.

    mIdToUnit.push_back(Angstroms);  //Default
    mIdToUnit.push_back(nanometre);
    mIdToUnit.push_back(BohrRadius);

    mIdToUnit.push_back(metres);

    for(unsigned long i = 0;i<mIdToUnit.size();i++) {
        unit u = mIdToUnit[i];
        mMenuLength->AppendRadioItem(ID_UNIT_START+i,wxString(u.get_name().c_str(),wxConvUTF8));
    }
    wxObjectEventFunction afterCast = 
        (wxObjectEventFunction)(wxEventFunction)(&RootFrame::OnUnitChange);
    Connect(ID_UNIT_START,ID_UNIT_START+mIdToUnit.size(),wxEVT_COMMAND_MENU_SELECTED,afterCast);

    //Setup the easyspin exporter
    mEasySpin = new EasySpinFrame(this);
    TRACE("Finished RootFrame::InitFrame");
}

void RootFrame::OnElementSelect(wxCommandEvent& e) {
    int element = e.GetId() - ID_ELEMENT_START;
    TRACE("RootFrame::OnElementSelect, element = " << element);

    ClearSelection();
    vector<Spin*> spins = GetRawSS()->GetSpins();
    foreach(Spin* spin,spins) {
        if(spin->GetElement() == element) {
            AddSelection(spin);
        }
    }
}

//Currently we only handle length changes
void RootFrame::OnUnitChange(wxCommandEvent& e) {
    unit u = mIdToUnit[e.GetId()-ID_UNIT_START];
    SetLengthUnit(u);
}

void RootFrame::OnNew(wxCommandEvent& e) {
    TRACE("RootFrame::OnNew");
    GetRawSS()->Clear();
    mOpenPath=wxT("Untitled");
    mOpenFile=wxT("Untitled");
    mOpenDir=wxT("");
    UpdateTitle();
}

void RootFrame::OnSupress(wxCommandEvent& e) {
    TRACE("RootFrame::OnSupress");
    SetSupressedSpins(GetSelection());
}

void RootFrame::OnUnSupress(wxCommandEvent& e) {
    TRACE("RootFrame::OnUnSupress");
    set<Spin*> empty; //Empty set
    SetSupressedSpins(empty);
}


void RootFrame::UpdateTitle() {
    SetTitle(wxString() << mOpenFile << wxT(" - Spinach (") << mOpenPath << wxT(")"));
}

void RootFrame::OnOpen(wxCommandEvent& e) {
    TRACE("RootFrame::OnOpen");
    wxString filter;
    bool first=true;
    foreach(ISpinSystemLoader* loader,GetIOFilters()) {
        if(loader->GetFilterType()==ISpinSystemLoader::LOAD ||
           loader->GetFilterType()==ISpinSystemLoader::LOADSAVE) {
            if(!first) {
                filter << wxT("|");
            } else {
                first=false;
            }
            wxString ThisFilter(loader->GetFilter(),wxConvUTF8);
            wxString ThisFilterName(loader->GetFormatName(),wxConvUTF8);
            filter << ThisFilterName << wxT(" (*.") << ThisFilter << wxT(")|*.") << ThisFilter;
        }
    }
    wxFileDialog* fd=new wxFileDialog(this,
                                      wxString(wxT("Choose a file")),
                                      wxString(wxT("")), //Default file
                                      wxString(wxT("")), //Default dir
                                      wxString(filter) ,
                                      wxFD_OPEN);
    if(fd->ShowModal() == wxID_OK) {
        LoadFromFile(mOpenPath=fd->GetPath(),mOpenDir=fd->GetDirectory(),fd->GetFilename());
    }
    TRACE("Done RootFrame::OnOpen");
}

void RootFrame::LoadFromFile(const wxString& path,const wxString& dir, const wxString& filename) {
    TRACE("RootFrame::LoadFromFile");

    mOpenPath=path;
    mOpenFile=filename;
    mOpenDir=dir;
    wxString ext=GetExtension(mOpenFile).Lower();

    ISpinSystemLoader* loader=NULL;
    foreach(ISpinSystemLoader* iofilter,GetIOFilters()) {
        if(iofilter->GetFilterType()==ISpinSystemLoader::LOAD ||
           iofilter->GetFilterType()==ISpinSystemLoader::LOADSAVE) {
            if(ext==wxString(iofilter->GetFilter(),wxConvUTF8).Lower()) {
                loader=iofilter;
                break;
            }
        }
    }
    if(loader==NULL) {
        wxLogError(wxString() << wxT("Unknown extension ") << ext);
    } else {
        try {
            GetRawSS()->LoadFromFile(mOpenPath.char_str(),loader);
        } catch(const runtime_error& e) {
            wxString msg(e.what(),wxConvUTF8);
            wxLogError(wxString() << wxT("File is corrupt:\n") << msg);

        }
    }
    SetFrame(GetRawSS()->GetLabFrame());
    Chkpoint(wxT("Load File"));
    UpdateTitle();

    TRACE("Done RootFrame::LoadFromFile");
}

void RootFrame::OnSave(wxCommandEvent& e) {
    TRACE("RootFrame::OnSave");
    if(GetExtension(mOpenFile) == wxT("xml")) {
        SaveToFile(mOpenPath,GetLoaderFromExtension("xml"));
    } else {
        SaveAs();
    }
    TRACE("Done RootFrame::OnSave");
}

void RootFrame::OnSaveAs(wxCommandEvent& e) {
    TRACE("RootFrame::OnSaveAs");
    SaveAs();
}

void RootFrame::SaveAs() {
    TRACE("RootFrame::OnSave");
    wxString filter;
    bool first=true;
    foreach(ISpinSystemLoader* ioFilter,GetIOFilters()) {
        if(ioFilter->GetFilterType()==ISpinSystemLoader::SAVE ||
           ioFilter->GetFilterType()==ISpinSystemLoader::LOADSAVE) {
            if(!first) {
                filter << wxT("|");
            } else {
                first=false;
            }
            wxString ThisFilter(ioFilter->GetFilter(),wxConvUTF8);
            wxString ThisFilterName(ioFilter->GetFormatName(),wxConvUTF8);
            filter << ThisFilterName << wxT(" (*.") << ThisFilter << wxT(")|*.") << ThisFilter;
        } 
    }
    wxFileDialog* fd=new wxFileDialog(this,
                                      wxString(wxT("Choose a file")),
                                      wxString(wxT("")), //Default file
                                      wxString(wxT("")), //Default dir
                                      filter,
                                      wxFD_SAVE);
    if(fd->ShowModal() == wxID_OK) {
        mOpenPath=fd->GetPath();
        mOpenFile=fd->GetFilename();
        mOpenDir=fd->GetDirectory();
        wxString ext=GetExtension(mOpenFile).Lower();
        ISpinSystemLoader* saver=NULL;
        foreach(ISpinSystemLoader* ioFilter,GetIOFilters()) {
            if(ioFilter->GetFilterType()==ISpinSystemLoader::SAVE ||ioFilter->GetFilterType()==ISpinSystemLoader::LOADSAVE) {
                if(ext==wxString(ioFilter->GetFilter(),wxConvUTF8).Lower()) {
                    saver=ioFilter;
                    break;
                }
            }
        }
        if(saver==NULL) {
            wxLogError(wxString() << wxT("Unknown extension ") << ext);
        } else {
            GetRawSS()->SaveToFile(mOpenPath.char_str(),saver);
        }
        UpdateTitle();
    }
    TRACE("done RootFrame::OnSave");
}

void RootFrame::OnExit(wxCommandEvent& e) {
    TRACE("RootFrame::OnExit");
    delete this;
    TRACE("Done RootFrame::OnExit");
}

void RootFrame::SaveToFile(const wxString& filename,ISpinSystemLoader* saver) {
    if(saver==NULL) {
        saver=mSaver;
    }
    GetRawSS()->SaveToFile(filename.char_str(),saver);
    mSaver=saver;
}

void RootFrame::OnNmrEpr(wxCommandEvent& e) {
	SetInterVisible(true,Interaction::HFC);
	SetInterVisible(true,Interaction::G_TENSER);
	SetInterVisible(true,Interaction::ZFS);
	SetInterVisible(true,Interaction::EXCHANGE);

	SetInterVisible(true,Interaction::SHIELDING);
	SetInterVisible(true,Interaction::SCALAR);

	SetInterVisible(true,Interaction::QUADRUPOLAR);
	SetInterVisible(true,Interaction::DIPOLAR);
	SetInterVisible(true,Interaction::CUSTOM_LINEAR);
	SetInterVisible(true,Interaction::CUSTOM_BILINEAR);
	SetInterVisible(true,Interaction::CUSTOM_QUADRATIC);
}

void RootFrame::OnNmr(wxCommandEvent& e) {
	SetInterVisible(false,Interaction::HFC);
	SetInterVisible(false,Interaction::G_TENSER);
	SetInterVisible(false,Interaction::ZFS);
	SetInterVisible(false,Interaction::EXCHANGE);

	SetInterVisible(true,Interaction::SHIELDING);
	SetInterVisible(true,Interaction::SCALAR);

	SetInterVisible(true,Interaction::QUADRUPOLAR);
	SetInterVisible(true,Interaction::DIPOLAR);
	SetInterVisible(true,Interaction::CUSTOM_LINEAR);
	SetInterVisible(true,Interaction::CUSTOM_BILINEAR);
	SetInterVisible(true,Interaction::CUSTOM_QUADRATIC);
}

void RootFrame::OnEpr(wxCommandEvent& e) {
	SetInterVisible(true,Interaction::HFC);
	SetInterVisible(true,Interaction::G_TENSER);
	SetInterVisible(true,Interaction::ZFS);
	SetInterVisible(true,Interaction::EXCHANGE);

	SetInterVisible(false,Interaction::SHIELDING);
	SetInterVisible(false,Interaction::SCALAR);

	SetInterVisible(true,Interaction::QUADRUPOLAR);
	SetInterVisible(true,Interaction::DIPOLAR);
	SetInterVisible(true,Interaction::CUSTOM_LINEAR);
	SetInterVisible(true,Interaction::CUSTOM_BILINEAR);
	SetInterVisible(true,Interaction::CUSTOM_QUADRATIC);
}


void RootFrame::OnResize(wxSizeEvent&e) {
    mAuiManager->Update();
}

void RootFrame::OnBondToggle(wxCommandEvent& e) {
    bool showBonds=e.IsChecked();
    mMenuItemBondToggle->Check(showBonds);
    mRootToolbar->ToggleTool(ID_BOND_TOGGLE,showBonds);
    SetShowBonds(showBonds);
}
             

void RootFrame::OnToggleGrid(wxCommandEvent& e) {
    AUIToggle(mSpinGrid);
}

void RootFrame::OnToggleTensorVis(wxCommandEvent& e) {
    AUIToggle(mInterSizePanel);
}

void RootFrame::OnToggleInterEdit(wxCommandEvent& e) {
    AUIToggle(mSpinInterEdit);
}

void RootFrame::OnToggleFrames(wxCommandEvent& e) {
    AUIToggle(mFrameTree);
}

void RootFrame::AUIToggle(wxWindow* p) {
    cout << "AUI Toggle" << endl;
    wxAuiPaneInfo& info = mAuiManager->GetPane(p);
    info.Show(!info.IsShown());
    mAuiManager->Update();
}

void RootFrame::OnMakeIso(wxCommandEvent& e) {
    //cleanup: put this algorithum somewhere more logical
    
    set<Spin*> selection = GetSelection();

    foreach(Spin* spin,selection) {
        vector<Interaction*> inters = GetRawSS()->GetInteractionsBySpin(spin);

        foreach(Interaction* inter,inters) {
            inter->ToScalar();
        }
    }
}


void RootFrame::OnCalcDipoles(wxCommandEvent& e) {
    GetRawSS()->CalcNuclearDipoleDipole();
}

void RootFrame::OnAxes(wxCommandEvent& e) {
    SetMonoDrawMode(MONO_AXES);
}

void RootFrame::OnEllipsoid(wxCommandEvent& e) {
    SetMonoDrawMode(MONO_ELIPSOID);
}

void RootFrame::OnMenu(wxMenuEvent& e) {
    wxMenu* menu = e.GetMenu();
    if(menu == mMenuSelection) {
        //Destory the old menu
        while(menu->GetMenuItemCount() >0) {
            menu->Destroy(menu->FindItemByPosition(0));
        }
        //Rebuild based on the list of elements present
        vector<Spin*> spins = GetRawSS()->GetSpins();
        set<int> elements;
        foreach(Spin* spin,spins) {
            elements.insert(spin->GetElement());
        }
        foreach(int element,elements) {
            wxString name(getElementName(element),wxConvUTF8);
            menu->Append(ID_ELEMENT_START + element, name);
        }
    }
}

void RootFrame::OnExportToEasyspin(wxCommandEvent& e) {
    //TODO: Does this leak memory or will RootFrame clean up?
    mEasySpin->SetSpinsys(GetRawSS());
    mEasySpin->Show();
}

BEGIN_EVENT_TABLE(RootFrame,wxFrame)

EVT_MENU_OPEN(RootFrame::OnMenu)

//File Menu
EVT_MENU(ID_NEW   ,RootFrame::OnNew   )
EVT_MENU(ID_OPEN  ,RootFrame::OnOpen  )
EVT_MENU(ID_SAVE  ,RootFrame::OnSave  )
EVT_MENU(ID_SAVEAS,RootFrame::OnSaveAs)
EVT_MENU(ID_EASYSPIN_EXPORT,RootFrame::OnExportToEasyspin)
EVT_MENU(ID_EXIT  ,RootFrame::OnExit  )

//Edit Menu
EVT_MENU(ID_EDIT_ISO,RootFrame::OnMakeIso)
//EVT_MENU(ID_CALC_DIPOLE,RootFrame::OnCalcDipoles)

//View Menu
EVT_MENU(ID_NMR_EPR,RootFrame::OnNmrEpr)
EVT_MENU(ID_NMR,    RootFrame::OnNmr)
EVT_MENU(ID_EPR,    RootFrame::OnEpr)

EVT_MENU(ID_BOND_TOGGLE,  RootFrame::OnBondToggle)

EVT_MENU(ID_VIEW_GRID,      RootFrame::OnToggleGrid)
EVT_MENU(ID_VIEW_TENSORVIS, RootFrame::OnToggleTensorVis)
EVT_MENU(ID_VIEW_TENSORVIS, RootFrame::OnToggleInterEdit)
EVT_MENU(ID_VIEW_FRAMES,    RootFrame::OnToggleFrames)

EVT_MENU(ID_SUPRESS_SELECTION,RootFrame::OnSupress)
EVT_MENU(ID_UNSUPRESS        ,RootFrame::OnUnSupress)

EVT_MENU(ID_AXES,             RootFrame::OnAxes)
EVT_MENU(ID_ELLIPSOIDS,      RootFrame::OnEllipsoid)

//Selection Menu
//Explicitly set up with Connect in the constructor

//Resize
EVT_SIZE(RootFrame::OnResize)

EVT_ERASE_BACKGROUND(RootFrame::OnEraseBackground)

END_EVENT_TABLE()


