

#include <3d/glgeometry.hpp>
#include <3d/displaySettings.hpp>
#include <gui/Display3D.hpp>
#include <gui/SpinsysDisplay3D.hpp>
#include <gui/SpinachApp.hpp>
#include <shared/spinsys.hpp>
#include <gui/RightClickMenu.hpp>

using namespace SpinXML;
using namespace std;

SpinsysDisplay3D::SpinsysDisplay3D(wxWindow* parent) 
    : Display3D(parent),
      mElectronScene(GetCamera()),
      mElectronInterDrawer(GetCamera()) {
        
    GetRawSS()->sigReloaded.connect(mem_fun(this,&Display3D::ResetView));
                
    //Make sure that if anything changes we redraw the scene
    SpinSystem::sigAnyChange.connect(mem_fun(this,&SpinsysDisplay3D::SlotSpinChange));
    Spin::sigAnyChange.connect(mem_fun(this,&SpinsysDisplay3D::SlotInterChange));
    Interaction::sigAnyChange.connect(mem_fun(this,&SpinsysDisplay3D::SlotSpinSysChange));

    sig3DChange.connect(sigc::mem_fun(this,&Display3D::Redraw));
}

void SpinsysDisplay3D::DrawScene() {
    lighting.On();

    mMolScene.Draw();

    translucent.On();
    mInteractionScene.Draw();
    translucent.Off();

    //Also draw the interactions as wireframes
    wire.On();
    mInteractionScene.Draw();
    wire.Off();

    lighting.Off();

    StartPicking();
    mMolScene.Draw();
    mInteractionScene.Draw();
    StopPicking();
}
void SpinsysDisplay3D::DrawForeground() {
    int mWidth, mHeight;
    GetClientSize(&mWidth,&mHeight);

    glColor3f(0.0,0.0,0.0);

    lighting.On();

    glPushMatrix();
    //Place the electron tensors along the top of the screne
    //starting from the left
    glTranslatef(40,mHeight-40,0);
    mElectronScene.Draw();

    translucent.On();
    mElectronInterDrawer.Draw();
    translucent.Off();

    wire.On();
    mElectronInterDrawer.Draw();
    wire.Off();

    lighting.Off();         

    glPopMatrix();

    lighting.Off();
}

void SpinsysDisplay3D::OnMouseOver3D(int stackLength,const GLuint* ClosestName) {
    if(stackLength == 0) {
        SetHover(NULL);
        return;
    }
    Spin* hover;
    Spin* newHover;
    switch(ClosestName[0]) {
    case NAME_MONO_INTERACTIONS:
        cout << "Moused over a spin" << endl;
    case NAME_SPINS:
        hover = GetHover();
        newHover = GetRawSS()->GetSpin(ClosestName[stackLength-1]);
        if(hover != newHover) {
            SetHover(newHover);
        }
        break;
    default:
        SetHover(NULL);
    }
}

GLLighting    SpinsysDisplay3D::lighting;
GLTranslucent SpinsysDisplay3D::translucent;
GLWire        SpinsysDisplay3D::wire;

void SpinsysDisplay3D::OnRightClick(wxMouseEvent& e) {
    if(!IsDragging()) {
        if(GetHover()!=NULL) {
            RightClickMenu* menu = new RightClickMenu(this);
            menu->Build();
            PopupMenu(menu);
            delete menu;
        }
    }
}

void SpinsysDisplay3D::OnLeftClick(wxMouseEvent& e) {
    if(IsDragging()) {
        return;
    }

    if(GetHover() == NULL) {
        ClearSelection();
        return;
    }

    if(!e.ShiftDown()) {
        SetSelection(GetHover());
    } else {
        if(GetSelection().find(GetHover()) == GetSelection().end()) {
            AddSelection(GetHover());
        } else {
            RemoveSelection(GetHover());
        }
    }

}
BEGIN_EVENT_TABLE(SpinsysDisplay3D,Display3D)

EVT_RIGHT_UP  (                       SpinsysDisplay3D::OnRightClick)
EVT_LEFT_UP   (                       SpinsysDisplay3D::OnLeftClick)

END_EVENT_TABLE()

