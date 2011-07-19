
#include <wx/app.h>
#include <gui/EasySpinFrame.hpp>
#include <shared/spinsys.hpp>

#include <shared/formats/g03.hpp>

using namespace SpinXML;

G03Loader g03Loader;

class EasySpinApp;
EasySpinApp* theApp = NULL;

SpinSystem spinsys;

class EasySpinApp : public wxApp {
public:
    virtual bool OnInit() {
        g03Loader.LoadFile(&spinsys,"examples/Gaussian/testing/methyl.log");

        theApp = this;
        EasySpinFrame* easySpinFrame = new EasySpinFrame(NULL);
        easySpinFrame->SetSpinsys(&spinsys);
        easySpinFrame->Show();
        return true;
    }
};



wxApp* wxGetApp() {
    return theApp;
}



#ifdef __LINUX__
int main(int argc,char** argv)
#else
    int WINAPI WinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPSTR lpCmdLine,
                       int nShowCmd)

#endif
{
    EasySpinApp* app = new EasySpinApp;
    wxApp::SetInstance(app);
#ifdef __LINUX__
    wxEntry(argc,argv);
#else
    wxEntry(hInstance,hPrevInstance,NULL,nShowCmd);
#endif
    return 0;
}
