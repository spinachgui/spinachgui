
#include <gui/RootFrame.hpp>
#include <gui/SpinachApp.hpp>
#include <gui/MolSceneGraph.hpp>
#include <stdexcept>
#include <wx/log.h>

//Input and output filters
#include <shared/formats/xyz.hpp>

using namespace std;

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
  //mNotebook=new wxAuiNotebook(mAuiPanel);
  mSplitter = new wxSplitterWindow(this);

  mSpinGridPanel=new SpinGridPanel(mSplitter);

  mDisplay3D=new Display3D(mSplitter);
  mDisplay3D->SetRootSGNode(new MoleculeNode(GetSS()));

  //mDisplay3D->SetRootFGNode(new MoleculeFG(GetSS()));
  mDisplay3D->SetRootFGNode(new OpenGLText(wxT("Hello World")));

  mDisplay3D->GetDC().depthOnly=false;


  // add the panes to the manager
  //mNotebook->AddPage(mDisplay3D, wxT("3D View"));
  //mNotebook->AddPage(mSpinGridPanel, wxT("Grid View"));

  mSplitter->Initialize(mSpinGridPanel);
  mSplitter->Initialize(mDisplay3D);
  mSplitter->SplitVertically(mSpinGridPanel,mDisplay3D);

  mAuiPanel->GetSizer()->Add(mSplitter,1,wxEXPAND);

  //Grey the undo and redo menu ideams. They can be ungreyed when
  //there is an undo history

  mMenuItemUndo->Enable(false);
  mMenuItemRedo->Enable(false);
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
    mOpenPath=fd->GetPath();
    mOpenFile=fd->GetFilename();
    mOpenDir=fd->GetDirectory();
    wxString ext=GetExtension(mOpenFile).Lower();

    ISpinSystemLoader* loader=NULL;
    for(vector<ISpinSystemLoader*>::const_iterator i=wxGetApp().GetIOFilters().begin();
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

END_EVENT_TABLE()


