
#include <gui/EasySpin.hpp>
#include <wx/valtext.h>

#include <gui/TextCtrlFocus.hpp>
#include <gui/NumericValidators.hpp>
#include <fstream>
#include <vector>
#include <map>
#include <wx/log.h>
#include <shared/foreach.hpp>
#include <climits>
#include <limits>
#include <gui/Spacegroup.hpp>
#include <boost/optional.hpp>
#include <wx/clipbrd.h>
#include <wx/dataobj.h>
#include <wx/image.h>
#include <wx/bitmap.h>
#include <wx/dcmemory.h>
#include <wx/imaglist.h>
#include <sigc++/sigc++.h>
#include <shared/spinsys.hpp>
#include <shared/nuclear_data.hpp>
#include <shared/easygen.hpp>

#define ORIENT_ICON_SIZE 41

#define SQRT_2LN2 1.1774100225154747
#define SQRT_3    1.7320508075688773

using namespace std;
using namespace sigc;
using namespace Eigen;
using namespace SpinXML;
using boost::optional;

//================================================================================//
// wxChoice decoder functions. Decode an wxChoice index into a
// meaninful value. It would generally be preferable to use the
// clientData but sometimes it's just neeter to use a lookup table.
// NB: These functions must be edited with the coresponding wxChoice
// changes

//Return a number which a quantity in the coresponding unit can be
//multiplied by to make it mT
double magneticFieldUnit(unsigned int index) {
    switch(index) {
    case 0: //mT
        return 1;
    case 1: //G
        return 0.1;
    case 2: //T
        return 1000;
    case 3: //kG
        return 100;
    default:
        cerr << "Unknown value passed to magneticFieldUnit(unsigned int index)" << endl;
        return numeric_limits<double>::quiet_NaN();
    }
}

//================================================================================//
// Orientation visualisation algorithm. Summarise an orientation in a
// visual way in a very small space

void drawOrientationIcon(wxBitmap& bitmap,const Orientation& o) {
    Matrix3d rot = o.GetAsDCM();

    wxMemoryDC dc;
    dc.SelectObject(bitmap);

    long centre = ORIENT_ICON_SIZE/2;

    dc.Clear();
    dc.DrawCircle(centre,centre,centre-4);

    dc.SetPen(*wxRED_PEN);
    dc.DrawLine(centre,centre,centre*(rot(0,0)+1),centre*(rot(0,1)+1));
    dc.SetPen(*wxGREEN_PEN);
    dc.DrawLine(centre,centre,centre*(rot(1,0)+1),centre*(rot(1,1)+1));
    dc.SetPen(wxPen(wxColour(0,0,255)));
    dc.DrawLine(centre,centre,centre*(rot(2,0)+1),centre*(rot(2,1)+1));


}
//These vectors map the selection to the enum. These *must* be updated
//if the order items in the corresponding selection is changed. They
//are initalised in the module initalisation code in __init's
//constructor
vector<EasySpinInput::Mode        > gModeSelectOrdering;    
vector<EasySpinInput::Method      > gMethodSelectOrdering;  
vector<EasySpinInput::Output      > gOutputSelectOrdering;  
vector<EasySpinInput::EasySpinFunc> gEasySpinSelectOrdering;

//================================================================================//
// GUI Functions

void loadSpaceGroups();
vector<wxString> gSpaceGroups;

EasySpinFrame::EasySpinFrame(wxWindow* parent,
                             wxWindowID id,
                             const wxString& title,
                             const wxPoint& pos,
                             const wxSize& size,
                             long style) 
    : EasySpinFrameBase(parent,id,title,pos,size,style),mPreviewEdited(false) {
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

    //Options section
    mCtrlKnots->SetRange(2,INT_MAX);
    mCtrlAngularRes->sigUnFocus.connect(mem_fun(this,&EasySpinFrame::SlotAngularResUnFocus));
    mCtrlInterp->SetRange(2,INT_MAX);

    //Broadernings
    mCtrlGaussFWHM-> SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    mCtrlGaussPP->   SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    mCtrlLorentFWHM->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    mCtrlLorentPP->  SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));

    mCtrlHX->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    mCtrlHY->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    mCtrlHZ->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));

    mCtrlGX->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    mCtrlGY->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    mCtrlGZ->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));

    mCtrlAX->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    mCtrlAY->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    mCtrlAZ->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
        
    mCtrlDD->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    mCtrlDE->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    mCtrlDCorrCoeff->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));

    //Make strain based broaderning hidded until the user selects pepper
    mSizerStrain->Show(false);

    HideCrystal(true);
}

