
#include <3d/glmode.hpp>
#include <iostream>
#include <shared/foreach.hpp>

using namespace std;

//============================================================//

void GLLighting::On()  {
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

void GLLighting::Off() {
	glDisable(GL_LIGHTING);
}

//============================================================//

void GLTranslucent::On()  {
	glEnable (GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLTranslucent::Off() {
	glDepthMask(GL_TRUE);
	glDisable (GL_BLEND);
}

//============================================================//

GLPicking::GLPicking(long len) 
	: mLen(len),mHits(0),mBuff(new GLuint[len]),mMouseX(0),mMouseY(0) {
}

GLPicking::~GLPicking() {
	if(mBuff) {
		delete [] mBuff;
	}
}

void GLPicking::SetMouseXY(long mouseX,long mouseY) {
	mMouseX = mouseX;
	mMouseY = mouseY;
}

std::pair<long,GLuint*> GLPicking::GetBuffer() const {
	return std::pair<long,GLuint*>(mHits,mBuff);
}

void GLPicking::On() {
	glSelectBuffer(mLen,mBuff);  //glSelectBuffer goes before glRenderMode
	glRenderMode(GL_SELECT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glClear(GL_DEPTH_BUFFER_BIT);
		
	GLint    viewport[4];
	GLdouble projmatrix[16];
	glGetIntegerv(GL_VIEWPORT,viewport);
	glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);
		
	gluPickMatrix(mMouseX,viewport[3]-mMouseY,3.0, 3.0, viewport);
	glMultMatrixd(projmatrix);

	glInitNames();
	glMatrixMode(GL_MODELVIEW);
}
void GLPicking::Off() {
	mHits=glRenderMode(GL_RENDER);
}

//============================================================//

ModeCollection::ModeCollection(const std::vector<GLMode*>& modes) 
	: mModes(modes) {
}

ModeCollection::ModeCollection(GLMode* mode1) {
	mModes.push_back(mode1);
}

ModeCollection::ModeCollection(GLMode* mode1,GLMode* mode2) {
	mModes.push_back(mode1);
	mModes.push_back(mode2);
}

ModeCollection::ModeCollection(GLMode* mode1,GLMode* mode2,GLMode* mode3) {
	mModes.push_back(mode1);
	mModes.push_back(mode2);
	mModes.push_back(mode3);
}

void ModeCollection::On() {
	foreach(GLMode* mode, mModes) {
		mode->On();
	}
}

void ModeCollection::Off() {
	foreach_reverse(GLMode* mode, mModes) {
		mode->Off();
	}
}

//============================================================//

