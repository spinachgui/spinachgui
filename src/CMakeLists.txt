cmake_minimum_required(VERSION 2.6)
project(popi)

############################################################
#Tell me what commands are being executed.
#set(CMAKE_VERBOSE_MAKEFILE TRUE)

# Make gcc output all the debug information it can! Enclose this in
#some sort of "if" if more than one compiler is ever supported
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ggdb -Wno-write-strings ")

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

set(xsd xsdcxx)

if(WIN32)
  set(xsd xsd)
  include("windows-config.cmake")
else()
  find_package(wxWidgets COMPONENTS gl adv aui REQUIRED)
  find_package(PythonLibs)
  find_package(Boost REQUIRED program_options signals)
  #set(CMAKE_CXX_COMPILER "g++-4.2")
endif()

############################################################
#List of source files for all the targets
set(guiSources "gui/SpinachApp.cpp")
set(guiSources ${guiSources} "gui/Display3D.cpp")
set(guiSources ${guiSources} "gui/MolSceneGraph.cpp")
set(guiSources ${guiSources} "gui/InteractionEdit.cpp")
set(guiSources ${guiSources} "gui/SpinInteractionEdit.cpp")
set(guiSources ${guiSources} "gui/OrientationEdit.cpp")
set(guiSources ${guiSources} "gui/SpinDialog.cpp")
set(guiSources ${guiSources} "gui/SpinGrid.cpp")
set(guiSources ${guiSources} "gui/SpinSysManager.cpp")
set(guiSources ${guiSources} "gui/RootFrame.cpp")
set(guiSources ${guiSources} "gui/RightClickMenu.cpp")
set(guiSources ${guiSources} "auto/SpinachGUI.cpp")

set(sharedSources "shared/nuclear_data.cpp")
set(sharedSources ${sharedSources} "shared/spinsys.cpp")
set(sharedSources ${sharedSources} "shared/mathtypes.cpp")
set(sharedSources ${sharedSources} "auto/spinxml_schema.cpp")

set(formatSources "shared/formats/xyz.cpp")
set(formatSources ${formatSources} "shared/formats/g03.cpp")
set(formatSources ${formatSources} "shared/formats/xml.cpp")

set(pySources              "shared/spinsys.cpp")
set(pySources ${pySources} "shared/swig/spinsys_wrap.cpp")
set(pySources ${pySources} "shared/spinxml_schema.cpp")

set(nuclearSources  "shared/nuclear_data.cpp")
set(nuclearSources ${nuclearSources} "shared/swig/nuclear_data_wrap.cpp")

set(cmdSources "commandln/main.cpp")

############################################################
 #If you include via absolute paths, you can move source files about
 #more easily.
include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(SYSTEM ${wxWidgets_INCLUDE_DIRS})
include_directories(SYSTEM ${CMAKE_CURRENT_BINARY_DIR})
include_directories(SYSTEM ${PYTHON_INCLUDE_PATH})

include_directories(SYSTEM ${CodeSynthesisXSD_INCLUDE_DIRS})
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(SYSTEM /usr/include/sigc++-2.0)
include_directories(SYSTEM /usr/lib/sigc++-2.0/include/)



link_directories(${CodeSynthesisXSD_LIBRARY_PATH})

#command line exe
set(spinachSources ${cmdSources} ${sharedSources} ${formatSources})
add_executable (spinach ${spinachSources})
target_link_libraries (spinach xerces-c -lsigc-2.0) 
target_link_libraries(spinach ${Boost_LIBRARIES})

#C++ GUI
set(wxSpinachSources ${guiSources} ${sharedSources} ${formatSources})
add_executable (wxSpinach ${wxSpinachSources})
add_definitions(${wxWidgets_DEFINITIONS})
target_link_libraries (wxSpinach ${wxWidgets_LIBRARIES}) 
target_link_libraries (wxSpinach GL GLEW) 
target_link_libraries (wxSpinach xerces-c -lsigc-2.0)
target_link_libraries (wxSpinach ${Boost_LIBRARIES})



