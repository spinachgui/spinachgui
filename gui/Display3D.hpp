
#ifndef _3DDISPLAY_H
#define _3DDISPLAY_H

#include <shared/spinsys.hpp>

#include <wx/glcanvas.h>
#include <boost/shared_ptr.hpp>

#ifdef __linux__
#include <GL/glx.h>
#endif

#include <GL/glu.h>
#include <sigc++/sigc++.h>

#include <list>
#include <map>

#include <gui/SpinSysManager.hpp>

///The characteristic length scale of the objects we are
///rendering. Affects the positioning of the camera
#define OPENGL_SCALE (1e-10)

enum LAYER {
	LAYER_DEFAULT = 0,
    LAYER_AXIS,
    LAYER_SPINS,
    LAYER_BONDS,
    LAYER_INTERACTIONS,
};

///A struct storing all 3D settings that are invariant over a single
///rendering pass, implying that this structure may be passed safely
///as a const reference. It would likely make sense to link settings
///here to a menu option of configuration file setting. Information
///such as the current render pass would not belong in this struct
struct DisplaySettings {

    void SetShowBonds(bool showBonds) {drawBonds=showBonds;}
    bool drawBonds;

    void SetScalling(double scalling,SpinXML::Interaction::Type t) {
        mScallings[t]=scalling;
    }
    void SetColour(float r,float g,float b,SpinXML::Interaction::Type t) {
        mInterColours[t]=Vector3d(r,g,b);
    }
    void SetVisible(bool v,SpinXML::Interaction::Type t) {
        mVisible[t]=v;
    }

    GLUquadric* mSolidQuadric;
    GLUquadric* mWireQuadric;
    int width,height;
    const float* mRotationMatrix;

    ///The size of the displayed interaction in units of energy/length,
    ///e.g. MHz/Nm
    std::map<SpinXML::Interaction::Type,double> mScallings;
    ///The colour of the displayed interactions
    std::map<SpinXML::Interaction::Type,Vector3d> mInterColours;
    ///The visibility of the displayed interactions
    std::map<SpinXML::Interaction::Type,bool> mVisible;
};


///A class for storing all drawing options
class SpinachDC : public sigc::trackable {
public:
    SpinachDC();
    ~SpinachDC();

    ///If true draw only to the depth buffer
    bool depthOnly;

    enum PASS {
        SOLID,
        TRANSLUCENT,
        PICKING
    };
    PASS pass;
};

class Renderer : public sigc::trackable {
public:
    ///Construct a dirty Renderer
    Renderer();

    ///Destruct the Renderer
    virtual ~Renderer();

    ///Mark this node as dirty, that is, needing to drewdraw its display
    ///list.
    void Dirty() {mDirty=true;sigDirty();}

    ///Draw, using RawDraw if needed or by calling the display list if
    ///one exists.
    void Draw(const DisplaySettings& settings,SpinachDC& dc);

    ///Set the translucency of the node
    void SetTranslucency(float level,bool use=true) {
        mTranslucent=use;
        mTranslucentLevel=level;
        Dirty();
    }
    ///
    void SetMaterial(const float material[4],bool use=true);

    sigc::signal<void,Renderer*> sigDying;
    sigc::signal<void> sigDirty; //Signals that a redraw is needed
private:
    ///Make whatever openGL calls are needed to draw the node.
    virtual void RawDraw(SpinachDC& dc)=0;

    bool mTranslucent;
    float mTranslucentLevel;

    bool mDirty;

    bool mUseMaterial;
    const float* mMaterial;

    ///If true, skip the transform step
protected:
    GLint mPickingName;

private:
};

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

    void SetRootRenderer(Renderer* node) {
        if(mRootNode) delete mRootNode; mRootNode=node;
        mRootNode->sigDirty.connect(mem_fun(this,&Display3D::OnDirty));
    }
    void SetRootFGNode(Renderer* node) {
        if(mForgroundNode) delete mForgroundNode; mForgroundNode=node;
        mForgroundNode->sigDirty.connect(mem_fun(this,&Display3D::OnDirty));
    }
    void OnDirty() {Refresh();}
    SpinachDC& GetDC() {return mDC;}
	DisplaySettings* GetDisplaySettings() {return &mDisplaySettings;}

    void PrintTransformMatricese();
protected:
    DECLARE_EVENT_TABLE();

    ///Call whenever the size of the viewport changes


    void EnableGL();
    void ChangeViewport();

	// Do an openGL picking render and record the objects found under the mouse.
	void DoPickingPass();
private:

    //These nodes can be rotated and translated  with the mouse
    Renderer* mRootNode;
    //This nodes stay fixed on the screne
    Renderer* mForgroundNode;

    SpinachDC mDC;
	DisplaySettings mDisplaySettings;

    bool mGLEnabled;
    wxGLContext* mGLContext;

    SpinXML::SpinSystem* mSS;

    //Textures
    GLuint mTexDepth;

    //Frame buffer objects
    GLuint mFB;

    //GUI State Variables
    double mHoverDist;
    long mMouseX,mMouseY;
    double mZoom;
    int mWidth,mHeight;

    //3D Variables
    double mCamX,mCamY,mCamZ;
    float mRotationMatrix[16];
    float mXTranslate,mYTranslate;
    float mXRotate,mYRotate;
};

#endif

