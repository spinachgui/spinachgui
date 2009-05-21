
#ifndef SPIN_PG_BIND_H
#define SPIN_PG_BIND_H

#include <wx/odcombo.h>
#include <wx/propgrid/propgrid.h>
//#include <wx/propgrid/advprops.h>

#include <shared/spin.h>

class rootFrame;

class SpinPropertyGrid : public wxPropertyGrid {
public:
    SpinPropertyGrid(wxWindow* parent,rootFrame* root);

    void LinkToSpin(SpinPnt S);

    void onChange(wxPropertyGridEvent& e);


protected:
    rootFrame* mParentFrame;
    SpinPnt mCurrentSpin;

  //Fields to hold ids for all the fields in the Property Grid.
  //Carn't think of a better way to do this right now.
    wxPGProperty* mIsotropicShieldPGId;
    wxPGProperty* mRedfieldPGId;

    DECLARE_EVENT_TABLE()

};

#endif
