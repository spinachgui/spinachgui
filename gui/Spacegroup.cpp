
#include <gui/Spacegroup.hpp>

#include <wx/html/htmlwin.h>
#include <wx/string.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/log.h>
#include <wx/file.h>

#include <shared/foreach.hpp>
#include <vector>

#include <string>
#include <fstream>
#include <streambuf>
#include <vector>
#include <wx/html/htmlwin.h>
using namespace std;

wxString testHTML;
vector<wxString> gCystalSystemNames;

SpaceGroupDialog::SpaceGroupDialog( wxWindow* parent,
                                    wxWindowID id,
                                    const wxString& title,
                                    const wxPoint& pos,
                                    const wxSize& size,
                                    long style )
    : wxDialog( parent, id, title, pos, size, style ),
      spacegroup(-1) {
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* boxSizer = new wxBoxSizer( wxVERTICAL );
	this->SetSizer(boxSizer);

	mNotebook = new wxNotebook(this,wxID_ANY);
    boxSizer->Add(mNotebook,1,wxEXPAND);

    foreach(wxString cystalSystemName,gCystalSystemNames) {
        wxPanel* panel= new wxPanel(mNotebook, wxID_ANY);
        wxBoxSizer* innerSizer = new wxBoxSizer( wxVERTICAL );

        panel->SetSizer(innerSizer);
        panel->Layout();

        wxHtmlWindow* htmlWindow = new wxHtmlWindow(panel, wxID_ANY);
        wxString filename  =  wxT("data/") + cystalSystemName + wxT(".html");
        htmlWindow->LoadPage(wxT("data/spacegroups.html"));
        innerSizer->Add(htmlWindow, 1, wxALL|wxEXPAND, 5);

        mNotebook->AddPage(panel,cystalSystemName,false);
    }

	this->Layout();
	this->Centre( wxBOTH );
}

void SpaceGroupDialog::LinkClicked(wxHtmlLinkEvent& e) {
    wxString ref = e.GetLinkInfo().GetHref();
    ref.ToLong(&spacegroup);
    EndModal(wxID_OK);
}

BEGIN_EVENT_TABLE(SpaceGroupDialog,wxDialog)
EVT_HTML_LINK_CLICKED(wxID_ANY,SpaceGroupDialog::LinkClicked)
END_EVENT_TABLE()


//================================================================================//

__SpacegroupInit::__SpacegroupInit() {
    gCystalSystemNames.push_back(wxT("triclinic"));
    gCystalSystemNames.push_back(wxT("monoclinic"));
    gCystalSystemNames.push_back(wxT("orthorhombic"));
    gCystalSystemNames.push_back(wxT("tetragonal"));
    gCystalSystemNames.push_back(wxT("hexagonal"));
    gCystalSystemNames.push_back(wxT("cubic"));
}
static __SpacegroupInit __spacegroupInit;



