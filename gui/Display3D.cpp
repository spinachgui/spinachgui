
#include <gui/SpinachApp.hpp>
#include <gui/Display3D.hpp>
#include <gui/RightClickMenu.hpp>
#include <wx/dcclient.h>
#include <wx/image.h>
#include <wx/dcmemory.h>
#include <iostream>
#include <wx/file.h>

#include <shared/spinsys.hpp>

#include <3d/glgeometry.hpp>
#include <3d/glmode.hpp>
#include <3d/camera.hpp>

using namespace std;
using namespace SpinXML;
using namespace sigc;

GLfloat defaultMaterial[3] = {0.5, 0.5,  0.5};


//============================================================//
// Display3D class
int gl_attribs[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 24, 0};

Display3D::Display3D(wxWindow* parent)
    : wxGLCanvas(parent,(wxGLContext*)NULL,wxID_ANY,
                 wxDefaultPosition,wxDefaultSize,
                 WX_GL_DOUBLEBUFFER,wxT("Display3D"),
                 gl_attribs) {
    mCamera = new Camera;
    mPicking = new GLPicking(2000);

	mDraging = false;

    mBandBoxOn = false;
    mBandBoxStartX = -1;
    mBandBoxStartY = -1;


    mMouseX = 0;
    mMouseY = 0;

    mGLContext=NULL;
    mGLEnabled=false;
    sig3DChange.connect(sigc::mem_fun(this,&Display3D::Redraw));
}

void Display3D::ResetView() {
	cout << "Not implimented Display3D::ResetView()" << endl;
    Refresh();
}

Display3D::~Display3D() {
	if(mCamera) {
		delete mCamera;
	}
	if(mPicking) {
		delete mPicking;
	}
}



