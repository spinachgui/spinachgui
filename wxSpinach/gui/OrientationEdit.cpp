#include <gui/OrientationEdit.hpp>

BEGIN_EVENT_TABLE(OrientEditPanel,wxPanel)


END_EVENT_TABLE()


//==========================================================//

OrientTextCtrl::OrientTextCtrl(wxWindow* parent,wxWindowID id)
:wxTextCtrl(parent,id) {

}

BEGIN_EVENT_TABLE(OrientTextCtrl,wxTextCtrl)


END_EVENT_TABLE()



