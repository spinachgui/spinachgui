
#include <gui/TextBitmap.hpp>
#include <wx/dcmemory.h>
#include <3d/opengl.hpp>
#include <wx/image.h>

#include <iostream>

using namespace std;

bool isWhite(const unsigned char* p) {
    //cout << "c1= "<< long(p[0]) << " c2= "<< long(p[1]) << " c3= "<< long(p[2]) << endl;
    return p[0] == 0xff && p[1] == 0xff && p[2] == 0xff;
}

TextBitmap::TextBitmap(const wxString& text) 
    : mText(text) {
    wxMemoryDC textDC;

    wxSize size = textDC.GetTextExtent(mText);
    Create(size.GetWidth(),size.GetHeight(),1);

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
    long w = image.GetWidth();
    long h = image.GetHeight();

    for(long i=0;i<w*h*3;i++) {
	cout << long(data[i]) << " "; 
	if(i % 40 == 0) cout << endl;
    }

    //If w*h is a multipul of 8 the bits fit exeactly into w*h bytes
    //otherwise we need an extra byte.
    long bitmapLen = w*h / 8 + (w*h % 8 == 0 ? 0 : 1);
    mBitmap = new unsigned char[bitmapLen];
    for(long i = 0; i < bitmapLen; i++){
	//Summarize each RGB pixel into a single bit using the rule
	//that pure white is 0 otherwise 1
	long rgbPos = i*24;
	unsigned char byte = 0;
	if(!isWhite(data+rgbPos     )) byte |=0x01;
	if(!isWhite(data+rgbPos + 3 )) byte |=0x02;
	if(!isWhite(data+rgbPos + 6 )) byte |=0x04;
	if(!isWhite(data+rgbPos + 9 )) byte |=0x08;
	if(!isWhite(data+rgbPos + 12)) byte |=0x10;
	if(!isWhite(data+rgbPos + 15)) byte |=0x20;
	if(!isWhite(data+rgbPos + 18)) byte |=0x40;
	if(!isWhite(data+rgbPos + 21)) byte |=0x80;
	mBitmap[i] = byte;
    }
    //TODO Last 1-7 bits, if applicable
}

void TextBitmap::glStamp() {
    GLfloat xorig = 0.0;
    GLfloat yorig = 0.0;
    GLfloat xmove = 0.0;
    GLfloat ymove = 0.0;
    glBitmap(GetWidth(),GetHeight(),xorig,yorig,xmove,ymove,mBitmap);
}