void Display3D::EnableGL() {
    if(mGLContext == NULL) {
        mGLContext = new wxGLContext(this);
    }
    this->SetCurrent(*mGLContext);

    cout << "OpenGL version is " << glGetString(GL_VERSION) << endl;

    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClearDepth(1.0);
	glEnable (GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glShadeModel(GL_SMOOTH);
    //Adpated from a detailed tutorail on opengl lighting located at
    //http://www.falloutsoftware.com/tutorials/gl/gl8.htm

    glMaterialf(GL_FRONT, GL_SHININESS, 10.0f);

    //Create a dictionary of colours
    glMatrixMode(GL_MODELVIEW);

	SetQuadric(QUAD_SOLID);

    //Generate a texture for saving the depth buffer to
    //glGenTextures(1,&mTexDepth);
    //Generate a texture for saving the depth buffer to
    //glGenFramebuffers(1,&mFB);

	glEnable(GL_NORMALIZE);
}

void Display3D::ChangeViewport() {
    cout << "Changing viewport" << endl;
    GetClientSize(&mWidth,&mHeight);
    glViewport(0,0,mWidth,mHeight);
}


void Display3D::OnMouseMove(wxMouseEvent& e) {
	if(e.Dragging()) {
		mDraging = true;
	} else {
		mDraging = false;
	}
    if(e.Dragging() && e.RightIsDown()) {
		mCamera->Translate(e.GetX()-mMouseX,e.GetY()-mMouseY);
    }  else if(e.Dragging() && e.LeftIsDown()) {
		if(e.ShiftDown()) {
			//Band box select
			if(!mBandBoxOn) {
				mBandBoxOn = true;
				mBandBoxStartX = mMouseX;
				mBandBoxStartY = mMouseY;
			}
		} else {
			//Ajust camera angle
			mCamera->Rotate(e.GetX()-mMouseX,e.GetY()-mMouseY);
		}
    } 
    mMouseX=e.GetX();
    mMouseY=e.GetY();
    Refresh();
}


void Display3D::OnWheel(wxMouseEvent& e) {
	mCamera->DeltaZoom(-(0.001)*e.GetWheelRotation()/e.GetWheelDelta());
    Refresh();
}

void Display3D::OnRightClick(wxMouseEvent& e) {
    if(!mDraging) {
		if(GetHover()!=NULL) {
			RightClickMenu* menu = new RightClickMenu(this);
			menu->Build();
			PopupMenu(menu);
			delete menu;
		}
    }
}

void Display3D::OnLeftClick(wxMouseEvent& e) {
    if(mBandBoxOn) {
		cout << "Band Box Up" << endl;
		mBandBoxOn = false;
		return;
    }
	if(mDraging) {
		return;
	}

	if(GetHover() == NULL) {
		ClearSelection();
		return;
	}

	if(!e.ShiftDown()) {
		SetSelection(GetHover());
	} else {
		if(GetSelection().find(GetHover()) != GetSelection().end()) {
			AddSelection(GetHover());
		} else {
			RemoveSelection(GetHover());
		}
	}

}

void Display3D::OnResize(wxSizeEvent& e) {
    int w,h;
    GetClientSize(&w,&h);

    if(mGLEnabled) {
        ChangeViewport();
    }
}


void Display3D::OnDeleteSpinHover(wxCommandEvent& e) {
}

void Display3D::Set2DView() {
    int width,height;
    GetClientSize(&width,&height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,width,0,height,-100,100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Display3D::OnPaint(wxPaintEvent& e) {
    wxPaintDC dc(this);

    if(!mGLEnabled) {
        mGLEnabled=true;
        EnableGL();
    }

    int width,height;
    GetClientSize(&width,&height);
    mCamera->Set(width,height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //Draw opaque objects first
    
    DrawScene();

    Set2DView();
    DrawForeground();
    if(mBandBoxOn) {
	//Draw any 2D components
	glColor3f(0.0,0.0,0.0);


	glBegin(GL_LINE);
	glVertex2i(mBandBoxStartX,height-mBandBoxStartY);
	glVertex2i(mMouseX       ,height-mBandBoxStartY);

	glVertex2i(mMouseX       ,height-mBandBoxStartY);
	glVertex2i(mMouseX       ,height-mMouseY       );

	glVertex2i(mMouseX       ,height-mMouseY       );
	glVertex2i(mBandBoxStartX,height-mMouseY       );

	glVertex2i(mBandBoxStartX,height-mMouseY       );
	glVertex2i(mBandBoxStartX,height-mBandBoxStartY);
	glEnd();
	
    }

    SwapBuffers();
    while (true) {
	GLenum x = glGetError() ;

	if ( x == GL_NO_ERROR )
	    break;
	cout << "OpenGL error:" << gluErrorString(x) << endl;
    }
}

void Display3D::StartPicking() {
    if(mBandBoxOn) {
	mPicking->SetBox(mMouseX,mMouseY,mBandBoxStartX-mMouseX,mBandBoxStartY-mMouseX);
    } else {
	mPicking->SetBox(mMouseX,mMouseY,3,3);
    }
    mPicking->On();
}

void Display3D::StopPicking() {
    mPicking->Off();

    pair<long,GLuint*> tupple = mPicking->GetBuffer();

    long hits = tupple.first;
    GLuint* buff = tupple.second;

    float closestDistance=HUGE_VAL;
    GLuint* closestNames=NULL;
    GLuint closestNameCount=0;
    if(hits <= 0) {
		OnMouseOver3D(0,NULL);
		return;
    }

    for(long i=0;i<hits;i++) {
		GLuint name_count = *(buff++);
		float d1=float(*(buff++))/0x7fffffff;
		float d2=float(*(buff++))/0x7fffffff;
		float thisDistance=d1<d2 ? d1 : d2;
		if(closestDistance > thisDistance) {
			closestDistance=thisDistance;
			closestNames=buff;
			closestNameCount=name_count;
		}
		buff+=name_count;
		cout << endl;
    }
    OnMouseOver3D(closestNameCount,closestNames);
}

BEGIN_EVENT_TABLE(Display3D,wxGLCanvas)

EVT_PAINT     (                       Display3D::OnPaint)
EVT_MOTION    (                       Display3D::OnMouseMove)
EVT_MOUSEWHEEL(                       Display3D::OnWheel)
EVT_RIGHT_UP  (                       Display3D::OnRightClick)
EVT_LEFT_UP   (                       Display3D::OnLeftClick)
EVT_SIZE      (                       Display3D::OnResize)
EVT_ERASE_BACKGROUND(Display3D::OnEraseBackground)

END_EVENT_TABLE()



