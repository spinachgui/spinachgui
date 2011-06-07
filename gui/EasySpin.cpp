
#include <gui/EasySpin.hpp>
#include <wx/valtext.h>

#include <gui/TextCtrlFocus.hpp>
#include <gui/NumericValidators.hpp>
#include <fstream>
#include <vector>
#include <wx/log.h>
#include <shared/foreach.hpp>
#include <climits>
#include <gui/Spacegroup.hpp>

#include <sigc++/sigc++.h>


using namespace std;
using namespace sigc;
using namespace SpinXML;

void loadSpaceGroups();
vector<wxString> gSpaceGroups;

EasySpinFrame::EasySpinFrame(wxWindow* parent,
                             wxWindowID id,
                             const wxString& title,
                             const wxPoint& pos,
                             const wxSize& size,
                             long style) 
    : EasySpinFrameBase(parent,id,title,pos,size,style) {
    loadSpaceGroups();

    //Add numeric validators to the numeric text boxes
    mCtrlMax->   SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    mCtrlMin->   SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    mCtrlCentre->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    mCtrlSweep-> SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));

    mCtrlMWFreq->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    mCtrlModAmp->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));

    mCtrlNPoints->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));

    SetMinMax(310,330);
    SetKnots(2);

    mCtrlMWFreq->ChangeValue(wxT("9.5"));

    mCtrlPhase->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));

    mOrientEditPanel = new OrientEditPanel(mPanelCystal);
    mSizerOrient->Add(mOrientEditPanel,1,wxEXPAND,2);
    Layout();
    mOrientEditPanel->Enable(false);

    //Options section
    mCtrlKnots->SetRange(2,INT_MAX);
    mCtrlAngularRes->sigUnFocus.connect(mem_fun(this,&EasySpinFrame::SlotAngularResUnFocus));
    mCtrlInterp->SetRange(2,INT_MAX);
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

void EasySpinFrame::SetKnots(unsigned long nKnots) {
    mCtrlKnots->SetValue(nKnots);
    double angularRes = 90.0 / nKnots;
    mCtrlAngularRes->ChangeValue(wxString() << angularRes);
}

void EasySpinFrame::OnKnotsChange(wxSpinEvent& e) {
    SetKnots(mCtrlKnots->GetValue());
}

void EasySpinFrame::OnAngularResText(wxCommandEvent& e) {
}

void EasySpinFrame::SlotAngularResUnFocus() {
    double angularRes;
    mCtrlAngularRes->GetValue().ToDouble(&angularRes);

    //Find the closest angular resolution to angularRes that divides
    //90 degrees

    double fractionalKnots = 90/angularRes;
    unsigned long knots = round(fractionalKnots);
    SetKnots(knots);
}

void EasySpinFrame::OnInterpCheck(wxCommandEvent& e) {
    bool checked = mCtrlInterpOn->GetValue();
    mCtrlInterp->Enable(checked);
}

void EasySpinFrame::OnCystal(wxCommandEvent& e) {
    bool cystalChecked = mRadioCrystal->GetValue();
    mPanelCystal->Enable(cystalChecked);
    mCtrlSpaceGroupButton->Enable(cystalChecked);
    mCtrlSpaceGroupTxt->Enable(cystalChecked);
    mOrientEditPanel->Enable(cystalChecked);
}

void EasySpinFrame::RepopulateCrystalOrients() {
    mCtrlOrients->Clear();
    foreach(Orientation o,mOrients) {
        //mListCtrl->Append();
    }
}

void EasySpinFrame::OnAddOrient(wxCommandEvent& e) {
    mOrients.push_back(Orientation());
}

void EasySpinFrame::OnDeleteOrient(wxCommandEvent& e) {

}

void EasySpinFrame::OnShowSpaceGroups(wxCommandEvent& e) {
    //TODO: Memory leak?
    SpaceGroupDialog* spaceGroupDialog = new SpaceGroupDialog(this);
    spaceGroupDialog->ShowModal();
}


BEGIN_EVENT_TABLE(EasySpinFrame,wxFrame)

//Experiment
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

EVT_RADIOBUTTON(ID_CRYSTAL,EasySpinFrame::OnCystal)
EVT_RADIOBUTTON(ID_POWDER,EasySpinFrame::OnCystal)

EVT_BUTTON(ID_ADD_ORIENT,   EasySpinFrame::OnAddOrient)
EVT_BUTTON(ID_DELETE_ORIENT,EasySpinFrame::OnDeleteOrient)

EVT_BUTTON(ID_SPACEGROUP_BUTTON,EasySpinFrame::OnShowSpaceGroups)


//Options
EVT_SPINCTRL(ID_KNOTS,      EasySpinFrame::OnKnotsChange)

EVT_CHECKBOX(ID_INTERPON,EasySpinFrame::OnInterpCheck)

END_EVENT_TABLE()


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
        fin >> symbol;
        if(symbol == "") {
            //Ignore blank lines
            continue;
        }
        gSpaceGroups.push_back(wxString(symbol.c_str(),wxConvUTF8));
        count++;
    }
    if(count != 230) {
        wxLogError(wxString() << wxT("Expecting 230 entries in data/spacegroups.txt, found\n") << count);
        return;
    }

    loaded = true;
    return;
}
