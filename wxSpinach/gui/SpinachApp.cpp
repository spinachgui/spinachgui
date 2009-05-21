/***************************************************************
 * Name:      SpinachApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Gareth Charnock (gareth.tpc@gmail.com)
 * Created:   2008-11-06
 * Copyright: Gareth Charnock ()
 * License:
 **************************************************************/

#include <shared/spin.h>

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include <gui/SpinachApp.h>
#include <gui/rootFrame.h>


IMPLEMENT_APP(SpinachApp);

bool SpinachApp::OnInit()
{
    rootFrame* frame = new rootFrame(NULL);
//    frame->SetIcon(wxICON(aaaa)); // To Set App Icon
    frame->Show();
//    frame->enableGL();

    return true;
}
