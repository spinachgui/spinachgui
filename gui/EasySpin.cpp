
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

#include <sigc++/sigc++.h>

#define ORIENT_ICON_SIZE 41

using namespace std;
using namespace sigc;
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

void drawOrientationIcon(wxBitmap& bitmap,const Orientation* o) {
    wxMemoryDC dc;
    dc.SelectObject(bitmap);

    dc.Clear();
    dc.DrawCircle(ORIENT_ICON_SIZE/2,ORIENT_ICON_SIZE/2,ORIENT_ICON_SIZE/2-4);
}

//================================================================================//
// EasySpin struct, a structure for storing an easyspin experiment

struct EasySpinInput {
    EasySpinInput() {
    }

    string generate() const {
        ostringstream oss;

        oss << "Sys = struct('g',[2.0088 2.0061 2.0027],'Nucs','14N','A',A);" << endl;

        oss << endl;

        oss << "Exp.mwFreq = " << mMWFreq << ";" << endl;
        oss << "Exp.CenterSweep = [" << mCentre << " " << mSweep << "];" << endl;
        oss << endl;
        oss << "Exp.Harmonic = " << mHarmonic << ";" << endl;
        oss << "Exp.nPoints = "  << mNPoints << ";" << endl;
        oss << "Exp.Mode = '"     << mModeNames[mMode] << "';" << endl;
        oss << "Exp.mwPhase = "  << mMWPhase << ";" << endl;

        oss << endl;
        if(mModAmp) {
            oss << "Exp.ModAmp = " << mModAmp.get() << ";" << endl;}
        if(mTemperature) {
            oss << "Exp.Temperature = " << mTemperature.get() << ";" << endl;}

        oss << mEasySpinNames[mEasySpinFunc] << "(Sys,Exp)" << endl;

        return oss.str();
    }

    //================================================================================//
    // EasySpin function
public:
    enum EasySpinFunc {
        GARLIC,
        CHILI,
        PEPPER
    };
    void setEasySpinFunction(EasySpinFunc easySpinFunc) {
        mEasySpinFunc = easySpinFunc;
    }
private:
    EasySpinFunc mEasySpinFunc;

    //================================================================================//
    // Experiment

public:
    void setCentreSweep(double centre,double sweep) {
        mCentre = centre;
        mSweep = sweep;
    }
private:
    double mCentre;
    double mSweep;

    //----------------------------------------//

public:
    void setMWFreq(double mwFreq) {
        mMWFreq = mwFreq;
    }
private:
    double mMWFreq;

    //----------------------------------------//

public:
    void setTemperature(double temperature) {
        mTemperature = temperature;
    }
private:
    optional<double> mTemperature;
    
    //----------------------------------------//

public:
    void setModAmp(double modAmp) {
        mModAmp = modAmp;
    }
private:
    optional<double> mModAmp;


    //----------------------------------------//
public:
    void setNPoints(unsigned long nPoints) {
        mNPoints = nPoints;
    }
private:
    unsigned long mNPoints;
    //----------------------------------------//
public:
    void setHarmonic(unsigned long harmonic) {
        mHarmonic = harmonic;
    }
private:
    unsigned long mHarmonic;
    //----------------------------------------//
public:
    enum Mode {
        PARALLEL,
        PERPENDICULAR
    };
    void setMode(Mode mode) {
        mMode = mode;
    }
private:
    Mode mMode;
    //----------------------------------------//

    //Should only be set iff mSampleState = Cystal
public:
    void setSpaceGroup(unsigned long spaceGroup) {
        mSpaceGroup = spaceGroup;
    }
private:
    boost::optional<unsigned long> mSpaceGroup;
    //----------------------------------------//
public:
    //TODO: Crystal Orientations
private:
    //----------------------------------------//
    //Valid range is 0 to 2Pi
public:
    void setMWPhase(unsigned long mwPhase) {
        mMWPhase = mwPhase;
    }
private:
    double mMWPhase;

