
#ifndef _MOL_SCENE_GRAPH_H_
#define _MOL_SCENE_GRAPH_H_

#include <sigc++/sigc++.h>
#include <gui/Display3D.hpp>



class MoleculeFG : public SGNode {
public:
    MoleculeFG(SpinSystem* ss);

    void OnNewElectron(SpinXML::Spin* newSpin,long number);

private:
    virtual void RawDraw(SpinachDC& dc) {}
    virtual void ToPovRay(wxString& src) {}
    SpinSystem* mSS;
};

//============================================================//
class MoleculeNodeNew : public SGNode {
public:
    MoleculeNodeNew(SpinSystem* ss);
private:
    virtual void RawDraw(SpinachDC& dc);
    virtual void ToPovRay(wxString& src);
    SpinSystem* mSS;
};

class SpinDrawerNode : public SGNode {
public:
    SpinDrawerNode(SpinSystem* ss);
private:
    virtual void RawDraw(SpinachDC& dc);
    virtual void ToPovRay(wxString& src);
    SpinSystem* mSS;
};


class BondDrawerNode : public SGNode {
public:
    BondDrawerNode(SpinSystem* ss);
private:
    virtual void RawDraw(SpinachDC& dc);
    virtual void ToPovRay(wxString& src);
    SpinSystem* mSS;
};

class InteractionDrawerNode : public SGNode {
public:
    InteractionDrawerNode(SpinSystem* ss);
private:
    virtual void RawDraw(SpinachDC& dc);
    virtual void ToPovRay(wxString& src);
    void RawDrawInterType(SpinachDC& dc,Interaction::SubType st);
    void RawDrawBilinInterType(SpinachDC& dc,Interaction::SubType st);
    SpinSystem* mSS;
};


#endif
