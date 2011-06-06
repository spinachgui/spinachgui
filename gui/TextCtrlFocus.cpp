
#include <gui/TextCtrlFocus.hpp>

TextCtrlFocus::TextCtrlFocus(wxWindow* parent,
                             wxWindowID id,
                             const wxString& value,
                             const wxPoint& pos,
                             const wxSize& size,
                             long style,
                             const wxValidator& validator,
                             const wxString& name) 
    : wxTextCtrl(parent,id,value,pos,size,style,validator,name) {
}


void TextCtrlFocus::OnUnFocus(wxFocusEvent& e) {
    sigUnFocus();
}

BEGIN_EVENT_TABLE(TextCtrlFocus,wxTextCtrl)

EVT_KILL_FOCUS(TextCtrlFocus::OnUnFocus)

END_EVENT_TABLE()

