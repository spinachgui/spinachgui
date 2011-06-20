///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov 18 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __easyspin__
#define __easyspin__

class TextCtrlFocus;

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/statbox.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/statline.h>
#include <wx/checkbox.h>
#include <wx/combobox.h>
#include <wx/radiobut.h>
#include <wx/listctrl.h>
#include <wx/choicebk.h>
#include <wx/spinctrl.h>
#include <wx/notebook.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

#define ID_EASYSPIN_F 1000
#define ID_GAUSS_FWHM 1001
#define ID_GAUSS_PP 1002
#define ID_LORENT_FWHM 1003
#define ID_LORENT_PP 1004
#define ID_CONV_UNIT 1005
#define ID_CORR_COEFF 1006
#define ID_MIN 1007
#define ID_CENTRE 1008
#define ID_MAX 1009
#define ID_SWEEP 1010
#define ID_RANGEUNIT 1011
#define ID_XBAND 1012
#define ID_QBAND 1013
#define ID_WBAND 1014
#define ID_TEMPON 1015
#define ID_TEMP 1016
#define ID_MODAMPON 1017
#define ID_MODAMP 1018
#define ID_MODAMPUNIT 1019
#define ID_POWDER 1020
#define ID_CRYSTAL 1021
#define ID_SPACEGROUP_TEXT 1022
#define ID_SPACEGROUP_BUTTON 1023
#define ID_ORIENT_LIST 1024
#define ID_ADD_ORIENT 1025
#define ID_EDIT_ORIENT 1026
#define ID_DELETE_ORIENT 1027
#define ID_DYNAMICS 1028
#define ID_TCORR 1029
#define ID_TCORR_ISO_UNIT 1030
#define ID_DIFF 1031
#define ID_DIFF_ISO_UNIT 1032
#define ID_TCORR_AX_UNIT 1033
#define ID_DIFF_AX_UNIT 1034
#define ID_TCORR_RH_UNIT 1035
#define ID_DIFF_RH_UNIT 1036
#define ID_LOG_TOGGLE 1037
#define ID_LAMBDA 1038
#define ID_KNOTS 1039
#define ID_ANGULARRES 1040
#define ID_INTERPON 1041
#define ID_INTERP 1042
#define ID_PREVIEW 1043
#define ID_COPY 1044
#define ID_EAYSPINDLG_SAVE 1045
#define ID_AUTOGEN 1046

///////////////////////////////////////////////////////////////////////////////
/// Class EasySpinFrameBase
///////////////////////////////////////////////////////////////////////////////
class EasySpinFrameBase : public wxFrame 
{
	private:
	
