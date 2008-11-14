

#include <wx/glcanvas.h>


class glMolDisplay : public wxGLCanvas {
public:
    glMolDisplay(wxWindow* parent);
    void glTick();
    void enableGL();

protected:
    void OnMouseMove(wxMouseEvent& e);
    DECLARE_EVENT_TABLE()

    GLuint list;

};


