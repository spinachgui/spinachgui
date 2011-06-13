
#include <gui/Spacegroup.hpp>

#include <wx/html/htmlwin.h>
#include <wx/string.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/log.h>
#include <wx/file.h>

#include <boost/algorithm/string/trim.hpp>

#include <shared/foreach.hpp>
#include <vector>

#include <string>
#include <fstream>
#include <streambuf>
#include <vector>
#include <wx/html/htmlwin.h>
#include <wx/log.h>


#include <shared/formats/spirit_common.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

namespace qi=boost::spirit::qi;

using boost::spirit::qi::char_;
using boost::spirit::qi::int_;
using boost::spirit::qi::omit;
using boost::spirit::qi::alpha;
using boost::spirit::qi::double_;
using boost::spirit::qi::long_;
using boost::spirit::qi::ulong_;
using boost::spirit::qi::lit;
using boost::spirit::qi::phrase_parse;
using boost::spirit::qi::rule;
using boost::spirit::qi::space;
using boost::spirit::qi::_1;
using boost::phoenix::ref;

typedef std::string::iterator stritor;


using namespace std;

//NB this number is as likey to change as pi
#define NUM_SPACEGROUPS 230

struct spacegroupNames_ : qi::symbols<char, unsigned long> {
    spacegroupNames_() {
        fstream fin("data/spacegroups.txt",ios::in);
        if(!fin.is_open()) {
            wxLogError(wxT("Could not open data/spacegroups.txt, no spacegroups will be available\n"));
            return;
        }

        long count = 0;
        while(!fin.eof()) {
            string symbol;
            if(!getline(fin,symbol)) {
                break;
            }
            boost::algorithm::trim(symbol); //Remove whitespace
            if(symbol == "") {                //Ignore blank lines
                continue;
            }
            add(symbol,count+1);
            count++;
        }
        if(count != NUM_SPACEGROUPS) {
            wxLogError(wxString() << wxT("Expecting 230 entries in data/spacegroups.txt, found\n") << count);
            return;
        }
    }
} spacegroupNames;

//================================================================================//
// Spacegroup Queries

vector<string> gSpaceGroups;

template <typename Expr>
bool maybeParse(string str, Expr const& expr) {
	stritor begin = str.begin();
	if(phrase_parse(begin,str.end(),expr,qi::space)) {
		//Check we have fully matched the patten
		if(begin == str.end()) {
			return true;
		}
	}
	return false;
}


unsigned long parseStandardForm(string str) {
    stritor begin = str.begin();
    unsigned long spaceGroupIndex = 0;
    if(phrase_parse(begin,
                    str.end(),
                    (ulong_ | spacegroupNames | (ulong_ >> omit["(" >> spacegroupNames >> ")"]))[ref(spaceGroupIndex) = _1] ,
                    qi::space)) {
        return spaceGroupIndex;
    } else {
        return 0;
    }
}

string findSpacegroup(string partial) {
    bool found1 = false;
    string found = "";
    foreach(string name,gSpaceGroups) {
        if(name.substr(0,partial.length()) == partial) {
            if(found1) {
                //ambiguous
                return "";
            } else {
                found1 = true;
                found = name;
            }
        }
    }
    return found;
}

unsigned long lookupSpacegroupName(std::string name) {
    for(unsigned long i = 0;i<gSpaceGroups.size();i++) {
        if(name == gSpaceGroups[i]) {
            return i+1;
        }
    }
    return -1;
}

string nameSpacegroup(unsigned long number) {
    if(number > 0 && number <= NUM_SPACEGROUPS) {
        return gSpaceGroups[number-1];
    }
    return "";
}

//================================================================================//
// Spacegroup GUI class

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
        htmlWindow->LoadPage(filename);
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


void loadSpaceGroups() {
    static bool loaded = false;
    if(loaded) {
        return;
    }

    fstream fin("data/spacegroups.txt",ios::in);
    if(!fin.is_open()) {
        wxLogError(wxT("Could not open data/spacegroups.txt, no spacegroups will be available\n"));
        return;
    }

    long count = 0;
    while(!fin.eof()) {
        //TODO: I'm not sure if this code will work on windows, where
        //wstrings need to be used rather than strings
        string symbol;
        if(!getline(fin,symbol)) {
            break;
        }
        boost::algorithm::trim(symbol); //Remove whitespace
        if(symbol == "") {
            //Ignore blank lines
            continue;
        }
        gSpaceGroups.push_back(symbol);
        count++;
    }
    if(count != NUM_SPACEGROUPS) {
        wxLogError(wxString() << wxT("Expecting 230 entries in data/spacegroups.txt, found\n") << count);
        return;
    }

    loaded = true;
    return;
}

__SpacegroupInit::__SpacegroupInit() {
    gCystalSystemNames.push_back(wxT("triclinic"));
    gCystalSystemNames.push_back(wxT("monoclinic"));
    gCystalSystemNames.push_back(wxT("orthorhombic"));
    gCystalSystemNames.push_back(wxT("tetragonal"));
    gCystalSystemNames.push_back(wxT("hexagonal"));
    gCystalSystemNames.push_back(wxT("cubic"));

    loadSpaceGroups();
}


static __SpacegroupInit __spacegroupInit;



