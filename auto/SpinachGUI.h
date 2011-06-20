///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov 18 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __SpinachGUI__
#define __SpinachGUI__

class SpinChoice;

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
#include <wx/toolbar.h>
#include <wx/frame.h>
#include <wx/choice.h>
#include <wx/listbox.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/choicebk.h>
#include <wx/dialog.h>
#include <gui/LogSlider.hpp>
#include <wx/checkbox.h>
#include <wx/clrpicker.h>

///////////////////////////////////////////////////////////////////////////

#define ID_NEW 1000
#define ID_OPEN 1001
#define ID_SAVE 1002
#define ID_SAVEAS 1003
#define ID_EASYSPIN_EXPORT 1004
#define ID_EXIT 1005
#define ID_EDIT_ISO 1006
#define ID_NMR_EPR 1007
#define ID_EPR 1008
#define ID_NMR 1009
#define ID_BOND_TOGGLE 1010
#define ID_VIEW_GRID 1011
#define ID_VIEW_TENSORVIS 1012
#define ID_VIEW_FRAMES 1013
#define ID_SUPRESS_SELECTION 1014
#define ID_UNSUPRESS 1015
#define ID_ELLIPSOIDS 1016
#define ID_AXES 1017
#define INTER_ADD 1018
#define INTER_DELETE 1019

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
		wxMenu* mMenuView;
		wxMenuItem* mMenuItemNmrEpr;
		wxMenuItem* mMenuItemEpr;
		wxMenuItem* mMenuItemNmr;
		wxMenuItem* mMenuItemBondToggle;
		wxMenuItem* mMenuItemGrid;
		wxMenuItem* mMenuItemTensorVis;
		wxMenuItem* mMenuItemIntEdit;
		wxMenuItem* mMenuItemFrames;
		wxMenu* mMenuUnits;
		wxMenu* mMenuLength;
		wxMenu* mMenuEnergy;
		wxMenu* mMenuSelection;
		wxMenu* mMenuHelp;
		wxToolBar* mRootToolbar;
	
	public:
		
		RootFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Spinach"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1000,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~RootFrameBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class SpinInterEditPanelBase
///////////////////////////////////////////////////////////////////////////////
class SpinInterEditPanelBase : public wxPanel 
{
	private:
	
	protected:
		SpinChoice* mSpinChoice;
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
		wxStaticText* m_staticText8;
		wxTextCtrl* mAlphaCtrl;
		wxStaticText* m_staticText411;
		wxStaticText* m_staticText9;
		wxTextCtrl* mBetaCtrl;
		wxStaticText* m_staticText421;
		wxStaticText* m_staticText10;
		wxTextCtrl* mGammaCtrl;
		wxStaticText* m_staticText431;
		wxPanel* angleAxisEditPanel;
		wxStaticText* m_staticText71;
		wxTextCtrl* mAngleCtrl;
		wxStaticText* m_staticText441;
		wxStaticText* m_staticText72;
		wxTextCtrl* mXCtrl;
		wxStaticText* m_staticText45;
		wxStaticText* m_staticText73;
		wxTextCtrl* mYCtrl;
		wxStaticText* m_staticText46;
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

///////////////////////////////////////////////////////////////////////////////
/// Class InterDisplaySettingsBase
///////////////////////////////////////////////////////////////////////////////
class InterDisplaySettingsBase : public wxPanel 
{
	private:
	
	protected:
		LogSlider* mLogSlider;
		wxCheckBox* mCheckbox;
		wxColourPickerCtrl* mColour;
	
	public:
		
		InterDisplaySettingsBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 260,-1 ), long style = wxTAB_TRAVERSAL ); 
		~InterDisplaySettingsBase();
	
};

#endif //__SpinachGUI__
