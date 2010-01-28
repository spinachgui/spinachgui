
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

using namespace boost;
using namespace SpinXML;



///A class for storing all drawing options
class SpinachDC {
public:
  GLUquadric* GetSolidQuadric() const {return mSolidQuadric;}
  GLUquadric* GetWireQuadric() const {return mWireQuadric;}
  ///If true draw only to the depth buffer
  bool depthOnly;
  GLUquadric* mSolidQuadric;
  GLUquadric* mWireQuadric;
  SpinachDC()
    : depthOnly(false),
      mSolidQuadric(gluNewQuadric()),
      mWireQuadric(gluNewQuadric()) {
    gluQuadricDrawStyle(mSolidQuadric,GLU_FILL);
    gluQuadricNormals  (mSolidQuadric,GLU_SMOOTH);
	
    gluQuadricDrawStyle(mWireQuadric,GLU_LINE);
    gluQuadricNormals  (mWireQuadric,GLU_SMOOTH);
  }
  int width,height;
  const float* mRotationMatrix;
};

class SGNode : public sigc::trackable {
public:
  ///Construct a dirty SGNode
  SGNode();

  ///Destruct the SGNode
  ~SGNode();

  ///Mark this node as dirty, that is, needing to drewdraw its display
  ///list.
  void Dirty() {mDirty=true;}

  ///Draw, using RawDraw if needed or by calling the display list if
  ///one exists.
  void Draw(const SpinachDC& dc);

  ///Add a node to the scenegraph. To remove it, just make sure its
  ///destructor is called.
  void AddNode(SGNode* node);

  ///
  void SetMaterial(const float material[3],bool use=true);

  ///Removes a child node
  void RemoveNode(SGNode* node);

  void SetTranslation(const Vector3& v);
  void SetIdentity() {mIdentity=true;}

  sigc::signal<void,SGNode*> sigDying;
private:
  ///Make whatever openGL calls are needed to draw the node.
  virtual void RawDraw(const SpinachDC& dc)=0;

  bool mDirty;
  ///Stores an openGL display list for rendering the node
  int mList;
  ///Stores an openGL display list for rendering the node
  ///geomatary. Lighting and materials are not included
  int mGeomOnlyList;

  bool mUseMaterial;
  const float* mMaterial;
  
  ///If true, skip the transform step
  bool mIdentity;
  GLfloat mat[16];

  std::list<SGNode*> mChildren;
  typedef std::list<SGNode*>::iterator itor;
};


class OpenGLText : public SGNode {
public:
  OpenGLText(const wxString& str)  {UpdateString(str);}
  void SetStr(const wxString& str) {UpdateString(str);}
  
private:
  virtual void RawDraw(const SpinachDC& dc);
  void UpdateString(const wxString& str);
  GLuint texName;
  int w,h;
};


class Display3D :  public wxGLCanvas, sigc::trackable {
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

  void SetRootSGNode(SGNode* node) {
    if(mRootNode) delete mRootNode; mRootNode=node;
  }
  void SetRootFGNode(SGNode* node) {
    if(mForgroundNode) delete mForgroundNode; mForgroundNode=node;
  }
  SpinachDC& GetDC() {return mDC;}
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

  SpinachDC mDC;

  bool mGLEnabled;
  wxGLContext* mGLContext;

  SpinXML::SpinSystem* mSS;

  //Textures
  GLuint mTexDepth;

  //GUI State Variables
  long mHover;
  double mHoverDist;
  long mMouseX,mMouseY;
  double mZoom;
  int mWidth,mHeight;

  //Quadrics
  GLUquadric* mQFilled;
  GLUquadric* mQWireframe;

  //3D Variables
  double mCamX,mCamY,mCamZ;
  float mRotationMatrix[16];
  float mXTranslate,mYTranslate;
  float mXRotate,mYRotate;
};

#endif

