///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov 18 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "gui/TextCtrlFocus.hpp"

#include "orientdialog.h"

///////////////////////////////////////////////////////////////////////////

OrientDialog2Base::OrientDialog2Base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );
	
	mOrientDisplay3D = new OrientDisplay3D(this);
	bSizer11->Add( mOrientDisplay3D, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer13;
	fgSizer13 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer13->SetFlexibleDirection( wxBOTH );
	fgSizer13->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Euler Angles") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer14;
	fgSizer14 = new wxFlexGridSizer( 2, 3, 0, 0 );
	fgSizer14->SetFlexibleDirection( wxBOTH );
	fgSizer14->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText47 = new wxStaticText( this, wxID_ANY, wxT("alpha"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText47->Wrap( -1 );
	fgSizer14->Add( m_staticText47, 0, wxALL, 5 );
	
	mAlpha = new TextCtrlFocus( this, ID_EULER, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	fgSizer14->Add( mAlpha, 0, wxALL, 0 );
	
	m_staticText50 = new wxStaticText( this, wxID_ANY, wxT("°"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText50->Wrap( -1 );
	fgSizer14->Add( m_staticText50, 0, wxALL, 5 );
	
	m_staticText48 = new wxStaticText( this, wxID_ANY, wxT("beta"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText48->Wrap( -1 );
	fgSizer14->Add( m_staticText48, 0, wxALL, 5 );
	
	mBeta = new TextCtrlFocus( this, ID_EULER, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	fgSizer14->Add( mBeta, 0, wxALL, 0 );
	
	m_staticText501 = new wxStaticText( this, wxID_ANY, wxT("°"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText501->Wrap( -1 );
	fgSizer14->Add( m_staticText501, 0, wxALL, 5 );
	
	m_staticText49 = new wxStaticText( this, wxID_ANY, wxT("gamma"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText49->Wrap( -1 );
	fgSizer14->Add( m_staticText49, 0, wxALL, 5 );
	
	mGamma = new TextCtrlFocus( this, ID_EULER, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	fgSizer14->Add( mGamma, 0, wxALL, 0 );
	
	m_staticText502 = new wxStaticText( this, wxID_ANY, wxT("°"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText502->Wrap( -1 );
	fgSizer14->Add( m_staticText502, 0, wxALL, 5 );
	
	sbSizer1->Add( fgSizer14, 1, wxEXPAND, 5 );
	
	fgSizer13->Add( sbSizer1, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Matrix") ), wxVERTICAL );
	
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 3, 3, 0, 0 );
	
	m00 = new TextCtrlFocus( this, ID_MATRIX, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	gSizer1->Add( m00, 0, wxALL, 0 );
	
	m01 = new TextCtrlFocus( this, ID_MATRIX, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	gSizer1->Add( m01, 0, wxALL, 0 );
	
	m02 = new TextCtrlFocus( this, ID_MATRIX, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	gSizer1->Add( m02, 0, wxALL, 0 );
	
	m10 = new TextCtrlFocus( this, ID_MATRIX, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	gSizer1->Add( m10, 0, wxALL, 0 );
	
	m11 = new TextCtrlFocus( this, ID_MATRIX, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	gSizer1->Add( m11, 0, wxALL, 0 );
	
	m12 = new TextCtrlFocus( this, ID_MATRIX, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	gSizer1->Add( m12, 0, wxALL, 0 );
	
	m20 = new TextCtrlFocus( this, ID_MATRIX, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	gSizer1->Add( m20, 0, wxALL, 0 );
	
	m21 = new TextCtrlFocus( this, ID_MATRIX, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	gSizer1->Add( m21, 0, wxALL, 0 );
	
	m22 = new TextCtrlFocus( this, ID_MATRIX, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	gSizer1->Add( m22, 0, wxALL, 0 );
	
	sbSizer2->Add( gSizer1, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	mTextDet = new wxStaticText( this, wxID_ANY, wxT("Det:"), wxDefaultPosition, wxDefaultSize, 0 );
	mTextDet->Wrap( -1 );
	bSizer6->Add( mTextDet, 0, wxALL, 5 );
	
	sbSizer2->Add( bSizer6, 0, wxEXPAND, 5 );
	
	fgSizer13->Add( sbSizer2, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Angle Axis") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer15;
	fgSizer15 = new wxFlexGridSizer( 4, 3, 0, 0 );
	fgSizer15->SetFlexibleDirection( wxBOTH );
	fgSizer15->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText56 = new wxStaticText( this, wxID_ANY, wxT("angle"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText56->Wrap( -1 );
	fgSizer15->Add( m_staticText56, 0, wxALL, 5 );
	
	mAngle = new TextCtrlFocus( this, ID_ANGLEAXIS, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	fgSizer15->Add( mAngle, 0, wxALL, 0 );
	
	m_staticText503 = new wxStaticText( this, wxID_ANY, wxT("°"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText503->Wrap( -1 );
	fgSizer15->Add( m_staticText503, 0, wxALL, 5 );
	
	m_staticText58 = new wxStaticText( this, wxID_ANY, wxT("x"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText58->Wrap( -1 );
	fgSizer15->Add( m_staticText58, 0, wxALL, 5 );
	
	mX = new TextCtrlFocus( this, ID_ANGLEAXIS, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	fgSizer15->Add( mX, 0, wxALL, 0 );
	
	m_staticText504 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText504->Wrap( -1 );
	fgSizer15->Add( m_staticText504, 0, wxALL, 5 );
	
	m_staticText60 = new wxStaticText( this, wxID_ANY, wxT("y"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText60->Wrap( -1 );
	fgSizer15->Add( m_staticText60, 0, wxALL, 5 );
	
	mY = new TextCtrlFocus( this, ID_ANGLEAXIS, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	fgSizer15->Add( mY, 0, wxALL, 0 );
	
	m_staticText505 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText505->Wrap( -1 );
	fgSizer15->Add( m_staticText505, 0, wxALL, 5 );
	
	m_staticText62 = new wxStaticText( this, wxID_ANY, wxT("z"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText62->Wrap( -1 );
	fgSizer15->Add( m_staticText62, 0, wxALL, 5 );
	
	mZ = new TextCtrlFocus( this, ID_ANGLEAXIS, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	fgSizer15->Add( mZ, 0, wxALL, 0 );
	
	m_staticText63 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText63->Wrap( -1 );
	fgSizer15->Add( m_staticText63, 0, wxALL, 5 );
	
	sbSizer3->Add( fgSizer15, 1, wxEXPAND, 5 );
	
	fgSizer13->Add( sbSizer3, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Quaternion") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer151;
	fgSizer151 = new wxFlexGridSizer( 4, 2, 0, 0 );
	fgSizer151->SetFlexibleDirection( wxBOTH );
	fgSizer151->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText561 = new wxStaticText( this, wxID_ANY, wxT("re"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText561->Wrap( -1 );
	fgSizer151->Add( m_staticText561, 0, wxALL, 5 );
	
	mRe = new TextCtrlFocus( this, ID_QUAT, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	fgSizer151->Add( mRe, 0, wxALL, 0 );
	
	m_staticText581 = new wxStaticText( this, wxID_ANY, wxT("i"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText581->Wrap( -1 );
	fgSizer151->Add( m_staticText581, 0, wxALL, 5 );
	
	mI = new TextCtrlFocus( this, ID_QUAT, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	fgSizer151->Add( mI, 0, wxALL, 0 );
	
	m_staticText601 = new wxStaticText( this, wxID_ANY, wxT("j"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText601->Wrap( -1 );
	fgSizer151->Add( m_staticText601, 0, wxALL, 5 );
	
	mJ = new TextCtrlFocus( this, ID_QUAT, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	fgSizer151->Add( mJ, 0, wxALL, 0 );
	
	m_staticText621 = new wxStaticText( this, wxID_ANY, wxT("k"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText621->Wrap( -1 );
	fgSizer151->Add( m_staticText621, 0, wxALL, 5 );
	
	mK = new TextCtrlFocus( this, ID_QUAT, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	fgSizer151->Add( mK, 0, wxALL, 0 );
	
	m_staticText631 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText631->Wrap( -1 );
	fgSizer151->Add( m_staticText631, 0, wxALL, 5 );
	
	sbSizer4->Add( fgSizer151, 1, wxEXPAND, 5 );
	
	fgSizer13->Add( sbSizer4, 1, wxEXPAND, 5 );
	
	bSizer17->Add( fgSizer13, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxVERTICAL );
	
	m_radioBtn1 = new wxRadioButton( this, wxID_ANY, wxT("active rotation"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( m_radioBtn1, 0, wxALL, 5 );
	
	m_radioBtn2 = new wxRadioButton( this, wxID_ANY, wxT("passive rotation"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( m_radioBtn2, 0, wxALL, 5 );
	
	bSizer5->Add( bSizer16, 0, wxEXPAND, 5 );
	
	mIdentity = new wxButton( this, ID_IDENTITY, wxT("Identity"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( mIdentity, 0, wxALL, 5 );
	
	bSizer17->Add( bSizer5, 0, wxEXPAND, 5 );
	
	bSizer11->Add( bSizer17, 1, wxEXPAND, 5 );
	
	bSizer14->Add( bSizer11, 1, wxEXPAND, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer14->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	m_sdbSizer2 = new wxStdDialogButtonSizer();
	m_sdbSizer2OK = new wxButton( this, wxID_OK );
	m_sdbSizer2->AddButton( m_sdbSizer2OK );
	m_sdbSizer2Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer2->AddButton( m_sdbSizer2Cancel );
	m_sdbSizer2->Realize();
	bSizer14->Add( m_sdbSizer2, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer14 );
	this->Layout();
	bSizer14->Fit( this );
	
	this->Centre( wxBOTH );
}

OrientDialog2Base::~OrientDialog2Base()
{
}
