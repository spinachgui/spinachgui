
#ifndef _MOL_SCENE_GRAPH_H_
#define _MOL_SCENE_GRAPH_H_

#include <sigc++/sigc++.h>
#include <gui/Display3D.hpp>



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
