
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

#include <shared/formats/spirit_common.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/karma.hpp>

namespace karma=boost::spirit::karma;

using boost::spirit::karma::char_;
using boost::spirit::karma::int_;
using boost::spirit::karma::alpha;
using boost::spirit::karma::double_;
using boost::spirit::karma::lit;
using boost::spirit::karma::generate;
using boost::spirit::karma::rule;
using boost::spirit::karma::space;
using boost::spirit::karma::_1;

template <typename OutputIterator>
bool generateHTML(OutputIterator sink) {
    return generate(sink,lit("<html></html>"));
}

using namespace std;

vector<wxString> gCystalSystemNames;

SpaceGroupDialog::SpaceGroupDialog( wxWindow* parent,
                                    wxWindowID id,
                                    const wxString& title,
                                    const wxPoint& pos,
                                    const wxSize& size,
                                    long style ) : wxDialog( parent, id, title, pos, size, style ) {
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
        htmlWindow->LoadPage(filename);
        innerSizer->Add(htmlWindow, 1, wxALL|wxEXPAND, 5);

        mNotebook->AddPage(panel,cystalSystemName,false);
    }
	this->Layout();
	this->Centre( wxBOTH );
}


//================================================================================//

__SpacegroupInit::__SpacegroupInit() {
    gCystalSystemNames.push_back(wxT("triclinic"));
    gCystalSystemNames.push_back(wxT("monoclinic"));
    gCystalSystemNames.push_back(wxT("orthorhombic"));
    gCystalSystemNames.push_back(wxT("tetragonal"));
    gCystalSystemNames.push_back(wxT("hexagonal"));
    gCystalSystemNames.push_back(wxT("cubic"));

    std::string generated;
    std::back_insert_iterator<std::string> sink(generated);
    generateHTML(sink);
}
static __SpacegroupInit __spacegroupInit;

