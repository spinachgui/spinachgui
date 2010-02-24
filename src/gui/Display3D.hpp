
#ifndef _3DDISPLAY_H
#define _3DDISPLAY_H

#include <wx/glcanvas.h>
#include <shared/spinsys.hpp>
#include <boost/shared_ptr.hpp>

#include <gui/SpinSysManager.hpp>

#include <GL/glx.h>
#include <GL/glu.h>
#include <sigc++/sigc++.h>

#include <list>
#include <map>

using namespace boost;
using namespace SpinXML;


///A class for storing all drawing options
class SpinachDC : public sigc::trackable {
public:
    SpinachDC();
    ~SpinachDC();

    GLUquadric* GetSolidQuadric() const {return mSolidQuadric;}
    GLUquadric* GetWireQuadric() const {return mWireQuadric;}

    void SetScalling(double scalling,Interaction::SubType st) {mScallings[st]=scalling;}

    ///If true draw only to the depth buffer
    bool depthOnly;

    ///If false then lighting will be off
    bool lighting;

    GLUquadric* mSolidQuadric;
    GLUquadric* mWireQuadric;
    int width,height;
    const float* mRotationMatrix;
  
    ///The size of the displayed interaction in units of energy/length,
    ///e.g. MHz/Nm
    std::map<SpinXML::Interaction::SubType,double> mScallings;

    bool GLSelectMode;
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

    ///
    void SetMaterial(const float material[3],bool use=true);

    ///Removes a child node
    void RemoveNode(SGNode* node);

    void SetTranslation(const Vector3& v);
    void SetIdentity() {mIdentity=true;}

    void GetPovRayString(wxString& str);

    sigc::signal<void,SGNode*> sigDying;
    sigc::signal<void> sigDirty; //Signals that a redraw is needed
private:
    ///Make whatever openGL calls are needed to draw the node.
    virtual void RawDraw(SpinachDC& dc)=0;

    virtual void ToPovRay(wxString& src)=0;

    bool mDirty;
    ///Stores an openGL display list for rendering the node

    bool mUseMaterial;
    const float* mMaterial;
  
    ///If true, skip the transform step
    bool mIdentity;
    GLfloat mat[16];

    std::list<SGNode*> mChildren;
    typedef std::list<SGNode*>::iterator itor;

    ///The globally unique name of this node
    GLuint mPickingName;

    void DoRender(SpinachDC& dc);
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

    ///Doesn't change any of the display settings, just executes the
    ///common code required to find the rotation.
    void DoRender(SpinachDC& dc);

    ///Does an openGL picking render to select an object. If an object
    ///is found this function sets its argument to not null.
    void DoPickingRender(SpinXML::Spin** spin);

    ///Renders the scene normally
    void DoNormalRender();

    ///A place to do various rendering add ons and experiments.
    void DoDebugRender();

    ///Instead of rendering to the screne, get a string suitable for
    ///use as a povray file
    wxString DoPovrayRender();

    ///As normal, but do not use fancy graphics.
    void LowGraphicsRender();

    ///Turn off lighting and such, render to the depth buffer
    void DepthOnlyMode();

protected:
    DECLARE_EVENT_TABLE();

    ///Call whenever the size of the viewport changes
  

    void EnableGL();
    void ChangeViewport();

private:

    //These nodes can be rotated and translated  with the mouse
    SGNode* mRootNode;
    //This nodes stay fixed on the screne
    SGNode* mForgroundNode;

    ///This object contains the rendering options used during normal
    ///rendering.
    SpinachDC mDC;

    bool mGLEnabled;
    wxGLContext* mGLContext;

    SpinXML::SpinSystem* mSS;

    //Textures
    GLuint mTexDepth;

    //Frame buffer objects
    GLuint mFB;

    //GUI State Variables
    long mHover;
    double mHoverDist;
    long mMouseX,mMouseY;
    double mZoom;
    int mWidth,mHeight;

    //3D Variables
    double mCamX,mCamY,mCamZ;
    float mRotationMatrix[16];
    Matrix4 mTransformMatrix;
    float mXTranslate,mYTranslate;
    float mXRotate,mYRotate;
};

#endif

