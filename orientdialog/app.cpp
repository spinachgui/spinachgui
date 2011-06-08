


#include <wx/app.h>
#include <gui/OrientDialog2.hpp>

class App;
App* theApp = NULL;

class App : public wxApp {
public:
    virtual bool OnInit() {
        theApp = this;
        OrientDialog2* frame = new OrientDialog2(NULL);
        frame->Show();
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
    App* app = new App;
    wxApp::SetInstance(app);
#ifdef __LINUX__
    wxEntry(argc,argv);
#else
    wxEntry(hInstance,hPrevInstance,NULL,nShowCmd);
#endif
    return 0;
}
