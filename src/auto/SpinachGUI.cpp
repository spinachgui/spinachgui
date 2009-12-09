///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 29 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "SpinachGUI.h"

#include "../res/document_new.xpm"
#include "../res/document_open.xpm"
#include "../res/document_save.xpm"
#include "../res/epr.xpm"
#include "../res/nmr.xpm"
#include "../res/nmr_epr.xpm"
#include "../res/noimage.xpm"

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
	
	mMenuFile->AppendSeparator();
	
	wxMenuItem* mMenuItemImport;
	mMenuItemImport = new wxMenuItem( mMenuFile, ID_IMPORT, wxString( wxT("Import...") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuFile->Append( mMenuItemImport );
	
	mMenuFile->AppendSeparator();
	
	wxMenuItem* mMenuItemOptions;
	mMenuItemOptions = new wxMenuItem( mMenuFile, ID_OPTIONS, wxString( wxT("&Options...") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuFile->Append( mMenuItemOptions );
	
	mMenuFile->AppendSeparator();
	
	wxMenuItem* mMenuItemExit;
	mMenuItemExit = new wxMenuItem( mMenuFile, ID_EXIT, wxString( wxT("E&xit") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuFile->Append( mMenuItemExit );
	
	mRootMenu->Append( mMenuFile, wxT("&File") );
	
	mMenuEdit = new wxMenu();
	mMenuItemUndo = new wxMenuItem( mMenuEdit, ID_UNDO, wxString( wxT("&Undo") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuEdit->Append( mMenuItemUndo );
	
	mMenuItemRedo = new wxMenuItem( mMenuEdit, ID_REDO, wxString( wxT("&Redo") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuEdit->Append( mMenuItemRedo );
	
	mRootMenu->Append( mMenuEdit, wxT("&Edit") );
	
	mMenuCalculate = new wxMenu();
	wxMenuItem* mMenuItemSetupCalc;
	mMenuItemSetupCalc = new wxMenuItem( mMenuCalculate, wxID_ANY, wxString( wxT("Setup Calculation") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuCalculate->Append( mMenuItemSetupCalc );
	
	wxMenuItem* mMenuItemExportCalc;
	mMenuItemExportCalc = new wxMenuItem( mMenuCalculate, wxID_ANY, wxString( wxT("Export Calculation") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuCalculate->Append( mMenuItemExportCalc );
	
	mRootMenu->Append( mMenuCalculate, wxT("C&alculate") );
	
	mMenuView = new wxMenu();
	wxMenuItem* mMenuItemWireframe;
	mMenuItemWireframe = new wxMenuItem( mMenuView, wxID_ANY, wxString( wxT("Wireframe Mode") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuView->Append( mMenuItemWireframe );
	
	wxMenuItem* mMenuItemFilled;
	mMenuItemFilled = new wxMenuItem( mMenuView, wxID_ANY, wxString( wxT("Solid") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuView->Append( mMenuItemFilled );
	
	mMenuView->AppendSeparator();
	
	mMenuItemNmrEpr = new wxMenuItem( mMenuView, ID_NMR_EPR, wxString( wxT("Mixed Mode") ) , wxEmptyString, wxITEM_RADIO );
	mMenuView->Append( mMenuItemNmrEpr );
	mMenuItemNmrEpr->Check( true );
	
	mMenuItemEpr = new wxMenuItem( mMenuView, ID_EPR, wxString( wxT("EPR Mode") ) , wxEmptyString, wxITEM_RADIO );
	mMenuView->Append( mMenuItemEpr );
	
	mMenuItemNmr = new wxMenuItem( mMenuView, ID_NMR, wxString( wxT("NMR Mode") ) , wxEmptyString, wxITEM_RADIO );
	mMenuView->Append( mMenuItemNmr );
	
	mRootMenu->Append( mMenuView, wxT("&View") );
	
	mMenuMode = new wxMenu();
	wxMenuItem* mMenuItemShielding;
	mMenuItemShielding = new wxMenuItem( mMenuMode, wxID_ANY, wxString( wxT("Shielding") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuMode->Append( mMenuItemShielding );
	
	wxMenuItem* mMenuItemJCoupling;
	mMenuItemJCoupling = new wxMenuItem( mMenuMode, wxID_ANY, wxString( wxT("J Coupling") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuMode->Append( mMenuItemJCoupling );
	
	wxMenuItem* mMenuItemClusters;
	mMenuItemClusters = new wxMenuItem( mMenuMode, wxID_ANY, wxString( wxT("Clusters") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuMode->Append( mMenuItemClusters );
	
	mMenuMode->AppendSeparator();
	
	wxMenuItem* mMenuItemBondLength;
	mMenuItemBondLength = new wxMenuItem( mMenuMode, wxID_ANY, wxString( wxT("Bond Length") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuMode->Append( mMenuItemBondLength );
	
	wxMenuItem* mMenuItemBondAngle;
	mMenuItemBondAngle = new wxMenuItem( mMenuMode, wxID_ANY, wxString( wxT("Bond Angle") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuMode->Append( mMenuItemBondAngle );
	
	wxMenuItem* mMenuItemDiAngle;
	mMenuItemDiAngle = new wxMenuItem( mMenuMode, wxID_ANY, wxString( wxT("Dihedral Angle") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuMode->Append( mMenuItemDiAngle );
	
	mRootMenu->Append( mMenuMode, wxT("Mode") );
	
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
	
	mAuiPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );
	
	mAuiPanel->SetSizer( bSizer15 );
	mAuiPanel->Layout();
	bSizer15->Fit( mAuiPanel );
	mRootSizer->Add( mAuiPanel, 1, wxALL|wxEXPAND, 0 );
	
	this->SetSizer( mRootSizer );
	this->Layout();
	mRootStatusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	mRootToolbar = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY ); 
	mRootToolbar->AddTool( ID_NEW, wxT("tool"), wxBitmap( document_new_xpm ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	mRootToolbar->AddTool( ID_OPEN, wxT("tool"), wxBitmap( document_open_xpm ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	mRootToolbar->AddTool( ID_SAVE, wxT("tool"), wxBitmap( document_save_xpm ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	mRootToolbar->AddSeparator();
	mRootToolbar->AddTool( wxID_ANY, wxT("tool"), wxBitmap( noimage_xpm ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	mRootToolbar->AddSeparator();
	mRootToolbar->AddTool( ID_NMR_EPR, wxT("Show All Interactions"), wxBitmap( nmr_epr_xpm ), wxNullBitmap, wxITEM_RADIO, wxEmptyString, wxEmptyString );
	mRootToolbar->AddTool( ID_EPR, wxT("EPR Mode"), wxBitmap( epr_xpm ), wxNullBitmap, wxITEM_RADIO, wxEmptyString, wxEmptyString );
	mRootToolbar->AddTool( ID_NMR, wxT("NMR Mode"), wxBitmap( nmr_xpm ), wxNullBitmap, wxITEM_RADIO, wxEmptyString, wxEmptyString );
	mRootToolbar->Realize();
	
	
	// Connect Events
	this->Connect( mMenuItemOptions->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( RootFrameBase::ShowOptions ) );
	this->Connect( mMenuItemSetupCalc->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( RootFrameBase::ShowCalc ) );
}

RootFrameBase::~RootFrameBase()
{
	// Disconnect Events
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( RootFrameBase::ShowOptions ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( RootFrameBase::ShowCalc ) );
}

optionFrameBase::optionFrameBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer4->AddGrowableCol( 0 );
	fgSizer4->AddGrowableRow( 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_listbook1 = new wxListbook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLB_DEFAULT );
	m_panel3 = new wxPanel( m_listbook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText5 = new wxStaticText( m_panel3, wxID_ANY, wxT("This is page 1"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	fgSizer1->Add( m_staticText5, 0, wxALL, 5 );
	
	m_panel3->SetSizer( fgSizer1 );
	m_panel3->Layout();
	fgSizer1->Fit( m_panel3 );
	m_listbook1->AddPage( m_panel3, wxT("a page"), true );
	m_panel4 = new wxPanel( m_listbook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText6 = new wxStaticText( m_panel4, wxID_ANY, wxT("This is page 2"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer2->Add( m_staticText6, 0, wxALL, 5 );
	
	m_panel4->SetSizer( fgSizer2 );
	m_panel4->Layout();
	fgSizer2->Fit( m_panel4 );
	m_listbook1->AddPage( m_panel4, wxT("a page"), false );
	m_panel5 = new wxPanel( m_listbook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText7 = new wxStaticText( m_panel5, wxID_ANY, wxT("This is page 3"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizer3->Add( m_staticText7, 0, wxALL, 5 );
	
	m_panel5->SetSizer( fgSizer3 );
	m_panel5->Layout();
	fgSizer3->Fit( m_panel5 );
	m_listbook1->AddPage( m_panel5, wxT("a page"), false );
	#ifndef __WXGTK__ // Small icon style not supported in GTK
	wxListView* m_listbook1ListView = m_listbook1->GetListView();
	long m_listbook1Flags = m_listbook1ListView->GetWindowStyleFlag();
	m_listbook1Flags = ( m_listbook1Flags & ~wxLC_ICON ) | wxLC_SMALL_ICON;
	m_listbook1ListView->SetWindowStyleFlag( m_listbook1Flags );
	#endif
	
	fgSizer4->Add( m_listbook1, 1, wxEXPAND, 0 );
	
	m_button3 = new wxButton( this, wxID_ANY, wxT("Okay"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( m_button3, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	this->SetSizer( fgSizer4 );
	this->Layout();
}

optionFrameBase::~optionFrameBase()
{
}

calcFrameBase::calcFrameBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	m_panel6 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer7;
	fgSizer7 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer7->SetFlexibleDirection( wxBOTH );
	fgSizer7->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText10 = new wxStaticText( m_panel6, wxID_ANY, wxT("Experiment:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	fgSizer7->Add( m_staticText10, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_comboBox3 = new wxComboBox( m_panel6, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
	m_comboBox3->Append( wxT("Pulse Aquire") );
	m_comboBox3->Append( wxT("COSY") );
	fgSizer7->Add( m_comboBox3, 0, wxALL, 5 );
	
	m_staticText8 = new wxStaticText( m_panel6, wxID_ANY, wxT("Magnetic Field:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	fgSizer7->Add( m_staticText8, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT|wxALL, 5 );
	
	m_comboBox2 = new wxComboBox( m_panel6, wxID_ANY, wxT("13.8"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	m_comboBox2->Append( wxT("13.8") );
	m_comboBox2->Append( wxT("12.9") );
	m_comboBox2->Append( wxT("3.34") );
	m_comboBox2->Append( wxT("Some other common NMR Field") );
	fgSizer7->Add( m_comboBox2, 0, wxALL, 5 );
	
	m_staticText9 = new wxStaticText( m_panel6, wxID_ANY, wxT("Maxium Coherence Order:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	fgSizer7->Add( m_staticText9, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT|wxALL, 5 );
	
	m_comboBox1 = new wxComboBox( m_panel6, wxID_ANY, wxT("2"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	m_comboBox1->Append( wxT("2") );
	m_comboBox1->Append( wxT("3") );
	m_comboBox1->Append( wxT("4") );
	m_comboBox1->Append( wxT("5") );
	m_comboBox1->Append( wxT("6") );
	fgSizer7->Add( m_comboBox1, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT|wxALL, 5 );
	
	m_panel6->SetSizer( fgSizer7 );
	m_panel6->Layout();
	fgSizer7->Fit( m_panel6 );
	bSizer6->Add( m_panel6, 1, wxEXPAND | wxALL, 0 );
	
	this->SetSizer( bSizer6 );
	this->Layout();
}

calcFrameBase::~calcFrameBase()
{
}

gridPanel::gridPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );
	
	spinGrid = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	
	// Grid
	spinGrid->CreateGrid( 1, 7 );
	spinGrid->EnableEditing( true );
	spinGrid->EnableGridLines( true );
	spinGrid->EnableDragGridSize( true );
	spinGrid->SetMargins( 0, 0 );
	
	// Columns
	spinGrid->SetColSize( 0, 73 );
	spinGrid->SetColSize( 1, 105 );
	spinGrid->SetColSize( 2, 70 );
	spinGrid->SetColSize( 3, 75 );
	spinGrid->SetColSize( 4, 50 );
	spinGrid->SetColSize( 5, 50 );
	spinGrid->SetColSize( 6, 50 );
	spinGrid->EnableDragColMove( false );
	spinGrid->EnableDragColSize( true );
	spinGrid->SetColLabelSize( 30 );
	spinGrid->SetColLabelValue( 0, wxT("Selected") );
	spinGrid->SetColLabelValue( 1, wxT("Spin Number") );
	spinGrid->SetColLabelValue( 2, wxT("Element") );
	spinGrid->SetColLabelValue( 3, wxT("Isotopes") );
	spinGrid->SetColLabelValue( 4, wxT("x") );
	spinGrid->SetColLabelValue( 5, wxT("y") );
	spinGrid->SetColLabelValue( 6, wxT("z") );
	spinGrid->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	spinGrid->SetRowSize( 0, 23 );
	spinGrid->EnableDragRowSize( true );
	spinGrid->SetRowLabelSize( 80 );
	spinGrid->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Label Appearance
	
	// Cell Defaults
	spinGrid->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	bSizer13->Add( spinGrid, 0, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( bSizer13 );
	this->Layout();
	bSizer13->Fit( this );
}

gridPanel::~gridPanel()
{
}

SpinInterEditPanelBase::SpinInterEditPanelBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer29;
	bSizer29 = new wxBoxSizer( wxVERTICAL );
	
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
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	mOrientTypeChoiceBook = new wxChoicebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCHB_DEFAULT );
	eulerEditPanel = new wxPanel( mOrientTypeChoiceBook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer19;
	fgSizer19 = new wxFlexGridSizer( 1, 4, 0, 0 );
	fgSizer19->SetFlexibleDirection( wxBOTH );
	fgSizer19->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxString mRConRadioBoxChoices[] = { wxT("Convention A"), wxT("Convention B") };
	int mRConRadioBoxNChoices = sizeof( mRConRadioBoxChoices ) / sizeof( wxString );
	mRConRadioBox = new wxRadioBox( eulerEditPanel, wxID_ANY, wxT("Convention"), wxDefaultPosition, wxDefaultSize, mRConRadioBoxNChoices, mRConRadioBoxChoices, 1, wxRA_SPECIFY_COLS );
	mRConRadioBox->SetSelection( 1 );
	fgSizer19->Add( mRConRadioBox, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer7;
	fgSizer7 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer7->SetFlexibleDirection( wxBOTH );
	fgSizer7->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText8 = new wxStaticText( eulerEditPanel, wxID_ANY, wxT("Alpha"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	fgSizer7->Add( m_staticText8, 0, wxALL, 5 );
	
	mAlphaCtrl = new wxTextCtrl( eulerEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	mAlphaCtrl->SetToolTip( wxT("alpha Euler angle (degrees)") );
	
	fgSizer7->Add( mAlphaCtrl, 0, wxALL, 5 );
	
	m_staticText9 = new wxStaticText( eulerEditPanel, wxID_ANY, wxT("Beta"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	fgSizer7->Add( m_staticText9, 0, wxALL, 5 );
	
	mBetaCtrl = new wxTextCtrl( eulerEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	mBetaCtrl->SetToolTip( wxT("beta Euler angle (degrees)") );
	
	fgSizer7->Add( mBetaCtrl, 0, wxALL, 5 );
	
	m_staticText10 = new wxStaticText( eulerEditPanel, wxID_ANY, wxT("Gamma"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	fgSizer7->Add( m_staticText10, 0, wxALL, 5 );
	
	mGammaCtrl = new wxTextCtrl( eulerEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	mGammaCtrl->SetToolTip( wxT("gamma Euler angle (degrees)") );
	
	fgSizer7->Add( mGammaCtrl, 0, wxALL, 5 );
	
	fgSizer19->Add( fgSizer7, 1, wxEXPAND, 5 );
	
	eulerEditPanel->SetSizer( fgSizer19 );
	eulerEditPanel->Layout();
	fgSizer19->Fit( eulerEditPanel );
	mOrientTypeChoiceBook->AddPage( eulerEditPanel, wxT("Euler Angles"), false );
	angleAxisEditPanel = new wxPanel( mOrientTypeChoiceBook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer29;
	fgSizer29 = new wxFlexGridSizer( 4, 2, 0, 0 );
	fgSizer29->SetFlexibleDirection( wxBOTH );
	fgSizer29->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText71 = new wxStaticText( angleAxisEditPanel, wxID_ANY, wxT("Angle"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText71->Wrap( -1 );
	fgSizer29->Add( m_staticText71, 0, wxALL, 5 );
	
	mAngleCtrl = new wxTextCtrl( angleAxisEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer29->Add( mAngleCtrl, 0, wxALL, 5 );
	
	m_staticText72 = new wxStaticText( angleAxisEditPanel, wxID_ANY, wxT("x:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText72->Wrap( -1 );
	fgSizer29->Add( m_staticText72, 0, wxALL, 5 );
	
	mXCtrl = new wxTextCtrl( angleAxisEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer29->Add( mXCtrl, 0, wxALL, 5 );
	
	m_staticText73 = new wxStaticText( angleAxisEditPanel, wxID_ANY, wxT("y:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText73->Wrap( -1 );
	fgSizer29->Add( m_staticText73, 0, wxALL, 5 );
	
	mYCtrl = new wxTextCtrl( angleAxisEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer29->Add( mYCtrl, 0, wxALL, 5 );
	
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
	
	m_staticText42 = new wxStaticText( eigensystemEditPanel, wxID_ANY, wxT("x Vector"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText42->Wrap( -1 );
	fgSizer21->Add( m_staticText42, 0, wxALL, 5 );
	
	mXXCtrl = new wxTextCtrl( eigensystemEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( mXXCtrl, 0, wxALL, 5 );
	
	mXYCtrl = new wxTextCtrl( eigensystemEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( mXYCtrl, 0, wxALL, 5 );
	
	mXZCtrl = new wxTextCtrl( eigensystemEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( mXZCtrl, 0, wxALL, 5 );
	
	m_staticText43 = new wxStaticText( eigensystemEditPanel, wxID_ANY, wxT("y Vector"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText43->Wrap( -1 );
	fgSizer21->Add( m_staticText43, 0, wxALL, 5 );
	
	mYXCtrl = new wxTextCtrl( eigensystemEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( mYXCtrl, 0, wxALL, 5 );
	
	mYYCtrl = new wxTextCtrl( eigensystemEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( mYYCtrl, 0, wxALL, 5 );
	
	mYZCtrl = new wxTextCtrl( eigensystemEditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( mYZCtrl, 0, wxALL, 5 );
	
	m_staticText44 = new wxStaticText( eigensystemEditPanel, wxID_ANY, wxT("z Vector"), wxDefaultPosition, wxDefaultSize, 0 );
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
	mOrientTypeChoiceBook->AddPage( eigensystemEditPanel, wxT("Eigensystem/Rotation Matrix"), false );
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
	
	wxString mFormBoxChoices[] = { wxT("Linear"), wxT("Bilinear"), wxT("Quadrapolar") };
	int mFormBoxNChoices = sizeof( mFormBoxChoices ) / sizeof( wxString );
	mFormBox = new wxRadioBox( this, wxID_ANY, wxT("Form"), wxDefaultPosition, wxDefaultSize, mFormBoxNChoices, mFormBoxChoices, 1, wxRA_SPECIFY_COLS );
	mFormBox->SetSelection( 2 );
	bSizer15->Add( mFormBox, 0, wxALL, 5 );
	
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

SpinDialogBase::SpinDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	mSpinDialogPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer31;
	sbSizer31 = new wxStaticBoxSizer( new wxStaticBox( mSpinDialogPanel, wxID_ANY, wxT("General Properties") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer191;
	fgSizer191 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer191->SetFlexibleDirection( wxBOTH );
	fgSizer191->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText35 = new wxStaticText( mSpinDialogPanel, wxID_ANY, wxT("Title:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText35->Wrap( -1 );
	fgSizer191->Add( m_staticText35, 0, wxALL, 5 );
	
	mSpinTitle = new wxTextCtrl( mSpinDialogPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer191->Add( mSpinTitle, 0, wxALL, 5 );
	
	sbSizer31->Add( fgSizer191, 1, wxEXPAND, 5 );
	
	bSizer10->Add( sbSizer31, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( mSpinDialogPanel, wxID_ANY, wxT("Chemical Properties") ), wxHORIZONTAL );
	
	wxFlexGridSizer* fgSizer16;
	fgSizer16 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer16->SetFlexibleDirection( wxBOTH );
	fgSizer16->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText29 = new wxStaticText( mSpinDialogPanel, wxID_ANY, wxT("x"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText29->Wrap( -1 );
	fgSizer16->Add( m_staticText29, 0, wxALL, 5 );
	
	mXPosCtrl = new wxTextCtrl( mSpinDialogPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer16->Add( mXPosCtrl, 0, wxALL, 5 );
	
	m_staticText30 = new wxStaticText( mSpinDialogPanel, wxID_ANY, wxT("y"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText30->Wrap( -1 );
	fgSizer16->Add( m_staticText30, 0, wxALL, 5 );
	
	mYPosCtrl = new wxTextCtrl( mSpinDialogPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer16->Add( mYPosCtrl, 0, wxALL, 5 );
	
	m_staticText31 = new wxStaticText( mSpinDialogPanel, wxID_ANY, wxT("z"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText31->Wrap( -1 );
	fgSizer16->Add( m_staticText31, 0, wxALL, 5 );
	
	mZPosCtrl = new wxTextCtrl( mSpinDialogPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer16->Add( mZPosCtrl, 0, wxALL, 5 );
	
	sbSizer2->Add( fgSizer16, 0, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer17;
	fgSizer17 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer17->SetFlexibleDirection( wxBOTH );
	fgSizer17->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText32 = new wxStaticText( mSpinDialogPanel, wxID_ANY, wxT("Element:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText32->Wrap( -1 );
	fgSizer17->Add( m_staticText32, 0, wxALL, 5 );
	
	wxArrayString mElementComboChoices;
	mElementCombo = new wxChoice( mSpinDialogPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, mElementComboChoices, 0 );
	mElementCombo->SetSelection( 0 );
	fgSizer17->Add( mElementCombo, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText33 = new wxStaticText( mSpinDialogPanel, wxID_ANY, wxT("Isotope:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText33->Wrap( -1 );
	fgSizer17->Add( m_staticText33, 0, wxALL, 5 );
	
	wxArrayString mIsotopeComboChoices;
	mIsotopeCombo = new wxChoice( mSpinDialogPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, mIsotopeComboChoices, 0 );
	mIsotopeCombo->SetSelection( 0 );
	fgSizer17->Add( mIsotopeCombo, 0, wxALL|wxEXPAND, 5 );
	
	sbSizer2->Add( fgSizer17, 1, wxEXPAND, 5 );
	
	bSizer10->Add( sbSizer2, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* interSizer;
	interSizer = new wxStaticBoxSizer( new wxStaticBox( mSpinDialogPanel, wxID_ANY, wxT("Interactions") ), wxVERTICAL );
	
	bSizer10->Add( interSizer, 1, wxEXPAND, 5 );
	
	okayCancel = new wxStdDialogButtonSizer();
	okayCancelOK = new wxButton( mSpinDialogPanel, wxID_OK );
	okayCancel->AddButton( okayCancelOK );
	okayCancelApply = new wxButton( mSpinDialogPanel, wxID_APPLY );
	okayCancel->AddButton( okayCancelApply );
	okayCancelCancel = new wxButton( mSpinDialogPanel, wxID_CANCEL );
	okayCancel->AddButton( okayCancelCancel );
	okayCancel->Realize();
	bSizer10->Add( okayCancel, 0, wxEXPAND, 5 );
	
	mSpinDialogPanel->SetSizer( bSizer10 );
	mSpinDialogPanel->Layout();
	bSizer10->Fit( mSpinDialogPanel );
	bSizer9->Add( mSpinDialogPanel, 1, wxEXPAND | wxALL, 0 );
	
	this->SetSizer( bSizer9 );
	this->Layout();
	bSizer9->Fit( this );
}

SpinDialogBase::~SpinDialogBase()
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
