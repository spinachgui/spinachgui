
#include <gui/RootFrame.hpp>
#include <gui/SpinachApp.hpp>
#include <gui/MolSceneGraph.hpp>
#include <stdexcept>
#include <wx/log.h>

//Input and output filters
#include <shared/formats/xyz.hpp>
#include <gui/InterDisplaySettings.hpp>

using namespace std;
using namespace sigc;
//============================================================//
// Utility Functions

wxString GetExtension(const wxString& filename) {
    int dot=filename.find(wxT("."));
    wxString ext=(dot != wxNOT_FOUND ? filename.Mid(dot+1) : wxT(""));
    return ext;
}

//============================================================//
// RootFrame

void RootFrame::InitFrame() {
    mAuiManager=new wxAuiManager(this);

    mInterSizePanel=new wxPanel(this);

    InterDisplaySettings* hfc_sp = new InterDisplaySettings(mInterSizePanel,Interaction::ST_HFC             );
    InterDisplaySettings* gt_sp  = new InterDisplaySettings(mInterSizePanel,Interaction::ST_G_TENSER        );
    InterDisplaySettings* zfs_sp = new InterDisplaySettings(mInterSizePanel,Interaction::ST_ZFS             );
    InterDisplaySettings* exc_sp = new InterDisplaySettings(mInterSizePanel,Interaction::ST_EXCHANGE        );
    InterDisplaySettings* shd_sp = new InterDisplaySettings(mInterSizePanel,Interaction::ST_SHIELDING       );
    InterDisplaySettings* sca_sp = new InterDisplaySettings(mInterSizePanel,Interaction::ST_SCALAR          );
    InterDisplaySettings* qp_sp  = new InterDisplaySettings(mInterSizePanel,Interaction::ST_QUADRUPOLAR     );
    InterDisplaySettings* dip_sp = new InterDisplaySettings(mInterSizePanel,Interaction::ST_DIPOLAR         );
    InterDisplaySettings* cl_sp  = new InterDisplaySettings(mInterSizePanel,Interaction::ST_CUSTOM_LINEAR   );
    InterDisplaySettings* cb_sp  = new InterDisplaySettings(mInterSizePanel,Interaction::ST_CUSTOM_BILINEAR );
    InterDisplaySettings* cq_sp  = new InterDisplaySettings(mInterSizePanel,Interaction::ST_CUSTOM_QUADRATIC);


    wxBoxSizer* bs=new wxBoxSizer(wxVERTICAL);
    bs->Add(hfc_sp,1,wxEXPAND);
    bs->Add(gt_sp ,1,wxEXPAND);
    bs->Add(zfs_sp,1,wxEXPAND);
    bs->Add(exc_sp,1,wxEXPAND);
    bs->Add(shd_sp,1,wxEXPAND);
    bs->Add(sca_sp,1,wxEXPAND);
    bs->Add(qp_sp ,1,wxEXPAND);
    bs->Add(dip_sp,1,wxEXPAND);
    bs->Add(cl_sp ,1,wxEXPAND);
    bs->Add(cb_sp ,1,wxEXPAND);
    bs->Add(cq_sp ,1,wxEXPAND);
    mInterSizePanel->SetSizer(bs);

    mSpinGrid      = new SpinGrid(this);
    mSpinInterEdit = new SpinInterEditPanel(this);
    mDisplay3D     = new Display3D(this);

    MoleculeNodeNew* mn = new MoleculeNodeNew(GetSS());

    SpinachDC& spinDC=mDisplay3D->GetDC();

    //Connect the scalling sliders to the scalling
    hfc_sp->GetLogSlider()->sigChange.connect(bind(mem_fun(spinDC,&SpinachDC::SetScalling),Interaction::ST_HFC             ));
    gt_sp ->GetLogSlider()->sigChange.connect(bind(mem_fun(spinDC,&SpinachDC::SetScalling),Interaction::ST_G_TENSER        )); 
    zfs_sp->GetLogSlider()->sigChange.connect(bind(mem_fun(spinDC,&SpinachDC::SetScalling),Interaction::ST_ZFS             ));
    exc_sp->GetLogSlider()->sigChange.connect(bind(mem_fun(spinDC,&SpinachDC::SetScalling),Interaction::ST_EXCHANGE        ));
    shd_sp->GetLogSlider()->sigChange.connect(bind(mem_fun(spinDC,&SpinachDC::SetScalling),Interaction::ST_SHIELDING       ));
    sca_sp->GetLogSlider()->sigChange.connect(bind(mem_fun(spinDC,&SpinachDC::SetScalling),Interaction::ST_SCALAR          ));
    qp_sp ->GetLogSlider()->sigChange.connect(bind(mem_fun(spinDC,&SpinachDC::SetScalling),Interaction::ST_QUADRUPOLAR     ));
    dip_sp->GetLogSlider()->sigChange.connect(bind(mem_fun(spinDC,&SpinachDC::SetScalling),Interaction::ST_DIPOLAR         ));
    cl_sp ->GetLogSlider()->sigChange.connect(bind(mem_fun(spinDC,&SpinachDC::SetScalling),Interaction::ST_CUSTOM_LINEAR   ));
    cb_sp ->GetLogSlider()->sigChange.connect(bind(mem_fun(spinDC,&SpinachDC::SetScalling),Interaction::ST_CUSTOM_BILINEAR )); 
    cq_sp ->GetLogSlider()->sigChange.connect(bind(mem_fun(spinDC,&SpinachDC::SetScalling),Interaction::ST_CUSTOM_QUADRATIC)); 

    //Connect the colour controls to the tensor colours
    hfc_sp->sigColour.connect(bind(mem_fun(spinDC,&SpinachDC::SetColour),Interaction::ST_HFC             ));
    gt_sp ->sigColour.connect(bind(mem_fun(spinDC,&SpinachDC::SetColour),Interaction::ST_G_TENSER        )); 
    zfs_sp->sigColour.connect(bind(mem_fun(spinDC,&SpinachDC::SetColour),Interaction::ST_ZFS             ));
    exc_sp->sigColour.connect(bind(mem_fun(spinDC,&SpinachDC::SetColour),Interaction::ST_EXCHANGE        ));
    shd_sp->sigColour.connect(bind(mem_fun(spinDC,&SpinachDC::SetColour),Interaction::ST_SHIELDING       ));
    sca_sp->sigColour.connect(bind(mem_fun(spinDC,&SpinachDC::SetColour),Interaction::ST_SCALAR          ));
    qp_sp ->sigColour.connect(bind(mem_fun(spinDC,&SpinachDC::SetColour),Interaction::ST_QUADRUPOLAR     ));
    dip_sp->sigColour.connect(bind(mem_fun(spinDC,&SpinachDC::SetColour),Interaction::ST_DIPOLAR         ));
    cl_sp ->sigColour.connect(bind(mem_fun(spinDC,&SpinachDC::SetColour),Interaction::ST_CUSTOM_LINEAR   ));
    cb_sp ->sigColour.connect(bind(mem_fun(spinDC,&SpinachDC::SetColour),Interaction::ST_CUSTOM_BILINEAR )); 
    cq_sp ->sigColour.connect(bind(mem_fun(spinDC,&SpinachDC::SetColour),Interaction::ST_CUSTOM_QUADRATIC)); 

    //Connect the visibility toggles to the tensor colours
    hfc_sp->sigVisible.connect(bind(mem_fun(spinDC,&SpinachDC::SetVisible),Interaction::ST_HFC             ));
    gt_sp ->sigVisible.connect(bind(mem_fun(spinDC,&SpinachDC::SetVisible),Interaction::ST_G_TENSER        )); 
    zfs_sp->sigVisible.connect(bind(mem_fun(spinDC,&SpinachDC::SetVisible),Interaction::ST_ZFS             ));
    exc_sp->sigVisible.connect(bind(mem_fun(spinDC,&SpinachDC::SetVisible),Interaction::ST_EXCHANGE        ));
    shd_sp->sigVisible.connect(bind(mem_fun(spinDC,&SpinachDC::SetVisible),Interaction::ST_SHIELDING       ));
    sca_sp->sigVisible.connect(bind(mem_fun(spinDC,&SpinachDC::SetVisible),Interaction::ST_SCALAR          ));
    qp_sp ->sigVisible.connect(bind(mem_fun(spinDC,&SpinachDC::SetVisible),Interaction::ST_QUADRUPOLAR     ));
    dip_sp->sigVisible.connect(bind(mem_fun(spinDC,&SpinachDC::SetVisible),Interaction::ST_DIPOLAR         ));
    cl_sp ->sigVisible.connect(bind(mem_fun(spinDC,&SpinachDC::SetVisible),Interaction::ST_CUSTOM_LINEAR   ));
    cb_sp ->sigVisible.connect(bind(mem_fun(spinDC,&SpinachDC::SetVisible),Interaction::ST_CUSTOM_BILINEAR )); 
    cq_sp ->sigVisible.connect(bind(mem_fun(spinDC,&SpinachDC::SetVisible),Interaction::ST_CUSTOM_QUADRATIC)); 


    //Set sensible default scallings
    hfc_sp->GetLogSlider()->SetValue(0.1);
    gt_sp ->GetLogSlider()->SetValue(1);
    zfs_sp->GetLogSlider()->SetValue(1);
    exc_sp->GetLogSlider()->SetValue(1);
    shd_sp->GetLogSlider()->SetValue(0.001);
    sca_sp->GetLogSlider()->SetValue(1);
    qp_sp ->GetLogSlider()->SetValue(1);
    dip_sp->GetLogSlider()->SetValue(1);
    cl_sp ->GetLogSlider()->SetValue(1);
    cb_sp ->GetLogSlider()->SetValue(1);
    cq_sp ->GetLogSlider()->SetValue(1);

    //Set sensible default visibility
    hfc_sp -> SetVisible(true);
    gt_sp  -> SetVisible(true);
    zfs_sp -> SetVisible(true);
    exc_sp -> SetVisible(false);
    shd_sp -> SetVisible(true);
    sca_sp -> SetVisible(false);
    qp_sp  -> SetVisible(true);
    dip_sp -> SetVisible(false);
    cl_sp  -> SetVisible(true);
    cb_sp  -> SetVisible(true);
    cq_sp  -> SetVisible(true);


    //Setup the default colours
    hfc_sp -> SetColour(0   ,0   ,1   );
    gt_sp  -> SetColour(0   ,0.7 ,0   );
    zfs_sp -> SetColour(0.7 ,0   ,0   );
    exc_sp -> SetColour(0   ,0.0 ,0.3 );
    shd_sp -> SetColour(0.4 ,0.4 ,0.4 );
    sca_sp -> SetColour(0   ,0.7 ,0   );
    qp_sp  -> SetColour(0   ,0.7 ,0   );
    dip_sp -> SetColour(0   ,0.7 ,0   );
    cl_sp  -> SetColour(0   ,0.7 ,0   );
    cb_sp  -> SetColour(0   ,0.7 ,0   );
    cq_sp  -> SetColour(0   ,0.7 ,0   );

    //Wire the DC up to the the bond toggle event
    sigSetShowBonds.connect(mem_fun(spinDC,&SpinachDC::SetShowBonds));


    mDisplay3D->SetRootSGNode(mn);

    mDisplay3D->SetRootFGNode(new MoleculeFG(GetSS()));
    // mDisplay3D->SetRootFGNode(new OpenGLText(wxT("Hello World")));

    mDisplay3D->GetDC().depthOnly=false;


    // add the panes to the manager
    wxAuiPaneInfo display3dinfo;
    display3dinfo.Center();
    display3dinfo.CaptionVisible(false);
    display3dinfo.CloseButton(false);
    display3dinfo.Movable(false);
    mAuiManager->AddPane(mDisplay3D,display3dinfo);
    mAuiManager->AddPane(mSpinGrid,wxBOTTOM,wxT("Grid View"));
    mAuiManager->AddPane(mInterSizePanel,wxLEFT,wxT("Tensor Visualisation"));
    mAuiManager->AddPane(mSpinInterEdit,wxBOTTOM,wxT("Interaction Editor"));

    //Grey the undo and redo menu ideams. They can be ungreyed when
    //there is an undo history

    mMenuItemUndo->Enable(false);
    mMenuItemRedo->Enable(false);

    //Connect up the signals
    mSpinGrid->sigSelect.connect(mem_fun(mSpinInterEdit,&SpinInterEditPanel::SetSpin));
    GetSS()->sigReloaded.connect(mem_fun(mDisplay3D,&Display3D::ResetView));
}


