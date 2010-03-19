//
// MATLAB Compiler: 4.11 (R2009b)
// Date: Fri Mar 19 18:05:52 2010
// Arguments: "-B" "macro_default" "dcm2euler.m" "euler2dcm.m" "-T"
// "compile:exe" "-W" "cpplib:mmath" 
//

#include <stdio.h>
#define EXPORTING_mmath 1
#include "mmath.h"
#ifdef __cplusplus
extern "C" {
#endif

extern mclComponentData __MCC_mmath_component_data;

#ifdef __cplusplus
}
#endif


static HMCRINSTANCE _mcr_inst = NULL;


#ifdef __cplusplus
extern "C" {
#endif

static int mclDefaultPrintHandler(const char *s)
{
  return mclWrite(1 /* stdout */, s, sizeof(char)*strlen(s));
}

#ifdef __cplusplus
} /* End extern "C" block */
#endif

#ifdef __cplusplus
extern "C" {
#endif

static int mclDefaultErrorHandler(const char *s)
{
  int written = 0;
  size_t len = 0;
  len = strlen(s);
  written = mclWrite(2 /* stderr */, s, sizeof(char)*len);
  if (len > 0 && s[ len-1 ] != '\n')
    written += mclWrite(2 /* stderr */, "\n", sizeof(char));
  return written;
}

#ifdef __cplusplus
} /* End extern "C" block */
#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_mmath_C_API
#define LIB_mmath_C_API /* No special import/export declaration */
#endif

LIB_mmath_C_API 
bool MW_CALL_CONV mmathInitializeWithHandlers(
    mclOutputHandlerFcn error_handler,
    mclOutputHandlerFcn print_handler)
{
  if (_mcr_inst != NULL)
    return true;
  if (!mclmcrInitialize())
    return false;
  if (!mclInitializeComponentInstance(&_mcr_inst, &__MCC_mmath_component_data, true, 
                                      NoObjectType, LibTarget, error_handler, 
                                      print_handler))
    return false;
  return true;
}

LIB_mmath_C_API 
bool MW_CALL_CONV mmathInitialize(void)
{
  return mmathInitializeWithHandlers(mclDefaultErrorHandler, mclDefaultPrintHandler);
}

LIB_mmath_C_API 
void MW_CALL_CONV mmathTerminate(void)
{
  if (_mcr_inst != NULL)
    mclTerminateInstance(&_mcr_inst);
}

LIB_mmath_C_API 
long MW_CALL_CONV mmathGetMcrID() 
{
  return mclGetID(_mcr_inst);
}

LIB_mmath_C_API 
void MW_CALL_CONV mmathPrintStackTrace(void) 
{
  char** stackTrace;
  int stackDepth = mclGetStackTrace(_mcr_inst, &stackTrace);
  int i;
  for(i=0; i<stackDepth; i++)
  {
    mclWrite(2 /* stderr */, stackTrace[i], sizeof(char)*strlen(stackTrace[i]));
    mclWrite(2 /* stderr */, "\n", sizeof(char)*strlen("\n"));
  }
  mclFreeStackTrace(&stackTrace, stackDepth);
}

