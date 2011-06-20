///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov 18 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __orientdialog__
#define __orientdialog__

class TextCtrlFocus;

#include <gui/OrientDisplay.hpp>
#include <wx/string.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/radiobut.h>
#include <wx/button.h>
#include <wx/statline.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

#define ID_EULER 1000
#define ID_MATRIX 1001
#define ID_ANGLEAXIS 1002
#define ID_QUAT 1003
#define ID_IDENTITY 1004

///////////////////////////////////////////////////////////////////////////////
/// Class OrientDialog2Base
///////////////////////////////////////////////////////////////////////////////
class OrientDialog2Base : public wxDialog 
{
	private:
	
	protected:
		OrientDisplay3D* mOrientDisplay3D;
		wxStaticText* m_staticText47;
		TextCtrlFocus* mAlpha;
		wxStaticText* m_staticText50;
		wxStaticText* m_staticText48;
		TextCtrlFocus* mBeta;
		wxStaticText* m_staticText501;
		wxStaticText* m_staticText49;
		TextCtrlFocus* mGamma;
		wxStaticText* m_staticText502;
		TextCtrlFocus* m00;
		TextCtrlFocus* m01;
		TextCtrlFocus* m02;
		TextCtrlFocus* m10;
		TextCtrlFocus* m11;
		TextCtrlFocus* m12;
		TextCtrlFocus* m20;
		TextCtrlFocus* m21;
		TextCtrlFocus* m22;
		wxStaticText* mTextDet;
		wxStaticText* m_staticText56;
		TextCtrlFocus* mAngle;
		wxStaticText* m_staticText503;
		wxStaticText* m_staticText58;
		TextCtrlFocus* mX;
		wxStaticText* m_staticText504;
		wxStaticText* m_staticText60;
		TextCtrlFocus* mY;
		wxStaticText* m_staticText505;
		wxStaticText* m_staticText62;
		TextCtrlFocus* mZ;
		wxStaticText* m_staticText63;
		wxStaticText* m_staticText561;
		TextCtrlFocus* mRe;
		wxStaticText* m_staticText581;
		TextCtrlFocus* mI;
		wxStaticText* m_staticText601;
		TextCtrlFocus* mJ;
		wxStaticText* m_staticText621;
		TextCtrlFocus* mK;
		wxStaticText* m_staticText631;
		wxRadioButton* m_radioBtn1;
		wxRadioButton* m_radioBtn2;
		wxButton* mIdentity;
		wxStaticLine* m_staticline1;
		wxStdDialogButtonSizer* m_sdbSizer2;
		wxButton* m_sdbSizer2OK;
		wxButton* m_sdbSizer2Cancel;
	
	public:
		
		OrientDialog2Base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Orientation"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~OrientDialog2Base();
	
};

#endif //__orientdialog__
