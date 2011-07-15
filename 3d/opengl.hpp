
#ifndef __OPENGL__HPP__
#define __OPENGL__HPP__

//Includes opengl in a platform agnostic manner

#ifdef _WINDOWS
#include <windows.h>
#include <GL/gl.h>
#include <wx/msw/winundef.h>
#endif


#ifdef __linux__
#include <GL/gl.h>
#include <GL/glx.h>
#endif

#include <GL/glu.h>

#endif
