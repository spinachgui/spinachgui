
#ifndef __TEXT_BITMAP__H__
#define __TEXT_BITMAP__H__

#include <wx/bitmap.h>

class TextBitmap : public wxBitmap {
public:
    TextBitmap(const wxString& text);

private:
    wxString mText;
};

#endif


