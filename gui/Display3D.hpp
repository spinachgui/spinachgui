
#ifndef _3DDISPLAY_H
#define _3DDISPLAY_H

#include <shared/spinsys.hpp>

#include <wx/glcanvas.h>
#include <sigc++/sigc++.h>

#include <map>

#include <3d/displaySettings.hpp>
#include <3d/glgeometry.hpp>


class Renderer;
class Camera;
class GLMode;
class GLPicking;

enum LAYER {
	LAYER_DEFAULT = 0,
    LAYER_AXIS,
    LAYER_SPINS,
    LAYER_BONDS,
    LAYER_INTERACTIONS,
};



///Manages interaction with the rest of the GUI including keeping the
///athorattive copy of DisplaySettings, managing the context, managing
///windowing events, setting the viewport etc. Only this class should
///depend on wxWidgets
class Display3D :  public wxGLCanvas, public sigc::trackable {
public:
    Display3D(wxWindow* parent);
    virtual ~Display3D();

    void OnPaint(wxPaintEvent& e);
    void OnMouseMove(wxMouseEvent& e);
    void OnWheel(wxMouseEvent& e);
    void OnRightClick(wxMouseEvent& e);
    void OnLeftClick(wxMouseEvent& e);
    void OnResize(wxSizeEvent& e);
    void OnDeleteSpinHover(wxCommandEvent& e);

    void ResetView();
    
    //To prevent flicker, overide this function to do nothing
    void OnEraseBackground(wxEraseEvent& e) {e.Skip(false);}

protected:
    DECLARE_EVENT_TABLE();

    ///Call whenever the size of the viewport changes

    void EnableGL();
    void ChangeViewport();
	void ProcessHits();
	
	//We only need this because binding Refresh directly in a signal
	//is difficult due to the default arguments
	void Redraw(){Refresh();}
private:

    //These nodes can be rotated and translated  with the mouse
    SpinSysScene     mMolScene;
    InteractionScene mInteractionScene;

	GLPicking* mPicking;

    bool mGLEnabled;
    wxGLContext* mGLContext;

    //GUI State Variables
    double mHoverDist;
    int mWidth,mHeight;

    //3D Variables
	Camera* mCamera;
    long mMouseX;
    long mMouseY;
};

#endif

