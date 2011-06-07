
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

#include <shared/formats/spirit_common.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/karma.hpp>

using namespace std;

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
struct table_grammar 
    : karma::grammar<OutputIterator,vector<vector<int> >()>
{
    table_grammar()
        : table_grammar::base_type(table)
    {
        i = 0;
        table = "<html><body><table><tr>" << *row << "</tr></table></body></html>";
        row = "<td>" << *("<td>" << int_ << "</td>") << "</td>";
    }

    int i;

    rule<OutputIterator,vector<vector<int> >()> table;
    rule<OutputIterator,vector<int>()> row;
};

template <typename OutputIterator>
bool generateHTML(OutputIterator sink) {
    vector<int> v;
    v.push_back(3);
    v.push_back(2);
    v.push_back(8);
    vector<vector<int> > vv;
    vv.push_back(v);
    vv.push_back(v);
    vv.push_back(v);

    table_grammar<OutputIterator> g;
    return generate(sink,g,vv);
}

using namespace std;

wxString testHTML;
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
        htmlWindow->SetPage(testHTML);
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

    cout << generated << endl;

    testHTML = wxString(generated.c_str(),wxConvUTF8);
}
static __SpacegroupInit __spacegroupInit;

