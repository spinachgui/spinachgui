
#ifndef _3DDISPLAY_H
#define _3DDISPLAY_H

#include <boost/foreach.hpp>
#define foreach         BOOST_FOREACH
#define foreach_reverse BOOST_REVERSE_FOREACH

#include <shared/spinsys.hpp>

#include <wx/glcanvas.h>

#ifdef __linux__
#include <GL/glx.h>
#endif

#include <GL/glu.h>
#include <sigc++/sigc++.h>

#include <map>

///The characteristic length scale of the objects we are
///rendering. Affects the positioning of the camera
#define OPENGL_SCALE 1.0 //(1e-10)

enum LAYER {
	LAYER_DEFAULT = 0,
    LAYER_AXIS,
    LAYER_SPINS,
    LAYER_BONDS,
    LAYER_INTERACTIONS,
};

///A struct storing all 3D settings that are invariant over a single
///rendering pass, implying that this structure may be passed safely
///as a const reference. It would likely make sense to link settings
///here to a menu option of configuration file setting. Information
///such as the current render pass would not belong in this struct
struct DisplaySettings {
    DisplaySettings()
       : mSolidQuadric(gluNewQuadric()),
	 mWireQuadric(gluNewQuadric()) {
	gluQuadricDrawStyle(mSolidQuadric,GLU_FILL);
	gluQuadricNormals  (mWireQuadric,GLU_SMOOTH);
    }
    ~DisplaySettings() {
	gluDeleteQuadric(mSolidQuadric);
        gluDeleteQuadric(mWireQuadric);
    }
    void SetShowBonds(bool showBonds) {drawBonds=showBonds;}
    bool drawBonds;

    void SetScalling(double scalling,SpinXML::Interaction::Type t) {
        mScallings[t]=scalling;
    }
    void SetColour(float r,float g,float b,SpinXML::Interaction::Type t) {
        mInterColours[t]=Vector3d(r,g,b);
    }
    void SetVisible(bool v,SpinXML::Interaction::Type t) {
        mVisible[t]=v;
    }

    GLUquadric* mSolidQuadric;
    GLUquadric* mWireQuadric;
    int width,height;
    const float* mRotationMatrix;

    ///The size of the displayed interaction in units of energy/length,
    ///e.g. MHz/Nm
    std::map<SpinXML::Interaction::Type,double> mScallings;
    ///The colour of the displayed interactions
    std::map<SpinXML::Interaction::Type,Vector3d> mInterColours;
    ///The visibility of the displayed interactions
    std::map<SpinXML::Interaction::Type,bool> mVisible;
};

//Maybe this only needs to be an enum. We'll see
enum PASS {
	SOLID,
	TRANSLUCENT,
	PICKING
};

///An abstract base class using RAII for setting and unsetting GL
///modes
class GLMode {
public:
	virtual void On()  const = 0;
	virtual void Off() const = 0;
};

class GLLighting : public GLMode {
public:
    virtual void On() const {
	std::cout << "Lighting On" << std::endl;
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	// GL_LIGHT0: the white light emitting light source
	// Create light components for GL_LIGHT0
	GLfloat ambientLight0[] =  {0.4, 0.4, 0.4, 1.0};
	GLfloat diffuseLight0[] =  {0.6, 0.6, 0.6, 1.0};
	GLfloat specularLight0[] = {0.8, 0.8, 0.8, 1.0};
	GLfloat position0[] =      {-1.5, 1.0,-4.0, 1.0};
	// Assign created components to GL_LIGHT0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight0);
	glLightfv(GL_LIGHT0, GL_POSITION, position0);

	// GL_LIGHT1: the red light emitting light source
	// Create light components for GL_LIGHT1
	GLfloat ambientLight1[] =  {0.4, 0.4, 0.4, 1.0};
	GLfloat diffuseLight1[] =  {0.6, 0.6, 0.6, 1.0};
	GLfloat specularLight1[] = {0.8, 0.8, 0.8, 1.0};
	GLfloat position1[] =      {1.5, 1.0, 4.0, 1.0};
	// Assign created components to GL_LIGHT1
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight1);
	glLightfv(GL_LIGHT1, GL_POSITION, position1);
    }
    virtual void Off() const {
	glDisable(GL_LIGHTING);
    }
};