	protected:
		wxPanel* m_panel1;
		wxStaticText* m_staticText16;
		wxChoice* mCtrlEasySpinF;
		wxNotebook* mNotebook;
		wxPanel* mPageBroadings;
		wxStaticText* m_staticText19;
		wxStaticText* m_staticText20;
		wxTextCtrl* mCtrlGaussFWHM;
		wxStaticText* m_staticText211;
		wxTextCtrl* mCtrlGaussPP;
		wxStaticText* m_staticText232;
		wxStaticText* m_staticText241;
		wxTextCtrl* mCtrlLorentFWHM;
		wxStaticText* m_staticText25;
		wxTextCtrl* mCtrlLorentPP;
		wxStaticBitmap* m_bitmap11;
		wxChoice* mCtrlConvUnit;
		wxStaticBoxSizer* mSizerStrain;
		wxStaticText* m_staticText27;
		wxStaticText* m_staticText281;
		wxStaticText* m_staticText29;
		wxStaticText* m_staticText30;
		wxStaticText* m_staticText31;
		wxStaticText* m_staticText32;
		wxTextCtrl* mCtrlHX;
		wxTextCtrl* mCtrlHY;
		wxTextCtrl* mCtrlHZ;
		wxStaticText* m_staticText33;
		wxStaticText* m_staticText34;
		wxTextCtrl* mCtrlGX;
		wxTextCtrl* mCtrlGY;
		wxTextCtrl* mCtrlGZ;
		wxStaticText* m_staticText35;
		wxStaticText* m_staticText36;
		wxTextCtrl* mCtrlAX;
		wxTextCtrl* mCtrlAY;
		wxTextCtrl* mCtrlAZ;
		wxStaticText* m_staticText37;
		wxStaticText* m_staticText39;
		wxStaticText* m_staticText40;
		wxStaticText* m_staticText41;
		wxStaticText* m_staticText42;
		wxStaticText* m_staticText43;
		wxStaticText* m_staticText38;
		wxTextCtrl* mCtrlDD;
		wxTextCtrl* mCtrlDE;
		wxTextCtrl* mCtrlDCorrCoeff;
		wxPanel* mExpNotebookPage;
		wxStaticText* m_staticText1;
		wxTextCtrl* mCtrlMin;
		wxStaticText* m_staticText5;
		wxTextCtrl* mCtrlCentre;
		wxStaticText* m_staticText3;
		wxTextCtrl* mCtrlMax;
		wxStaticText* m_staticText7;
		wxTextCtrl* mCtrlSweep;
		wxStaticBitmap* m_bitmap1;
		wxChoice* mCtrlRangeUnit;
		wxStaticText* m_staticText10;
		wxTextCtrl* mCtrlMWFreq;
		wxStaticText* m_staticText231;
		wxButton* mCtrlXBand;
		wxButton* mCtrlQBand;
		wxButton* mCtrlWBand;
		wxStaticLine* m_staticline2;
		wxCheckBox* mCtrlTempOn;
		wxComboBox* mCtrlTemp;
		wxStaticText* m_staticText11;
		wxComboBox* mCtrlNPoints;
		wxStaticText* m_staticText12;
		wxChoice* mCtrlHarmonic;
		wxStaticText* m_staticText13;
		wxChoice* mCtrlMode;
		wxStaticText* m_staticText21;
		wxComboBox* mCtrlPhase;
		wxCheckBox* mCtrlModAmpOn;
		wxTextCtrl* mCtrlModAmp;
		wxChoice* mCtrlModAmpUnit;
		wxStaticLine* m_staticline4;
		wxRadioButton* mRadioPowder;
		wxRadioButton* mRadioCrystal;
		wxStaticText* mSpaceGroupText;
		TextCtrlFocus* mCtrlSpaceGroupTxt;
		wxButton* mCtrlSpaceGroupButton;
		wxPanel* mPanelCrystal;
		wxBoxSizer* mSizerOrient;
		wxListCtrl* mCtrlOrients;
		wxButton* mCtrlAdd;
		wxButton* mCtrlEditOrient;
		wxButton* mCtrlDeleteOrient;
		wxPanel* mDynamicsPage;
		wxStaticBoxSizer* mRotationCorrelationSizer;
		wxChoicebook* mCtrlRotationCorrType;
		wxPanel* m_panel8;
		wxStaticText* m_staticText57;
		wxTextCtrl* mCtrlTCorr;
		wxChoice* mCtrlTCorrIsoUnit;
		wxStaticText* m_staticText611;
		wxTextCtrl* mCtrlDiff;
		wxChoice* mCtrlDiffIsoUnit;
		wxPanel* m_panel9;
		wxStaticText* m_staticText77;
		wxStaticText* m_staticText78;
		wxStaticText* m_staticText79;
		wxStaticText* m_staticText80;
		wxStaticText* m_staticText59;
		wxTextCtrl* mCtrlTCorrXY;
		wxTextCtrl* mCtrlTCorrAxialZ;
		wxChoice* mCtrlTCorrAxUnit;
		wxStaticText* m_staticText621;
		wxTextCtrl* mCtrlDiffXY;
		wxTextCtrl* mCtrlDiffAxialZ;
		wxChoice* mCtrlDiffAxUnit;
		wxPanel* m_panel10;
		wxStaticText* m_staticText72;
		wxStaticText* m_staticText73;
		wxStaticText* m_staticText74;
		wxStaticText* m_staticText75;
		wxStaticText* m_staticText76;
		wxStaticText* m_staticText61;
		wxTextCtrl* mCtrlTCorrX;
		wxTextCtrl* mCtrlTCorrY;
		wxTextCtrl* mCtrlTCorrZ;
		wxChoice* mCtrlTCorrRhUnit;
		wxStaticText* m_staticText64;
		wxTextCtrl* mCtrlDiffX;
		wxTextCtrl* mCtrlDiffY;
		wxTextCtrl* mCtrlDiffZ;
		wxChoice* mCtrlDiffRhUnit;
		wxBoxSizer* mSizerRotCorrOrient;
		wxStaticText* m_staticText81;
		wxCheckBox* mDiffusionLog10;
		wxStaticText* m_staticText471;
		wxTextCtrl* mCtrlLambda20;
		wxStaticText* m_staticText52;
		wxStaticText* m_staticText48;
		wxTextCtrl* mCtrlLambda22;
		wxStaticText* m_staticText53;
		wxStaticText* m_staticText49;
		wxTextCtrl* mCtrlLambda40;
		wxStaticText* m_staticText54;
		wxStaticText* m_staticText50;
		wxTextCtrl* mCtrlLambda42;
		wxStaticText* m_staticText55;
		wxStaticText* m_staticText51;
		wxTextCtrl* mCtrlLambda44;
		wxStaticText* m_staticText56;
		wxStaticText* m_staticText70;
		wxTextCtrl* mCtrlExchange;
		wxStaticText* m_staticText71;
		wxPanel* mOptionsPage;
		wxFlexGridSizer* mSizerLLKM;
		wxStaticText* m_staticText44;
		wxSpinCtrl* mCtrlEvenLMax;
		wxStaticText* m_staticText45;
		wxSpinCtrl* mCtrlOddLMax;
		wxStaticText* m_staticText46;
		wxSpinCtrl* mCtrlKMax;
		wxStaticText* m_staticText47;
		wxSpinCtrl* mCtrlMMax;
		wxStaticLine* mLnTop;
		wxBoxSizer* mSizerPepperMethod;
		wxStaticText* m_staticText22;
		wxChoice* mCtrlPepperMethod;
		wxBoxSizer* mSizerGarlicMethod;
		wxStaticText* m_staticText222;
		wxChoice* mCtrlGarlicMethod;
		wxFlexGridSizer* mSizerKnots;
		wxStaticText* m_staticText23;
		wxSpinCtrl* mCtrlKnots;
		wxStaticText* m_staticText24;
		TextCtrlFocus* mCtrlAngularRes;
		wxCheckBox* mCtrlInterpOn;
		wxSpinCtrl* mCtrlInterp;
		wxStaticLine* mLnBottom;
		wxFlexGridSizer* mSizerOutput;
		wxStaticText* m_staticText28;
		wxChoice* mCtrlOutput;
		wxCheckBox* mCtrlUseMOND;
		wxPanel* m_panel2;
		wxTextCtrl* mCtrlPreview;
		wxButton* m_button2;
		wxButton* mCtrlSave;
		wxCheckBox* mCtrlAutoGen;
	
	public:
		
		EasySpinFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 870,550 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~EasySpinFrameBase();
	
};

#endif //__easyspin__
