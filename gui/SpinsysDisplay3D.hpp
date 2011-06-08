
#ifndef __SpinsysDisplay3D__H__
#define __SpinsysDisplay3D__H__

#include <gui/Display3D.hpp>
#include <3d/glgeometry.hpp>

class SpinsysDisplay3D : public Display3D {
public:
    SpinsysDisplay3D(wxWindow* parent);

    void OnRightClick(wxMouseEvent& e);
    void OnLeftClick(wxMouseEvent& e);

    virtual void OnMouseOver3D(int stackLength,const GLuint* ClosestName);
    virtual void DrawScene();
    virtual void DrawForeground();

    void SlotSpinChange() {Refresh();}
    void SlotInterChange() {Refresh();}
    void SlotSpinSysChange() {Refresh();}
protected:
    DECLARE_EVENT_TABLE();
        
private:
    //These nodes can be rotated and translated  with the mouse
    static GLLighting lighting;
    static GLTranslucent translucent;
    static GLWire wire;

    SpinSysScene     mMolScene;
    InteractionScene mInteractionScene;
    ElectronScene    mElectronScene;
    ElectronInterDrawer mElectronInterDrawer;
};

#endif
