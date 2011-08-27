
#include <gui/TextBitmap.hpp>
#include <wx/dcmemory.h>
#include <3d/opengl.hpp>
#include <wx/image.h>

/*
On MSVC draw text is a macro that is defined as DrawTextW.
This causes conflicts with wxDC::DrawText, so undef it.
*/
#ifdef DrawText
#undef DrawText
#endif

using namespace std;

bool isWhite(const unsigned char* p) {
    return p[0] == 0xff && p[1] == 0xff && p[2] == 0xff;
}

TextBitmap::TextBitmap(const wxString& text) 
    : mText(text) {
    wxMemoryDC textDC;

    wxSize size = textDC.GetTextExtent(mText);
    long w = size.GetWidth() * 2; //The *2 is to fix an apparent bug
				  //where GetTextExtend doesn't seem
				  //to return a big enough value
    long h = size.GetHeight();
    //Make sure the width is a multipul of eight
    w = w + 8 - (w % 8);
    Create(w,h,1);

    textDC.SelectObject(*this);

    //White forground and black background
    textDC.SetBackground(*wxWHITE_BRUSH);
    textDC.SetBrush(*wxBLACK_BRUSH);

    textDC.Clear();
    textDC.DrawText(mText,0,0);

    this->SaveFile(wxT("text.xpm"),wxBITMAP_TYPE_XPM);

    wxImage image = ConvertToImage();
    //Data now contains an height*width*3 array of colours in RGB
    //format.
    const unsigned char* data = image.GetData();

    //If w*h is a multipul of 8 the bits fit exeactly into w*h bytes
    //otherwise we need an extra byte.
    long bitmapLen = w*h / 8;
    mBitmap = new unsigned char[bitmapLen];

    //Summarize each RGB pixel into a single bit using the rule
    //that pure white is 0 otherwise 1

    //The loop indexes represent the x-y coordinates of a pixel in the
    //wxImage
    for(long rgbRow = 0; rgbRow < h; rgbRow++){
        for(long rgbCol = 0; rgbCol < w; rgbCol+=8) {
            long rgbPos = 3*(w*rgbRow + rgbCol);

            unsigned char byte = 0;

            if(!isWhite(data+rgbPos     )) byte |=0x80;
            if(!isWhite(data+rgbPos + 3 )) byte |=0x40;
            if(!isWhite(data+rgbPos + 6 )) byte |=0x20;
            if(!isWhite(data+rgbPos + 9 )) byte |=0x10;
            if(!isWhite(data+rgbPos + 12)) byte |=0x08;
            if(!isWhite(data+rgbPos + 15)) byte |=0x04;
            if(!isWhite(data+rgbPos + 18)) byte |=0x02;
            if(!isWhite(data+rgbPos + 21)) byte |=0x01;
            mBitmap[(h - rgbRow - 1)*w/8 + rgbCol/8] = byte;
        }
    }
}

TextBitmap::~TextBitmap() {
    delete [] mBitmap;
}

void TextBitmap::glStamp() const {
    GLfloat xorig = 0.0;
    GLfloat yorig = 0.0;
    GLfloat xmove = 0.0;
    GLfloat ymove = 0.0;

    //Set the packing alignment. Here each row ends on a byte
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);

    glBitmap(GetWidth(),GetHeight(),xorig,yorig,xmove,ymove,mBitmap);
}
