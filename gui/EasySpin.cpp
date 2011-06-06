
#include <gui/EasySpin.hpp>
#include <wx/valtext.h>

EasySpinFrame::EasySpinFrame(wxWindow* parent,
                             wxWindowID id,
                             const wxString& title,
                             const wxPoint& pos,
                             const wxSize& size,
                             long style) 
    : EasySpinFrameBase(parent,id,title,pos,size,style) {

    //Add numeric validators to the numeric text boxes
    mCtrlMax->   SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    mCtrlMin->   SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    mCtrlCentre->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    mCtrlSweep-> SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    
}

void EasySpinFrame::OnMaxMin(wxCommandEvent& e) {
    double min,max;
    mCtrlMax->GetValue().ToDouble(&min);
    mCtrlMin->GetValue().ToDouble(&max);

    mCtrlCentre->ChangeValue(wxString() << (min+max)/2);
    mCtrlSweep ->ChangeValue(wxString() << max-min);
}

void EasySpinFrame::OnCentreSweep(wxCommandEvent& e) {
    double centre,sweep;
    mCtrlCentre->GetValue().ToDouble(&centre);
    mCtrlSweep ->GetValue().ToDouble(&sweep);

    mCtrlMin->ChangeValue(wxString() << centre-sweep/2);
    mCtrlMax->ChangeValue(wxString() << centre+sweep/2);
}



BEGIN_EVENT_TABLE(EasySpinFrame,wxFrame)

EVT_TEXT(ID_MAX,   EasySpinFrame::OnMaxMin)
EVT_TEXT(ID_MIN,   EasySpinFrame::OnMaxMin)
EVT_TEXT(ID_CENTRE,EasySpinFrame::OnCentreSweep)
EVT_TEXT(ID_SWEEP, EasySpinFrame::OnCentreSweep)

END_EVENT_TABLE()

