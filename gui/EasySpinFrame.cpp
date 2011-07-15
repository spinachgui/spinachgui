 
#include <gui/EasySpinFrame.hpp>
#include <wx/valtext.h>

#include <gui/OrientationEdit.hpp>
#include <gui/TextCtrlFocus.hpp>
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
#include <wx/filedlg.h>
#include <fstream>
#include <math.h>

#define ORIENT_ICON_SIZE 41

#define SQRT_2LN2 1.1774100225154747
#define SQRT_3    1.7320508075688773
#define LOG10_6   0.77815125038364363  //Log10(6)

//These numbers correspon to the order of the pages in the dropdown,
//i.e. if mCtrlEasySpinF->GetSelection() == GARLIC_TAB is true the selection is garlic
#define PAGE_GARLIC 0
#define PAGE_CHILI 1
#define PAGE_PEPPER 2


using namespace std;
using namespace sigc;
using namespace Eigen;
using namespace SpinXML;
using boost::optional;

typedef optional<double> maybeDouble;

#ifdef __WINDOWS__
double round(double r) {
    return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}
#endif

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
vector<EasySpinInput::Method      > gGarlicMethodSelectOrdering;  
vector<EasySpinInput::Method      > gPepperMethodSelectOrdering;  
vector<EasySpinInput::Output      > gOutputSelectOrdering;  
vector<EasySpinInput::EasySpinFunc> gEasySpinSelectOrdering;

//================================================================================//
// GUI Functions


template<typename T>
wxString maybeToWXString(optional<T> maybe) {
    if(maybe) {
        return wxString() << maybe.get();
    } else {
        return wxString();
    }
}

optional<double> getOptionalValue(const wxTextCtrl* textCtrl) {
    wxString str = textCtrl->GetValue();
    if(str == wxT("")) {
         //Return an empty optional
        return optional<double>();
    }
    double value;
    str.ToDouble(&value);
    return value;
}

optional<double> getOptionalValue(const wxComboBox* textCtrl) {
    wxString str = textCtrl->GetValue();
    if(str == wxT("")) {
         //Return an empty optional
        return optional<double>();
    }
    double value;
    str.ToDouble(&value);
    return value;
}

optional<unsigned long> getOptionalULong(const wxComboBox* textCtrl) {
    wxString str = textCtrl->GetValue();
    if(str == wxT("")) {
         //Return an empty optional
        return optional<unsigned long>();
    }
    unsigned long value;
    str.ToULong(&value);
    return value;
}

template<typename T,typename T2>
optional<T> multOptional(optional<T> o,T2 n) {
    return o ? o.get()*T(n) : optional<T>();
}

EasySpinFrame::EasySpinFrame(wxWindow* parent,
                             wxWindowID id,
                             const wxString& title,
                             const wxPoint& pos,
                             const wxSize& size,
                             long style) 
    : EasySpinFrameBase(parent,id,title,pos,size,style),mSpinSystem(NULL),mSpacegroupSelected(-1),mOrientDialogCombo(NULL) {
    //It's best to make sure everything is instanciated first before customising properties
	//in case events get fired. Fun fact, wxSpinCtrl may emit events when wxSpinCtrl::SetRange is called on windows
	//but it doens't seem to under gtk.
	mOrientDialogCombo = new OrientDialogCombo(mDynamicsPage,-1);
	mSpaceGroupDialog = new SpaceGroupDialog(this);

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

    mCtrlSpaceGroupTxt->sigFocus.connect  (mem_fun(this,&EasySpinFrame::OnSpaceGroupFocus));
    mCtrlSpaceGroupTxt->sigUnFocus.connect(mem_fun(this,&EasySpinFrame::OnSpaceGroupUnfocus));

    //Options section
    mCtrlKnots->SetRange(2,INT_MAX);
    mCtrlAngularRes->sigUnFocus.connect(mem_fun(this,&EasySpinFrame::SlotAngularResUnFocus));
    mCtrlInterp->SetRange(2,INT_MAX);
    
    //No bounds checking on these yet
    mCtrlEvenLMax->SetRange(INT_MIN,INT_MAX);
    mCtrlOddLMax->SetRange(INT_MIN,INT_MAX);
    mCtrlKMax->SetRange(INT_MIN,INT_MAX);
    mCtrlMMax->SetRange(INT_MIN,INT_MAX);

    //Set the MOND checkbox to the correct inital state
    {wxCommandEvent tmp; OnLambda(tmp);}

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

    //Dynamics for Chili
    mCtrlLambda20->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    mCtrlLambda22->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    mCtrlLambda40->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    mCtrlLambda42->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    mCtrlLambda44->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));

    mCtrlTCorr->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    mCtrlDiff->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));

    mCtrlTCorrXY->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    mCtrlTCorrAxialZ->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    mCtrlDiffXY->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    mCtrlDiffAxialZ->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));

    mCtrlTCorrX->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    mCtrlTCorrY->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    mCtrlTCorrZ->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    mCtrlDiffX->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    mCtrlDiffY->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));
    mCtrlDiffZ->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));

    mCtrlExchange->SetValidator(wxTextValidator(wxFILTER_NUMERIC,NULL));

	mSizerRotCorrOrient->Add(mOrientDialogCombo,0,wxALIGN_CENTER,0);

    //Make strain based broaderning hidded until the user selects pepper
    mSizerStrain->Show(false);

    //Start on the experiment page
    mNotebook->SetSelection(1);
    //Hide the options sections that should be hidden
    {wxCommandEvent tmp;OnEasySpinFunc(tmp);}


    HideCrystal(true);
}

