
#ifndef __ACTION__H__
#define __ACTION__H__

#include <wx/event.h>
#include <wx/bitmap.h>

struct Action {
	Action(const wxString& text,const wxString& tooltip,wxBitmap bitmap = wxNullBitmap);
	virtual void Exec(wxCommandEvent& e) = 0;
	virtual const wxString& GetTooltip();
	virtual const wxString& GetText();
	virtual wxBitmap GetBitmap();
	virtual wxItemKind GetItemKind();
	virtual bool Enabled();
private:
	wxString mText;
	wxString mTooltip;
	wxBitmap mBitmap;
};



#endif