void EasySpinFrame::SetSpinsys(SpinXML::SpinSystem* spinsys) {
    mSpinSystem = spinsys;
}


void EasySpinFrame::OnMin(wxCommandEvent& e) {
    e.Skip();

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
    e.Skip();

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
    e.Skip();

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
    e.Skip();
    SetMWFreq(9.5);
}

void EasySpinFrame::OnQBand(wxCommandEvent& e) {
    e.Skip();
    SetMWFreq(35);
}

void EasySpinFrame::OnWBand(wxCommandEvent& e) {
    e.Skip();
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
    e.Skip();

    bool checked = mCtrlTempOn->GetValue();
    mCtrlTemp->Enable(checked);
}

void EasySpinFrame::OnModAmpCheck(wxCommandEvent& e) {
    e.Skip();

    bool checked = mCtrlModAmpOn->GetValue();
    mCtrlModAmp->Enable(checked);
    mCtrlModAmpUnit->Enable(checked);
}

void EasySpinFrame::OnModAmpUnit(wxCommandEvent& e) {
    e.Skip();

    mCtrlRangeUnit->SetSelection(mCtrlModAmpUnit->GetSelection());
}

void EasySpinFrame::OnRangeUnit(wxCommandEvent& e) {
    e.Skip();

    mCtrlModAmpUnit->SetSelection(mCtrlRangeUnit->GetSelection());    
}

void EasySpinFrame::SetKnots(unsigned long nKnots) {

    mCtrlKnots->SetValue(nKnots);
    double angularRes = 90.0 / (nKnots-1);
    mCtrlAngularRes->ChangeValue(wxString() << angularRes);
}

void EasySpinFrame::OnKnotsChange(wxSpinEvent& e) {
    e.Skip();

    SetKnots(mCtrlKnots->GetValue());
}

void EasySpinFrame::SlotAngularResUnFocus() {
    double angularRes;
    mCtrlAngularRes->GetValue().ToDouble(&angularRes);

    //Find the closest angular resolution to angularRes that divides
    //90 degrees

    double fractionalKnots = 90/angularRes + 1;
    unsigned long knots = round(fractionalKnots);
    SetKnots(knots);
}

void EasySpinFrame::OnInterpCheck(wxCommandEvent& e) {
    bool checked = mCtrlInterpOn->GetValue();
    mCtrlInterp->Enable(checked);
}

void EasySpinFrame::OnCrystal(wxCommandEvent& e) {
    e.Skip();
    bool crystalChecked = mRadioCrystal->GetValue();
    HideCrystal(!crystalChecked);
}

void EasySpinFrame::HideCrystal(bool hide) {
    mRadioPowder->SetValue(hide);

    mPanelCrystal->Show(!hide);
    mCtrlSpaceGroupButton->Show(!hide);
    mCtrlSpaceGroupTxt->Show(!hide);
    mSpaceGroupText->Show(!hide);

    mExpNotebookPage->Layout();
    mPanelCrystal->Layout();
}

void EasySpinFrame::RepopulateCrystalOrients() {
    wxImageList* imageList = new wxImageList(ORIENT_ICON_SIZE,ORIENT_ICON_SIZE);
    mCtrlOrients->AssignImageList(imageList,wxIMAGE_LIST_NORMAL);

    mCtrlOrients->ClearAll();
    long i = 0;
    foreach(Orientation o,mOrients) {
        wxBitmap bm(ORIENT_ICON_SIZE,ORIENT_ICON_SIZE,24);
        drawOrientationIcon(bm,o);
        imageList->Add(bm);
        mCtrlOrients->InsertItem(i,wxString() << wxT("Orientation") << i);
        mCtrlOrients->SetItemImage(i,i);
        i++;
    }

}

void EasySpinFrame::OnAddOrient(wxCommandEvent& e) {
    mOrients.push_back(Orientation());
    RepopulateCrystalOrients();
}

void EasySpinFrame::OnDeleteOrient(wxCommandEvent& e) {
    long itemIndex = mCtrlOrients->GetNextItem(-1,
                                               wxLIST_NEXT_ALL,
                                               wxLIST_STATE_SELECTED);
    if (itemIndex != -1) {
            mOrients.erase(mOrients.begin()+itemIndex);
            RepopulateCrystalOrients();
    }
}


