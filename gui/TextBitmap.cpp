
#include <gui/TextBitmap.hpp>
#include <wx/dcmemory.h>

TextBitmap::TextBitmap(const wxString& text) 
    : mText(text) {
    wxMemoryDC textDC;

    wxSize size = textDC.GetTextExtent(mText);
    Create(size.GetWidth(),size.GetHeight(),-1);

    textDC.SelectObject(*this);
    textDC.Clear();
    textDC.DrawText(mText,0,0);
    SaveFile(wxT("test.png"),wxBITMAP_TYPE_PNG);
}
