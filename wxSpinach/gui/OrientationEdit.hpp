
#ifndef ORIENTEDIT_H
#define ORIENTEDIT_H

#include <res/SpinachGUI.h>
#include <shared/spinsys.hpp>


class OrientTextCtrl : public wxTextCtrl {
public:
  OrientTextCtrl(wxWindow* parent,wxWindowID id=-1);
protected:
  DECLARE_EVENT_TABLE();
private:
};

class OrientEditPanel : public OrientEditPanelBase {
public:
  
protected:
  DECLARE_EVENT_TABLE();
private:

};

#endif
