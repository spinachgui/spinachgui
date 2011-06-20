///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov 18 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "gui/TextCtrlFocus.hpp"

#include "easyspin.h"

#include "res/right_squiggle.xpm"

///////////////////////////////////////////////////////////////////////////

EasySpinFrameBase::EasySpinFrameBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );
	
	m_panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText16 = new wxStaticText( m_panel1, wxID_ANY, wxT("EasySpin function:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	bSizer25->Add( m_staticText16, 0, wxALIGN_CENTER|wxALL, 5 );
	
	wxString mCtrlEasySpinFChoices[] = { wxT("garlic - Isotropic limit & fast-motion regime"), wxT("chili - Slow-motion regime"), wxT("pepper - Rigid limit") };
	int mCtrlEasySpinFNChoices = sizeof( mCtrlEasySpinFChoices ) / sizeof( wxString );
	mCtrlEasySpinF = new wxChoice( m_panel1, ID_EASYSPIN_F, wxDefaultPosition, wxDefaultSize, mCtrlEasySpinFNChoices, mCtrlEasySpinFChoices, 0 );
	mCtrlEasySpinF->SetSelection( 0 );
	bSizer25->Add( mCtrlEasySpinF, 1, wxALL, 2 );
	
	bSizer5->Add( bSizer25, 0, wxEXPAND, 5 );
	
	mNotebook = new wxNotebook( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	mPageBroadings = new wxPanel( mNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( mPageBroadings, wxID_ANY, wxT("convolutions") ), wxHORIZONTAL );
	
	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 2, 5, 0, 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText19 = new wxStaticText( mPageBroadings, wxID_ANY, wxT("Gaussian"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( -1 );
	fgSizer5->Add( m_staticText19, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT|wxALL, 5 );
	
	m_staticText20 = new wxStaticText( mPageBroadings, wxID_ANY, wxT("FWHM"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText20->Wrap( -1 );
	fgSizer5->Add( m_staticText20, 0, wxALIGN_CENTER|wxALL, 5 );
	
	mCtrlGaussFWHM = new wxTextCtrl( mPageBroadings, ID_GAUSS_FWHM, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( mCtrlGaussFWHM, 0, wxALL, 5 );
	
	m_staticText211 = new wxStaticText( mPageBroadings, wxID_ANY, wxT("pp"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText211->Wrap( -1 );
	fgSizer5->Add( m_staticText211, 0, wxALIGN_CENTER|wxALL, 5 );
	
	mCtrlGaussPP = new wxTextCtrl( mPageBroadings, ID_GAUSS_PP, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( mCtrlGaussPP, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticText232 = new wxStaticText( mPageBroadings, wxID_ANY, wxT("Lorentzian"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText232->Wrap( -1 );
	fgSizer5->Add( m_staticText232, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	m_staticText241 = new wxStaticText( mPageBroadings, wxID_ANY, wxT("FWHM"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText241->Wrap( -1 );
	fgSizer5->Add( m_staticText241, 0, wxALIGN_CENTER|wxALL, 5 );
	
	mCtrlLorentFWHM = new wxTextCtrl( mPageBroadings, ID_LORENT_FWHM, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( mCtrlLorentFWHM, 0, wxALL, 5 );
	
	m_staticText25 = new wxStaticText( mPageBroadings, wxID_ANY, wxT("pp"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText25->Wrap( -1 );
	fgSizer5->Add( m_staticText25, 0, wxALIGN_CENTER|wxALL, 5 );
	
	mCtrlLorentPP = new wxTextCtrl( mPageBroadings, ID_LORENT_PP, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( mCtrlLorentPP, 0, wxALL, 5 );
	
	sbSizer2->Add( fgSizer5, 0, wxALIGN_CENTER_VERTICAL|wxEXPAND, 0 );
	
	m_bitmap11 = new wxStaticBitmap( mPageBroadings, wxID_ANY, wxBitmap( right_squiggle_xpm ), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( m_bitmap11, 0, wxALL, 5 );
	
	wxString mCtrlConvUnitChoices[] = { wxT("mT"), wxT("G") };
	int mCtrlConvUnitNChoices = sizeof( mCtrlConvUnitChoices ) / sizeof( wxString );
	mCtrlConvUnit = new wxChoice( mPageBroadings, ID_CONV_UNIT, wxDefaultPosition, wxDefaultSize, mCtrlConvUnitNChoices, mCtrlConvUnitChoices, 0 );
	mCtrlConvUnit->SetSelection( 0 );
	sbSizer2->Add( mCtrlConvUnit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );
	
	bSizer20->Add( sbSizer2, 0, wxEXPAND, 5 );
	
	mSizerStrain = new wxStaticBoxSizer( new wxStaticBox( mPageBroadings, wxID_ANY, wxT("strains") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer61;
	fgSizer61 = new wxFlexGridSizer( 5, 5, 0, 0 );
	fgSizer61->SetFlexibleDirection( wxBOTH );
	fgSizer61->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText27 = new wxStaticText( mPageBroadings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText27->Wrap( -1 );
	fgSizer61->Add( m_staticText27, 0, wxALL, 5 );
	
	m_staticText281 = new wxStaticText( mPageBroadings, wxID_ANY, wxT("x"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText281->Wrap( -1 );
	fgSizer61->Add( m_staticText281, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText29 = new wxStaticText( mPageBroadings, wxID_ANY, wxT("y"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText29->Wrap( -1 );
	fgSizer61->Add( m_staticText29, 0, wxALL, 5 );
	
	m_staticText30 = new wxStaticText( mPageBroadings, wxID_ANY, wxT("z"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText30->Wrap( -1 );
	fgSizer61->Add( m_staticText30, 0, wxALL, 5 );
	
	m_staticText31 = new wxStaticText( mPageBroadings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText31->Wrap( -1 );
	fgSizer61->Add( m_staticText31, 0, wxALL, 5 );
	
	m_staticText32 = new wxStaticText( mPageBroadings, wxID_ANY, wxT("HStrain"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText32->Wrap( -1 );
	fgSizer61->Add( m_staticText32, 0, wxALIGN_CENTER|wxALL, 5 );
	
	mCtrlHX = new wxTextCtrl( mPageBroadings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer61->Add( mCtrlHX, 0, wxALL, 5 );
	
	mCtrlHY = new wxTextCtrl( mPageBroadings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer61->Add( mCtrlHY, 0, wxALL, 5 );
	
	mCtrlHZ = new wxTextCtrl( mPageBroadings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer61->Add( mCtrlHZ, 0, wxALL, 5 );
	
	m_staticText33 = new wxStaticText( mPageBroadings, wxID_ANY, wxT("MHz"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText33->Wrap( -1 );
	fgSizer61->Add( m_staticText33, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticText34 = new wxStaticText( mPageBroadings, wxID_ANY, wxT("gStrain"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText34->Wrap( -1 );
	fgSizer61->Add( m_staticText34, 0, wxALIGN_CENTER|wxALL, 5 );
	
	mCtrlGX = new wxTextCtrl( mPageBroadings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer61->Add( mCtrlGX, 0, wxALL, 5 );
	
	mCtrlGY = new wxTextCtrl( mPageBroadings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer61->Add( mCtrlGY, 0, wxALL, 5 );
	
	mCtrlGZ = new wxTextCtrl( mPageBroadings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer61->Add( mCtrlGZ, 0, wxALL, 5 );
	
	m_staticText35 = new wxStaticText( mPageBroadings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText35->Wrap( -1 );
	fgSizer61->Add( m_staticText35, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticText36 = new wxStaticText( mPageBroadings, wxID_ANY, wxT("AStrain"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText36->Wrap( -1 );
	fgSizer61->Add( m_staticText36, 0, wxALIGN_CENTER|wxALL, 5 );
	
	mCtrlAX = new wxTextCtrl( mPageBroadings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer61->Add( mCtrlAX, 0, wxALL, 5 );
	
	mCtrlAY = new wxTextCtrl( mPageBroadings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer61->Add( mCtrlAY, 0, wxALL, 5 );
	
	mCtrlAZ = new wxTextCtrl( mPageBroadings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer61->Add( mCtrlAZ, 0, wxALL, 5 );
	
	m_staticText37 = new wxStaticText( mPageBroadings, wxID_ANY, wxT("MHz"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText37->Wrap( -1 );
	fgSizer61->Add( m_staticText37, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticText39 = new wxStaticText( mPageBroadings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText39->Wrap( -1 );
	fgSizer61->Add( m_staticText39, 0, wxALL, 5 );
	
	m_staticText40 = new wxStaticText( mPageBroadings, wxID_ANY, wxT("D /MHz"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText40->Wrap( -1 );
	fgSizer61->Add( m_staticText40, 0, wxALL, 5 );
	
	m_staticText41 = new wxStaticText( mPageBroadings, wxID_ANY, wxT("E /MHz"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText41->Wrap( -1 );
	fgSizer61->Add( m_staticText41, 0, wxALL, 5 );
	
	m_staticText42 = new wxStaticText( mPageBroadings, wxID_ANY, wxT("corr. coeff."), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_staticText42->Wrap( -1 );
	fgSizer61->Add( m_staticText42, 0, wxALL, 5 );
	
	m_staticText43 = new wxStaticText( mPageBroadings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText43->Wrap( -1 );
	fgSizer61->Add( m_staticText43, 0, wxALL, 5 );
	
	m_staticText38 = new wxStaticText( mPageBroadings, wxID_ANY, wxT("DStrain"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText38->Wrap( -1 );
	fgSizer61->Add( m_staticText38, 0, wxALIGN_CENTER|wxALL, 5 );
	
	mCtrlDD = new wxTextCtrl( mPageBroadings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer61->Add( mCtrlDD, 0, wxALL, 5 );
	
	mCtrlDE = new wxTextCtrl( mPageBroadings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer61->Add( mCtrlDE, 0, wxALL, 5 );
	
	mCtrlDCorrCoeff = new wxTextCtrl( mPageBroadings, ID_CORR_COEFF, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer61->Add( mCtrlDCorrCoeff, 0, wxALL, 5 );
	
	mSizerStrain->Add( fgSizer61, 1, wxEXPAND, 5 );
	
	bSizer20->Add( mSizerStrain, 0, wxEXPAND, 5 );
	
	mPageBroadings->SetSizer( bSizer20 );
	mPageBroadings->Layout();
	bSizer20->Fit( mPageBroadings );
	mNotebook->AddPage( mPageBroadings, wxT("Broadenings"), false );
	mExpNotebookPage = new wxPanel( mNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer101;
	bSizer101 = new wxBoxSizer( wxHORIZONTAL );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 2, 4, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText1 = new wxStaticText( mExpNotebookPage, wxID_ANY, wxT("Min"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer1->Add( m_staticText1, 0, wxALIGN_CENTER|wxALL, 2 );
	
	mCtrlMin = new wxTextCtrl( mExpNotebookPage, ID_MIN, wxEmptyString, wxDefaultPosition, wxSize( 40,-1 ), 0 );
	fgSizer1->Add( mCtrlMin, 0, wxALIGN_CENTER|wxALL, 2 );
	
	m_staticText5 = new wxStaticText( mExpNotebookPage, wxID_ANY, wxT("Centre"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	fgSizer1->Add( m_staticText5, 0, wxALIGN_CENTER|wxALL, 5 );
	
	mCtrlCentre = new wxTextCtrl( mExpNotebookPage, ID_CENTRE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( mCtrlCentre, 0, wxALL, 5 );
	
	m_staticText3 = new wxStaticText( mExpNotebookPage, wxID_ANY, wxT("Max"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer1->Add( m_staticText3, 0, wxALIGN_CENTER|wxALL, 2 );
	
	mCtrlMax = new wxTextCtrl( mExpNotebookPage, ID_MAX, wxEmptyString, wxDefaultPosition, wxSize( 40,-1 ), 0 );
	fgSizer1->Add( mCtrlMax, 0, wxALL, 5 );
	
	m_staticText7 = new wxStaticText( mExpNotebookPage, wxID_ANY, wxT("Sweep"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizer1->Add( m_staticText7, 0, wxALIGN_CENTER|wxALL, 5 );
	
	mCtrlSweep = new wxTextCtrl( mExpNotebookPage, ID_SWEEP, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( mCtrlSweep, 0, wxALL, 5 );
	
	bSizer101->Add( fgSizer1, 0, wxALIGN_CENTER, 5 );
	
	m_bitmap1 = new wxStaticBitmap( mExpNotebookPage, wxID_ANY, wxBitmap( right_squiggle_xpm ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer101->Add( m_bitmap1, 0, wxALL, 5 );
	
	wxString mCtrlRangeUnitChoices[] = { wxT("mT"), wxT("G"), wxT("T"), wxT("kG") };
	int mCtrlRangeUnitNChoices = sizeof( mCtrlRangeUnitChoices ) / sizeof( wxString );
	mCtrlRangeUnit = new wxChoice( mExpNotebookPage, ID_RANGEUNIT, wxDefaultPosition, wxDefaultSize, mCtrlRangeUnitNChoices, mCtrlRangeUnitChoices, 0 );
	mCtrlRangeUnit->SetSelection( 0 );
	bSizer101->Add( mCtrlRangeUnit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	bSizer6->Add( bSizer101, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText10 = new wxStaticText( mExpNotebookPage, wxID_ANY, wxT("microwave"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	bSizer9->Add( m_staticText10, 0, wxALIGN_CENTER|wxALL, 5 );
	
	mCtrlMWFreq = new wxTextCtrl( mExpNotebookPage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( mCtrlMWFreq, 0, wxALL, 5 );
	
	m_staticText231 = new wxStaticText( mExpNotebookPage, wxID_ANY, wxT("GHz"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText231->Wrap( -1 );
	bSizer9->Add( m_staticText231, 0, wxALIGN_CENTER|wxALL, 5 );
	
	mCtrlXBand = new wxButton( mExpNotebookPage, ID_XBAND, wxT("X"), wxDefaultPosition, wxSize( 25,20 ), 0 );
	bSizer9->Add( mCtrlXBand, 0, wxALIGN_CENTER|wxALL, 5 );
	
	mCtrlQBand = new wxButton( mExpNotebookPage, ID_QBAND, wxT("Q"), wxDefaultPosition, wxSize( 25,20 ), 0 );
	bSizer9->Add( mCtrlQBand, 0, wxALIGN_CENTER|wxALL, 5 );
	
	mCtrlWBand = new wxButton( mExpNotebookPage, ID_WBAND, wxT("W"), wxDefaultPosition, wxSize( 25,20 ), 0 );
	bSizer9->Add( mCtrlWBand, 0, wxALIGN_CENTER|wxALL, 5 );
	
	bSizer6->Add( bSizer9, 0, wxEXPAND, 5 );
	
	m_staticline2 = new wxStaticLine( mExpNotebookPage, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer6->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 2, 4, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	mCtrlTempOn = new wxCheckBox( mExpNotebookPage, ID_TEMPON, wxT("Temperature"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( mCtrlTempOn, 0, wxALIGN_CENTER|wxALL, 5 );
	
	mCtrlTemp = new wxComboBox( mExpNotebookPage, ID_TEMP, wxT("77"), wxDefaultPosition, wxSize( 60,-1 ), 0, NULL, 0 ); 
	mCtrlTemp->Enable( false );
	
	fgSizer2->Add( mCtrlTemp, 0, wxALL, 5 );
	
	m_staticText11 = new wxStaticText( mExpNotebookPage, wxID_ANY, wxT("# Points"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	fgSizer2->Add( m_staticText11, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	mCtrlNPoints = new wxComboBox( mExpNotebookPage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 80,-1 ), 0, NULL, 0 );
	mCtrlNPoints->Append( wxT("1024") );
	mCtrlNPoints->Append( wxT("2048") );
	mCtrlNPoints->Append( wxT("8192") );
	fgSizer2->Add( mCtrlNPoints, 0, wxALL, 5 );
	
	m_staticText12 = new wxStaticText( mExpNotebookPage, wxID_ANY, wxT("Harmonic"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	fgSizer2->Add( m_staticText12, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString mCtrlHarmonicChoices[] = { wxT("0"), wxT("1"), wxT("2") };
	int mCtrlHarmonicNChoices = sizeof( mCtrlHarmonicChoices ) / sizeof( wxString );
	mCtrlHarmonic = new wxChoice( mExpNotebookPage, wxID_ANY, wxDefaultPosition, wxDefaultSize, mCtrlHarmonicNChoices, mCtrlHarmonicChoices, 0 );
	mCtrlHarmonic->SetSelection( 1 );
	fgSizer2->Add( mCtrlHarmonic, 0, wxALL, 5 );
	
	m_staticText13 = new wxStaticText( mExpNotebookPage, wxID_ANY, wxT("Mode"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	fgSizer2->Add( m_staticText13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString mCtrlModeChoices[] = { wxT("parallel"), wxT("perpendicular") };
	int mCtrlModeNChoices = sizeof( mCtrlModeChoices ) / sizeof( wxString );
	mCtrlMode = new wxChoice( mExpNotebookPage, wxID_ANY, wxDefaultPosition, wxDefaultSize, mCtrlModeNChoices, mCtrlModeChoices, 0 );
	mCtrlMode->SetSelection( 0 );
	fgSizer2->Add( mCtrlMode, 0, wxALL, 5 );
	
	m_staticText21 = new wxStaticText( mExpNotebookPage, wxID_ANY, wxT("Phase"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	fgSizer2->Add( m_staticText21, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	mCtrlPhase = new wxComboBox( mExpNotebookPage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0, NULL, 0 );
	mCtrlPhase->Append( wxT("0") );
	mCtrlPhase->Append( wxT("90") );
	fgSizer2->Add( mCtrlPhase, 0, wxALL, 5 );
	
	mCtrlModAmpOn = new wxCheckBox( mExpNotebookPage, ID_MODAMPON, wxT("Mod. amp."), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( mCtrlModAmpOn, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxHORIZONTAL );
	
	mCtrlModAmp = new wxTextCtrl( mExpNotebookPage, ID_MODAMP, wxT("0.1"), wxDefaultPosition, wxSize( 40,-1 ), 0 );
	mCtrlModAmp->Enable( false );
	
	bSizer12->Add( mCtrlModAmp, 0, wxALIGN_CENTER|wxALL, 5 );
	
	wxString mCtrlModAmpUnitChoices[] = { wxT("mT"), wxT("G"), wxT("T"), wxT("kG") };
	int mCtrlModAmpUnitNChoices = sizeof( mCtrlModAmpUnitChoices ) / sizeof( wxString );
	mCtrlModAmpUnit = new wxChoice( mExpNotebookPage, ID_MODAMPUNIT, wxDefaultPosition, wxDefaultSize, mCtrlModAmpUnitNChoices, mCtrlModAmpUnitChoices, 0 );
	mCtrlModAmpUnit->SetSelection( 0 );
	mCtrlModAmpUnit->Enable( false );
	
	bSizer12->Add( mCtrlModAmpUnit, 0, wxALIGN_CENTER|wxALL, 5 );
	
	fgSizer2->Add( bSizer12, 1, wxEXPAND, 5 );
	
	bSizer6->Add( fgSizer2, 0, wxEXPAND, 5 );
	
	m_staticline4 = new wxStaticLine( mExpNotebookPage, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer6->Add( m_staticline4, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );
	
	mRadioPowder = new wxRadioButton( mExpNotebookPage, ID_POWDER, wxT("Powder"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer11->Add( mRadioPowder, 0, wxALIGN_CENTER|wxALL, 5 );
	
	mRadioCrystal = new wxRadioButton( mExpNotebookPage, ID_CRYSTAL, wxT("Crystal"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer11->Add( mRadioCrystal, 0, wxALIGN_CENTER|wxALL, 0 );
	
	mSpaceGroupText = new wxStaticText( mExpNotebookPage, wxID_ANY, wxT("Space group"), wxDefaultPosition, wxDefaultSize, 0 );
	mSpaceGroupText->Wrap( -1 );
	bSizer11->Add( mSpaceGroupText, 0, wxALIGN_CENTER|wxALL, 5 );
	
	mCtrlSpaceGroupTxt = new TextCtrlFocus( mExpNotebookPage, ID_SPACEGROUP_TEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer11->Add( mCtrlSpaceGroupTxt, 1, wxALIGN_CENTER|wxBOTTOM|wxLEFT|wxTOP, 5 );
	
	mCtrlSpaceGroupButton = new wxButton( mExpNotebookPage, ID_SPACEGROUP_BUTTON, wxT("..."), wxDefaultPosition, wxSize( 40,20 ), 0 );
	bSizer11->Add( mCtrlSpaceGroupButton, 0, wxALIGN_CENTER|wxBOTTOM|wxRIGHT|wxTOP, 5 );
	
	bSizer7->Add( bSizer11, 0, wxEXPAND, 5 );
	
	mPanelCrystal = new wxPanel( mExpNotebookPage, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	mSizerOrient = new wxBoxSizer( wxVERTICAL );
	
	mCtrlOrients = new wxListCtrl( mPanelCrystal, ID_ORIENT_LIST, wxDefaultPosition, wxDefaultSize, wxLC_ICON );
	mSizerOrient->Add( mCtrlOrients, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxHORIZONTAL );
	
	mCtrlAdd = new wxButton( mPanelCrystal, ID_ADD_ORIENT, wxT("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	mCtrlAdd->SetMinSize( wxSize( 50,20 ) );
	
	bSizer15->Add( mCtrlAdd, 0, wxALL, 5 );
	
	mCtrlEditOrient = new wxButton( mPanelCrystal, ID_EDIT_ORIENT, wxT("Edit"), wxDefaultPosition, wxDefaultSize, 0 );
	mCtrlEditOrient->SetMinSize( wxSize( 50,20 ) );
	
	bSizer15->Add( mCtrlEditOrient, 0, wxALL, 5 );
	
	mCtrlDeleteOrient = new wxButton( mPanelCrystal, ID_DELETE_ORIENT, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	mCtrlDeleteOrient->SetMinSize( wxSize( 50,20 ) );
	
	bSizer15->Add( mCtrlDeleteOrient, 0, wxALL, 5 );
	
	mSizerOrient->Add( bSizer15, 0, wxEXPAND, 5 );
	
	mPanelCrystal->SetSizer( mSizerOrient );
	mPanelCrystal->Layout();
	mSizerOrient->Fit( mPanelCrystal );
	bSizer7->Add( mPanelCrystal, 1, wxEXPAND | wxALL, 0 );
	
	bSizer6->Add( bSizer7, 1, wxEXPAND, 5 );
	
	mExpNotebookPage->SetSizer( bSizer6 );
	mExpNotebookPage->Layout();
	bSizer6->Fit( mExpNotebookPage );
	mNotebook->AddPage( mExpNotebookPage, wxT("Experiment"), false );
	mDynamicsPage = new wxPanel( mNotebook, ID_DYNAMICS, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	mRotationCorrelationSizer = new wxStaticBoxSizer( new wxStaticBox( mDynamicsPage, wxID_ANY, wxT("Rotational correlation") ), wxVERTICAL );
	
	mCtrlRotationCorrType = new wxChoicebook( mDynamicsPage, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCHB_DEFAULT );
	m_panel8 = new wxPanel( mCtrlRotationCorrType, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer9;
	fgSizer9 = new wxFlexGridSizer( 2, 3, 0, 0 );
	fgSizer9->SetFlexibleDirection( wxBOTH );
	fgSizer9->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText57 = new wxStaticText( m_panel8, wxID_ANY, wxT("Correlation time"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText57->Wrap( -1 );
	fgSizer9->Add( m_staticText57, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	mCtrlTCorr = new wxTextCtrl( m_panel8, ID_TCORR, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer9->Add( mCtrlTCorr, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );
	
	wxString mCtrlTCorrIsoUnitChoices[] = { wxT("ns"), wxT("us") };
	int mCtrlTCorrIsoUnitNChoices = sizeof( mCtrlTCorrIsoUnitChoices ) / sizeof( wxString );
	mCtrlTCorrIsoUnit = new wxChoice( m_panel8, ID_TCORR_ISO_UNIT, wxDefaultPosition, wxDefaultSize, mCtrlTCorrIsoUnitNChoices, mCtrlTCorrIsoUnitChoices, 0 );
	mCtrlTCorrIsoUnit->SetSelection( 0 );
	fgSizer9->Add( mCtrlTCorrIsoUnit, 0, wxALL, 0 );
	
	m_staticText611 = new wxStaticText( m_panel8, wxID_ANY, wxT("Diffusion tensor"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText611->Wrap( -1 );
	fgSizer9->Add( m_staticText611, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	mCtrlDiff = new wxTextCtrl( m_panel8, ID_DIFF, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer9->Add( mCtrlDiff, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );
	
	wxString mCtrlDiffIsoUnitChoices[] = { wxT("GHz"), wxT("MHz") };
	int mCtrlDiffIsoUnitNChoices = sizeof( mCtrlDiffIsoUnitChoices ) / sizeof( wxString );
	mCtrlDiffIsoUnit = new wxChoice( m_panel8, ID_DIFF_ISO_UNIT, wxDefaultPosition, wxDefaultSize, mCtrlDiffIsoUnitNChoices, mCtrlDiffIsoUnitChoices, 0 );
	mCtrlDiffIsoUnit->SetSelection( 1 );
	fgSizer9->Add( mCtrlDiffIsoUnit, 0, wxALL, 0 );
	
	m_panel8->SetSizer( fgSizer9 );
	m_panel8->Layout();
	fgSizer9->Fit( m_panel8 );
	mCtrlRotationCorrType->AddPage( m_panel8, wxT("isotropic diffusion"), false );
	m_panel9 = new wxPanel( mCtrlRotationCorrType, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer10;
	fgSizer10 = new wxFlexGridSizer( 2, 4, 0, 0 );
	fgSizer10->SetFlexibleDirection( wxBOTH );
	fgSizer10->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText77 = new wxStaticText( m_panel9, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText77->Wrap( -1 );
	fgSizer10->Add( m_staticText77, 0, wxALL, 5 );
	
	m_staticText78 = new wxStaticText( m_panel9, wxID_ANY, wxT("xy"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText78->Wrap( -1 );
	fgSizer10->Add( m_staticText78, 0, wxALL, 5 );
	
	m_staticText79 = new wxStaticText( m_panel9, wxID_ANY, wxT("z"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText79->Wrap( -1 );
	fgSizer10->Add( m_staticText79, 0, wxALL, 5 );
	
	m_staticText80 = new wxStaticText( m_panel9, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText80->Wrap( -1 );
	fgSizer10->Add( m_staticText80, 0, wxALL, 5 );
	
	m_staticText59 = new wxStaticText( m_panel9, wxID_ANY, wxT("Correlation time"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText59->Wrap( -1 );
	fgSizer10->Add( m_staticText59, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	mCtrlTCorrXY = new wxTextCtrl( m_panel9, ID_TCORR, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer10->Add( mCtrlTCorrXY, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );
	
	mCtrlTCorrAxialZ = new wxTextCtrl( m_panel9, ID_TCORR, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer10->Add( mCtrlTCorrAxialZ, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );
	
	wxString mCtrlTCorrAxUnitChoices[] = { wxT("ns"), wxT("us") };
	int mCtrlTCorrAxUnitNChoices = sizeof( mCtrlTCorrAxUnitChoices ) / sizeof( wxString );
	mCtrlTCorrAxUnit = new wxChoice( m_panel9, ID_TCORR_AX_UNIT, wxDefaultPosition, wxDefaultSize, mCtrlTCorrAxUnitNChoices, mCtrlTCorrAxUnitChoices, 0 );
	mCtrlTCorrAxUnit->SetSelection( 0 );
	fgSizer10->Add( mCtrlTCorrAxUnit, 0, wxALL, 0 );
	
	m_staticText621 = new wxStaticText( m_panel9, wxID_ANY, wxT("Diffusion tensor"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText621->Wrap( -1 );
	fgSizer10->Add( m_staticText621, 0, wxALL, 5 );
	
	mCtrlDiffXY = new wxTextCtrl( m_panel9, ID_DIFF, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer10->Add( mCtrlDiffXY, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );
	
	mCtrlDiffAxialZ = new wxTextCtrl( m_panel9, ID_DIFF, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer10->Add( mCtrlDiffAxialZ, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );
	
	wxString mCtrlDiffAxUnitChoices[] = { wxT("GHz"), wxT("MHz") };
	int mCtrlDiffAxUnitNChoices = sizeof( mCtrlDiffAxUnitChoices ) / sizeof( wxString );
	mCtrlDiffAxUnit = new wxChoice( m_panel9, ID_DIFF_AX_UNIT, wxDefaultPosition, wxDefaultSize, mCtrlDiffAxUnitNChoices, mCtrlDiffAxUnitChoices, 0 );
	mCtrlDiffAxUnit->SetSelection( 0 );
	fgSizer10->Add( mCtrlDiffAxUnit, 0, wxALL, 0 );
	
	m_panel9->SetSizer( fgSizer10 );
	m_panel9->Layout();
	fgSizer10->Fit( m_panel9 );
	mCtrlRotationCorrType->AddPage( m_panel9, wxT("axial anisotropic diffusion"), false );
	m_panel10 = new wxPanel( mCtrlRotationCorrType, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer12;
	fgSizer12 = new wxFlexGridSizer( 2, 5, 0, 0 );
	fgSizer12->SetFlexibleDirection( wxBOTH );
	fgSizer12->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText72 = new wxStaticText( m_panel10, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText72->Wrap( -1 );
	fgSizer12->Add( m_staticText72, 0, wxALL, 5 );
	
	m_staticText73 = new wxStaticText( m_panel10, wxID_ANY, wxT("x"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText73->Wrap( -1 );
	fgSizer12->Add( m_staticText73, 0, wxALL, 5 );
	
	m_staticText74 = new wxStaticText( m_panel10, wxID_ANY, wxT("y"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText74->Wrap( -1 );
	fgSizer12->Add( m_staticText74, 0, wxALL, 5 );
	
	m_staticText75 = new wxStaticText( m_panel10, wxID_ANY, wxT("z"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText75->Wrap( -1 );
	fgSizer12->Add( m_staticText75, 0, wxALL, 5 );
	
	m_staticText76 = new wxStaticText( m_panel10, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText76->Wrap( -1 );
	fgSizer12->Add( m_staticText76, 0, wxALL, 5 );
	
	m_staticText61 = new wxStaticText( m_panel10, wxID_ANY, wxT("Correlation time"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText61->Wrap( -1 );
	fgSizer12->Add( m_staticText61, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	mCtrlTCorrX = new wxTextCtrl( m_panel10, ID_TCORR, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	fgSizer12->Add( mCtrlTCorrX, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );
	
	mCtrlTCorrY = new wxTextCtrl( m_panel10, ID_TCORR, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	fgSizer12->Add( mCtrlTCorrY, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );
	
	mCtrlTCorrZ = new wxTextCtrl( m_panel10, ID_TCORR, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	fgSizer12->Add( mCtrlTCorrZ, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );
	
	wxString mCtrlTCorrRhUnitChoices[] = { wxT("ns"), wxT("us") };
	int mCtrlTCorrRhUnitNChoices = sizeof( mCtrlTCorrRhUnitChoices ) / sizeof( wxString );
	mCtrlTCorrRhUnit = new wxChoice( m_panel10, ID_TCORR_RH_UNIT, wxDefaultPosition, wxDefaultSize, mCtrlTCorrRhUnitNChoices, mCtrlTCorrRhUnitChoices, 0 );
	mCtrlTCorrRhUnit->SetSelection( 0 );
	fgSizer12->Add( mCtrlTCorrRhUnit, 0, wxALL, 0 );
	
	m_staticText64 = new wxStaticText( m_panel10, wxID_ANY, wxT("Diffusion tensor"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText64->Wrap( -1 );
	fgSizer12->Add( m_staticText64, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	mCtrlDiffX = new wxTextCtrl( m_panel10, ID_DIFF, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	fgSizer12->Add( mCtrlDiffX, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );
	
	mCtrlDiffY = new wxTextCtrl( m_panel10, ID_DIFF, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	fgSizer12->Add( mCtrlDiffY, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );
	
	mCtrlDiffZ = new wxTextCtrl( m_panel10, ID_DIFF, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	fgSizer12->Add( mCtrlDiffZ, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );
	
	wxString mCtrlDiffRhUnitChoices[] = { wxT("GHz"), wxT("MHz") };
	int mCtrlDiffRhUnitNChoices = sizeof( mCtrlDiffRhUnitChoices ) / sizeof( wxString );
	mCtrlDiffRhUnit = new wxChoice( m_panel10, ID_DIFF_RH_UNIT, wxDefaultPosition, wxDefaultSize, mCtrlDiffRhUnitNChoices, mCtrlDiffRhUnitChoices, 0 );
	mCtrlDiffRhUnit->SetSelection( 0 );
	fgSizer12->Add( mCtrlDiffRhUnit, 0, wxALL, 0 );
	
	m_panel10->SetSizer( fgSizer12 );
	m_panel10->Layout();
	fgSizer12->Fit( m_panel10 );
	mCtrlRotationCorrType->AddPage( m_panel10, wxT("rhombic anisotropic diffusion"), true );
	mRotationCorrelationSizer->Add( mCtrlRotationCorrType, 1, wxEXPAND | wxALL, 5 );
	
	mSizerRotCorrOrient = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText81 = new wxStaticText( mDynamicsPage, wxID_ANY, wxT("Tensor orientation"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText81->Wrap( -1 );
	mSizerRotCorrOrient->Add( m_staticText81, 0, wxALL, 5 );
	
	mRotationCorrelationSizer->Add( mSizerRotCorrOrient, 0, wxEXPAND, 5 );
	
	mDiffusionLog10 = new wxCheckBox( mDynamicsPage, ID_LOG_TOGGLE, wxT("Specify log10 of the principle components "), wxDefaultPosition, wxDefaultSize, 0 );
	mRotationCorrelationSizer->Add( mDiffusionLog10, 0, wxALL, 5 );
	
	bSizer18->Add( mRotationCorrelationSizer, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( mDynamicsPage, wxID_ANY, wxT("Ordering potential") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer8;
	fgSizer8 = new wxFlexGridSizer( 2, 6, 0, 0 );
	fgSizer8->SetFlexibleDirection( wxBOTH );
	fgSizer8->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText471 = new wxStaticText( mDynamicsPage, wxID_ANY, wxT("λ20"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText471->Wrap( -1 );
	fgSizer8->Add( m_staticText471, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	mCtrlLambda20 = new wxTextCtrl( mDynamicsPage, ID_LAMBDA, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer8->Add( mCtrlLambda20, 0, wxALL, 5 );
	
	m_staticText52 = new wxStaticText( mDynamicsPage, wxID_ANY, wxT("MHz"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText52->Wrap( -1 );
	fgSizer8->Add( m_staticText52, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText48 = new wxStaticText( mDynamicsPage, wxID_ANY, wxT("λ22"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText48->Wrap( -1 );
	fgSizer8->Add( m_staticText48, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	mCtrlLambda22 = new wxTextCtrl( mDynamicsPage, ID_LAMBDA, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer8->Add( mCtrlLambda22, 0, wxALL, 5 );
	
	m_staticText53 = new wxStaticText( mDynamicsPage, wxID_ANY, wxT("MHz"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText53->Wrap( -1 );
	fgSizer8->Add( m_staticText53, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText49 = new wxStaticText( mDynamicsPage, wxID_ANY, wxT("λ40"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText49->Wrap( -1 );
	fgSizer8->Add( m_staticText49, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	mCtrlLambda40 = new wxTextCtrl( mDynamicsPage, ID_LAMBDA, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer8->Add( mCtrlLambda40, 0, wxALL, 5 );
	
	m_staticText54 = new wxStaticText( mDynamicsPage, wxID_ANY, wxT("MHz"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText54->Wrap( -1 );
	fgSizer8->Add( m_staticText54, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText50 = new wxStaticText( mDynamicsPage, wxID_ANY, wxT("λ42"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText50->Wrap( -1 );
	fgSizer8->Add( m_staticText50, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	mCtrlLambda42 = new wxTextCtrl( mDynamicsPage, ID_LAMBDA, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer8->Add( mCtrlLambda42, 0, wxALL, 5 );
	
	m_staticText55 = new wxStaticText( mDynamicsPage, wxID_ANY, wxT("MHz"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText55->Wrap( -1 );
	fgSizer8->Add( m_staticText55, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText51 = new wxStaticText( mDynamicsPage, wxID_ANY, wxT("λ44"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText51->Wrap( -1 );
	fgSizer8->Add( m_staticText51, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	mCtrlLambda44 = new wxTextCtrl( mDynamicsPage, ID_LAMBDA, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer8->Add( mCtrlLambda44, 0, wxALL, 5 );
	
	m_staticText56 = new wxStaticText( mDynamicsPage, wxID_ANY, wxT("MHz"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText56->Wrap( -1 );
	fgSizer8->Add( m_staticText56, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	sbSizer3->Add( fgSizer8, 1, wxEXPAND, 5 );
	
	bSizer18->Add( sbSizer3, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer201;
	bSizer201 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText70 = new wxStaticText( mDynamicsPage, wxID_ANY, wxT("Heisnberg Exchange"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText70->Wrap( -1 );
	bSizer201->Add( m_staticText70, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	mCtrlExchange = new wxTextCtrl( mDynamicsPage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer201->Add( mCtrlExchange, 0, wxALL, 5 );
	
	m_staticText71 = new wxStaticText( mDynamicsPage, wxID_ANY, wxT("MHz"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText71->Wrap( -1 );
	bSizer201->Add( m_staticText71, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	bSizer18->Add( bSizer201, 0, wxEXPAND, 5 );
	
	mDynamicsPage->SetSizer( bSizer18 );
	mDynamicsPage->Layout();
	bSizer18->Fit( mDynamicsPage );
	mNotebook->AddPage( mDynamicsPage, wxT("Dynamics"), false );
	mOptionsPage = new wxPanel( mNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );
	
	mSizerLLKM = new wxFlexGridSizer( 2, 4, 0, 0 );
	mSizerLLKM->SetFlexibleDirection( wxBOTH );
	mSizerLLKM->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText44 = new wxStaticText( mOptionsPage, wxID_ANY, wxT("Even L Max"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText44->Wrap( -1 );
	mSizerLLKM->Add( m_staticText44, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	mCtrlEvenLMax = new wxSpinCtrl( mOptionsPage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	mSizerLLKM->Add( mCtrlEvenLMax, 0, wxALL, 5 );
	
	m_staticText45 = new wxStaticText( mOptionsPage, wxID_ANY, wxT("Odd L max"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText45->Wrap( -1 );
	mSizerLLKM->Add( m_staticText45, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxRIGHT, 5 );
	
	mCtrlOddLMax = new wxSpinCtrl( mOptionsPage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	mSizerLLKM->Add( mCtrlOddLMax, 0, wxALL, 5 );
	
	m_staticText46 = new wxStaticText( mOptionsPage, wxID_ANY, wxT("K max"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText46->Wrap( -1 );
	mSizerLLKM->Add( m_staticText46, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	mCtrlKMax = new wxSpinCtrl( mOptionsPage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	mSizerLLKM->Add( mCtrlKMax, 0, wxALL, 5 );
	
	m_staticText47 = new wxStaticText( mOptionsPage, wxID_ANY, wxT("M max"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText47->Wrap( -1 );
	mSizerLLKM->Add( m_staticText47, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	mCtrlMMax = new wxSpinCtrl( mOptionsPage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	mSizerLLKM->Add( mCtrlMMax, 0, wxALL, 5 );
	
	bSizer10->Add( mSizerLLKM, 0, wxEXPAND, 5 );
	
	mLnTop = new wxStaticLine( mOptionsPage, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer10->Add( mLnTop, 0, wxEXPAND | wxALL, 5 );
	
	mSizerPepperMethod = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText22 = new wxStaticText( mOptionsPage, wxID_ANY, wxT("Method"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText22->Wrap( -1 );
	mSizerPepperMethod->Add( m_staticText22, 0, wxALIGN_CENTER|wxALL, 5 );
	
	wxString mCtrlPepperMethodChoices[] = { wxT("perturbation 1"), wxT("perturbation 2"), wxT("matrix") };
	int mCtrlPepperMethodNChoices = sizeof( mCtrlPepperMethodChoices ) / sizeof( wxString );
	mCtrlPepperMethod = new wxChoice( mOptionsPage, wxID_ANY, wxDefaultPosition, wxDefaultSize, mCtrlPepperMethodNChoices, mCtrlPepperMethodChoices, 0 );
	mCtrlPepperMethod->SetSelection( 0 );
	mSizerPepperMethod->Add( mCtrlPepperMethod, 0, wxALL, 5 );
	
	bSizer10->Add( mSizerPepperMethod, 0, wxEXPAND, 5 );
	
	mSizerGarlicMethod = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText222 = new wxStaticText( mOptionsPage, wxID_ANY, wxT("Method"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText222->Wrap( -1 );
	mSizerGarlicMethod->Add( m_staticText222, 0, wxALIGN_CENTER|wxALL, 5 );
	
	wxString mCtrlGarlicMethodChoices[] = { wxT("exact"), wxT("perturbation 1"), wxT("perturbation 2"), wxT("perturbation 3"), wxT("perturbation 4"), wxT("perturbation 5") };
	int mCtrlGarlicMethodNChoices = sizeof( mCtrlGarlicMethodChoices ) / sizeof( wxString );
	mCtrlGarlicMethod = new wxChoice( mOptionsPage, wxID_ANY, wxDefaultPosition, wxDefaultSize, mCtrlGarlicMethodNChoices, mCtrlGarlicMethodChoices, 0 );
	mCtrlGarlicMethod->SetSelection( 0 );
	mSizerGarlicMethod->Add( mCtrlGarlicMethod, 0, wxALL, 5 );
	
	bSizer10->Add( mSizerGarlicMethod, 0, wxEXPAND, 5 );
	
	mSizerKnots = new wxFlexGridSizer( 2, 4, 0, 0 );
	mSizerKnots->SetFlexibleDirection( wxBOTH );
	mSizerKnots->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText23 = new wxStaticText( mOptionsPage, wxID_ANY, wxT("Knots"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText23->Wrap( -1 );
	mSizerKnots->Add( m_staticText23, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	mCtrlKnots = new wxSpinCtrl( mOptionsPage, ID_KNOTS, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxSP_ARROW_KEYS, 0, 10, 2 );
	mSizerKnots->Add( mCtrlKnots, 0, wxALL, 5 );
	
	m_staticText24 = new wxStaticText( mOptionsPage, wxID_ANY, wxT("Angular resolution"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText24->Wrap( -1 );
	mSizerKnots->Add( m_staticText24, 0, wxALIGN_CENTER|wxALL, 5 );
	
	mCtrlAngularRes = new TextCtrlFocus( mOptionsPage, ID_ANGULARRES, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	mSizerKnots->Add( mCtrlAngularRes, 0, wxALL, 5 );
	
	mCtrlInterpOn = new wxCheckBox( mOptionsPage, ID_INTERPON, wxT("Interpolate"), wxDefaultPosition, wxDefaultSize, 0 );
	mCtrlInterpOn->SetValue(true); 
	mSizerKnots->Add( mCtrlInterpOn, 0, wxALL, 5 );
	
	mCtrlInterp = new wxSpinCtrl( mOptionsPage, ID_INTERP, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxSP_ARROW_KEYS, 2, 10, 0 );
	mSizerKnots->Add( mCtrlInterp, 0, wxALL, 5 );
	
	bSizer10->Add( mSizerKnots, 0, wxEXPAND, 5 );
	
	mLnBottom = new wxStaticLine( mOptionsPage, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer10->Add( mLnBottom, 0, wxEXPAND | wxALL, 5 );
	
	mSizerOutput = new wxFlexGridSizer( 2, 2, 0, 0 );
	mSizerOutput->SetFlexibleDirection( wxBOTH );
	mSizerOutput->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText28 = new wxStaticText( mOptionsPage, wxID_ANY, wxT("Output"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText28->Wrap( -1 );
	mSizerOutput->Add( m_staticText28, 0, wxALIGN_CENTER|wxALL, 5 );
	
	wxString mCtrlOutputChoices[] = { wxT("summed"), wxT("separate") };
	int mCtrlOutputNChoices = sizeof( mCtrlOutputChoices ) / sizeof( wxString );
	mCtrlOutput = new wxChoice( mOptionsPage, wxID_ANY, wxDefaultPosition, wxDefaultSize, mCtrlOutputNChoices, mCtrlOutputChoices, 0 );
	mCtrlOutput->SetSelection( 0 );
	mSizerOutput->Add( mCtrlOutput, 0, wxALL, 5 );
	
	bSizer10->Add( mSizerOutput, 0, wxEXPAND, 5 );
	
	mCtrlUseMOND = new wxCheckBox( mOptionsPage, wxID_ANY, wxT("Use MOND model "), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer10->Add( mCtrlUseMOND, 0, wxALL, 5 );
	
	mOptionsPage->SetSizer( bSizer10 );
	mOptionsPage->Layout();
	bSizer10->Fit( mOptionsPage );
	mNotebook->AddPage( mOptionsPage, wxT("Options"), true );
	
	bSizer5->Add( mNotebook, 1, wxEXPAND | wxALL, 5 );
	
	m_panel1->SetSizer( bSizer5 );
	m_panel1->Layout();
	bSizer5->Fit( m_panel1 );
	bSizer1->Add( m_panel1, 1, wxEXPAND | wxALL, 0 );
	
	m_panel2 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	mCtrlPreview = new wxTextCtrl( m_panel2, ID_PREVIEW, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP|wxTE_MULTILINE );
	bSizer3->Add( mCtrlPreview, 1, wxALL|wxEXPAND, 2 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button2 = new wxButton( m_panel2, ID_COPY, wxT("Copy"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_button2, 0, wxALL, 2 );
	
	mCtrlSave = new wxButton( m_panel2, ID_EAYSPINDLG_SAVE, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( mCtrlSave, 0, wxALL, 2 );
	
	mCtrlAutoGen = new wxCheckBox( m_panel2, ID_AUTOGEN, wxT("Automatically generate"), wxDefaultPosition, wxDefaultSize, 0 );
	mCtrlAutoGen->SetValue(true); 
	bSizer4->Add( mCtrlAutoGen, 0, wxALIGN_CENTER|wxALL, 0 );
	
	bSizer3->Add( bSizer4, 0, wxEXPAND, 5 );
	
	m_panel2->SetSizer( bSizer3 );
	m_panel2->Layout();
	bSizer3->Fit( m_panel2 );
	bSizer1->Add( m_panel2, 1, wxEXPAND | wxALL, 0 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

EasySpinFrameBase::~EasySpinFrameBase()
{
}
