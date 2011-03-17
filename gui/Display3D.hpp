
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


///A class for storing all drawing options
class SpinachDC : public sigc::trackable {
public:
    SpinachDC();
    ~SpinachDC();

    GLUquadric* GetSolidQuadric() const {return mSolidQuadric;}
    GLUquadric* GetWireQuadric() const {return mWireQuadric;}

    void SetScalling(double scalling,SpinXML::Interaction::Type t) {
        mScallings[t]=scalling;
    }
    void SetColour(float r,float g,float b,SpinXML::Interaction::Type t) {
        mInterColours[t]=Vector3d(r,g,b);
    }
    void SetVisible(bool v,SpinXML::Interaction::Type t) {
        mVisible[t]=v;
    }

    ///If true draw only to the depth buffer
    bool depthOnly;

    enum PASS {
        SOLID,
        TRANSLUCENT,
        PICKING
    };
    PASS pass;

    void SetShowBonds(bool showBonds) {drawBonds=showBonds;}
    bool drawBonds;


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

class SGNode : public sigc::trackable {
public:
    ///Construct a dirty SGNode
    SGNode();

    ///Destruct the SGNode
    virtual ~SGNode();

    ///Mark this node as dirty, that is, needing to drewdraw its display
    ///list.
    void Dirty() {mDirty=true;sigDirty();}

    ///Called when a parent becomes dirty. Pass the signal up to the top
    ///node to trigger a redraw
    void OnChildDirty() {mDirty=true;sigDirty();}

    ///Draw, using RawDraw if needed or by calling the display list if
    ///one exists.
    void Draw(SpinachDC& dc);

    ///Add a node to the scenegraph. To remove it, just make sure its
    ///destructor is called.
    void AddNode(SGNode* node);

    ///Set the translucency of the node
    void SetTranslucency(float level,bool use=true) {
        mTranslucent=use;
        mTranslucentLevel=level;
        Dirty();
    }

    ///
    void SetMaterial(const float material[4],bool use=true);

    ///Removes a child node
    void RemoveNode(SGNode* node);

    void SetTranslation(const Vector3d& v);
    void SetIdentity() {mIdentity=true;}

    void GetPovRayString(wxString& str);

    sigc::signal<void,SGNode*> sigDying;
    sigc::signal<void> sigDirty; //Signals that a redraw is needed
private:
    ///Make whatever openGL calls are needed to draw the node.
    virtual void RawDraw(SpinachDC& dc)=0;

    virtual void ToPovRay(wxString& src)=0;

    bool mTranslucent;
    float mTranslucentLevel;

    bool mDirty;

    bool mUseMaterial;
    const float* mMaterial;

    ///If true, skip the transform step
protected:
    bool mIdentity;
    GLfloat mat[16];

    GLint mPickingName;

private:
    std::list<SGNode*> mChildren;
    typedef std::list<SGNode*>::iterator itor;
};


class OpenGLText : public SGNode {
public:
    OpenGLText(const wxString& str)  {UpdateString(str);}
    void SetStr(const wxString& str) {UpdateString(str);}

private:
    virtual void RawDraw(SpinachDC& dc);
    virtual void ToPovRay(wxString& src) {}
    void UpdateString(const wxString& str);
    GLuint texName;
    int w,h;
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

    void SetRootSGNode(SGNode* node) {
        if(mRootNode) delete mRootNode; mRootNode=node;
        mRootNode->sigDirty.connect(mem_fun(this,&Display3D::OnDirty));
    }
    void SetRootFGNode(SGNode* node) {
        if(mForgroundNode) delete mForgroundNode; mForgroundNode=node;
        mForgroundNode->sigDirty.connect(mem_fun(this,&Display3D::OnDirty));
    }
    void OnDirty() {Refresh();}
    SpinachDC& GetDC() {return mDC;}

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
    SGNode* mRootNode;
    //This nodes stay fixed on the screne
    SGNode* mForgroundNode;

    SpinachDC mDC;

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

