
#ifndef __GL_GEOM_HPP__
#define __GL_GEOM_HPP__

#include <3d/displaySettings.hpp>

#include <3d/glmode.hpp>
#include <3d/opengl.hpp>



//Maybe this only needs to be an enum. We'll see
enum PASS {
	SOLID,
	TRANSLUCENT,
	PICKING
};





///An abstract class who's job is to visualise part of a spin system
///such as spins, linear interactions or bonds.
class Renderer {
public:
    Renderer();
    virtual ~Renderer();
    void DrawWith(GLMode& mode,const DisplaySettings& settings, PASS pass) const;
    void Draw(const DisplaySettings& settings, PASS pass) const;
protected:
    virtual void Geometary(const DisplaySettings& settings, PASS pass) const = 0;
private:
};


///Keeps a collection of renderers and manages gl state common to a
///scene, such as camera position, global rotation and lighting.
class Scene : public Renderer {
public:
    typedef std::vector<Renderer*> TRenderVec;
    typedef std::vector<Renderer*>::const_iterator TRenderIt;
    Scene(const TRenderVec& renderers);
    ~Scene();
protected:
    void Geometary(const DisplaySettings& displaySettings,PASS pass) const;
private:
    TRenderVec mRenderers;
};



class MoleculeFG : public Renderer {
public:
    MoleculeFG();
    void OnNewElectron(SpinXML::Spin* newSpin,long number);
protected:
    virtual void Geometary(const DisplaySettings& settings, PASS pass) const {}
};

//============================================================//

class SpinDrawer : public Renderer {
public:
    SpinDrawer();
protected:
    virtual void Geometary(const DisplaySettings& settings, PASS pass) const;
};


class BondDrawer : public Renderer {
public:
    BondDrawer();
protected:
    virtual void Geometary(const DisplaySettings& settings, PASS pass) const;
};

class LinearInterDrawer : public Renderer {
public:
    LinearInterDrawer();
protected:
    virtual void Geometary(const DisplaySettings& settings, PASS pass) const;
};

class FrameDrawer : public Renderer {
public:
	FrameDrawer();
protected:
	virtual void Geometary(const DisplaySettings& settings, PASS pass) const;
};

class SpinSysScene : public Scene {
public:
    SpinSysScene()
	: Scene(GetRenderers()) {
    }

private:
    std::vector<Renderer*> GetRenderers() {
	std::vector<Renderer*> out;
	out.push_back(new SpinDrawer);
	out.push_back(new BondDrawer);
	out.push_back(new LinearInterDrawer);
	return out;
    }
};

#endif
