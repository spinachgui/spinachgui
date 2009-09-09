
#ifndef GLDISPLAY_H
#define GLDISPLAY_H

#include <wx/glcanvas.h>
#include <shared/spinsys.hpp>
#include <boost/shared_ptr.hpp>

#include <GL/glx.h>
#include <GL/glu.h>


using namespace boost;
using namespace SpinXML;

class glDisplay : public wxGLCanvas {
public:
  glDisplay(wxWindow* parent,wxWindowID id= -1);

  void OnPaint(wxPaintEvent& e);
  void OnMouseMove(wxMouseEvent& e);
  void OnWheel(wxMouseEvent& e);
  void OnRightClick(wxMouseEvent& e);
  void OnLeftClick(wxMouseEvent& e);
  void OnResize(wxSizeEvent& e);

  void OnDisplaySpinDialog(wxCommandEvent& e);

protected:
  void ChangeViewport();
  void EnableGL();
  void CreateBondList();
  void Geometary();

  DECLARE_EVENT_TABLE();

private:
  bool mGLEnabled;
  wxGLContext* mGLContext;

  shared_ptr<SpinSystem> mSS;

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

  //3D Variables
  double mCamX,mCamY,mCamZ;
  float mRotationMatrix[16];
  float mXTranslate,mYTranslate;
  float mXRotate,mYRotate;
};

#endif