void EasySpinFrame::SetSpinsys(SpinXML::SpinSystem* spinsys) {
    mSpinSystem = spinsys;
    if(mSpinSystem == NULL) {
        return;
    }

    //gStrain and AStrain are only applicable for one electron spin, D
    //strain is only applicable for multipul electron spins
    unsigned long nElectrons = 0;
    foreach(Spin* spin,mSpinSystem->GetSpins()) {
        if(spin->GetElement() == 0) {
            nElectrons++;
            if(nElectrons >= 2) {
                break;
            }
        }
    }
    mCtrlGX->Enable(nElectrons == 1);
    mCtrlGY->Enable(nElectrons == 1);
    mCtrlGZ->Enable(nElectrons == 1);

    mCtrlAX->Enable(nElectrons == 1);
    mCtrlAY->Enable(nElectrons == 1);
    mCtrlAZ->Enable(nElectrons == 1);

    mCtrlDD->Enable(nElectrons > 1);
    mCtrlDE->Enable(nElectrons > 1);
    mCtrlDCorrCoeff->Enable(nElectrons > 1);
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
    e.Skip();
    mOrients.push_back(Orientation());
    RepopulateCrystalOrients();
}

void EasySpinFrame::OnDeleteOrient(wxCommandEvent& e) {
    e.Skip();
    long itemIndex = mCtrlOrients->GetNextItem(-1,
                                               wxLIST_NEXT_ALL,
                                               wxLIST_STATE_SELECTED);
    if (itemIndex != -1) {
            mOrients.erase(mOrients.begin()+itemIndex);
            RepopulateCrystalOrients();
    }
}


void EasySpinFrame::OnOrientDClick(wxListEvent& e) {
    e.Skip();
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
    e.Skip();
    //TODO: if we keep the constructed object around it would probably
    //load faster the second time around
    if(mSpaceGroupDialog->ShowModal() == wxID_OK) {
        mCtrlSpaceGroupTxt->ChangeValue(wxString() << mSpaceGroupDialog->spacegroup);
        OnSpaceGroupUnfocus();
    }
}

void EasySpinFrame::OnSpaceGroupTxt(wxCommandEvent& e) {
    e.Skip();
    //Try to decide which space group the user ment and display that

    //NB: Currently this doesn't really seem to work
    /*wxString value = mCtrlSpaceGroupTxt->GetValue();

    string compleated_value = findSpacegroup(string(value.mb_str()));
    if(compleated_value != "") {
        mCtrlSpaceGroupTxt->ChangeValue(wxString(compleated_value.c_str(),wxConvUTF8));
        }*/
}

void EasySpinFrame::OnSpaceGroupFocus() {
    if(mCtrlSpaceGroupTxt->GetValue() == wxT("(Unknown space group)")) {
        mCtrlSpaceGroupTxt->ChangeValue(wxT(""));
    }
}

void EasySpinFrame::OnSpaceGroupUnfocus() {
    //Try to decide which space group the user ment and display that
    wxString value = mCtrlSpaceGroupTxt->GetValue();
    
    unsigned long number = parseStandardForm(string(value.mb_str()));
    if(number != 0) {
        string name = nameSpacegroup(number);
        mCtrlSpaceGroupTxt->ChangeValue(wxString() << number 
                                        << wxT(" (") << wxString(name.c_str(),wxConvUTF8) << wxT(")"));
    } else {
        mCtrlSpaceGroupTxt->ChangeValue(wxT("(Unknown space group)"));
    }
}

void EasySpinFrame::OnAutoGen(wxCommandEvent& e) {
    //User has decided to discard changes
    OnGenerate(e);
}

