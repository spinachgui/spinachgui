
#ifndef __GLMODE_HPP__
#define __GLMODE_HPP__

#include <3d/opengl.hpp>
#include <utility>  //For std::pair
#include <vector>

///An abstract base class using RAII for setting and unsetting GL
///modes
class GLMode {
public:
	virtual void On()  = 0;
	virtual void Off() = 0;
};


class GLLighting : public GLMode {
public:
    virtual void On();
    virtual void Off();
};

class GLWire : public GLMode {
public:
    virtual void On();
    virtual void Off();
};

class GLTranslucent : public GLMode {
public:
	virtual void On();
	virtual void Off();
};

class GLPicking : public GLMode {
public:
    GLPicking(long len);
    ~GLPicking();
    void SetBox(long top,long left,long width, long height);
    std::pair<long,GLuint*> GetBuffer() const;
    virtual void On();
    virtual void Off();
private:
    long mLen;
    long mHits;
    GLuint* mBuff;
    GLdouble mProjmatrix[16];
    long mMouseX,mMouseY;
    long mWidth,mHeight;
};


class ModeCollection : public GLMode {
public:
	ModeCollection(const std::vector<GLMode*>& modes);
	ModeCollection(GLMode* mode1);
	ModeCollection(GLMode* mode1,GLMode* mode2);
	ModeCollection(GLMode* mode1,GLMode* mode2,GLMode* mode3);

	virtual void On();
	virtual void Off();
private:
	std::vector<GLMode*> mModes;
};


#endif