    //================================================================================//
    // Options
public:
    enum Method {
        MATRIX,
        PERT1,
        PERT2
    };
    void setMethod(Method method) {
        mMethod = method;
    }
private:
    Method mMethod;

public:
    void setNKnots(unsigned long nKnots) {
        mNKnots = nKnots;
    }
private:
    //0 => unused/default
    unsigned long mNKnots;

public:
    void setInterpolate(unsigned long interpolate) {
        mInterpolate = interpolate;
    }
private:
    //0 => no interpolation
    unsigned long mInterpolate;

public:
    enum Output {
        SUMMED,
        SEPERATE
    };
    void setOutput(Output output) {
        mOutput = output;
    }
private:
    Output mOutput;

public:
    //Static stuff
    static void staticCtor() {
        mModeNames[PARALLEL] = "parallel";
        mModeNames[PERPENDICULAR] = "perpendicular";

        mMethodNames[MATRIX] = "matrix?";
        mMethodNames[PERT1] = "pert1?";
        mMethodNames[PERT2] = "pert2?";

        mOutputNames[SUMMED] = "summed?";
        mOutputNames[SEPERATE] = "seperate?";

        mEasySpinNames[GARLIC] = "garlic";
        mEasySpinNames[CHILI] = "chili";
        mEasySpinNames[PEPPER] = "pepper";
    }
    struct __Init {
        __Init() {
            staticCtor();
        }
    };
    static map<Mode  ,string> mModeNames;
    static map<Method,string> mMethodNames;
    static map<Output,string> mOutputNames;
    static map<EasySpinFunc,string> mEasySpinNames;


private:
    static __Init __init;
};
// NB Make sure the maps are defined before __init or the static
// constructor called by __init will try to use them before they are
// constructed.
map<EasySpinInput::Mode        ,string> EasySpinInput::mModeNames;
map<EasySpinInput::Method      ,string> EasySpinInput::mMethodNames;
map<EasySpinInput::Output      ,string> EasySpinInput::mOutputNames;
map<EasySpinInput::EasySpinFunc,string> EasySpinInput::mEasySpinNames;
EasySpinInput::__Init EasySpinInput::__init;

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

    mOrientEditPanel = new OrientEditPanel(mPanelCrystal);
    mOrientEditPanel->Enable(true);
    mSizerOrient->Add(mOrientEditPanel,1,wxEXPAND,2);

    //Options section
    mCtrlKnots->SetRange(2,INT_MAX);
    mCtrlAngularRes->sigUnFocus.connect(mem_fun(this,&EasySpinFrame::SlotAngularResUnFocus));
    mCtrlInterp->SetRange(2,INT_MAX);

    HideCrystal(true);
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
    mOrientEditPanel->Show(!hide);
    mSpaceGroupText->Show(!hide);

    mExpNotebookPage->Layout();
    mPanelCrystal->Layout();
}

void EasySpinFrame::RepopulateCrystalOrients() {
    mCtrlOrients->Clear();
    long i = 0;
    foreach(Orientation o,mOrients) {
        mCtrlOrients->Append(wxString() << wxT("Orientation") << i);
        i++;
    }
}

void EasySpinFrame::OnAddOrient(wxCommandEvent& e) {
    mOrients.push_back(Orientation());
    RepopulateCrystalOrients();
}

void EasySpinFrame::OnDeleteOrient(wxCommandEvent& e) {
    if(mCtrlOrients->GetSelection() != wxNOT_FOUND) {
        mOrients.erase(mOrients.begin()+mCtrlOrients->GetSelection());
        RepopulateCrystalOrients();
    }
}

void EasySpinFrame::OnOrientSelect(wxCommandEvent& e) {

}

void EasySpinFrame::OnOrientDClick(wxCommandEvent& e) {
    if(mCtrlOrients->GetSelection() == wxNOT_FOUND) {
        //Not sure if this can actually happen
        return;
    }
    OrientEditDialog* orientDialog = new OrientEditDialog(this);
    orientDialog->SetOrient(mOrients[mCtrlOrients->GetSelection()]);
    if (orientDialog->ShowModal() == wxID_OK) {
        mOrients[mCtrlOrients->GetSelection()] = orientDialog->GetOrient();
    }
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
    string easyCode = easySpinInput.generate();
    
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
}

void EasySpinFrame::OnCopy(wxCommandEvent& e) {
    if (wxTheClipboard->Open()) {
        wxTheClipboard->SetData(new wxTextDataObject(mCtrlPreview->GetValue()));
        wxTheClipboard->Close();
    } else {
        wxLogError(wxT("Could not copy to clipboard. Clipboard may be being used by another application."));
    }
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

EVT_LISTBOX(ID_ORIENT_LIST,EasySpinFrame::OnOrientSelect)
EVT_LISTBOX_DCLICK(ID_ORIENT_LIST,EasySpinFrame::OnOrientDClick)

//Options
EVT_SPINCTRL(ID_KNOTS,      EasySpinFrame::OnKnotsChange)
EVT_CHECKBOX(ID_INTERPON,EasySpinFrame::OnInterpCheck)

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