class ModeCollection : public GLMode {
public:
	ModeCollection(const std::vector<GLMode*>& modes) 
		: mModes(modes) {
	}
	ModeCollection(GLMode* mode1) {
		mModes.push_back(mode1);
	}
	ModeCollection(GLMode* mode1,GLMode* mode2) {
		mModes.push_back(mode1);
		mModes.push_back(mode2);
	}
	ModeCollection(GLMode* mode1,GLMode* mode2,GLMode* mode3) {
		mModes.push_back(mode1);
		mModes.push_back(mode2);
		mModes.push_back(mode3);
	}

	virtual void On() const {
		foreach(GLMode* mode, mModes) {
			mode->On();
		}
	}
	virtual void Off() const {
		foreach_reverse(GLMode* mode, mModes) {
			mode->Off();
		}
	}
private:
	std::vector<GLMode*> mModes;
};

///An abstract class who's job is to visualise part of a spin system
///such as spins, linear interactions or bonds.
class Renderer {
public:
    Renderer();
    virtual ~Renderer();
    void DrawWith(const GLMode& mode,const DisplaySettings& settings, PASS pass) const;
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
    Scene(const TRenderVec& renderers) 
	: mRenderers(renderers) {
    }
    ~Scene() {
	for(TRenderIt i = mRenderers.begin();i != mRenderers.end();++i) {
	    delete (*i);
	}
    }
protected:
    void Geometary(const DisplaySettings& displaySettings,PASS pass) const {
	//loop and render
	for(TRenderIt i = mRenderers.begin();i != mRenderers.end();++i) {
	    (*i)->Draw(displaySettings,pass);
	}
    }
private:
    TRenderVec mRenderers;
};

class Camera {
public:
	Camera() 
		: mEyeLocation(5,5,5), mEyeTarget(0,0,0),mZoom(0.001) {
	}
	void SetZoom(double zoom);
	void Rotate(float deltaX,float deltaY) {
		float dotProduct=(deltaX*deltaX+deltaY*deltaY);
		float norm=sqrt(dotProduct);
	}
	void Set(int width,int height) const {
		glMatrixMode(GL_PROJECTION);
		//NB: glOrtho multiplies the current matrix so glLoadIdentity is
		//vital
		glLoadIdentity();
		glOrtho(-width*mZoom ,width*mZoom,
				-height*mZoom,height*mZoom,
				1.0, 40.0);

		glMatrixMode(GL_MODELVIEW);

		glLoadIdentity();
		gluLookAt(mEyeLocation.x(),mEyeLocation.y(),mEyeLocation.z(),
				  mEyeTarget.x(),mEyeTarget.y(),mEyeTarget.z(),
				  0,1,0);
	}
private:
	//The camera starts at the origin
	Vector3f mEyeLocation;
	Vector3f mEyeTarget;

	float mZoom;
};

///Manages interaction with the rest of the GUI including keeping the
///athorattive copy of DisplaySettings, managing the context, managing
///windowing events, setting the viewport etc. Only this class should
///depend on wxWidgets
class Display3D :  public wxGLCanvas, public sigc::trackable {
public:
    Display3D(wxWindow* parent);
    virtual ~Display3D();

    void OnPaint(wxPaintEvent& e);
    void OnMouseMove(wxMouseEvent& e);
    void OnWheel(wxMouseEvent& e);
    void OnRightClick(wxMouseEvent& e);
    void OnLeftClick(wxMouseEvent& e);
    void OnResize(wxSizeEvent& e);
    void OnDeleteSpinHover(wxCommandEvent& e);

    void ResetView();

    void SetScene(Renderer* scene) {
        if(mScene) delete mScene; mScene=scene;
    }
    void OnDirty() {Refresh();}
	DisplaySettings* GetDisplaySettings() {return &mDisplaySettings;}

    void PrintTransformMatricese();
protected:
    DECLARE_EVENT_TABLE();

    ///Call whenever the size of the viewport changes

    void EnableGL();
    void ChangeViewport();
private:

    //These nodes can be rotated and translated  with the mouse
    Renderer* mScene;

	DisplaySettings mDisplaySettings;

    bool mGLEnabled;
    wxGLContext* mGLContext;

    //GUI State Variables
    double mHoverDist;
    long mMouseX,mMouseY;
    double mZoom;
    int mWidth,mHeight;

    //3D Variables
	Camera* mCamera;
    float mRotationMatrix[16];
    float mXTranslate,mYTranslate;
    float mXRotate,mYRotate;
};

#endif

