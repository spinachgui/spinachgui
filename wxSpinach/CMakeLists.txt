cmake_minimum_required(VERSION 2.6)
project(cspinach)

#Tell me what commands are being executed.
#set(CMAKE_VERBOSE_MAKEFILE TRUE)

#find_package(wxWidgets COMPONENTS net xml adv aui core gl html propgrid qa richtext stc xrc REQUIRED)
find_package(wxWidgets COMPONENTS gl REQUIRED)

#set the version of gcc to use. Currently g++-4.3 doesn't work.
set(CMAKE_CXX_COMPILER "g++-4.2")


#Enclose this in some sort of "if" if more than one 
#compiler is ever supported
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ggdb") #Make gcc output all the debug information it can!

#Choose a build type

#Default build. The worst of both worlds
#set(CMAKE_BUILD_TYPE None)

#Debug build, can use with gdb
set(CMAKE_BUILD_TYPE Debug)

#Compile a release build
#set(CMAKE_BUILD_TYPE Release)

#GDB can debug an optermised executable. It's one of a very small number of debuggers that can.
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ggdb")

set(wxWidgets_USE_DEBUG,ON)
set(wxWidgets_USE_UNICODE,ON)
set(wxWidgets_USE_UNIVERSAL,OFF)
set(wxWidgets_USE_STATIC,ON)

set(guiSources "gui/SpinachApp.cpp")
set(guiSources ${guiSources} "gui/calcFrame.cpp")
set(guiSources ${guiSources} "gui/glMolDisplay.cpp")
set(guiSources ${guiSources} "gui/optionFrame.cpp")
set(guiSources ${guiSources} "gui/rootFrame.cpp")
set(guiSources ${guiSources} "gui/rotationDialog.cpp")
set(guiSources ${guiSources} "gui/spinpgbind.cpp")
set(guiSources ${guiSources} "gui/spinachGUI.cpp")

set(sharedSources "shared/nuclear_data.cpp")
set(sharedSources ${sharedSources} "shared/spin.cpp")
set(sharedSources ${sharedSources} "shared/spin_system.cpp")
set(sharedSources ${sharedSources} "../shared/spinsys_spec.cpp")


#Use the Spin System XSD spec. to generate parsing classes
#There's a bit of a hack here. We're assuming that we're building
#in a subdirectory such as build/ so this build process would fail
#if it were ever tried elsewhere. I'm not sure how to fix this yet.
set(xsd_args cxx-tree --generate-serialization --type-naming java --function-naming java  --generate-doxygen --cxx-suffix .cpp --hxx-suffix .hpp --output-dir ../shared ../data/spinsys_spec.xsd )
add_custom_command(OUTPUT shared/spinsys_spec.cpp
				   MAIN_DEPENDENCY data/spinsys_spec.xsd
                   COMMAND COMMAND xsdcxx ${xsd_args}
				   COMMENT "Automatically generating XML parsing classes")

set(cmdSources "commandln/main.cpp")

include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}) #If you include via absolute paths, you can move source files about more easily.
include_directories(SYSTEM ${wxWidgets_INCLUDE_DIRS})

add_definitions(${wxWidgets_DEFINITIONS})

link_directories("/usr/lib/sse2")

set(wxSpinachSources ${guiSources} ${sharedSources})
add_executable (wxSpinach ${wxSpinachSources})

target_link_libraries (wxSpinach ${wxWidgets_LIBRARIES}) 
target_link_libraries (wxSpinach atlas) 
target_link_libraries (wxSpinach GL GLEW) 
target_link_libraries (wxSpinach xerces-c)

#target_link_libraries (wxSpinach wxcode_gtk2u_propgrid-2.8)

set(spinachSources ${cmdSources} ${sharedSources})
add_executable (spinach ${spinachSources})
target_link_libraries (spinach atlas  xerces-c) 


