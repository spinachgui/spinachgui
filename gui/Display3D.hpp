
#ifndef _3DDISPLAY_H
#define _3DDISPLAY_H

#include <wx/glcanvas.h>
#include <sigc++/sigc++.h>

#include <map>

class Renderer;
class Camera;
class GLMode;
class GLPicking;

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
    void OnLeftClick(wxMouseEvent& e);
    void OnResize(wxSizeEvent& e);

    void ResetView();

    void Set2DView();
    
    //To prevent flicker, overide this function to do nothing
    void OnEraseBackground(wxEraseEvent& e) {e.Skip(false);}

    void StartPicking();
    void StopPicking();
    Camera* GetCamera() {return mCamera;}

    //We only need this because binding Refresh directly in a signal
    //is difficult due to the default arguments
    void Redraw(){Refresh();}

    bool IsDragging(){return mDraging;}
protected:
    DECLARE_EVENT_TABLE();

    ///Call whenever the size of the viewport changes

    void EnableGL();
    void ChangeViewport();

    //Returns the name stack of the closest object moused over
    virtual void OnMouseOver3D(int stackLength,const GLuint* ClosestName) =0;
        
    virtual void DrawScene() = 0;
    virtual void DrawForeground() = 0;

private:


    GLPicking* mPicking;

    bool mGLEnabled;
    wxGLContext* mGLContext;

    //GUI State Variables
    double mHoverDist;
    int mWidth,mHeight;
    
    bool mBandBoxOn;
    int mBandBoxStartX;
    int mBandBoxStartY;

    bool mDraging;

    //3D Variables
    Camera* mCamera;
    Camera* m2DCamera;

    long mMouseX;
    long mMouseY;
};

#endif

