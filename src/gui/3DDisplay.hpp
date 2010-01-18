
#ifndef _3DDISPLAY_H
#define _3DDISPLAY_H

#include <wx/glcanvas.h>
#include <shared/spinsys.hpp>
#include <boost/shared_ptr.hpp>

#include <gui/SpinSysManager.hpp>

#include <GL/glx.h>
#include <GL/glu.h>

#include <gui/Event.hpp>

#include <list>

using namespace boost;
using namespace SpinXML;



///A class for storing all drawing options
class spinachDC {
public:
  ///If true draw only to the depth buffer
  bool depthOnly;
  spinachDC()
    : depthOnly(false) {
  }
};

class SGNode {
public:
  ///Construct a dirty SGNode
  SGNode();

  ///Attach a child node to the SGNode
  void Attach(SGNode* node);

  ///Detatch a given node, throw if node not found.
  void Detach(SGNode* node);

  ///Mark this node as dirty, that is, needing to drewdraw its display
  ///list.
  void Dirty();

  ///Draw, using RawDraw if needed or by calling the display list if
  ///one exists.
  void Draw(const spinachDC& dc);

private:
  ///Make whatever openGL calls are needed to draw the node.
  virtual void RawDraw(const spinachDC& dc)=0;

  bool mDirty;
  int mList;
  int mGeomOnlyList;

  std::list<SGNode*> mChildren;
};

///Works like an SGNode, but manages its material
class SGNodeWithMaterial : public SGNode {
public:
  void SetMaterial();
  ///Sets the material, then calls SGNode::Draw();
  void Draw(const spinachDC& dc);
private:
  float material[3];
};

///Scene graph node that draws a sphere at the origin
class SphereNode : public SGNodeWithMaterial {
  ///Create a node that draws a sphere of unit radius
  SphereNode() : mRadius(1.0f) {}
  ///Create a node that draws a sphere a given radius
  SphereNode(float radius) : mRadius(radius) {}
  ///Change the radius of the sphere
  void SetRadius(float radius){mRadius=radius; Dirty();}
private:
  virtual void RawDraw(const spinachDC& dc)=0;
  float mRadius;
};

///Scene graph node that draws a cylinder between two points
class CyclinderNode : public SGNodeWithMaterial {
  ///Create a node that draws a cyclinder of unit radius between
  ///(0,0,0) and (0,0,1)
  CyclinderNode()
    : mRadius(1.0f),mR1(Vector3(0,0,0)),mR2(Vector3(0,0,0)) {}
  ///Create a node that draws a cyclinder of a given radius between r1
  ///and r2
  CyclinderNode(Vector3& r1, Vector3& r2, float radius)
    : mRadius(mRadius),mR1(r1),mR2(r2) {}
  ///Set the paramiters of the cyclinder
  void SetCyclinder(Vector3& r1, Vector3& r2, float radius) {
    mRadius=radius; mR1=r1; mR2=r2; Dirty();
  }
private:
  virtual void RawDraw(const spinachDC& dc)=0;
  float mRadius;
  Vector3 mR1;
  Vector3 mR2;
};

class EllipsoidNode : public SGNodeWithMaterial {
  ///Create a node that draws an wireframe unit sphere at the origin
  EllipsoidNode();
  ///Create a node that draws an wireframe ellipsoid based on matrix A
  EllipsoidNode(const Matrix3& mat);
  ///Change the matrix transform performed
  void SetMatrix(const Matrix3& mat);
private:
  virtual void RawDraw(const spinachDC& dc);
  ///Unpack a 3x3 matrix into a 4x4 opengl matrix
  void Unpack(const Matrix3& mat);
  float mMat[16];
};

class MoleculeNode : public SGNode {
  MoleculeNode(SpinSystem* ss) : mSS(ss) {}
private:
  virtual void RawDraw(const spinachDC& dc);
  SpinSystem* mSS;
};


class Display3D :  public wxGLCanvas, public IEventListener {
public:
private:
  SGNode* mRootNode;
};

#endif