void EasySpinFrame::OnGenerate(wxCommandEvent& e) {
    e.Skip();
    if(!mCtrlAutoGen->GetValue()) {
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

    easySpinInput.mNPoints = getOptionalULong(mCtrlNPoints);

    easySpinInput.setHarmonic(mCtrlHarmonic->GetSelection());

    easySpinInput.setMode(gModeSelectOrdering[mCtrlMode->GetSelection()]);

    if(mRadioCrystal->GetValue()) {
        string value = string(mCtrlSpaceGroupTxt->GetValue().mb_str());
        unsigned long sgNum = parseStandardForm(value);
        if(sgNum != 0) {
            easySpinInput.mSpaceGroup = sgNum;
        }
        easySpinInput.mOrients = mOrients;
    }

    

    if(mCtrlModAmpOn->GetValue()) {
        double modAmp;
        mCtrlModAmp->GetValue().ToDouble(&modAmp);
        easySpinInput.setModAmp(modAmp*toMiliT);
    }

    easySpinInput.mMWPhase = getOptionalValue(mCtrlPhase);

    if(mCtrlEasySpinF->GetSelection() == PAGE_PEPPER) {
        easySpinInput.mMethod = gPepperMethodSelectOrdering[mCtrlPepperMethod->GetSelection()];
    } else if(mCtrlEasySpinF->GetSelection() == PAGE_GARLIC) {
        easySpinInput.mMethod = gGarlicMethodSelectOrdering[mCtrlGarlicMethod->GetSelection()];
    }

    easySpinInput.setNKnots(mCtrlKnots->GetValue());
    easySpinInput.setInterpolate(mCtrlInterp->GetValue());

    easySpinInput.setOutput(gOutputSelectOrdering[mCtrlOutput->GetSelection()]);
    
    //Chili Dynamics
    easySpinInput.mLambda20    =  getOptionalValue(mCtrlLambda20   );
    easySpinInput.mLambda22    =  getOptionalValue(mCtrlLambda22   );
    easySpinInput.mLambda40    =  getOptionalValue(mCtrlLambda40   );
    easySpinInput.mLambda42    =  getOptionalValue(mCtrlLambda42   );
    easySpinInput.mLambda44    =  getOptionalValue(mCtrlLambda44   );

    switch(mCtrlRotationCorrType->GetSelection()) {
    case 0:
        easySpinInput.mTCorr       =  getOptionalValue(mCtrlTCorr      );
        break;
    case 1:                                                     
        easySpinInput.mTCorrXY     =  getOptionalValue(mCtrlTCorrXY    );
        easySpinInput.mTCorrAxialZ =  getOptionalValue(mCtrlTCorrAxialZ);
        break;
    case 2:                                                
        easySpinInput.mTCorrX      =  getOptionalValue(mCtrlTCorrX     );
        easySpinInput.mTCorrY      =  getOptionalValue(mCtrlTCorrY     );
        easySpinInput.mTCorrZ      =  getOptionalValue(mCtrlTCorrZ     );
        break;
    }
    easySpinInput.mDynamicsOrient = mOrientDialogCombo->GetOrient();
    //Okay, what we have the mTCorr<X> variables is either the
    //required value in some unit OR the logarithm. we don't need to
    //do anything with the logarithm but we do need to convert down to
    //SI native units
    easySpinInput.mTCorrUseLog = mDiffusionLog10->GetValue();

    if(!easySpinInput.mTCorrUseLog) {
        //0 is ns, 1 is us
        double unit = mCtrlTCorrIsoUnit->GetSelection() == 0 ? 1e-9 : 1e-6;
        easySpinInput.mTCorr       =  multOptional(getOptionalValue(mCtrlTCorr      ),unit);

        easySpinInput.mTCorrXY     =  multOptional(getOptionalValue(mCtrlTCorrXY    ),unit);
        easySpinInput.mTCorrAxialZ =  multOptional(getOptionalValue(mCtrlTCorrAxialZ),unit);

        easySpinInput.mTCorrX      =  multOptional(getOptionalValue(mCtrlTCorrX     ),unit);
        easySpinInput.mTCorrY      =  multOptional(getOptionalValue(mCtrlTCorrY     ),unit);
        easySpinInput.mTCorrZ      =  multOptional(getOptionalValue(mCtrlTCorrZ     ),unit);
    }

    easySpinInput.mExchange    =  getOptionalValue(mCtrlExchange   );

    //Broadernings
    double unit = mCtrlConvUnit->GetSelection() == 0 ? 1 : 0.1;
    easySpinInput.mGaussianFWHM = multOptional(getOptionalValue(mCtrlGaussFWHM), unit);
    easySpinInput.mLorentFWHM =   multOptional(getOptionalValue(mCtrlLorentFWHM),unit);

    if(mCtrlEasySpinF->GetSelection() == PAGE_PEPPER) {
        easySpinInput.mHStrainX = getOptionalValue(mCtrlHX);
        easySpinInput.mHStrainY = getOptionalValue(mCtrlHY);
        easySpinInput.mHStrainZ = getOptionalValue(mCtrlHZ);
                                         
        easySpinInput.mGStrainX = getOptionalValue(mCtrlGX);
        easySpinInput.mGStrainY = getOptionalValue(mCtrlGY);
        easySpinInput.mGStrainZ = getOptionalValue(mCtrlGZ);
                                         
        easySpinInput.mAStrainX = getOptionalValue(mCtrlAX);
        easySpinInput.mAStrainY = getOptionalValue(mCtrlAY);
        easySpinInput.mAStrainZ = getOptionalValue(mCtrlAZ);
                                         
        easySpinInput.mDStrainD = getOptionalValue(mCtrlDD);
        easySpinInput.mDStrainE = getOptionalValue(mCtrlDE);
        easySpinInput.mDStrainCorrCoeff = getOptionalValue(mCtrlDCorrCoeff);
    }

    easySpinInput.mMOND = mCtrlUseMOND->GetValue();

    easySpinInput.mEvenLMax = mCtrlEvenLMax->GetValue();
    easySpinInput.mOddLMax  = mCtrlOddLMax->GetValue();
    easySpinInput.mKMax     = mCtrlKMax->GetValue();
    easySpinInput.mMMax     = mCtrlMMax->GetValue();

    //Do code generation
    string easyCode = easySpinInput.generate(mSpinSystem);
    
    mCtrlPreview->ChangeValue(wxString(easyCode.c_str(),wxConvUTF8));
}

void EasySpinFrame::PreviewEdit(wxCommandEvent& e) {
    mCtrlAutoGen->SetValue(false);
}

void EasySpinFrame::OnEasySpinFunc(wxCommandEvent& e) {
    e.Skip();
    unsigned int n = mCtrlEasySpinF->GetSelection();

    mSizerLLKM->Show(n == PAGE_CHILI);
    mSizerPepperMethod->Show(n==PAGE_PEPPER);
    mSizerGarlicMethod->Show(n==PAGE_GARLIC);
    mSizerKnots->Show(n == PAGE_PEPPER || n == PAGE_CHILI);
    mSizerOutput->Show(n == PAGE_PEPPER);

    //Make sure the hoizonal lines look right
    mLnTop->Show(n == PAGE_CHILI);
    mLnBottom->Show(n == PAGE_PEPPER || n == PAGE_CHILI);

    mCtrlUseMOND->Show(n == PAGE_CHILI);
    mOptionsPage->Layout();

    mDynamicsPage->Show(n == PAGE_CHILI);
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
    e.Skip();
    maybeDouble value = getOptionalValue(mCtrlGaussFWHM);
    if(value) {
        mCtrlGaussPP->ChangeValue(wxString() << value.get()/SQRT_2LN2);
    } else {
        mCtrlGaussPP->ChangeValue(wxT(""));
    }
}

void EasySpinFrame::OnGaussPP(wxCommandEvent& e) {
    e.Skip();
    maybeDouble value = getOptionalValue(mCtrlGaussPP);
    if(value) {
        mCtrlGaussFWHM->ChangeValue(wxString() << value.get()*SQRT_2LN2);
    } else {
        mCtrlGaussFWHM->ChangeValue(wxT(""));
    }
}

void EasySpinFrame::OnLorentFWHM(wxCommandEvent& e) {
    e.Skip();
    maybeDouble value = getOptionalValue(mCtrlLorentFWHM);
    if(value) {
        mCtrlLorentPP->ChangeValue(wxString() << value.get()/SQRT_3);
    } else {
        mCtrlLorentPP->ChangeValue(wxT(""));
    }
}

void EasySpinFrame::OnLorentPP(wxCommandEvent& e) {
    e.Skip();
    maybeDouble value = getOptionalValue(mCtrlLorentPP);
    if(value) {
        mCtrlLorentFWHM->ChangeValue(wxString() << value.get()*SQRT_3);
    } else {
        mCtrlLorentFWHM->ChangeValue(wxT(""));
    }
}

void EasySpinFrame::OnCorrCoeff(wxCommandEvent& e) {
    e.Skip();
    double value;
    mCtrlDCorrCoeff->GetValue().ToDouble(&value);
    if(value > 1) {
        mCtrlDCorrCoeff->ChangeValue(wxT("1"));
    } else if(value < -1) {
        mCtrlDCorrCoeff->ChangeValue(wxT("-1"));
    }
}

void EasySpinFrame::OnSave(wxCommandEvent& e) {
    wxFileDialog* fd=new wxFileDialog(this,
                                      wxString(wxT("Choose a file")),
                                      wxString(wxT("")), //Default file
                                      wxString(wxT("")), //Default dir
                                      wxT("MATLAB files (*.m)|*.m"),
                                      wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if(fd->ShowModal() == wxID_OK) {
        string filename(fd->GetPath().mb_str());
        ofstream file(filename.c_str());
        if(!file.is_open()) {
            wxLogError(wxT("Unknown error while saving file"));
            delete fd;
            return;
        }
        file << mCtrlPreview->GetValue().mb_str();
    }
    delete fd;
}

void EasySpinFrame::OnOrientEdit(wxCommandEvent& e) {
    e.Skip();
    wxListEvent listEvent;
    //The double click handler isn't compleatly trivial, so re-use the
    //logic by fakeing the call
    OnOrientDClick(listEvent);
}

//TCorr = 1/(6*Diff). This function does the interconversion both ways
template<typename T>
maybeDouble maybeOver6X(optional<T> maybe) {
    if(maybe) {
        return optional<T>(1/(6*maybe.get()));
    } else {
        return optional<T>();
    }
}

//log(TCorr) = log(1/(6*Diff)). This function does the interconversion both ways
template<typename T>
maybeDouble maybeLogOver6X(optional<T> maybe) {
    if(maybe) {
        double ans = -log10(maybe.get())-LOG10_6;
        return optional<T>(ans);
    } else {
        return optional<T>();
    }
}

//Does what it says on the tin. Haskell maybe monad would be really
//useful about now.
template<typename T>
optional<T> maybeLog(optional<T> maybe) {
    if(maybe) {
        double ans = log10(maybe.get());
        return optional<T>(ans);
    } else {
        return optional<T>();
    }
}

//Does what it says on the tin. Haskell maybe monad would be really
//useful about now.
template<typename T>
optional<T> maybePow10(optional<T> maybe) {
    if(maybe) {
        double ans = pow(10,maybe.get());
        return optional<T>(ans);
    } else {
        return optional<T>();
    }
}


void EasySpinFrame::OnLog10Toggle(wxCommandEvent& e) {
    e.Skip();
    mCtrlTCorrIsoUnit->Enable(!mDiffusionLog10->GetValue());
    mCtrlTCorrAxUnit->Enable( !mDiffusionLog10->GetValue());
    mCtrlTCorrRhUnit->Enable( !mDiffusionLog10->GetValue());

    mCtrlDiffIsoUnit->Enable( !mDiffusionLog10->GetValue());
    mCtrlDiffAxUnit->Enable(  !mDiffusionLog10->GetValue());
    mCtrlDiffRhUnit->Enable(  !mDiffusionLog10->GetValue());

    double tCorrToSi  = mCtrlTCorrIsoUnit->GetSelection() == 0 ? 1e-9 : 1e-6;
    double diffFromSi = mCtrlDiffIsoUnit->GetSelection()  == 0 ? 1e-9 : 1e-6;

    /*
      Okay, so these lines got kind of crazy with the f(g(h(x))) style
      composed functions. Just read it right to left and imagine it's a unix pipe
     */

    if(mDiffusionLog10->GetValue()) {
        //Converting to log
        mCtrlTCorr      ->ChangeValue(maybeToWXString(maybeLog(multOptional(getOptionalValue(mCtrlTCorr)      ,tCorrToSi))));
                                                                                                                        
        mCtrlTCorrXY    ->ChangeValue(maybeToWXString(maybeLog(multOptional(getOptionalValue(mCtrlTCorrXY)    ,tCorrToSi))));
        mCtrlTCorrAxialZ->ChangeValue(maybeToWXString(maybeLog(multOptional(getOptionalValue(mCtrlTCorrAxialZ),tCorrToSi))));
                                                                                                                        
        mCtrlTCorrX     ->ChangeValue(maybeToWXString(maybeLog(multOptional(getOptionalValue(mCtrlTCorrX)     ,tCorrToSi))));
        mCtrlTCorrY     ->ChangeValue(maybeToWXString(maybeLog(multOptional(getOptionalValue(mCtrlTCorrY)     ,tCorrToSi))));
        mCtrlTCorrZ     ->ChangeValue(maybeToWXString(maybeLog(multOptional(getOptionalValue(mCtrlTCorrZ)     ,tCorrToSi))));

        mCtrlDiff       ->ChangeValue(maybeToWXString(maybeLog(multOptional(getOptionalValue(mCtrlDiff      ),diffFromSi))));
                                                                                                                        
        mCtrlDiffXY     ->ChangeValue(maybeToWXString(maybeLog(multOptional(getOptionalValue(mCtrlDiffXY    ),diffFromSi))));
        mCtrlDiffAxialZ ->ChangeValue(maybeToWXString(maybeLog(multOptional(getOptionalValue(mCtrlDiffAxialZ),diffFromSi))));
                                                                                                                        
        mCtrlDiffX      ->ChangeValue(maybeToWXString(maybeLog(multOptional(getOptionalValue(mCtrlDiffX     ),diffFromSi))));
        mCtrlDiffY      ->ChangeValue(maybeToWXString(maybeLog(multOptional(getOptionalValue(mCtrlDiffY     ),diffFromSi))));
        mCtrlDiffZ      ->ChangeValue(maybeToWXString(maybeLog(multOptional(getOptionalValue(mCtrlDiffZ     ),diffFromSi))));
    } else {
        //Converting from log
        mCtrlTCorr      ->ChangeValue(maybeToWXString(multOptional(maybePow10(getOptionalValue(mCtrlTCorr))      ,1/tCorrToSi)));
                                                                                                                          
        mCtrlTCorrXY    ->ChangeValue(maybeToWXString(multOptional(maybePow10(getOptionalValue(mCtrlTCorrXY))    ,1/tCorrToSi)));
        mCtrlTCorrAxialZ->ChangeValue(maybeToWXString(multOptional(maybePow10(getOptionalValue(mCtrlTCorrAxialZ)),1/tCorrToSi)));
                                                                                                                          
        mCtrlTCorrX     ->ChangeValue(maybeToWXString(multOptional(maybePow10(getOptionalValue(mCtrlTCorrX))     ,1/tCorrToSi)));
        mCtrlTCorrY     ->ChangeValue(maybeToWXString(multOptional(maybePow10(getOptionalValue(mCtrlTCorrY))     ,1/tCorrToSi)));
        mCtrlTCorrZ     ->ChangeValue(maybeToWXString(multOptional(maybePow10(getOptionalValue(mCtrlTCorrZ))     ,1/tCorrToSi)));
                                                                             
        mCtrlDiff       ->ChangeValue(maybeToWXString(multOptional(maybePow10(getOptionalValue(mCtrlDiff      )) ,1/diffFromSi)));
                                                                                                                           
        mCtrlDiffXY     ->ChangeValue(maybeToWXString(multOptional(maybePow10(getOptionalValue(mCtrlDiffXY    )) ,1/diffFromSi)));
        mCtrlDiffAxialZ ->ChangeValue(maybeToWXString(multOptional(maybePow10(getOptionalValue(mCtrlDiffAxialZ)) ,1/diffFromSi)));
                                                                                                                           
        mCtrlDiffX      ->ChangeValue(maybeToWXString(multOptional(maybePow10(getOptionalValue(mCtrlDiffX     )) ,1/diffFromSi)));
        mCtrlDiffY      ->ChangeValue(maybeToWXString(multOptional(maybePow10(getOptionalValue(mCtrlDiffY     )) ,1/diffFromSi)));
        mCtrlDiffZ      ->ChangeValue(maybeToWXString(multOptional(maybePow10(getOptionalValue(mCtrlDiffZ     )) ,1/diffFromSi)));
    }
}

wxString maybeOver6XComposed(wxTextCtrl* mCtrl,double inputUnit,double outputUnit) {
    maybeDouble inSI = multOptional(getOptionalValue(mCtrl),inputUnit);
    return maybeToWXString(multOptional(maybeOver6X(inSI),outputUnit));
}

void EasySpinFrame::OnTCorr(wxCommandEvent& e) {
    e.Skip();
    if(!mDiffusionLog10->GetValue()) {
        double tCorrToSi  = mCtrlTCorrIsoUnit->GetSelection() == 0 ? 1e-9 : 1e-6;
        double diffFromSi = mCtrlDiffIsoUnit->GetSelection()  == 0 ? 1e-9 : 1e-6;

        mCtrlDiff      ->ChangeValue(maybeOver6XComposed(mCtrlTCorr      ,tCorrToSi,diffFromSi));
                                                                                              
        mCtrlDiffXY    ->ChangeValue(maybeOver6XComposed(mCtrlTCorrXY    ,tCorrToSi,diffFromSi));
        mCtrlDiffAxialZ->ChangeValue(maybeOver6XComposed(mCtrlTCorrAxialZ,tCorrToSi,diffFromSi));
                                                                                              
        mCtrlDiffX     ->ChangeValue(maybeOver6XComposed(mCtrlTCorrX     ,tCorrToSi,diffFromSi));
        mCtrlDiffY     ->ChangeValue(maybeOver6XComposed(mCtrlTCorrY     ,tCorrToSi,diffFromSi));
        mCtrlDiffZ     ->ChangeValue(maybeOver6XComposed(mCtrlTCorrZ     ,tCorrToSi,diffFromSi));
    } else {
        mCtrlDiff      ->ChangeValue(maybeToWXString(maybeLogOver6X(getOptionalValue(mCtrlTCorr))));

        mCtrlDiffXY    ->ChangeValue(maybeToWXString(maybeLogOver6X(getOptionalValue(mCtrlTCorrXY))));
        mCtrlDiffAxialZ->ChangeValue(maybeToWXString(maybeLogOver6X(getOptionalValue(mCtrlTCorrAxialZ))));

        mCtrlDiffX ->ChangeValue(maybeToWXString(maybeLogOver6X(getOptionalValue(mCtrlTCorrX))));
        mCtrlDiffY ->ChangeValue(maybeToWXString(maybeLogOver6X(getOptionalValue(mCtrlTCorrY))));
        mCtrlDiffZ ->ChangeValue(maybeToWXString(maybeLogOver6X(getOptionalValue(mCtrlTCorrZ))));
    }
}

void EasySpinFrame::OnDiff(wxCommandEvent& e) {
    e.Skip();
    if(!mDiffusionLog10->GetValue()) {
        double diffToSi    = mCtrlDiffIsoUnit->GetSelection() == 0 ? 1e+9 : 1e+6;
        double tCorrFromSi = mCtrlTCorrIsoUnit->GetSelection()  == 0 ? 1e+9 : 1e+6;

        mCtrlTCorr      ->ChangeValue(maybeOver6XComposed(mCtrlDiff      ,diffToSi,tCorrFromSi));
                                                                                                         
        mCtrlTCorrXY    ->ChangeValue(maybeOver6XComposed(mCtrlDiffXY    ,diffToSi,tCorrFromSi));
        mCtrlTCorrAxialZ->ChangeValue(maybeOver6XComposed(mCtrlDiffAxialZ,diffToSi,tCorrFromSi));
                                                                                                         
        mCtrlTCorrX     ->ChangeValue(maybeOver6XComposed(mCtrlDiffX     ,diffToSi,tCorrFromSi));
        mCtrlTCorrY     ->ChangeValue(maybeOver6XComposed(mCtrlDiffY     ,diffToSi,tCorrFromSi));
        mCtrlTCorrZ     ->ChangeValue(maybeOver6XComposed(mCtrlDiffZ     ,diffToSi,tCorrFromSi));
    } else {
        mCtrlTCorr      ->ChangeValue(maybeToWXString(maybeLogOver6X(getOptionalValue(mCtrlDiff      ))));
                                                                                        
        mCtrlTCorrXY    ->ChangeValue(maybeToWXString(maybeLogOver6X(getOptionalValue(mCtrlDiffXY    ))));
        mCtrlTCorrAxialZ->ChangeValue(maybeToWXString(maybeLogOver6X(getOptionalValue(mCtrlDiffAxialZ))));
                                                                                        
        mCtrlTCorrX     ->ChangeValue(maybeToWXString(maybeLogOver6X(getOptionalValue(mCtrlDiffX     ))));
        mCtrlTCorrY     ->ChangeValue(maybeToWXString(maybeLogOver6X(getOptionalValue(mCtrlDiffY     ))));
        mCtrlTCorrZ     ->ChangeValue(maybeToWXString(maybeLogOver6X(getOptionalValue(mCtrlDiffZ     ))));
    }
}

void EasySpinFrame::OnGenerateNotebook(wxChoicebookEvent& e) {
    wxCommandEvent commandEvent;
    OnGenerate(commandEvent);
}
void EasySpinFrame::OnGenerateList(wxListEvent& e) {
    wxCommandEvent commandEvent;
    OnGenerate(commandEvent);
}

void EasySpinFrame::OnTCorrIsoUnit(wxCommandEvent& e) {
    e.Skip();
    unsigned long selection = mCtrlTCorrIsoUnit->GetSelection();

    mCtrlTCorrAxUnit-> SetSelection(selection);
    mCtrlTCorrRhUnit-> SetSelection(selection);
    SetTCorrUnit(selection);
}

void EasySpinFrame::OnTCorrAxUnit(wxCommandEvent& e) {
    e.Skip();
    unsigned long selection = mCtrlTCorrAxUnit->GetSelection();

    mCtrlTCorrIsoUnit->SetSelection(selection);
    mCtrlTCorrRhUnit-> SetSelection(selection);
    SetTCorrUnit(selection);
}

void EasySpinFrame::OnTCorrRhUnit(wxCommandEvent& e) {
    e.Skip();
    unsigned long selection = mCtrlTCorrRhUnit->GetSelection();

    mCtrlTCorrIsoUnit->SetSelection(selection);
    mCtrlTCorrAxUnit-> SetSelection(selection);
    SetTCorrUnit(selection);
}
void EasySpinFrame::SetTCorrUnit(unsigned long selection) {
    //Actually, we can get the required behavior by calling the TCorr
    //onchange handler
    wxCommandEvent event;
    OnTCorr(event);
}

void EasySpinFrame::OnDiffIsoUnit(wxCommandEvent& e) {
    e.Skip();
    unsigned long selection = mCtrlDiffIsoUnit->GetSelection();

    mCtrlDiffAxUnit-> SetSelection(selection);
    mCtrlDiffRhUnit-> SetSelection(selection);
    SetDiffUnit(selection);
}

void EasySpinFrame::OnDiffAxUnit(wxCommandEvent& e) {
    e.Skip();
    unsigned long selection = mCtrlDiffAxUnit->GetSelection();

    mCtrlDiffAxUnit-> SetSelection(selection);
    mCtrlDiffRhUnit-> SetSelection(selection);
    SetDiffUnit(selection);
}

void EasySpinFrame::OnDiffRhUnit(wxCommandEvent& e) {
    e.Skip();
    unsigned long selection = mCtrlDiffRhUnit->GetSelection();

    mCtrlDiffIsoUnit->SetSelection(selection);
    mCtrlDiffAxUnit-> SetSelection(selection);
    SetDiffUnit(selection);
}
void EasySpinFrame::SetDiffUnit(unsigned long selection) {
    //Actually, we can get the required behavior by calling the TCorr
    //onchange handler
    wxCommandEvent event;
    OnDiff(event);
}

void EasySpinFrame::OnLambda(wxCommandEvent& e) {
    e.Skip();
    bool usingLambda = !(mCtrlLambda20->GetValue() == wxT("") &&
                         mCtrlLambda22->GetValue() == wxT("") &&
                         mCtrlLambda40->GetValue() == wxT("") &&
                         mCtrlLambda42->GetValue() == wxT("") &&
                         mCtrlLambda44->GetValue() == wxT(""));

    if(usingLambda) {
        mCtrlUseMOND->Enable(true);
        mCtrlUseMOND->SetLabel(wxT("Use MOND model"));
    } else {
        mCtrlUseMOND->Enable(false);
        mCtrlUseMOND->SetLabel(wxT("Enter an ordering potential to use the MOND model"));
    }
    mOptionsPage->Layout();
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

EVT_TEXT(ID_SPACEGROUP_TEXT, EasySpinFrame::OnSpaceGroupTxt)
EVT_BUTTON(ID_SPACEGROUP_BUTTON,EasySpinFrame::OnShowSpaceGroups)

EVT_LIST_ITEM_ACTIVATED(ID_ORIENT_LIST,EasySpinFrame::OnOrientDClick)
EVT_BUTTON(ID_EDIT_ORIENT,    EasySpinFrame::OnOrientEdit)
//Options
EVT_SPINCTRL(ID_KNOTS,      EasySpinFrame::OnKnotsChange)
EVT_CHECKBOX(ID_INTERPON,EasySpinFrame::OnInterpCheck)

//Broadernings
EVT_TEXT(ID_GAUSS_FWHM ,EasySpinFrame::OnGaussFWHM)
EVT_TEXT(ID_GAUSS_PP   ,EasySpinFrame::OnGaussPP)
EVT_TEXT(ID_LORENT_FWHM,EasySpinFrame::OnLorentFWHM)
EVT_TEXT(ID_LORENT_PP  ,EasySpinFrame::OnLorentPP)

EVT_TEXT(ID_CORR_COEFF, EasySpinFrame::OnCorrCoeff)

//Chili Dynamics
EVT_TEXT(ID_TCORR, EasySpinFrame::OnTCorr)
EVT_TEXT(ID_DIFF,  EasySpinFrame::OnDiff)
EVT_CHECKBOX(ID_LOG_TOGGLE,  EasySpinFrame::OnLog10Toggle)

EVT_CHOICE(ID_TCORR_ISO_UNIT,EasySpinFrame::OnTCorrIsoUnit)
EVT_CHOICE(ID_TCORR_AX_UNIT ,EasySpinFrame::OnTCorrAxUnit )
EVT_CHOICE(ID_TCORR_RH_UNIT ,EasySpinFrame::OnTCorrRhUnit )
EVT_CHOICE(ID_DIFF_ISO_UNIT ,EasySpinFrame::OnDiffIsoUnit )
EVT_CHOICE(ID_DIFF_AX_UNIT  ,EasySpinFrame::OnDiffAxUnit  )
EVT_CHOICE(ID_DIFF_RH_UNIT  ,EasySpinFrame::OnDiffRhUnit  )

EVT_TEXT(ID_LAMBDA,EasySpinFrame::OnLambda)

//Other
EVT_TEXT(ID_PREVIEW,     EasySpinFrame::PreviewEdit)
EVT_CHOICE(ID_EASYSPIN_F,EasySpinFrame::OnEasySpinFunc)
EVT_BUTTON(ID_COPY,      EasySpinFrame::OnCopy)
EVT_BUTTON(ID_EAYSPINDLG_SAVE,      EasySpinFrame::OnSave)
EVT_CHECKBOX(ID_AUTOGEN,EasySpinFrame::OnAutoGen)

//Catch everything and trigger a regeneration
EVT_TEXT(wxID_ANY, EasySpinFrame::OnGenerate)
EVT_BUTTON(wxID_ANY, EasySpinFrame::OnGenerate)
EVT_CHECKBOX(wxID_ANY, EasySpinFrame::OnGenerate)
EVT_CHOICE(wxID_ANY, EasySpinFrame::OnGenerate)
EVT_RADIOBUTTON(wxID_ANY, EasySpinFrame::OnGenerate)
EVT_CHOICEBOOK_PAGE_CHANGED(wxID_ANY,EasySpinFrame::OnGenerateNotebook)
EVT_LIST_ITEM_ACTIVATED(wxID_ANY,EasySpinFrame::OnGenerateList)

END_EVENT_TABLE()


struct __Init {
    __Init() {
        gModeSelectOrdering.push_back(EasySpinInput::PARALLEL);
        gModeSelectOrdering.push_back(EasySpinInput::PERPENDICULAR);

        gPepperMethodSelectOrdering.push_back(EasySpinInput::PERT1);
        gPepperMethodSelectOrdering.push_back(EasySpinInput::PERT2);
        gPepperMethodSelectOrdering.push_back(EasySpinInput::MATRIX);

        gGarlicMethodSelectOrdering.push_back(EasySpinInput::EXACT);
        gGarlicMethodSelectOrdering.push_back(EasySpinInput::PERT1);
        gGarlicMethodSelectOrdering.push_back(EasySpinInput::PERT2);
        gGarlicMethodSelectOrdering.push_back(EasySpinInput::PERT3);
        gGarlicMethodSelectOrdering.push_back(EasySpinInput::PERT4);
        gGarlicMethodSelectOrdering.push_back(EasySpinInput::PERT5);

        gOutputSelectOrdering.push_back(EasySpinInput::SUMMED);
        gOutputSelectOrdering.push_back(EasySpinInput::SEPERATE);

        gEasySpinSelectOrdering.push_back(EasySpinInput::GARLIC);
        gEasySpinSelectOrdering.push_back(EasySpinInput::CHILI);
        gEasySpinSelectOrdering.push_back(EasySpinInput::PEPPER);
    }
};
static __Init __init;