void EasySpinFrame::OnOrientDClick(wxListEvent& e) {
    long itemIndex = mCtrlOrients->GetNextItem(-1,
                                               wxLIST_NEXT_ALL,
                                               wxLIST_STATE_SELECTED);
    if(itemIndex == -1) {
        //Not sure if this can actually happen
        return;
    }
    OrientEditDialog* orientDialog = new OrientEditDialog(this);
    orientDialog->SetOrient(mOrients[itemIndex]);
    if (orientDialog->ShowModal() == wxID_OK) {
        mOrients[itemIndex] = orientDialog->GetOrient();
    }
    RepopulateCrystalOrients();
}

void EasySpinFrame::OnShowSpaceGroups(wxCommandEvent& e) {
    //TODO: Memory leak?
    SpaceGroupDialog* spaceGroupDialog = new SpaceGroupDialog(this);
    spaceGroupDialog->ShowModal();
}

void EasySpinFrame::OnGenerateButton(wxCommandEvent& e) {
    //User has decided to discard changes
    mPreviewEdited = false;
    mTempGenerate->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
    OnGenerate(e);
}

void EasySpinFrame::OnGenerate(wxCommandEvent& e) {
    e.Skip();
    if(mPreviewEdited) {
        mTempGenerate->SetForegroundColour(*wxRED);
        return;
    }
    double toMiliT = magneticFieldUnit(mCtrlRangeUnit->GetSelection());

    EasySpinInput easySpinInput;

    //Collect values from the GUI
    unsigned int easyspinf_select = mCtrlEasySpinF->GetSelection();
    easySpinInput.setEasySpinFunction(gEasySpinSelectOrdering[easyspinf_select]);

    double centre,sweep;
    mCtrlCentre->GetValue().ToDouble(&centre);
    mCtrlSweep->GetValue().ToDouble(&sweep);
    easySpinInput.setCentreSweep(centre*toMiliT,sweep*toMiliT);

    double mwFreq;
    mCtrlMWFreq->GetValue().ToDouble(&mwFreq);
    easySpinInput.setMWFreq(mwFreq);

    if(mCtrlTempOn->GetValue()) {
        double temperature;
        mCtrlTemp->GetValue().ToDouble(&temperature);
        easySpinInput.setTemperature(temperature);
    }

    unsigned long nPoints;
    mCtrlNPoints->GetValue().ToULong(&nPoints);
    easySpinInput.setNPoints(nPoints);

    easySpinInput.setHarmonic(mCtrlHarmonic->GetSelection());

    easySpinInput.setMode(gModeSelectOrdering[mCtrlMode->GetSelection()]);

    if(mRadioCrystal->GetValue()) {
        easySpinInput.setSpaceGroup(1);
    }

    if(mCtrlModAmpOn->GetValue()) {
        double modAmp;
        mCtrlModAmp->GetValue().ToDouble(&modAmp);
        easySpinInput.setModAmp(modAmp*toMiliT);
    }

    double mwPhase;
    mCtrlPhase->GetValue().ToDouble(&mwPhase);
    easySpinInput.setMWPhase(mwPhase);

    easySpinInput.setMethod(gMethodSelectOrdering[mCtrlMethod->GetSelection()]);

    easySpinInput.setNKnots(mCtrlKnots->GetValue());
    easySpinInput.setInterpolate(mCtrlInterp->GetValue());

    easySpinInput.setOutput(gOutputSelectOrdering[mCtrlOutput->GetSelection()]);

    //Do code generation
    string easyCode = easySpinInput.generate(mSpinSystem);
    
    mCtrlPreview->ChangeValue(wxString(easyCode.c_str(),wxConvUTF8));
}

void EasySpinFrame::PreviewEdit(wxCommandEvent& e) {
    mPreviewEdited = true;
}

void EasySpinFrame::OnEasySpinFunc(wxCommandEvent& e) {
    e.Skip();
    unsigned int n = mCtrlEasySpinF->GetSelection();

    mGarlicOptions->Show(n == 0);
    mChiliOptions->Show(n == 1);
    mPepperOptions->Show(n == 2);

    mOptionsPage->Layout();

    //Strain based line broaderning is only present in pepper
    mSizerStrain->Show(n == 2);
}

