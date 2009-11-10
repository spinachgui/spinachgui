
#ifndef GLDISPLAY_H
#define GLDISPLAY_H

#include <wx/glcanvas.h>
#include <shared/spinsys.hpp>
#include <boost/shared_ptr.hpp>

#include <gui/SpinSysManager.hpp>

#include <GL/glx.h>
#include <GL/glu.h>

#include <gui/Event.hpp>

using namespace boost;
using namespace SpinXML;

class glDisplay : public wxGLCanvas {
public:
  glDisplay(wxWindow* parent,wxWindowID id= -1);
  ~glDisplay();

  void OnPaint(wxPaintEvent& e);
  void OnMouseMove(wxMouseEvent& e);
  void OnWheel(wxMouseEvent& e);
  void OnRightClick(wxMouseEvent& e);
  void OnLeftClick(wxMouseEvent& e);
  void OnResize(wxSizeEvent& e);
  void OnDeleteSpinHover(wxCommandEvent& e);

  enum INTERACTION_MODE {
    NMR,
    EPR,
    NMR_EPR,
    NONE
  };

  void SetMode(INTERACTION_MODE m);

protected:

  void EnableGL();

  //Display lists
  void CreateBondList();
  
  //State Changing functions
  void ChangeViewport();
  void FindHover();

  //Drawing functions
  void Geometary(bool depthOnly=false);
  void DrawAtoms(bool depthOnly=false);
  void DrawBonds();
  void DrawSingleAtomInteraction(long atom,Interaction::Form form);
  void DrawBilinear();

  DECLARE_EVENT_TABLE();
private:
  INTERACTION_MODE mMode;

  bool mGLEnabled;
  wxGLContext* mGLContext;

  const SpinSysPtr* mSS;

  //Display Lists
  int mDLSphereWire;
  int mDLSphereSolid;
  int mDLBonds;

  //Textures
  GLuint mTexDepth;

  //GUI State Variables
  long mHover;
  double mHoverDist;
  long mMouseX,mMouseY;
  double mZoom;

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
