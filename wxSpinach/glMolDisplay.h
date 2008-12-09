

#include <wx/glcanvas.h>
#include <wx/timer.h>

class glMolDisplay : public wxGLCanvas {
public:
    glMolDisplay(wxWindow* parent);
    void glTick();
    void enableGL();

protected:
    void OnMouseMove(wxMouseEvent& e);
    void OnResize(wxSizeEvent& e);
    void DoReize();
    void OnWheel(wxMouseEvent& e);
    void OnTimer(wxTimerEvent& e);
    DECLARE_EVENT_TABLE()

    GLuint list;
    long mousex;
    long mousey;

  //A place to store the window size between OnResize and the
  //window being redrawn.
    int width;
    int height;

  //The current zoom level, multiplys the screen width and screen height in glOrth
    double mZoom;

    wxTimer mTimer;

};


