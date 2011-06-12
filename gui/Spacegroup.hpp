
#ifndef __SPACEGROUP__H_
#define __SPACEGROUP__H_

#include <wx/notebook.h>
#include <wx/dialog.h>

class wxHtmlLinkEvent;

class SpaceGroupDialog : public wxDialog {
public:
    SpaceGroupDialog(wxWindow* parent,
                     wxWindowID id = wxID_ANY,
                     const wxString& title = wxEmptyString,
                     const wxPoint& pos = wxDefaultPosition,
                     const wxSize& size = wxSize( 600,400 ),
                     long style = wxDEFAULT_DIALOG_STYLE );
    long spacegroup;
protected:
    DECLARE_EVENT_TABLE()
    void LinkClicked(wxHtmlLinkEvent& e);
    wxNotebook* mNotebook;
};

std::string findSpacegroup(std::string partial);
std::string nameSpacegroup(unsigned long number);
unsigned long lookupSpacegroupName(std::string name);
unsigned long parseStandardForm(std::string str);

//Private

struct __SpacegroupInit {
    __SpacegroupInit();
};



#endif
