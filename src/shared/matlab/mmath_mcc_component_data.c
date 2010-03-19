//
// MATLAB Compiler: 4.11 (R2009b)
// Date: Fri Mar 19 18:05:52 2010
// Arguments: "-B" "macro_default" "dcm2euler.m" "euler2dcm.m" "-T"
// "compile:exe" "-W" "cpplib:mmath" 
//

#include "mclmcrrt.h"

#ifdef __cplusplus
extern "C" {
#endif
extern const unsigned char __MCC_mmath_session_key[] = {
    'B', 'B', 'F', 'F', '4', '6', 'E', 'A', 'F', '8', '3', '7', '2', '3', '3',
    '4', '4', 'D', 'A', '6', '7', '2', '5', 'E', 'B', 'F', '0', 'B', '7', '7',
    '6', '0', '8', '9', 'A', '9', '7', 'B', '6', '1', 'A', '9', '7', 'F', 'C',
    '4', 'D', '4', '7', 'F', 'B', '7', '8', '9', 'C', '0', 'C', '5', '7', '8',
    '1', '6', 'B', '7', 'C', '0', 'F', '5', 'D', 'B', 'D', '7', '0', '8', 'C',
    '8', 'B', '1', '5', '8', '5', 'A', '5', 'B', 'B', '8', 'D', 'C', '8', '0',
    '3', '0', '0', 'F', '7', 'A', 'D', '0', '4', '8', '4', 'C', '0', 'C', 'D',
    '5', '8', '3', '6', 'A', 'E', '9', '1', 'F', '3', '9', 'C', 'C', '8', '6',
    'E', 'B', '5', 'F', 'D', '2', 'D', '6', 'D', '4', 'F', 'F', '8', 'F', 'B',
    '2', '1', '1', 'D', 'A', 'E', '8', '2', '9', 'A', '5', '3', '1', '8', '4',
    '0', 'B', '7', 'A', '1', 'F', '6', 'F', '0', '0', '9', 'C', 'A', '7', 'C',
    '7', '0', '4', '6', '2', '2', '7', 'D', '9', 'B', 'B', '5', '0', '2', 'B',
    '0', '9', '7', '6', 'B', '8', 'E', '2', '1', 'C', '2', 'E', '9', '2', '0',
    '3', 'E', '5', '2', '8', 'E', '1', 'B', '1', '4', '8', 'D', 'A', 'C', '7',
    '8', 'B', 'B', '7', '7', '8', '1', '5', '0', '4', '3', 'E', '7', 'D', '1',
    'D', '9', 'E', 'E', 'B', '7', '0', 'A', 'C', '4', '4', 'F', 'E', '4', 'A',
    'E', '4', '7', 'B', 'F', 'C', 'A', 'C', 'C', 'E', '1', '9', '5', '9', 'D',
    '4', '\0'};

extern const unsigned char __MCC_mmath_public_key[] = {
    '3', '0', '8', '1', '9', 'D', '3', '0', '0', 'D', '0', '6', '0', '9', '2',
    'A', '8', '6', '4', '8', '8', '6', 'F', '7', '0', 'D', '0', '1', '0', '1',
    '0', '1', '0', '5', '0', '0', '0', '3', '8', '1', '8', 'B', '0', '0', '3',
    '0', '8', '1', '8', '7', '0', '2', '8', '1', '8', '1', '0', '0', 'C', '4',
    '9', 'C', 'A', 'C', '3', '4', 'E', 'D', '1', '3', 'A', '5', '2', '0', '6',
    '5', '8', 'F', '6', 'F', '8', 'E', '0', '1', '3', '8', 'C', '4', '3', '1',
    '5', 'B', '4', '3', '1', '5', '2', '7', '7', 'E', 'D', '3', 'F', '7', 'D',
    'A', 'E', '5', '3', '0', '9', '9', 'D', 'B', '0', '8', 'E', 'E', '5', '8',
    '9', 'F', '8', '0', '4', 'D', '4', 'B', '9', '8', '1', '3', '2', '6', 'A',
    '5', '2', 'C', 'C', 'E', '4', '3', '8', '2', 'E', '9', 'F', '2', 'B', '4',
    'D', '0', '8', '5', 'E', 'B', '9', '5', '0', 'C', '7', 'A', 'B', '1', '2',
    'E', 'D', 'E', '2', 'D', '4', '1', '2', '9', '7', '8', '2', '0', 'E', '6',
    '3', '7', '7', 'A', '5', 'F', 'E', 'B', '5', '6', '8', '9', 'D', '4', 'E',
    '6', '0', '3', '2', 'F', '6', '0', 'C', '4', '3', '0', '7', '4', 'A', '0',
    '4', 'C', '2', '6', 'A', 'B', '7', '2', 'F', '5', '4', 'B', '5', '1', 'B',
    'B', '4', '6', '0', '5', '7', '8', '7', '8', '5', 'B', '1', '9', '9', '0',
    '1', '4', '3', '1', '4', 'A', '6', '5', 'F', '0', '9', '0', 'B', '6', '1',
    'F', 'C', '2', '0', '1', '6', '9', '4', '5', '3', 'B', '5', '8', 'F', 'C',
    '8', 'B', 'A', '4', '3', 'E', '6', '7', '7', '6', 'E', 'B', '7', 'E', 'C',
    'D', '3', '1', '7', '8', 'B', '5', '6', 'A', 'B', '0', 'F', 'A', '0', '6',
    'D', 'D', '6', '4', '9', '6', '7', 'C', 'B', '1', '4', '9', 'E', '5', '0',
    '2', '0', '1', '1', '1', '\0'};

static const char * MCC_mmath_matlabpath_data[] = 
  { "mmath/", "$TOOLBOXDEPLOYDIR/", "$TOOLBOXMATLABDIR/general/",
    "$TOOLBOXMATLABDIR/ops/", "$TOOLBOXMATLABDIR/lang/",
    "$TOOLBOXMATLABDIR/elmat/", "$TOOLBOXMATLABDIR/randfun/",
    "$TOOLBOXMATLABDIR/elfun/", "$TOOLBOXMATLABDIR/specfun/",
    "$TOOLBOXMATLABDIR/matfun/", "$TOOLBOXMATLABDIR/datafun/",
    "$TOOLBOXMATLABDIR/polyfun/", "$TOOLBOXMATLABDIR/funfun/",
    "$TOOLBOXMATLABDIR/sparfun/", "$TOOLBOXMATLABDIR/scribe/",
    "$TOOLBOXMATLABDIR/graph2d/", "$TOOLBOXMATLABDIR/graph3d/",
    "$TOOLBOXMATLABDIR/specgraph/", "$TOOLBOXMATLABDIR/graphics/",
    "$TOOLBOXMATLABDIR/uitools/", "$TOOLBOXMATLABDIR/strfun/",
    "$TOOLBOXMATLABDIR/imagesci/", "$TOOLBOXMATLABDIR/iofun/",
    "$TOOLBOXMATLABDIR/audiovideo/", "$TOOLBOXMATLABDIR/timefun/",
    "$TOOLBOXMATLABDIR/datatypes/", "$TOOLBOXMATLABDIR/verctrl/",
    "$TOOLBOXMATLABDIR/codetools/", "$TOOLBOXMATLABDIR/helptools/",
    "$TOOLBOXMATLABDIR/demos/", "$TOOLBOXMATLABDIR/timeseries/",
    "$TOOLBOXMATLABDIR/hds/", "$TOOLBOXMATLABDIR/guide/",
    "$TOOLBOXMATLABDIR/plottools/", "toolbox/local/",
    "toolbox/shared/dastudio/", "$TOOLBOXMATLABDIR/datamanager/",
    "toolbox/compiler/", "toolbox/shared/optimlib/", "toolbox/optim/optim/" };

static const char * MCC_mmath_classpath_data[] = 
  { "" };

static const char * MCC_mmath_libpath_data[] = 
  { "" };

static const char * MCC_mmath_app_opts_data[] = 
  { "" };

static const char * MCC_mmath_run_opts_data[] = 
  { "" };

static const char * MCC_mmath_warning_state_data[] = 
  { "off:MATLAB:dispatcher:nameConflict" };


mclComponentData __MCC_mmath_component_data = { 

  /* Public key data */
  __MCC_mmath_public_key,

  /* Component name */
  "mmath",

  /* Component Root */
  "",

  /* Application key data */
  __MCC_mmath_session_key,

  /* Component's MATLAB Path */
  MCC_mmath_matlabpath_data,

  /* Number of directories in the MATLAB Path */
  40,

  /* Component's Java class path */
  MCC_mmath_classpath_data,
  /* Number of directories in the Java class path */
  0,

  /* Component's load library path (for extra shared libraries) */
  MCC_mmath_libpath_data,
  /* Number of directories in the load library path */
  0,

  /* MCR instance-specific runtime options */
  MCC_mmath_app_opts_data,
  /* Number of MCR instance-specific runtime options */
  0,

  /* MCR global runtime options */
  MCC_mmath_run_opts_data,
  /* Number of MCR global runtime options */
  0,
  
  /* Component preferences directory */
  "mmath_3FBBCE0308B49077FD102E9D774571AC",

  /* MCR warning status data */
  MCC_mmath_warning_state_data,
  /* Number of MCR warning status modifiers */
  1,

  /* Path to component - evaluated at runtime */
  NULL

};

#ifdef __cplusplus
}
#endif


