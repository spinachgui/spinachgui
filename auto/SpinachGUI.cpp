///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov 18 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "gui/SpinChoice.hpp"

#include "SpinachGUI.h"

#include "res/bonds.xpm"
#include "res/document_new.xpm"
#include "res/document_open.xpm"
#include "res/document_save.xpm"
#include "res/epr.xpm"
#include "res/nmr.xpm"
#include "res/nmr_epr.xpm"

///////////////////////////////////////////////////////////////////////////

RootFrameBase::RootFrameBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	mRootMenu = new wxMenuBar( 0 );
	mMenuFile = new wxMenu();
	wxMenuItem* mMenuItemNew;
	mMenuItemNew = new wxMenuItem( mMenuFile, ID_NEW, wxString( wxT("&New") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	mMenuItemNew->SetBitmaps( wxBitmap( document_new_xpm ) );
	#elif defined( __WXGTK__ )
	mMenuItemNew->SetBitmap( wxBitmap( document_new_xpm ) );
	#endif
	mMenuFile->Append( mMenuItemNew );
	
	wxMenuItem* mMenuItemOpen;
	mMenuItemOpen = new wxMenuItem( mMenuFile, ID_OPEN, wxString( wxT("Open") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	mMenuItemOpen->SetBitmaps( wxBitmap( document_open_xpm ) );
	#elif defined( __WXGTK__ )
	mMenuItemOpen->SetBitmap( wxBitmap( document_open_xpm ) );
	#endif
	mMenuFile->Append( mMenuItemOpen );
	
	wxMenuItem* mMenuItemSave;
	mMenuItemSave = new wxMenuItem( mMenuFile, ID_SAVE, wxString( wxT("Save") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	mMenuItemSave->SetBitmaps( wxBitmap( document_save_xpm ) );
	#elif defined( __WXGTK__ )
	mMenuItemSave->SetBitmap( wxBitmap( document_save_xpm ) );
	#endif
	mMenuFile->Append( mMenuItemSave );
	
	wxMenuItem* mMenuItemSaveAs;
	mMenuItemSaveAs = new wxMenuItem( mMenuFile, ID_SAVEAS, wxString( wxT("Save &As") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuFile->Append( mMenuItemSaveAs );
	
	wxMenuItem* mEasyspin;
	mEasyspin = new wxMenuItem( mMenuFile, ID_EASYSPIN_EXPORT, wxString( wxT("Export to Easyspin...") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuFile->Append( mEasyspin );
	
	wxMenuItem* m_separator1;
	m_separator1 = mMenuFile->AppendSeparator();
	
	wxMenuItem* mMenuItemExit;
	mMenuItemExit = new wxMenuItem( mMenuFile, ID_EXIT, wxString( wxT("E&xit") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuFile->Append( mMenuItemExit );
	
	mRootMenu->Append( mMenuFile, wxT("&File") ); 
	
	mMenuEdit = new wxMenu();
	wxMenuItem* mMenuItemIso;
	mMenuItemIso = new wxMenuItem( mMenuEdit, ID_EDIT_ISO, wxString( wxT("Make Selection Isotropic") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuEdit->Append( mMenuItemIso );
	
	mRootMenu->Append( mMenuEdit, wxT("&Edit") ); 
	
	mMenuView = new wxMenu();
	wxMenuItem* mMenuItemWireframe;
	mMenuItemWireframe = new wxMenuItem( mMenuView, wxID_ANY, wxString( wxT("Wireframe Mode") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuView->Append( mMenuItemWireframe );
	
	wxMenuItem* mMenuItemFilled;
	mMenuItemFilled = new wxMenuItem( mMenuView, wxID_ANY, wxString( wxT("Solid") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuView->Append( mMenuItemFilled );
	
	wxMenuItem* m_separator4;
	m_separator4 = mMenuView->AppendSeparator();
	
	mMenuItemNmrEpr = new wxMenuItem( mMenuView, ID_NMR_EPR, wxString( wxT("Set Mixed Mode") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuView->Append( mMenuItemNmrEpr );
	
	mMenuItemEpr = new wxMenuItem( mMenuView, ID_EPR, wxString( wxT("Set EPR Mode") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuView->Append( mMenuItemEpr );
	
	mMenuItemNmr = new wxMenuItem( mMenuView, ID_NMR, wxString( wxT("Set NMR Mode") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuView->Append( mMenuItemNmr );
	
	wxMenuItem* m_separator5;
	m_separator5 = mMenuView->AppendSeparator();
	
	mMenuItemBondToggle = new wxMenuItem( mMenuView, ID_BOND_TOGGLE, wxString( wxT("Toggle Bonds") ) , wxEmptyString, wxITEM_CHECK );
	mMenuView->Append( mMenuItemBondToggle );
	
	wxMenuItem* m_separator7;
	m_separator7 = mMenuView->AppendSeparator();
	
	mMenuItemGrid = new wxMenuItem( mMenuView, ID_VIEW_GRID, wxString( wxT("Grid") ) , wxEmptyString, wxITEM_CHECK );
	mMenuView->Append( mMenuItemGrid );
	
	mMenuItemTensorVis = new wxMenuItem( mMenuView, ID_VIEW_TENSORVIS, wxString( wxT("Interaction Tensor Visualisation") ) , wxEmptyString, wxITEM_CHECK );
	mMenuView->Append( mMenuItemTensorVis );
	
	mMenuItemIntEdit = new wxMenuItem( mMenuView, wxID_ANY, wxString( wxT("Interaction Editor") ) , wxEmptyString, wxITEM_CHECK );
	mMenuView->Append( mMenuItemIntEdit );
	
	mMenuItemFrames = new wxMenuItem( mMenuView, ID_VIEW_FRAMES, wxString( wxT("Reference Frames") ) , wxEmptyString, wxITEM_CHECK );
	mMenuView->Append( mMenuItemFrames );
	
	wxMenuItem* m_separator51;
	m_separator51 = mMenuView->AppendSeparator();
	
	wxMenuItem* mMenuItemSupress;
	mMenuItemSupress = new wxMenuItem( mMenuView, ID_SUPRESS_SELECTION, wxString( wxT("Supress Interactions on Selection") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuView->Append( mMenuItemSupress );
	
	wxMenuItem* mMenuItemUnspress;
	mMenuItemUnspress = new wxMenuItem( mMenuView, ID_UNSUPRESS, wxString( wxT("Unsupress Interactions") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuView->Append( mMenuItemUnspress );
	
	wxMenuItem* m_separator71;
	m_separator71 = mMenuView->AppendSeparator();
	
	wxMenuItem* mMenuItemEllipsoid;
	mMenuItemEllipsoid = new wxMenuItem( mMenuView, ID_ELLIPSOIDS, wxString( wxT("Draw Ellipsoids") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuView->Append( mMenuItemEllipsoid );
	
	wxMenuItem* mMenuItemAxes;
	mMenuItemAxes = new wxMenuItem( mMenuView, ID_AXES, wxString( wxT("Tensor Axes") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuView->Append( mMenuItemAxes );
	
	mRootMenu->Append( mMenuView, wxT("&View") ); 
	
	mMenuUnits = new wxMenu();
	mMenuLength = new wxMenu();
	mMenuUnits->Append( -1, wxT("Length"), mMenuLength );
	
	mMenuEnergy = new wxMenu();
	mMenuUnits->Append( -1, wxT("Energy"), mMenuEnergy );
	
	mRootMenu->Append( mMenuUnits, wxT("Units") ); 
	
	mMenuSelection = new wxMenu();
	mRootMenu->Append( mMenuSelection, wxT("Selection") ); 
	
	mMenuHelp = new wxMenu();
	wxMenuItem* mMenuItemHelp;
	mMenuItemHelp = new wxMenuItem( mMenuHelp, wxID_ANY, wxString( wxT("Help...") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuHelp->Append( mMenuItemHelp );
	
	wxMenuItem* mMenuItemAbout;
	mMenuItemAbout = new wxMenuItem( mMenuHelp, wxID_ANY, wxString( wxT("About") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuHelp->Append( mMenuItemAbout );
	
	mRootMenu->Append( mMenuHelp, wxT("&Help") ); 
	
	this->SetMenuBar( mRootMenu );
	
	wxBoxSizer* mRootSizer;
	mRootSizer = new wxBoxSizer( wxHORIZONTAL );
	
	this->SetSizer( mRootSizer );
	this->Layout();
	mRootToolbar = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY ); 
	mRootToolbar->AddTool( ID_NEW, wxT("tool"), wxBitmap( document_new_xpm ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	mRootToolbar->AddTool( ID_OPEN, wxT("tool"), wxBitmap( document_open_xpm ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	mRootToolbar->AddTool( ID_SAVE, wxT("tool"), wxBitmap( document_save_xpm ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	mRootToolbar->AddSeparator(); 
	mRootToolbar->AddTool( ID_NMR_EPR, wxT("Show All Interactions"), wxBitmap( nmr_epr_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Show All Interactions"), wxEmptyString, NULL ); 
	mRootToolbar->AddTool( ID_EPR, wxT("EPR Mode"), wxBitmap( epr_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Show EPR Interactions"), wxEmptyString, NULL ); 
	mRootToolbar->AddTool( ID_NMR, wxT("NMR Mode"), wxBitmap( nmr_xpm ), wxNullBitmap, wxITEM_NORMAL, wxT("Show NMR Interactions"), wxEmptyString, NULL ); 
	mRootToolbar->AddSeparator(); 
	mRootToolbar->AddTool( ID_BOND_TOGGLE, wxT("tool"), wxBitmap( bonds_xpm ), wxNullBitmap, wxITEM_CHECK, wxT("Toggle Bonds"), wxEmptyString, NULL ); 
	mRootToolbar->Realize(); 
	
}

RootFrameBase::~RootFrameBase()
{
}

SpinInterEditPanelBase::SpinInterEditPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer29;
	bSizer29 = new wxBoxSizer( wxVERTICAL );
	
	wxArrayString mSpinChoiceChoices;
	mSpinChoice = new SpinChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, mSpinChoiceChoices, 0 );
	mSpinChoice->SetSelection( 0 );
	bSizer29->Add( mSpinChoice, 0, wxALL|wxEXPAND, 5 );
	
	mInterListBox = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	mInterListBox->SetMinSize( wxSize( 200,200 ) );
	
	bSizer29->Add( mInterListBox, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer30;
	bSizer30 = new wxBoxSizer( wxHORIZONTAL );
	
	addButton = new wxButton( this, INTER_ADD, wxT("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer30->Add( addButton, 0, wxALL, 5 );
	
	deleteButton = new wxButton( this, INTER_DELETE, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer30->Add( deleteButton, 0, wxALL, 5 );
	
	bSizer29->Add( bSizer30, 0, wxEXPAND, 5 );
	
	bSizer12->Add( bSizer29, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer12 );
	this->Layout();
}

SpinInterEditPanelBase::~SpinInterEditPanelBase()
{
}

OrientEditPanelBase::OrientEditPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxHORIZONTAL );
	
	mOrientTypeChoiceBook = new wxChoicebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCHB_DEFAULT );
	eulerEditPanel = new wxPanel( mOrientTypeChoiceBook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer19;
	fgSizer19 = new wxFlexGridSizer( 1, 4, 0, 0 );
	fgSizer19->SetFlexibleDirection( wxBOTH );
	fgSizer19->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer7;
	fgSizer7 = new wxFlexGridSizer( 2, 3, 0, 0 );
	fgSizer7->SetFlexibleDirection( wxBOTH );
	fgSizer7->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText8 = new wxStaticText( eulerEditPanel, wxID_ANY, wxT("Alpha"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	fgSizer7->Add( m_staticText8, 0, wxALL, 5 );
	
	mAlphaCtrl = new wxTextCtrl( eulerEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	mAlphaCtrl->SetToolTip( wxT("alpha Euler angle (degrees)") );
	
	fgSizer7->Add( mAlphaCtrl, 0, wxALL, 5 );
	
	m_staticText411 = new wxStaticText( eulerEditPanel, wxID_ANY, wxT("degrees"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText411->Wrap( -1 );
	fgSizer7->Add( m_staticText411, 0, wxALL, 5 );
	
	m_staticText9 = new wxStaticText( eulerEditPanel, wxID_ANY, wxT("Beta"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	fgSizer7->Add( m_staticText9, 0, wxALL, 5 );
	
	mBetaCtrl = new wxTextCtrl( eulerEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	mBetaCtrl->SetToolTip( wxT("beta Euler angle (degrees)") );
	
	fgSizer7->Add( mBetaCtrl, 0, wxALL, 5 );
	
	m_staticText421 = new wxStaticText( eulerEditPanel, wxID_ANY, wxT("degrees"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText421->Wrap( -1 );
	fgSizer7->Add( m_staticText421, 0, wxALL, 5 );
	
	m_staticText10 = new wxStaticText( eulerEditPanel, wxID_ANY, wxT("Gamma"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	fgSizer7->Add( m_staticText10, 0, wxALL, 5 );
	
	mGammaCtrl = new wxTextCtrl( eulerEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	mGammaCtrl->SetToolTip( wxT("gamma Euler angle (degrees)") );
	
	fgSizer7->Add( mGammaCtrl, 0, wxALL, 5 );
	
	m_staticText431 = new wxStaticText( eulerEditPanel, wxID_ANY, wxT("degrees"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText431->Wrap( -1 );
	fgSizer7->Add( m_staticText431, 0, wxALL, 5 );
	
	fgSizer19->Add( fgSizer7, 1, wxEXPAND, 5 );
	
	eulerEditPanel->SetSizer( fgSizer19 );
	eulerEditPanel->Layout();
	fgSizer19->Fit( eulerEditPanel );
	mOrientTypeChoiceBook->AddPage( eulerEditPanel, wxT("Euler Angles"), false );
	angleAxisEditPanel = new wxPanel( mOrientTypeChoiceBook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer29;
	fgSizer29 = new wxFlexGridSizer( 4, 3, 0, 0 );
	fgSizer29->SetFlexibleDirection( wxBOTH );
	fgSizer29->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText71 = new wxStaticText( angleAxisEditPanel, wxID_ANY, wxT("Angle"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText71->Wrap( -1 );
	fgSizer29->Add( m_staticText71, 0, wxALL, 5 );
	
	mAngleCtrl = new wxTextCtrl( angleAxisEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer29->Add( mAngleCtrl, 0, wxALL, 5 );
	
	m_staticText441 = new wxStaticText( angleAxisEditPanel, wxID_ANY, wxT("degrees"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText441->Wrap( -1 );
	fgSizer29->Add( m_staticText441, 0, wxALL, 5 );
	
	m_staticText72 = new wxStaticText( angleAxisEditPanel, wxID_ANY, wxT("x:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText72->Wrap( -1 );
	fgSizer29->Add( m_staticText72, 0, wxALL, 5 );
	
	mXCtrl = new wxTextCtrl( angleAxisEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer29->Add( mXCtrl, 0, wxALL, 5 );
	
	m_staticText45 = new wxStaticText( angleAxisEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText45->Wrap( -1 );
	fgSizer29->Add( m_staticText45, 0, wxALL, 5 );
	
	m_staticText73 = new wxStaticText( angleAxisEditPanel, wxID_ANY, wxT("y:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText73->Wrap( -1 );
	fgSizer29->Add( m_staticText73, 0, wxALL, 5 );
	
	mYCtrl = new wxTextCtrl( angleAxisEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer29->Add( mYCtrl, 0, wxALL, 5 );
	
	m_staticText46 = new wxStaticText( angleAxisEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText46->Wrap( -1 );
	fgSizer29->Add( m_staticText46, 0, wxALL, 5 );
	
	m_staticText75 = new wxStaticText( angleAxisEditPanel, wxID_ANY, wxT("x:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText75->Wrap( -1 );
	fgSizer29->Add( m_staticText75, 0, wxALL, 5 );
	
	mZCtrl = new wxTextCtrl( angleAxisEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer29->Add( mZCtrl, 0, wxALL, 5 );
	
	angleAxisEditPanel->SetSizer( fgSizer29 );
	angleAxisEditPanel->Layout();
	fgSizer29->Fit( angleAxisEditPanel );
	mOrientTypeChoiceBook->AddPage( angleAxisEditPanel, wxT("Angle Axis"), true );
	quaternionEditPanel = new wxPanel( mOrientTypeChoiceBook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer291;
	fgSizer291 = new wxFlexGridSizer( 4, 2, 0, 0 );
	fgSizer291->SetFlexibleDirection( wxBOTH );
	fgSizer291->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText711 = new wxStaticText( quaternionEditPanel, wxID_ANY, wxT("Re:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText711->Wrap( -1 );
	fgSizer291->Add( m_staticText711, 0, wxALL, 5 );
	
	mReCtrl = new wxTextCtrl( quaternionEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer291->Add( mReCtrl, 0, wxALL, 5 );
	
	m_staticText721 = new wxStaticText( quaternionEditPanel, wxID_ANY, wxT("i:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText721->Wrap( -1 );
	fgSizer291->Add( m_staticText721, 0, wxALL, 5 );
	
	mICtrl = new wxTextCtrl( quaternionEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer291->Add( mICtrl, 0, wxALL, 5 );
	
	m_staticText731 = new wxStaticText( quaternionEditPanel, wxID_ANY, wxT("j:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText731->Wrap( -1 );
	fgSizer291->Add( m_staticText731, 0, wxALL, 5 );
	
	mJCtrl = new wxTextCtrl( quaternionEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer291->Add( mJCtrl, 0, wxALL, 5 );
	
	m_staticText751 = new wxStaticText( quaternionEditPanel, wxID_ANY, wxT("k:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText751->Wrap( -1 );
	fgSizer291->Add( m_staticText751, 0, wxALL, 5 );
	
	mKCtrl = new wxTextCtrl( quaternionEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer291->Add( mKCtrl, 0, wxALL, 5 );
	
	quaternionEditPanel->SetSizer( fgSizer291 );
	quaternionEditPanel->Layout();
	fgSizer291->Fit( quaternionEditPanel );
	mOrientTypeChoiceBook->AddPage( quaternionEditPanel, wxT("Quaternion"), false );
	eigensystemEditPanel = new wxPanel( mOrientTypeChoiceBook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer21;
	fgSizer21 = new wxFlexGridSizer( 4, 4, 0, 0 );
	fgSizer21->SetFlexibleDirection( wxBOTH );
	fgSizer21->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText38 = new wxStaticText( eigensystemEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText38->Wrap( -1 );
	fgSizer21->Add( m_staticText38, 0, wxALL, 5 );
	
	m_staticText39 = new wxStaticText( eigensystemEditPanel, wxID_ANY, wxT("x"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText39->Wrap( -1 );
	fgSizer21->Add( m_staticText39, 0, wxALL, 5 );
	
	m_staticText40 = new wxStaticText( eigensystemEditPanel, wxID_ANY, wxT("y"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText40->Wrap( -1 );
	fgSizer21->Add( m_staticText40, 0, wxALL, 5 );
	
	m_staticText41 = new wxStaticText( eigensystemEditPanel, wxID_ANY, wxT("z"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText41->Wrap( -1 );
	fgSizer21->Add( m_staticText41, 0, wxALL, 5 );
	
	m_staticText42 = new wxStaticText( eigensystemEditPanel, wxID_ANY, wxT("x"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText42->Wrap( -1 );
	fgSizer21->Add( m_staticText42, 0, wxALL, 5 );
	
	mXXCtrl = new wxTextCtrl( eigensystemEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( mXXCtrl, 0, wxALL, 5 );
	
	mXYCtrl = new wxTextCtrl( eigensystemEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( mXYCtrl, 0, wxALL, 5 );
	
	mXZCtrl = new wxTextCtrl( eigensystemEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( mXZCtrl, 0, wxALL, 5 );
	
	m_staticText43 = new wxStaticText( eigensystemEditPanel, wxID_ANY, wxT("y"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText43->Wrap( -1 );
	fgSizer21->Add( m_staticText43, 0, wxALL, 5 );
	
	mYXCtrl = new wxTextCtrl( eigensystemEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( mYXCtrl, 0, wxALL, 5 );
	
	mYYCtrl = new wxTextCtrl( eigensystemEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( mYYCtrl, 0, wxALL, 5 );
	
	mYZCtrl = new wxTextCtrl( eigensystemEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( mYZCtrl, 0, wxALL, 5 );
	
	m_staticText44 = new wxStaticText( eigensystemEditPanel, wxID_ANY, wxT("z"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText44->Wrap( -1 );
	fgSizer21->Add( m_staticText44, 0, wxALL, 5 );
	
	mZXCtrl = new wxTextCtrl( eigensystemEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( mZXCtrl, 0, wxALL, 5 );
	
	mZYCtrl = new wxTextCtrl( eigensystemEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( mZYCtrl, 0, wxALL, 5 );
	
	mZZCtrl = new wxTextCtrl( eigensystemEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( mZZCtrl, 0, wxALL, 5 );
	
	eigensystemEditPanel->SetSizer( fgSizer21 );
	eigensystemEditPanel->Layout();
	fgSizer21->Fit( eigensystemEditPanel );
	mOrientTypeChoiceBook->AddPage( eigensystemEditPanel, wxT("DCM/Rotation Matrix"), false );
	bSizer18->Add( mOrientTypeChoiceBook, 1, wxEXPAND | wxALL, 5 );
	
	this->SetSizer( bSizer18 );
	this->Layout();
}

OrientEditPanelBase::~OrientEditPanelBase()
{
}

InterEditPanelBase::InterEditPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxHORIZONTAL );
	
	bSizer16->Add( bSizer15, 0, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer19;
	fgSizer19 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer19->AddGrowableCol( 1 );
	fgSizer19->SetFlexibleDirection( wxBOTH );
	fgSizer19->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText51 = new wxStaticText( this, wxID_ANY, wxT("Type:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText51->Wrap( -1 );
	fgSizer19->Add( m_staticText51, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxArrayString mSubTypeComboChoices;
	mSubTypeCombo = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, mSubTypeComboChoices, 0 );
	mSubTypeCombo->SetSelection( 0 );
	fgSizer19->Add( mSubTypeCombo, 1, wxALL|wxEXPAND, 5 );
	
	mcoupspin_static_text = new wxStaticText( this, wxID_ANY, wxT("Coupled Spin:"), wxDefaultPosition, wxDefaultSize, 0 );
	mcoupspin_static_text->Wrap( -1 );
	fgSizer19->Add( mcoupspin_static_text, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxArrayString mSpin2ComboChoices;
	mSpin2Combo = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, mSpin2ComboChoices, 0 );
	mSpin2Combo->SetSelection( 0 );
	fgSizer19->Add( mSpin2Combo, 1, wxALL|wxEXPAND, 5 );
	
	bSizer16->Add( fgSizer19, 1, wxEXPAND, 5 );
	
	bSizer23->Add( bSizer16, 0, wxEXPAND, 5 );
	
	mTypeChoiceBook = new wxChoicebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCHB_DEFAULT );
	mScalarEditPanel = new wxPanel( mTypeChoiceBook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer20;
	fgSizer20 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer20->SetFlexibleDirection( wxBOTH );
	fgSizer20->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText36 = new wxStaticText( mScalarEditPanel, wxID_ANY, wxT("Scalar Value:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText36->Wrap( -1 );
	fgSizer20->Add( m_staticText36, 0, wxALL, 5 );
	
	mScalarCtrl = new wxTextCtrl( mScalarEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer20->Add( mScalarCtrl, 0, wxALL, 5 );
	
	mScalarEditPanel->SetSizer( fgSizer20 );
	mScalarEditPanel->Layout();
	fgSizer20->Fit( mScalarEditPanel );
	mTypeChoiceBook->AddPage( mScalarEditPanel, wxT("Scalar"), false );
	mMatrixEditPanel = new wxPanel( mTypeChoiceBook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer21;
	fgSizer21 = new wxFlexGridSizer( 4, 4, 0, 0 );
	fgSizer21->SetFlexibleDirection( wxBOTH );
	fgSizer21->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText38 = new wxStaticText( mMatrixEditPanel, wxID_ANY, wxT("\\"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText38->Wrap( -1 );
	fgSizer21->Add( m_staticText38, 0, wxALL, 5 );
	
	m_staticText39 = new wxStaticText( mMatrixEditPanel, wxID_ANY, wxT("x"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText39->Wrap( -1 );
	fgSizer21->Add( m_staticText39, 0, wxALL, 5 );
	
	m_staticText40 = new wxStaticText( mMatrixEditPanel, wxID_ANY, wxT("y"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText40->Wrap( -1 );
	fgSizer21->Add( m_staticText40, 0, wxALL, 5 );
	
	m_staticText41 = new wxStaticText( mMatrixEditPanel, wxID_ANY, wxT("z"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText41->Wrap( -1 );
	fgSizer21->Add( m_staticText41, 0, wxALL, 5 );
	
	m_staticText42 = new wxStaticText( mMatrixEditPanel, wxID_ANY, wxT("x"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText42->Wrap( -1 );
	fgSizer21->Add( m_staticText42, 0, wxALL, 5 );
	
	mMatXXCtrl = new wxTextCtrl( mMatrixEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( mMatXXCtrl, 1, wxALL|wxEXPAND, 5 );
	
	mMatYXCtrl = new wxTextCtrl( mMatrixEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( mMatYXCtrl, 1, wxALL|wxEXPAND, 5 );
	
	mMatZXCtrl = new wxTextCtrl( mMatrixEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( mMatZXCtrl, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText43 = new wxStaticText( mMatrixEditPanel, wxID_ANY, wxT("y"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText43->Wrap( -1 );
	fgSizer21->Add( m_staticText43, 0, wxALL, 5 );
	
	mMatXYCtrl = new wxTextCtrl( mMatrixEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( mMatXYCtrl, 1, wxALL|wxEXPAND, 5 );
	
	mMatYYCtrl = new wxTextCtrl( mMatrixEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( mMatYYCtrl, 1, wxALL|wxEXPAND, 5 );
	
	mMatZYCtrl = new wxTextCtrl( mMatrixEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( mMatZYCtrl, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText44 = new wxStaticText( mMatrixEditPanel, wxID_ANY, wxT("z"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText44->Wrap( -1 );
	fgSizer21->Add( m_staticText44, 0, wxALL, 5 );
	
	mMatXZCtrl = new wxTextCtrl( mMatrixEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( mMatXZCtrl, 1, wxALL|wxEXPAND, 5 );
	
	mMatYZCtrl = new wxTextCtrl( mMatrixEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( mMatYZCtrl, 1, wxALL|wxEXPAND, 5 );
	
	mMatZZCtrl = new wxTextCtrl( mMatrixEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( mMatZZCtrl, 1, wxALL|wxEXPAND, 5 );
	
	mMatrixEditPanel->SetSizer( fgSizer21 );
	mMatrixEditPanel->Layout();
	fgSizer21->Fit( mMatrixEditPanel );
	mTypeChoiceBook->AddPage( mMatrixEditPanel, wxT("Matrix"), false );
	mEigenEditPanel = new wxPanel( mTypeChoiceBook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer29;
	fgSizer29 = new wxFlexGridSizer( 4, 2, 0, 0 );
	fgSizer29->SetFlexibleDirection( wxBOTH );
	fgSizer29->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText71 = new wxStaticText( mEigenEditPanel, wxID_ANY, wxT("XX"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText71->Wrap( -1 );
	fgSizer29->Add( m_staticText71, 0, wxALL, 5 );
	
	mEigenXXCtrl = new wxTextCtrl( mEigenEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer29->Add( mEigenXXCtrl, 0, wxALL, 5 );
	
	m_staticText72 = new wxStaticText( mEigenEditPanel, wxID_ANY, wxT("YY"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText72->Wrap( -1 );
	fgSizer29->Add( m_staticText72, 0, wxALL, 5 );
	
	mEigenYYCtrl = new wxTextCtrl( mEigenEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer29->Add( mEigenYYCtrl, 0, wxALL, 5 );
	
	m_staticText73 = new wxStaticText( mEigenEditPanel, wxID_ANY, wxT("ZZ"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText73->Wrap( -1 );
	fgSizer29->Add( m_staticText73, 0, wxALL, 5 );
	
	mEigenZZCtrl = new wxTextCtrl( mEigenEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer29->Add( mEigenZZCtrl, 0, wxALL, 5 );
	
	m_staticText75 = new wxStaticText( mEigenEditPanel, wxID_ANY, wxT("Orientation:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText75->Wrap( -1 );
	fgSizer29->Add( m_staticText75, 0, wxALL, 5 );
	
	mEigenEditPanel->SetSizer( fgSizer29 );
	mEigenEditPanel->Layout();
	fgSizer29->Fit( mEigenEditPanel );
	mTypeChoiceBook->AddPage( mEigenEditPanel, wxT("Eigenvalues"), true );
	mAxRhomEditPanel = new wxPanel( mTypeChoiceBook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer291;
	fgSizer291 = new wxFlexGridSizer( 4, 2, 0, 0 );
	fgSizer291->SetFlexibleDirection( wxBOTH );
	fgSizer291->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText711 = new wxStaticText( mAxRhomEditPanel, wxID_ANY, wxT("Axiality:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText711->Wrap( -1 );
	fgSizer291->Add( m_staticText711, 0, wxALL, 5 );
	
	mAxCtrl = new wxTextCtrl( mAxRhomEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer291->Add( mAxCtrl, 0, wxALL, 5 );
	
	m_staticText721 = new wxStaticText( mAxRhomEditPanel, wxID_ANY, wxT("Rhombicity:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText721->Wrap( -1 );
	fgSizer291->Add( m_staticText721, 0, wxALL, 5 );
	
	mRhomCtrl = new wxTextCtrl( mAxRhomEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer291->Add( mRhomCtrl, 0, wxALL, 5 );
	
	m_staticText731 = new wxStaticText( mAxRhomEditPanel, wxID_ANY, wxT("Isotropic:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText731->Wrap( -1 );
	fgSizer291->Add( m_staticText731, 0, wxALL, 5 );
	
	mAxRhomIsoCtrl = new wxTextCtrl( mAxRhomEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer291->Add( mAxRhomIsoCtrl, 0, wxALL, 5 );
	
	m_staticText751 = new wxStaticText( mAxRhomEditPanel, wxID_ANY, wxT("Orientation:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText751->Wrap( -1 );
	fgSizer291->Add( m_staticText751, 0, wxALL, 5 );
	
	mAxRhomEditPanel->SetSizer( fgSizer291 );
	mAxRhomEditPanel->Layout();
	fgSizer291->Fit( mAxRhomEditPanel );
	mTypeChoiceBook->AddPage( mAxRhomEditPanel, wxT("Axiality-Rhombicity"), false );
	mSpanSkewEditPanel = new wxPanel( mTypeChoiceBook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer292;
	fgSizer292 = new wxFlexGridSizer( 4, 2, 0, 0 );
	fgSizer292->SetFlexibleDirection( wxBOTH );
	fgSizer292->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText712 = new wxStaticText( mSpanSkewEditPanel, wxID_ANY, wxT("Span:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText712->Wrap( -1 );
	fgSizer292->Add( m_staticText712, 0, wxALL, 5 );
	
	mSpanCtrl = new wxTextCtrl( mSpanSkewEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer292->Add( mSpanCtrl, 0, wxALL, 5 );
	
	m_staticText722 = new wxStaticText( mSpanSkewEditPanel, wxID_ANY, wxT("Skew:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText722->Wrap( -1 );
	fgSizer292->Add( m_staticText722, 0, wxALL, 5 );
	
	mSkewCtrl = new wxTextCtrl( mSpanSkewEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer292->Add( mSkewCtrl, 0, wxALL, 5 );
	
	m_staticText732 = new wxStaticText( mSpanSkewEditPanel, wxID_ANY, wxT("Isotropic:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText732->Wrap( -1 );
	fgSizer292->Add( m_staticText732, 0, wxALL, 5 );
	
	mSpanSkewIsoCtrl = new wxTextCtrl( mSpanSkewEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer292->Add( mSpanSkewIsoCtrl, 0, wxALL, 5 );
	
	m_staticText752 = new wxStaticText( mSpanSkewEditPanel, wxID_ANY, wxT("Orientation:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText752->Wrap( -1 );
	fgSizer292->Add( m_staticText752, 0, wxALL, 5 );
	
	mSpanSkewEditPanel->SetSizer( fgSizer292 );
	mSpanSkewEditPanel->Layout();
	fgSizer292->Fit( mSpanSkewEditPanel );
	mTypeChoiceBook->AddPage( mSpanSkewEditPanel, wxT("Span-Skew"), false );
	bSizer23->Add( mTypeChoiceBook, 1, wxEXPAND | wxALL, 5 );
	
	this->SetSizer( bSizer23 );
	this->Layout();
}

InterEditPanelBase::~InterEditPanelBase()
{
}

OrientDialogBase::OrientDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	mOrientDialogPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );
	
	okayCancel = new wxStdDialogButtonSizer();
	okayCancelOK = new wxButton( mOrientDialogPanel, wxID_OK );
	okayCancel->AddButton( okayCancelOK );
	okayCancelApply = new wxButton( mOrientDialogPanel, wxID_APPLY );
	okayCancel->AddButton( okayCancelApply );
	okayCancelCancel = new wxButton( mOrientDialogPanel, wxID_CANCEL );
	okayCancel->AddButton( okayCancelCancel );
	okayCancel->Realize();
	bSizer10->Add( okayCancel, 0, wxEXPAND, 5 );
	
	mOrientDialogPanel->SetSizer( bSizer10 );
	mOrientDialogPanel->Layout();
	bSizer10->Fit( mOrientDialogPanel );
	bSizer9->Add( mOrientDialogPanel, 1, wxEXPAND | wxALL, 0 );
	
	this->SetSizer( bSizer9 );
	this->Layout();
	bSizer9->Fit( this );
}

OrientDialogBase::~OrientDialogBase()
{
}

InterDisplaySettingsBase::InterDisplaySettingsBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxFlexGridSizer* fgSizer24;
	fgSizer24 = new wxFlexGridSizer( 1, 3, 0, 0 );
	fgSizer24->AddGrowableCol( 0 );
	fgSizer24->AddGrowableRow( 0 );
	fgSizer24->SetFlexibleDirection( wxBOTH );
	fgSizer24->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	mLogSlider = new LogSlider(this,0.8);
	mLogSlider->SetMinSize( wxSize( 0,0 ) );
	
	fgSizer24->Add( mLogSlider, 1, wxALIGN_CENTER_VERTICAL|wxEXPAND, 0 );
	
	mCheckbox = new wxCheckBox( this, wxID_ANY, wxT("Interaction"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer24->Add( mCheckbox, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );
	
	mColour = new wxColourPickerCtrl( this, wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxSize( 30,30 ), wxCLRP_DEFAULT_STYLE );
	mColour->SetMinSize( wxSize( 30,30 ) );
	mColour->SetMaxSize( wxSize( 30,30 ) );
	
	fgSizer24->Add( mColour, 0, wxALIGN_CENTER, 0 );
	
	this->SetSizer( fgSizer24 );
	this->Layout();
}

InterDisplaySettingsBase::~InterDisplaySettingsBase()
{
}
