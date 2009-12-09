///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 29 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __SpinachGUI__
#define __SpinachGUI__

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/statusbr.h>
#include <wx/toolbar.h>
#include <wx/frame.h>
#include <wx/stattext.h>
#include <wx/listbook.h>
#include <wx/listctrl.h>
#include <wx/button.h>
#include <wx/combobox.h>
#include <wx/grid.h>
#include <wx/listbox.h>
#include <wx/radiobox.h>
#include <wx/textctrl.h>
#include <wx/choicebk.h>
#include <wx/choice.h>
#include <wx/statbox.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

#define ID_NEW 1000
#define ID_OPEN 1001
#define ID_SAVE 1002
#define ID_SAVEAS 1003
#define ID_IMPORT 1004
#define ID_OPTIONS 1005
#define ID_EXIT 1006
#define ID_UNDO 1007
#define ID_REDO 1008
#define ID_NMR_EPR 1009
#define ID_EPR 1010
#define ID_NMR 1011
#define INTER_ADD 1012
#define INTER_DELETE 1013

///////////////////////////////////////////////////////////////////////////////
/// Class RootFrameBase
///////////////////////////////////////////////////////////////////////////////
class RootFrameBase : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* mRootMenu;
		wxMenu* mMenuFile;
		wxMenu* mMenuEdit;
		wxMenuItem* mMenuItemUndo;
		wxMenuItem* mMenuItemRedo;
		wxMenu* mMenuCalculate;
		wxMenu* mMenuView;
		wxMenuItem* mMenuItemNmrEpr;
		wxMenuItem* mMenuItemEpr;
		wxMenuItem* mMenuItemNmr;
		wxMenu* mMenuMode;
		wxMenu* mMenuHelp;
		wxPanel* mAuiPanel;
		wxStatusBar* mRootStatusBar;
		wxToolBar* mRootToolbar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void ShowOptions( wxCommandEvent& event ){ event.Skip(); }
		virtual void ShowCalc( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		RootFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Spinach"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1000,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~RootFrameBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class optionFrameBase
///////////////////////////////////////////////////////////////////////////////
class optionFrameBase : public wxFrame 
{
	private:
	
	protected:
		wxListbook* m_listbook1;
		wxPanel* m_panel3;
		wxStaticText* m_staticText5;
		wxPanel* m_panel4;
		wxStaticText* m_staticText6;
		wxPanel* m_panel5;
		wxStaticText* m_staticText7;
		wxButton* m_button3;
	
	public:
		optionFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Options"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~optionFrameBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class calcFrameBase
///////////////////////////////////////////////////////////////////////////////
class calcFrameBase : public wxFrame 
{
	private:
	
	protected:
		wxPanel* m_panel6;
		wxStaticText* m_staticText10;
		wxComboBox* m_comboBox3;
		wxStaticText* m_staticText8;
		wxComboBox* m_comboBox2;
		wxStaticText* m_staticText9;
		wxComboBox* m_comboBox1;
	
	public:
		calcFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~calcFrameBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class gridPanel
///////////////////////////////////////////////////////////////////////////////
class gridPanel : public wxPanel 
{
	private:
	
	protected:
		wxGrid* spinGrid;
	
	public:
		gridPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL );
		~gridPanel();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class SpinInterEditPanelBase
///////////////////////////////////////////////////////////////////////////////
class SpinInterEditPanelBase : public wxPanel 
{
	private:
	
	protected:
		wxListBox* mInterListBox;
		wxButton* addButton;
		wxButton* deleteButton;
	
	public:
		SpinInterEditPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL );
		~SpinInterEditPanelBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class OrientEditPanelBase
///////////////////////////////////////////////////////////////////////////////
class OrientEditPanelBase : public wxPanel 
{
	private:
	
	protected:
		wxChoicebook* mOrientTypeChoiceBook;
		wxPanel* eulerEditPanel;
		wxRadioBox* mRConRadioBox;
		wxStaticText* m_staticText8;
		wxTextCtrl* mAlphaCtrl;
		wxStaticText* m_staticText9;
		wxTextCtrl* mBetaCtrl;
		wxStaticText* m_staticText10;
		wxTextCtrl* mGammaCtrl;
		wxPanel* angleAxisEditPanel;
		wxStaticText* m_staticText71;
		wxTextCtrl* mAngleCtrl;
		wxStaticText* m_staticText72;
		wxTextCtrl* mXCtrl;
		wxStaticText* m_staticText73;
		wxTextCtrl* mYCtrl;
		wxStaticText* m_staticText75;
		wxTextCtrl* mZCtrl;
		wxPanel* quaternionEditPanel;
		wxStaticText* m_staticText711;
		wxTextCtrl* mReCtrl;
		wxStaticText* m_staticText721;
		wxTextCtrl* mICtrl;
		wxStaticText* m_staticText731;
		wxTextCtrl* mJCtrl;
		wxStaticText* m_staticText751;
		wxTextCtrl* mKCtrl;
		wxPanel* eigensystemEditPanel;
		wxStaticText* m_staticText38;
		wxStaticText* m_staticText39;
		wxStaticText* m_staticText40;
		wxStaticText* m_staticText41;
		wxStaticText* m_staticText42;
		wxTextCtrl* mXXCtrl;
		wxTextCtrl* mXYCtrl;
		wxTextCtrl* mXZCtrl;
		wxStaticText* m_staticText43;
		wxTextCtrl* mYXCtrl;
		wxTextCtrl* mYYCtrl;
		wxTextCtrl* mYZCtrl;
		wxStaticText* m_staticText44;
		wxTextCtrl* mZXCtrl;
		wxTextCtrl* mZYCtrl;
		wxTextCtrl* mZZCtrl;
	
	public:
		OrientEditPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL );
		~OrientEditPanelBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class InterEditPanelBase
///////////////////////////////////////////////////////////////////////////////
class InterEditPanelBase : public wxPanel 
{
	private:
	
	protected:
		wxRadioBox* mFormBox;
		wxStaticText* m_staticText51;
		wxChoice* mSubTypeCombo;
		wxStaticText* mcoupspin_static_text;
		wxChoice* mSpin2Combo;
		wxChoicebook* mTypeChoiceBook;
		wxPanel* mScalarEditPanel;
		wxStaticText* m_staticText36;
		wxTextCtrl* mScalarCtrl;
		wxPanel* mMatrixEditPanel;
		wxStaticText* m_staticText38;
		wxStaticText* m_staticText39;
		wxStaticText* m_staticText40;
		wxStaticText* m_staticText41;
		wxStaticText* m_staticText42;
		wxTextCtrl* mMatXXCtrl;
		wxTextCtrl* mMatYXCtrl;
		wxTextCtrl* mMatZXCtrl;
		wxStaticText* m_staticText43;
		wxTextCtrl* mMatXYCtrl;
		wxTextCtrl* mMatYYCtrl;
		wxTextCtrl* mMatZYCtrl;
		wxStaticText* m_staticText44;
		wxTextCtrl* mMatXZCtrl;
		wxTextCtrl* mMatYZCtrl;
		wxTextCtrl* mMatZZCtrl;
		wxPanel* mEigenEditPanel;
		wxStaticText* m_staticText71;
		wxTextCtrl* mEigenXXCtrl;
		wxStaticText* m_staticText72;
		wxTextCtrl* mEigenYYCtrl;
		wxStaticText* m_staticText73;
		wxTextCtrl* mEigenZZCtrl;
		wxStaticText* m_staticText75;
		wxPanel* mAxRhomEditPanel;
		wxStaticText* m_staticText711;
		wxTextCtrl* mAxCtrl;
		wxStaticText* m_staticText721;
		wxTextCtrl* mRhomCtrl;
		wxStaticText* m_staticText731;
		wxTextCtrl* mAxRhomIsoCtrl;
		wxStaticText* m_staticText751;
		wxPanel* mSpanSkewEditPanel;
		wxStaticText* m_staticText712;
		wxTextCtrl* mSpanCtrl;
		wxStaticText* m_staticText722;
		wxTextCtrl* mSkewCtrl;
		wxStaticText* m_staticText732;
		wxTextCtrl* mSpanSkewIsoCtrl;
		wxStaticText* m_staticText752;
	
	public:
		InterEditPanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL );
		~InterEditPanelBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class SpinDialogBase
///////////////////////////////////////////////////////////////////////////////
class SpinDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxPanel* mSpinDialogPanel;
		wxStaticText* m_staticText35;
		wxTextCtrl* mSpinTitle;
		wxStaticText* m_staticText29;
		wxTextCtrl* mXPosCtrl;
		wxStaticText* m_staticText30;
		wxTextCtrl* mYPosCtrl;
		wxStaticText* m_staticText31;
		wxTextCtrl* mZPosCtrl;
		wxStaticText* m_staticText32;
		wxChoice* mElementCombo;
		wxStaticText* m_staticText33;
		wxChoice* mIsotopeCombo;
		wxStdDialogButtonSizer* okayCancel;
		wxButton* okayCancelOK;
		wxButton* okayCancelApply;
		wxButton* okayCancelCancel;
	
	public:
		SpinDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~SpinDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class OrientDialogBase
///////////////////////////////////////////////////////////////////////////////
class OrientDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxPanel* mOrientDialogPanel;
		wxStdDialogButtonSizer* okayCancel;
		wxButton* okayCancelOK;
		wxButton* okayCancelApply;
		wxButton* okayCancelCancel;
	
	public:
		OrientDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~OrientDialogBase();
	
};

#endif //__SpinachGUI__
