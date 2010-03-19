//
// MATLAB Compiler: 4.11 (R2009b)
// Date: Fri Mar 19 18:05:52 2010
// Arguments: "-B" "macro_default" "dcm2euler.m" "euler2dcm.m" "-T"
// "compile:exe" "-W" "cpplib:mmath" 
//

#ifndef __mmath_h
#define __mmath_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" {
#endif

#if defined(__SUNPRO_CC)
/* Solaris shared libraries use __global, rather than mapfiles
 * to define the API exported from a shared library. __global is
 * only necessary when building the library -- files including
 * this header file to use the library do not need the __global
 * declaration; hence the EXPORTING_<library> logic.
 */

#ifdef EXPORTING_mmath
#define PUBLIC_mmath_C_API __global
#else
#define PUBLIC_mmath_C_API /* No import statement needed. */
#endif

#define LIB_mmath_C_API PUBLIC_mmath_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_mmath
#define PUBLIC_mmath_C_API __declspec(dllexport)
#else
#define PUBLIC_mmath_C_API __declspec(dllimport)
#endif

#define LIB_mmath_C_API PUBLIC_mmath_C_API


#else

#define LIB_mmath_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_mmath_C_API 
#define LIB_mmath_C_API /* No special import/export declaration */
#endif

extern LIB_mmath_C_API 
bool MW_CALL_CONV mmathInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_mmath_C_API 
bool MW_CALL_CONV mmathInitialize(void);

extern LIB_mmath_C_API 
void MW_CALL_CONV mmathTerminate(void);



extern LIB_mmath_C_API 
void MW_CALL_CONV mmathPrintStackTrace(void);

extern LIB_mmath_C_API 
long MW_CALL_CONV mmathGetMcrID();


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__BORLANDC__)

#ifdef EXPORTING_mmath
#define PUBLIC_mmath_CPP_API __declspec(dllexport)
#else
#define PUBLIC_mmath_CPP_API __declspec(dllimport)
#endif

#define LIB_mmath_CPP_API PUBLIC_mmath_CPP_API

#else

#if !defined(LIB_mmath_CPP_API)
#if defined(LIB_mmath_C_API)
#define LIB_mmath_CPP_API LIB_mmath_C_API
#else
#define LIB_mmath_CPP_API /* empty! */ 
#endif
#endif

#endif

#endif
#endif
