
#ifndef _MOL_SCENE_GRAPH_H_
#define _MOL_SCENE_GRAPH_H_

#include <sigc++/sigc++.h>
#include <gui/Display3D.hpp>


///Scenegraph node that draws a spin
class SpinNode : public SGNode {
public:
  SpinNode(Spin* spin);
  void OnSpinDying(Spin*) {delete this;} //Arguments are usused
private:
  SpinNode();

  Spin* mSpin;
  virtual void RawDraw(const SpinachDC& dc);
};

///Scene graph node that draws a cylinder between two points
class CyclinderNode : public SGNode {
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
  virtual void RawDraw(const SpinachDC& dc);
  float mRadius;
  Vector3 mR1;
  Vector3 mR2;
};

class EllipsoidNode : public SGNode {
  ///Create a node that draws an wireframe unit sphere at the origin
  EllipsoidNode();
  ///Create a node that draws an wireframe ellipsoid based on matrix A
  EllipsoidNode(const Matrix3& mat);
  ///Change the matrix transform performed
  void SetMatrix(const Matrix3& mat);
private:
  virtual void RawDraw(const SpinachDC& dc);
  ///Unpack a 3x3 matrix into a 4x4 opengl matrix
  void Unpack(const Matrix3& mat);
  float mat[16];
};

class MoleculeNode : public SGNode, public sigc::trackable {
public:
  MoleculeNode(SpinSystem* ss);

  void OnNewSpin(SpinXML::Spin* newSpin,long number);

private:
  virtual void RawDraw(const SpinachDC& dc);
  SpinSystem* mSS;
};


#endif
