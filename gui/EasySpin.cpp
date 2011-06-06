
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

    mCtrlMWFreq->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    mCtrlModAmp->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));

    mCtrlNPoints->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));

    SetMinMax(310,330);

    mCtrlMWFreq->ChangeValue(wxT("9.5"));
}


void EasySpinFrame::OnMin(wxCommandEvent& e) {
    double min,max;
    mCtrlMax->GetValue().ToDouble(&max);
    mCtrlMin->GetValue().ToDouble(&min);

    if(min>max) {
        max = min;
        mCtrlMax->ChangeValue(wxString() << max);
    }

    mCtrlCentre->ChangeValue(wxString() << (min+max)/2);
    mCtrlSweep ->ChangeValue(wxString() << max-min);
}
void EasySpinFrame::OnMax(wxCommandEvent& e) {
    double min,max;
    mCtrlMax->GetValue().ToDouble(&max);
    mCtrlMin->GetValue().ToDouble(&min);

    if(min>max) {
        min = max;
        mCtrlMin->ChangeValue(wxString() << min);
    }

    mCtrlCentre->ChangeValue(wxString() << (min+max)/2);
    mCtrlSweep ->ChangeValue(wxString() << max-min);
}

void EasySpinFrame::OnCentreSweep(wxCommandEvent& e) {
    double centre,sweep;
    mCtrlCentre->GetValue().ToDouble(&centre);
    mCtrlSweep ->GetValue().ToDouble(&sweep);

    if(sweep < 0) {
        sweep = 0;
    }

    mCtrlMin->ChangeValue(wxString() << centre-sweep/2);
    mCtrlMax->ChangeValue(wxString() << centre+sweep/2);
}

void EasySpinFrame::OnXBand(wxCommandEvent& e) {
    SetMWFreq(9.5);
}

void EasySpinFrame::OnQBand(wxCommandEvent& e) {
    SetMWFreq(35);
}

void EasySpinFrame::OnWBand(wxCommandEvent& e) {
    SetMWFreq(95);
}

void EasySpinFrame::SetMWFreq(double freq) {
    mCtrlMWFreq->ChangeValue(wxString() << freq);
}

void EasySpinFrame::SetMinMax(double min,double max) {
    mCtrlMin->ChangeValue(wxString() << min);
    mCtrlMax->ChangeValue(wxString() << max);

    mCtrlCentre->ChangeValue(wxString() << (min+max)/2);
    mCtrlSweep ->ChangeValue(wxString() << max-min);
}

void EasySpinFrame::OnTempCheck(wxCommandEvent& e)  {
    bool checked = mCtrlTempOn->GetValue();
    mCtrlTemp->Enable(checked);
}

void EasySpinFrame::OnModAmpCheck(wxCommandEvent& e) {
    bool checked = mCtrlModAmpOn->GetValue();
    mCtrlModAmp->Enable(checked);
    mCtrlModAmpUnit->Enable(checked);
}

void EasySpinFrame::OnModAmpUnit(wxCommandEvent& e) {
    mCtrlRangeUnit->SetSelection(mCtrlModAmpUnit->GetSelection());
}

void EasySpinFrame::OnRangeUnit(wxCommandEvent& e) {
    mCtrlModAmpUnit->SetSelection(mCtrlRangeUnit->GetSelection());    
}


BEGIN_EVENT_TABLE(EasySpinFrame,wxFrame)

EVT_TEXT(ID_MAX,   EasySpinFrame::OnMax)
EVT_TEXT(ID_MIN,   EasySpinFrame::OnMin)
EVT_TEXT(ID_CENTRE,EasySpinFrame::OnCentreSweep)
EVT_TEXT(ID_SWEEP, EasySpinFrame::OnCentreSweep)

EVT_BUTTON(ID_XBAND,EasySpinFrame::OnXBand)
EVT_BUTTON(ID_QBAND,EasySpinFrame::OnQBand)
EVT_BUTTON(ID_WBAND,EasySpinFrame::OnWBand)

EVT_CHECKBOX(ID_TEMPON,EasySpinFrame::OnTempCheck)
EVT_CHECKBOX(ID_MODAMPON,EasySpinFrame::OnModAmpCheck)

EVT_CHOICE(ID_MODAMPUNIT,EasySpinFrame::OnModAmpUnit)
EVT_CHOICE(ID_RANGEUNIT ,EasySpinFrame::OnRangeUnit)

END_EVENT_TABLE()

