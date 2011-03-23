
#ifndef _MOL_SCENE_GRAPH_H_
#define _MOL_SCENE_GRAPH_H_

#include <sigc++/sigc++.h>
#include <gui/Display3D.hpp>



class MoleculeFG : public Renderer {
public:
    MoleculeFG(SpinXML::SpinSystem* ss);

    void OnNewElectron(SpinXML::Spin* newSpin,long number);

private:
    virtual void RawDraw(SpinachDC& dc) {}
};

//============================================================//

class SpinDrawer : public Renderer {
public:
    SpinDrawer(SpinXML::SpinSystem* ss);
private:
    virtual void RawDraw(SpinachDC& dc);
};


class BondDrawer : public Renderer {
public:
    BondDrawer(SpinXML::SpinSystem* ss);
private:
    virtual void RawDraw(SpinachDC& dc);
};

class LinearInterDrawer : public Renderer {
public:
    InteractionDrawer(SpinXML::SpinSystem* ss);
private:
    virtual void RawDraw(SpinachDC& dc);
};

class FrameDrawer : public Renderer {
public:
	FrameDrawer(SpinXML::SpinSystem* ss);
private:
	virtual void RawDraw(SpinachDC& dc);
};

#endif
