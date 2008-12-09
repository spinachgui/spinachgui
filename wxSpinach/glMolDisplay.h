

#include <wx/glcanvas.h>


class glMolDisplay : public wxGLCanvas {
public:
    glMolDisplay(wxWindow* parent);
    void glTick();
    void enableGL();

protected:
    void OnMouseMove(wxMouseEvent& e);
    void OnResize(wxSizeEvent& e);
    DECLARE_EVENT_TABLE()

    GLuint list;
    long mousex;
    long mousey;

  //A place to store the window size between OnResize and the
  //window being redrawn.
    int width;
    int height;
};


