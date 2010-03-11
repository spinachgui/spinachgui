
#ifndef _MOL_SCENE_GRAPH_H_
#define _MOL_SCENE_GRAPH_H_

#include <sigc++/sigc++.h>
#include <gui/Display3D.hpp>


///Scenegraph node that draws a spin
class SpinNode : public SGNode {
public:
    SpinNode(Spin* spin);
    void OnSpinDying(Spin*) {delete this;} //Arguments are usused

    void OnSpinHover(Spin* spin);
    void OnSpinSelect(std::vector<SpinXML::Spin*> spins);
private:

    Spin* mSpin;
    virtual void RawDraw(SpinachDC& dc);
    virtual void ToPovRay(wxString& src);

    bool mHover;
    bool mSelected;
};

class InterNode : public SGNode {
public:
    InterNode(SpinXML::Spin* spin, SpinXML::Interaction::SubType st);
    void OnSpinDying(Spin*) {delete this;}
    void LoadInteractionMatrix();

    void OnNewInteraction(Interaction* inter);
private:
    SpinXML::Spin* mSpin;
    SpinXML::Interaction::SubType mType;

    void SetMatrix(const Matrix3& mat);
    virtual void RawDraw(SpinachDC& dc);
    virtual void ToPovRay(wxString& src);
    float mat[16];
};

class BilinInterNode : public SGNode {
public:
    BilinInterNode(SpinXML::Spin* spin1,SpinXML::Spin* spin2, SpinXML::Interaction::SubType st);
    void OnSpinDying(Spin*) {delete this;}
    void LoadInteractionTrace();

    void OnNewInteraction(Interaction* inter);
private:
    SpinXML::Spin* mSpin1;
    SpinXML::Spin* mSpin2;
    SpinXML::Interaction::SubType mType;

    void SetMatrix(const Matrix3& mat);
    virtual void RawDraw(SpinachDC& dc);
    virtual void ToPovRay(wxString& src);
    double total_trace;
};

class MoleculeNode : public SGNode {
public:
    MoleculeNode(SpinSystem* ss);
    void OnReload();
    void OnNewSpin(SpinXML::Spin* newSpin,long number);

private:
    virtual void RawDraw(SpinachDC& dc);
    SpinSystem* mSS;
    virtual void ToPovRay(wxString& src);
};

class MoleculeFG : public SGNode {
public:
    MoleculeFG(SpinSystem* ss);

    void OnNewElectron(SpinXML::Spin* newSpin,long number);

private:
    virtual void RawDraw(SpinachDC& dc) {}
    virtual void ToPovRay(wxString& src) {}
    SpinSystem* mSS;
};

#endif
