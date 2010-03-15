cmake_minimum_required(VERSION 2.6)
project(popi)
enable_language(Fortran)


############################################################
#Tell me what commands are being executed.
#set(CMAKE_VERBOSE_MAKEFILE TRUE)

# Make gcc output all the debug information it can! Enclose this in
#some sort of "if" if more than one compiler is ever supported
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ggdb -Wno-write-strings -Wall")

SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS}")


#Choose a build type:

#Default build. The worst of both worlds
# set(CMAKE_BUILD_TYPE None)

#Debug build, can use with gdb
set(CMAKE_BUILD_TYPE Debug)

#Compile a release build
#set(CMAKE_BUILD_TYPE Release)

############################################################
#Work out where all the built files are going
set(buildDir ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY})

#GDB can debug an optermised executable. It's one of a very small
#number of debuggers that can.
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ggdb")


set(wxWidgets_USE_DEBUG,ON)
set(wxWidgets_USE_UNICODE,ON)
set(wxWidgets_USE_UNIVERSAL,OFF)
set(wxWidgets_USE_STATIC,ON)

if(WIN32)
  include("windows-config.cmake")
else()
  find_package(wxWidgets COMPONENTS gl adv aui REQUIRED)
  find_package(PythonLibs)
  find_package(Boost REQUIRED program_options)
  #set(CMAKE_CXX_COMPILER "g++-4.2")
endif()

############################################################
#List of source files for all the targets

file(GLOB lapackSources lapack/double/*.f)
file(GLOB guiSources gui/*.cpp "auto/SpinachGUI.cpp")
file(GLOB sharedSources shared/*.cpp)

set(formatSources 
  "shared/formats/xyz.cpp"
  "shared/formats/g03.cpp"
  #  "shared/formats/xml.cpp"
  "shared/formats/castep.cpp"
  "shared/formats/simpson.cpp"
)

file(GLOB cmdSources commandln/*.cpp)


############################################################
# Link and include directories
# If you include via absolute paths, you can move source files about
# more easily.
include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(SYSTEM ${wxWidgets_INCLUDE_DIRS})
include_directories(SYSTEM ${CMAKE_CURRENT_BINARY_DIR})
include_directories(SYSTEM ${PYTHON_INCLUDE_PATH})

include_directories(SYSTEM ${CodeSynthesisXSD_INCLUDE_DIRS})
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(SYSTEM /usr/include/sigc++-2.0)
include_directories(SYSTEM /usr/lib/sigc++-2.0/include/)

link_directories(${CodeSynthesisXSD_LIBRARY_PATH})
link_directories(/home/gareth/src/CLAPACK-3.2.1)
link_directories(/home/gareth/src/CLAPACK-3.2.1/F2CLIBS)

############################################################
# Libary Targets

add_library(lapack_sample STATIC ${lapackSources})
add_library(spinxml STATIC       ${sharedSources} ${formatSources})


############################################################
# Targets are speicified here

#command line exe
set(spinachSources ${cmdSources})
add_executable (spinach ${spinachSources})
target_link_libraries (spinach 
  xerces-c
  sigc-2.0
  spinxml
  ${Boost_LIBRARIES}
  lapack_LINUX 
  blas_LINUX
  f2c
)
#C++ GUI
set(wxSpinachSources ${guiSources})
add_executable (wxSpinach ${wxSpinachSources})
add_definitions(${wxWidgets_DEFINITIONS})
target_link_libraries (wxSpinach 
${wxWidgets_LIBRARIES}  ${Boost_LIBRARIES}
GL spinxml f2c xerces-c sigc-2.0 lapack_LINUX
)



