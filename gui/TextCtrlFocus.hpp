
#ifndef __TEXTCtrlFocus__H__
#define __TEXTCtrlFocus__H__

#include <wx/textctrl.h>
#include <sigc++/sigc++.h>

class TextCtrlFocus : public wxTextCtrl {
public:
    TextCtrlFocus(wxWindow* parent,
                  wxWindowID id,
                  const wxString& value = wxT(""),
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize,
                  long style = 0,
                  const wxValidator& validator = wxDefaultValidator,
                  const wxString& name = wxTextCtrlNameStr);
    sigc::signal<void> sigUnFocus;
    sigc::signal<void> sigFocus;
    
    DECLARE_EVENT_TABLE()

    void OnFocus(wxFocusEvent& e);
    void OnUnFocus(wxFocusEvent& e);
};


#endif
