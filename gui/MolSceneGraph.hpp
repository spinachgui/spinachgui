
#ifndef _MOL_SCENE_GRAPH_H_
#define _MOL_SCENE_GRAPH_H_

#include <sigc++/sigc++.h>
#include <gui/Display3D.hpp>



class MoleculeFG : public SGNode {
public:
    MoleculeFG(SpinXML::SpinSystem* ss);

    void OnNewElectron(SpinXML::Spin* newSpin,long number);

private:
    virtual void RawDraw(SpinachDC& dc) {}
    virtual void ToPovRay(wxString& src) {}
    SpinXML::SpinSystem* mSS;
};

//============================================================//
class MoleculeNodeNew : public SGNode {
public:
    MoleculeNodeNew(SpinXML::SpinSystem* ss);
private:
    virtual void RawDraw(SpinachDC& dc);
    virtual void ToPovRay(wxString& src);
    SpinXML::SpinSystem* mSS;
};

class SpinDrawerNode : public SGNode {
public:
    SpinDrawerNode(SpinXML::SpinSystem* ss);
private:
    virtual void RawDraw(SpinachDC& dc);
    virtual void ToPovRay(wxString& src);
    SpinXML::SpinSystem* mSS;
};


class BondDrawerNode : public SGNode {
public:
    BondDrawerNode(SpinXML::SpinSystem* ss);
private:
    virtual void RawDraw(SpinachDC& dc);
    virtual void ToPovRay(wxString& src);
    SpinXML::SpinSystem* mSS;
};

class InteractionDrawerNode : public SGNode {
public:
    InteractionDrawerNode(SpinXML::SpinSystem* ss);
private:
    virtual void RawDraw(SpinachDC& dc);
    virtual void ToPovRay(wxString& src);
    void RawDrawInterType(SpinachDC& dc,SpinXML::Interaction::Type t);
    void RawDrawBilinInterType(SpinachDC& dc,SpinXML::Interaction::Type t);
	SpinXML::SpinSystem* mSS;
};

class FrameDrawerNode : public SGNode {
public:
	FrameDrawerNode(SpinXML::SpinSystem* ss);
private:
	virtual void RawDraw(SpinachDC& dc);
	virtual void ToPovRay(wxString& src) {}
};

#endif