void RootFrame::OnUndo(wxCommandEvent& e) {

}

void RootFrame::OnRedo(wxCommandEvent& e) {

}

void RootFrame::OnNew(wxCommandEvent& e) {
    GetSS()->Clear();
    mOpenPath=wxT("Untitled");
    mOpenFile=wxT("Untitled");
    mOpenDir=wxT("");
    UpdateTitle();
}

void RootFrame::UpdateTitle() {
    SetTitle(wxString() << mOpenFile << wxT(" - Spinach (") << mOpenPath << wxT(")"));
}

void RootFrame::OnOpen(wxCommandEvent& e) {
    wxString filter;
    bool first=true;
    for(vector<ISpinSystemLoader*>::const_iterator i=wxGetApp().GetIOFilters().begin();
        i!=wxGetApp().GetIOFilters().end();
        ++i) {
        if((*i)->GetFilterType()==ISpinSystemLoader::LOAD ||
           (*i)->GetFilterType()==ISpinSystemLoader::LOADSAVE) {
            if(!first) {
                filter << wxT("|");
            } else {
                first=false;
            }
            wxString ThisFilter((*i)->GetFilter(),wxConvUTF8);
            wxString ThisFilterName((*i)->GetFormatName(),wxConvUTF8);
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

}

void RootFrame::LoadFromFile(const wxString& path,const wxString& dir, const wxString& filename) {
    mOpenPath=path;
    mOpenFile=filename;
    mOpenDir=dir;
    wxString ext=GetExtension(mOpenFile).Lower();

    ISpinSystemLoader* loader=NULL;
    typedef vector<ISpinSystemLoader*>::const_iterator itor;
    for(itor i=wxGetApp().GetIOFilters().begin();
        i!=wxGetApp().GetIOFilters().end();
        ++i) {
        if((*i)->GetFilterType()==ISpinSystemLoader::LOAD ||
           (*i)->GetFilterType()==ISpinSystemLoader::LOADSAVE) {
            if(ext==wxString((*i)->GetFilter(),wxConvUTF8).Lower()) {
                loader=*i;
                break;
            }
        }
    }
    if(loader==NULL) {
        wxLogError(wxString() << wxT("Unknown extension ") << ext);
    } else {
        try {
            GetSS()->LoadFromFile(mOpenPath.char_str(),loader);
        } catch(const runtime_error& e) {
            wxString msg(e.what(),wxConvUTF8);
            wxLogError(wxString() << wxT("File is corrupt:\n") << msg);

        }
    }
    Chkpoint(wxT("Load File"));
    UpdateTitle();
}

void RootFrame::OnSave(wxCommandEvent& e) {
    if(GetExtension(mOpenFile) == wxT("xml")) {
        SaveToFile(mOpenPath);
    } else {
        SaveAs();
    }
}

void RootFrame::OnSaveAs(wxCommandEvent& e) {
    SaveAs();
}

void RootFrame::SaveAs() {
    wxString filter;
    bool first=true;
    for(vector<ISpinSystemLoader*>::const_iterator i=wxGetApp().GetIOFilters().begin();
        i!=wxGetApp().GetIOFilters().end();
        ++i) {
        if((*i)->GetFilterType()==ISpinSystemLoader::SAVE ||
           (*i)->GetFilterType()==ISpinSystemLoader::LOADSAVE) {
            if(!first) {
                filter << wxT("|");
            } else {
                first=false;
            }
            wxString ThisFilter((*i)->GetFilter(),wxConvUTF8);
            wxString ThisFilterName((*i)->GetFormatName(),wxConvUTF8);
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
        for(vector<ISpinSystemLoader*>::const_iterator i=wxGetApp().GetIOFilters().begin();
            i!=wxGetApp().GetIOFilters().end();
            ++i) {
            if((*i)->GetFilterType()==ISpinSystemLoader::SAVE ||(*i)->GetFilterType()==ISpinSystemLoader::LOADSAVE) {
                if(ext==wxString((*i)->GetFilter(),wxConvUTF8).Lower()) {
                    saver=*i;
                    break;
                }
            }
        }
        if(saver==NULL) {
            wxLogError(wxString() << wxT("Unknown extension ") << ext);
        } else {
            GetSS()->SaveToFile(mOpenPath.char_str(),saver);
        }
        UpdateTitle();
    }
}

void RootFrame::OnExit(wxCommandEvent& e) {
    delete this;
}

void RootFrame::SaveToFile(const wxString& filename,ISpinSystemLoader* saver) {
    if(saver==NULL) {
        saver=mSaver;
    }
    GetSS()->SaveToFile(filename.char_str(),saver);
    mSaver=saver;
}

void RootFrame::OnNmrEpr(wxCommandEvent& e) {
    mMenuItemNmrEpr->Check(true);
    mRootToolbar->ToggleTool(ID_NMR_EPR,true);
}

void RootFrame::OnNmr(wxCommandEvent& e) {
    mMenuItemNmr->Check(true);
    mRootToolbar->ToggleTool(ID_NMR,true);
}

void RootFrame::OnEpr(wxCommandEvent& e) {
    mMenuItemEpr->Check(true);
    mRootToolbar->ToggleTool(ID_EPR,true);
}


void RootFrame::OnResize(wxSizeEvent&e) {
    mAuiManager->Update();
    static bool firstTime=true;
    if(firstTime) {
        firstTime=false;
        //Execute this testing code once when the main window is
        //created. Doing this in an onrezise should ensure that the window
        //is on the screne at the time.
        LoadFromFile(wxT("../../testing_kit/Gaussian/ESR spectroscopy/cpdyad_cation.log"),
                     wxT("../../testing_kit/Gaussian/ESR spectroscopy/"),
                     wxT("cpdyad_cation.log"));
        LoadFromFile(wxT("data/OFNAPH01_NMR.magres"),
                     wxT("data/"),
                     wxT("OFNAPH01_NMR.magres"));
        LoadFromFile(wxT("data/tryosine.log"),
                     wxT("data/"),
                     wxT("tryosine.log"));
        //GetSS()->CalcNuclearDipoleDipole();
        for(unsigned long i=0;i<wxGetApp().GetIOFilters().size();i++) {
            ISpinSystemLoader* saver=wxGetApp().GetIOFilters()[i];
            if(saver->GetFilterType()==ISpinSystemLoader::SAVE ||
               saver->GetFilterType()==ISpinSystemLoader::LOADSAVE) {
                GetSS()->SaveToFile((string("test.")+saver->GetFilter()).c_str(),saver);
            }
        }
        /*GetSS()->Clear();
        Spin* spin0=new Spin(Vector3l(length(0),length(0),length(0)),"0,0,0",1);
        Spin* spinx=new Spin(Vector3l(length(3e-10),length(0),length(0)),"1,0,0",6);
        Spin* spiny=new Spin(Vector3l(length(0),length(3e-10),length(0)),"0,1,0",7);
        Spin* spinz=new Spin(Vector3l(length(0),length(0),length(3e-10)),"0,0,1",8);
        //Interaction* inter0=new Interaction(eigenvalues_t(10e-6*eV,1e-6*eV,1e-6*eV  ,Orientation(AngleAxis(3.141592654/2*0.0,Vector3(1,0,0)))));
        //Interaction* inter1=new Interaction(eigenvalues_t(10e-6*eV,1e-6*eV,1e-6*eV  ,Orientation(AngleAxis(3.141592654/2*0.5,Vector3(1,0,0)))));
        Interaction* inter2=new Interaction(eigenvalues_t(10e-6*eV,1e-6*eV,1e-6*eV  ,Orientation(AngleAxis(3.141592654/2*0.5,Vector3(0.5,0.5,0)))));
        //Interaction* inter3=new Interaction(eigenvalues_t(10e-6*eV,1.1e-6*eV,1e-6*eV  ,Orientation(AngleAxis(3.141592654/2*0.5,Vector3(0,0,1)))));
        //inter0->SetSubType(Interaction::ST_SHIELDING,spin0);
        //inter1->SetSubType(Interaction::ST_SHIELDING,spinx);
        inter2->SetSubType(Interaction::ST_SHIELDING,spiny);
        //inter3->SetSubType(Interaction::ST_SHIELDING,spinz);
        GetSS()->InsertSpin(spin0);
        GetSS()->InsertSpin(spinx);
        GetSS()->InsertSpin(spiny);
        GetSS()->InsertSpin(spinz);*/
    }
}


void RootFrame::OnGLReset(wxCommandEvent& e) {
    mDisplay3D->SetRootSGNode(new MoleculeNodeNew(GetSS()));  
}

void RootFrame::OnBondToggle(wxCommandEvent& e) {
    bool showBonds=e.IsChecked();
    mMenuItemBondToggle->Check(showBonds);
    mRootToolbar->ToggleTool(ID_BOND_TOGGLE,showBonds);
    sigSetShowBonds(showBonds);
}

BEGIN_EVENT_TABLE(RootFrame,wxFrame)

//File Menu
EVT_MENU(ID_NEW   ,RootFrame::OnNew   )
EVT_MENU(ID_OPEN  ,RootFrame::OnOpen  )
EVT_MENU(ID_SAVE  ,RootFrame::OnSave  )
EVT_MENU(ID_SAVEAS,RootFrame::OnSaveAs)
EVT_MENU(ID_EXIT  ,RootFrame::OnExit  )

//Edit Menu
EVT_MENU(ID_UNDO,RootFrame::OnUndo)
EVT_MENU(ID_REDO,RootFrame::OnRedo)

//View Menu
EVT_MENU(ID_NMR_EPR,RootFrame::OnNmrEpr)
EVT_MENU(ID_NMR,    RootFrame::OnNmr)
EVT_MENU(ID_EPR,    RootFrame::OnEpr)

EVT_MENU(ID_BOND_TOGGLE,  RootFrame::OnBondToggle)

//Resize
EVT_SIZE(RootFrame::OnResize)

//Debug
EVT_MENU(ID_GL_RESET,RootFrame::OnGLReset)

END_EVENT_TABLE()


