
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

  MoleculeNode* mn = new MoleculeNode(GetSS());

  SpinachDC* spinDC=&mDisplay3D->GetDC();

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

  //Set sensible default scallings
  hfc_sp->GetLogSlider()->SetValue(0.04);
  gt_sp ->GetLogSlider()->SetValue(0.04);
  zfs_sp->GetLogSlider()->SetValue(0.04);
  exc_sp->GetLogSlider()->SetValue(0.04);
  shd_sp->GetLogSlider()->SetValue(0.04);
  sca_sp->GetLogSlider()->SetValue(0.04);
  qp_sp ->GetLogSlider()->SetValue(0.04);
  dip_sp->GetLogSlider()->SetValue(0.04);
  cl_sp ->GetLogSlider()->SetValue(0.04);
  cb_sp ->GetLogSlider()->SetValue(0.04);
  cq_sp ->GetLogSlider()->SetValue(0.04);

  mDisplay3D->SetRootSGNode(mn);

  //mDisplay3D->SetRootFGNode(new MoleculeFG(GetSS()));
  mDisplay3D->SetRootFGNode(new OpenGLText(wxT("Hello World")));

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
      wxLogError(wxT("File is corrupt"));
      wxString(e.what(),wxConvUTF8);
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
				    filter ,
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
      if((*i)->GetFilterType()==ISpinSystemLoader::LOAD ||
	 (*i)->GetFilterType()==ISpinSystemLoader::LOADSAVE) {
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
  #warning "OnNmrEpr Doesn't work"
  mMenuItemNmrEpr->Check(true);
  mRootToolbar->ToggleTool(ID_NMR_EPR,true);
}

void RootFrame::OnNmr(wxCommandEvent& e) {
  #warning "OnNmr Doesn't work"
  mMenuItemNmr->Check(true);
  mRootToolbar->ToggleTool(ID_NMR,true);
}

void RootFrame::OnEpr(wxCommandEvent& e) {
  #warning "OnEpr Doesn't work"
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
    LoadFromFile(wxT("data/tryosine.log"),
		 wxT("data/"),
		 wxT("tryosine.log"));
    LoadFromFile(wxT("data/OFNAPH01_NMR.magres"),
		 wxT("data/"),
		 wxT("OFNAPH01_NMR.magres"));

  }
}

void RootFrame::OnGLReset(wxCommandEvent& e) {
  mDisplay3D->SetRootSGNode(new MoleculeNode(GetSS()));  
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

//Resize
EVT_SIZE(RootFrame::OnResize)

//Debug
EVT_MENU(ID_GL_RESET,RootFrame::OnGLReset)

END_EVENT_TABLE()