void EasySpinFrame::OnCopy(wxCommandEvent& e) {
    if (wxTheClipboard->Open()) {
        wxTheClipboard->SetData(new wxTextDataObject(mCtrlPreview->GetValue()));
        wxTheClipboard->Close();
    } else {
        wxLogError(wxT("Could not copy to clipboard. Clipboard may be being used by another application."));
    }
}

void EasySpinFrame::OnGaussFWHM(wxCommandEvent& e) {
    double value;
    mCtrlGaussFWHM->GetValue().ToDouble(&value);
    mCtrlGaussPP->ChangeValue(wxString() << value/SQRT_2LN2);
}

void EasySpinFrame::OnGaussPP(wxCommandEvent& e) {
    double value;
    mCtrlGaussPP->GetValue().ToDouble(&value);
    mCtrlGaussFWHM->ChangeValue(wxString() << value*SQRT_2LN2);
}

void EasySpinFrame::OnLorentFWHM(wxCommandEvent& e) {
    double value;
    mCtrlLorentFWHM->GetValue().ToDouble(&value);
    mCtrlLorentPP->ChangeValue(wxString() << value/SQRT_3);
}

void EasySpinFrame::OnLorentPP(wxCommandEvent& e) {
    double value;
    mCtrlLorentPP->GetValue().ToDouble(&value);
    mCtrlLorentFWHM->ChangeValue(wxString() << value*SQRT_3);
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

EVT_RADIOBUTTON(ID_CRYSTAL,EasySpinFrame::OnCrystal)
EVT_RADIOBUTTON(ID_POWDER,EasySpinFrame::OnCrystal)

EVT_BUTTON(ID_ADD_ORIENT,   EasySpinFrame::OnAddOrient)
EVT_BUTTON(ID_DELETE_ORIENT,EasySpinFrame::OnDeleteOrient)

EVT_BUTTON(ID_SPACEGROUP_BUTTON,EasySpinFrame::OnShowSpaceGroups)

EVT_LIST_ITEM_ACTIVATED(ID_ORIENT_LIST,EasySpinFrame::OnOrientDClick)

//Options
EVT_SPINCTRL(ID_KNOTS,      EasySpinFrame::OnKnotsChange)
EVT_CHECKBOX(ID_INTERPON,EasySpinFrame::OnInterpCheck)

//Broadernings
EVT_TEXT(ID_GAUSS_FWHM ,EasySpinFrame::OnGaussFWHM)
EVT_TEXT(ID_GAUSS_PP   ,EasySpinFrame::OnGaussPP)
EVT_TEXT(ID_LORENT_FWHM,EasySpinFrame::OnLorentFWHM)
EVT_TEXT(ID_LORENT_PP  ,EasySpinFrame::OnLorentPP)


//Other
EVT_BUTTON(ID_TMP_GEN,   EasySpinFrame::OnGenerateButton)
EVT_TEXT(ID_PREVIEW,     EasySpinFrame::PreviewEdit)
EVT_CHOICE(ID_EASYSPIN_F,EasySpinFrame::OnEasySpinFunc)
EVT_BUTTON(ID_COPY,      EasySpinFrame::OnCopy)

//Catch everything and trigger a regeneration
EVT_TEXT(wxID_ANY, EasySpinFrame::OnGenerate)
EVT_BUTTON(wxID_ANY, EasySpinFrame::OnGenerate)
EVT_CHECKBOX(wxID_ANY, EasySpinFrame::OnGenerate)
EVT_CHOICE(wxID_ANY, EasySpinFrame::OnGenerate)
EVT_RADIOBUTTON(wxID_ANY, EasySpinFrame::OnGenerate)

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

struct __Init {
    __Init() {
        gModeSelectOrdering.push_back(EasySpinInput::PARALLEL);
        gModeSelectOrdering.push_back(EasySpinInput::PERPENDICULAR);

        gMethodSelectOrdering.push_back(EasySpinInput::MATRIX);
        gMethodSelectOrdering.push_back(EasySpinInput::PERT1);
        gMethodSelectOrdering.push_back(EasySpinInput::PERT2);

        gOutputSelectOrdering.push_back(EasySpinInput::SUMMED);
        gOutputSelectOrdering.push_back(EasySpinInput::SEPERATE);

        gEasySpinSelectOrdering.push_back(EasySpinInput::GARLIC);
        gEasySpinSelectOrdering.push_back(EasySpinInput::CHILI);
        gEasySpinSelectOrdering.push_back(EasySpinInput::PEPPER);
    }
};
static __Init __init;
