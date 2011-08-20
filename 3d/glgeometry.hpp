
#ifndef __GL_GEOM_HPP__
#define __GL_GEOM_HPP__

#include <3d/displaySettings.hpp>

#include <3d/glmode.hpp>
#include <3d/opengl.hpp>
#include <3d/camera.hpp>

#include <gui/TextBitmap.hpp>

enum GLName {
    NAME_NONE = 0,
    NAME_AXIS,
    NAME_SPINS,
    NAME_BONDS,
    NAME_MONO_INTERACTIONS,
    NAME_BINARY_INTERACTIONS,
    NAME_FG,
    NAME_FRAME
};

///An abstract class who's job is to visualise part of a spin system
///such as spins, linear interactions or bonds.
class Renderer {
public:
    Renderer(GLuint glName);
    virtual ~Renderer();
    void DrawWith(GLMode& mode) const;
    void Draw() const;
protected:
    virtual void Geometary() const = 0;
private:
    int mName;
};


class MoleculeFG : public Renderer {
public:
    MoleculeFG();
    void OnNewElectron(SpinXML::Spin* newSpin,long number);
protected:
    virtual void Geometary() const {}
};

//============================================================//

class SpinDrawer : public Renderer {
public:
    SpinDrawer();
protected:
    virtual void Geometary() const;
};


class ElectronScene : public Renderer {
public:
    ElectronScene(Camera* camera);
protected:
    virtual void Geometary() const;
    Camera* mCamera;
};


class ElectronInterDrawer : public Renderer {
public:
    ElectronInterDrawer(Camera* camera);
protected:
    virtual void Geometary() const;
    Camera* mCamera;
};

class BondDrawer : public Renderer {
public:
    BondDrawer();
protected:
    virtual void Geometary() const;
};

class MonoInterDrawer : public Renderer {
public:
    MonoInterDrawer();
protected:
    virtual void Geometary() const;
};

class BilinearInterDrawer : public Renderer {
public:
    BilinearInterDrawer();
protected:
    virtual void Geometary() const;
};

class FrameDrawer : public Renderer {
public:
    FrameDrawer();
protected:
    virtual void Geometary() const;
    const TextBitmap mX;
    const TextBitmap mY;
    const TextBitmap mZ;
private:
    void DrawFrameRecursive(const SpinXML::Frame* frame) const;
};

//================================================================================//

class SpinSysScene : public Renderer {
public:
    SpinSysScene();
protected:
    virtual void Geometary() const;
private:
    SpinDrawer          mSpinDrawer;
    BondDrawer          mBondDrawer;
    FrameDrawer         mFrameDrawer;
};

class InteractionScene : public Renderer {
public:
    InteractionScene();
protected:
    virtual void Geometary() const;
private:
    MonoInterDrawer     mMonoDrawer;
    BilinearInterDrawer mBinaryDrawer;

};

#endif
