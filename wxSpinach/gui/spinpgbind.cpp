
#include "spinpgbind.h"
#include "rootFrame.h"
#include "rotationDialog.h"
#include <wx/log.h>
#include <wx/validate.h>
#include <wx/valtext.h>

class SpinPropertyGrid;
/*
class SpinValidator : public wxValidator {
    SpinValidator(SpinPropertyGrid* grid) {

    }

    bool TransferFromWindow() {
    }

    bool TransferToWindow() {
    }

};
*/

SpinPropertyGrid::SpinPropertyGrid(wxWindow* parent,rootFrame* root)
    : wxPropertyGrid(parent, EV_SPIN_PROP_GRID_CHANGE, wxDefaultPosition, wxSize( 250,-1 ), wxPG_DEFAULT_STYLE) {
    mParentFrame=root;

    this->SetMinSize( wxSize( 250,-1 ) );


    // Populate all the fields in mSpinPropGrid
    // (isotropic shielding, anisotropic shilding... etc)
    this->Append( new wxPropertyCategory(wxT("Sheilding")) );
    mIsotropicShieldPGId=this->Append( new wxFloatProperty(wxT("Isotropic (ppm)"), wxPG_LABEL, 0) );

    wxPGProperty* pid=this->Append( new wxStringProperty(wxT("Anisotropic (ppm)"), wxPG_LABEL,  wxT("<composed>")) );
        wxPGProperty* r_id=this->AppendIn(pid,new RotationProperty(wxT("Rotation"), wxPG_LABEL,  wxT("ax 0,0,0")) );

        this->SetPropertyValidator(r_id,wxTextValidator(wxFILTER_ALPHA));

        this->AppendIn(pid,new wxFloatProperty(wxT("x"),wxPG_LABEL,1.0) );
        this->AppendIn(pid,new wxFloatProperty(wxT("y"),wxPG_LABEL,1.0) );
        this->AppendIn(pid,new wxFloatProperty(wxT("z"),wxPG_LABEL,1.0) );


    this->Append( new wxPropertyCategory(wxT("Relaxation")) );
    mRedfieldPGId=this->Append( new wxBoolProperty(wxT("Redfield"), wxPG_LABEL, false) );
    this->Append( new wxFloatProperty(wxT("Rate"), wxPG_LABEL,0) );

    this->Append( new wxPropertyCategory(wxT("Coordinates")) );
    this->Append( new wxFloatProperty(wxT("x"), wxPG_LABEL, 0) );
    this->Append( new wxFloatProperty(wxT("y"), wxPG_LABEL, 0) );
    this->Append( new wxFloatProperty(wxT("z"), wxPG_LABEL, 0) );
}

void SpinPropertyGrid::LinkToSpin(SpinPnt S) {
    mCurrentSpin=S;
    this->SetPropertyValue(mIsotropicShieldPGId,S->mIsotropic);
    this->SetPropertyValue(mRedfieldPGId         ,S->mRedfield);
}

void SpinPropertyGrid::onChange(wxPropertyGridEvent& e) {
  //There really shouldn't be any harm in just copying everything over
     mCurrentSpin->mIsotropic=this->GetPropertyValueAsDouble(mIsotropicShieldPGId);
     mCurrentSpin->mRedfield=this->GetPropertyValueAsBool(mRedfieldPGId);

     return;
}


BEGIN_EVENT_TABLE(SpinPropertyGrid, wxPropertyGrid)
  EVT_PG_CHANGED(EV_SPIN_PROP_GRID_CHANGE, SpinPropertyGrid::onChange)
END_EVENT_TABLE()


