
#include <wx/app.h>
#include <gui/EasySpin.hpp>

class EasySpinApp;
EasySpinApp* theApp = NULL;

class EasySpinApp : public wxApp {
public:
    virtual bool OnInit() {
        theApp = this;
        EasySpinFrame* easySpinFrame = new EasySpinFrame(NULL);
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
