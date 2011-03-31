
#ifndef __OPENGL__HPP__
#define __OPENGL__HPP__

//Includes opengl in a platform agnostic manner

#include <GL/gl.h>

#ifdef __linux__
#include <GL/glx.h>
#endif

#include <GL/glu.h>

#endif
