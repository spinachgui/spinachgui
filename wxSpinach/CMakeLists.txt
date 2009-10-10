cmake_minimum_required(VERSION 2.6)
project(cspinach)

#Tell me what commands are being executed.
#set(CMAKE_VERBOSE_MAKEFILE TRUE)



#Enclose this in some sort of "if" if more than one 
#compiler is ever supported
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ggdb -Wno-write-strings") #Make gcc output all the debug information it can!

#Choose a build type

#Default build. The worst of both worlds
#set(CMAKE_BUILD_TYPE None)

#Debug build, can use with gdb
set(CMAKE_BUILD_TYPE Debug)

#Compile a release build
#set(CMAKE_BUILD_TYPE Release)

#Work out where all the built files are going
set(buildDir ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY})

#GDB can debug an optermised executable. It's one of a very small number of debuggers that can.
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
  #find_package(wxWidgets COMPONENTS net xml adv aui core gl html propgrid qa richtext stc xrc REQUIRED)
  find_package(wxWidgets COMPONENTS gl adv aui REQUIRED)

  #Work out where python is installed.
  find_package(PythonLibs)

  #Work out where boost is installed
  find_package(Boost REQUIRED)

  #set the version of gcc to use. Currently g++-4.3 doesn't work.
  #set(CMAKE_CXX_COMPILER "g++-4.2")
endif()

#List of source files for all the targets
set(guiSources "gui/SpinachApp.cpp")
set(guiSources ${guiSources} "gui/glDisplay.cpp")
set(guiSources ${guiSources} "gui/InteractionEdit.cpp")
set(guiSources ${guiSources} "gui/OrientationEdit.cpp")
set(guiSources ${guiSources} "gui/EventSystem.cpp")
set(guiSources ${guiSources} "gui/SpinDialog.cpp")
set(guiSources ${guiSources} "gui/SpinGrid.cpp")
set(guiSources ${guiSources} "gui/SpinSysManager.cpp")
set(guiSources ${guiSources} "gui/RootFrame.cpp")
set(guiSources ${guiSources} "gui/StdEvents.cpp")
set(guiSources ${guiSources} "res/SpinachGUI.cpp")

set(sharedSources "shared/nuclear_data.cpp")
set(sharedSources ${sharedSources} "shared/spinsys.cpp")
set(sharedSources ${sharedSources} "shared/xmlFileOpps.cpp")
set(sharedSources ${sharedSources} "shared/spinxml_schema.cpp")

set(pySources              "shared/spinsys.cpp")
set(pySources ${pySources} "shared/xmlFileOpps.cpp")
set(pySources ${pySources} "shared/swig/spinsys_wrap.cpp")
set(pySources ${pySources} "shared/spinxml_schema.cpp")

set(nuclearSources  "shared/nuclear_data.cpp")
set(nuclearSources ${nuclearSources} "shared/swig/nuclear_data_wrap.cpp")

set(cmdSources "commandln/main.cpp")

#Use the Spin System XSD schema. to generate parsing classes
#There's a bit of a hack here. We're assuming that we're building
#in a subdirectory such as build/ so this build process would fail
#if it were ever tried elsewhere. I'm not sure how to fix this yet.

set(xsd_args cxx-tree )
#set(xsd_args ${xsd_args} --generate-default-ctor)
set(xsd_args ${xsd_args} --generate-serialization)
#set(xsd_args ${xsd_args} --type-naming java)
#set(xsd_args ${xsd_args} --function-naming java)
set(xsd_args ${xsd_args} --generate-doxygen)
set(xsd_args ${xsd_args} --cxx-suffix .cpp --hxx-suffix .hpp)
set(xsd_args ${xsd_args} --output-dir ${CMAKE_CURRENT_BINARY_DIR}/shared)
set(xsd_args ${xsd_args} --generate-doxygen)

add_custom_command(OUTPUT shared/spinxml_schema.cpp
                   MAIN_DEPENDENCY data/spinxml_schema.xsd
                   COMMAND COMMAND ${xsd} ${xsd_args}
                   ${CMAKE_CURRENT_SOURCE_DIR}/data/spinxml_schema.xsd
                   COMMENT "Automatically generating XML parsing classes")

#Use swig to generate the python bindings for the C++ code.
set(swig_args -c++ -python 
              -I${CMAKE_CURRENT_SOURCE_DIR}
              -outdir ${CMAKE_CURRENT_BINARY_DIR})


#Regenerate the documentation as part of the build
#There's a bit of a hack here. We're assuming that we're building
#in a subdirectory such as build/ so this build process would fail
#if it were ever tried elsewhere. I'm not sure how to fix this yet.
add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/doc/html/index.html
				   COMMAND doxygen ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile
				   MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile
				   IMPLICIT_DEPENDS CXX ${sharedSources} ${guiSources} ${cmdSources}
				   COMMENT "Regenerating the documentation")




set(swig_file -o ${CMAKE_CURRENT_BINARY_DIR}/shared/swig/spinsys_wrap.cpp
              -oh ${CMAKE_CURRENT_BINARY_DIR}/shared/swig/spinsys_wrap.hpp
               ${CMAKE_CURRENT_SOURCE_DIR}/shared/swig/spinsys.i) 
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shared/swig/spinsys_wrap.cpp
                   MAIN_DEPENDENCY shared/swig/spinsys.i
		   DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/shared/spinxml_schema.cpp sxohared/spinsys.cpp
                   COMMAND COMMAND swig ${swig_args} ${swig_file}
                   COMMENT "Automatically generating  python bindings")



set(swig_file_nuclear -o ${CMAKE_CURRENT_BINARY_DIR}/shared/swig/nuclear_data_wrap.cpp
                      -oh ${CMAKE_CURRENT_BINARY_DIR}/shared/swig/nuclear_data_wrap.hpp
                      ${CMAKE_CURRENT_SOURCE_DIR}/shared/swig/nuclear_data.i) 
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shared/swig/nuclear_data_wrap.cpp
                   MAIN_DEPENDENCY shared/swig/nuclear_data.i
		   DEPENDS shared/nuclear_data.cpp 
                   COMMAND COMMAND swig ${swig_args} ${swig_file_nuclear}
                   COMMENT "Automatically exporting the nuclear data tables to python")




include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}) #If you include via absolute paths, you can move source files about more easily.
include_directories(SYSTEM ${wxWidgets_INCLUDE_DIRS})
include_directories(SYSTEM ${CMAKE_CURRENT_BINARY_DIR})
include_directories(SYSTEM ${PYTHON_INCLUDE_PATH})

include_directories(SYSTEM ${CodeSynthesisXSD_INCLUDE_DIRS})
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

link_directories(${CodeSynthesisXSD_LIBRARY_PATH})

#python bindings

#add_library(_nuclear_data SHARED  ${nuclearSources})
#target_link_libraries (_nuclear_data  ${PYTHON_LIBRARIES})

#add_library(_spinsys SHARED  ${pySources})
#target_link_libraries (_spinsys _nuclear_data xerces-c ${PYTHON_LIBRARIES})



#C++ GUI
set(wxSpinachSources ${guiSources} ${sharedSources})
add_executable (wxSpinach ${wxSpinachSources})
add_definitions(${wxWidgets_DEFINITIONS})
target_link_libraries (wxSpinach ${wxWidgets_LIBRARIES}) 
#target_link_libraries (wxSpinach atlas) 
target_link_libraries (wxSpinach GL GLEW) 
target_link_libraries (wxSpinach xerces-c)


#command line exe
#set(spinachSources ${cmdSources} ${sharedSources})
#add_executable (spinach ${spinachSources})
#target_link_libraries (spinach atlas  xerces-c) 
#target_link_libraries (spinach  xerces-c) 


#Create an "install" target (copy everything to a test directory)
#This is probaly not how to do it in a production build, but it works and will
#do for now
#add_custom_target(install-res ALL cp ${CMAKE_CURRENT_SOURCE_DIR}/res/* ${CMAKE_CURRENT_BINARY_DIR}/install/res
#                          DEPENDS _spinsys  
#                          COMMENT "Copying files to /install")
#
#
#add_custom_target(install-py ALL cp ${CMAKE_CURRENT_SOURCE_DIR}/py/* ${CMAKE_CURRENT_BINARY_DIR}/install
#                          DEPENDS _spinsys  
#                          COMMENT "Copying files to /install")
#
#

#add_custom_target(install ALL cp ${CMAKE_CURRENT_BINARY_DIR}/*.py  ${CMAKE_CURRENT_BINARY_DIR}/install/
#                          DEPENDS _spinsys  
#                          COMMENT "Copying files to /install")
#
#

#add_custom_target(install-so ALL cp ${CMAKE_CURRENT_BINARY_DIR}/lib_spinsys.so ${CMAKE_CURRENT_BINARY_DIR}/install/_spinsys.so
#                          DEPENDS _spinsys  
#                          COMMENT "Copying files to /install")
#
#add_custom_target(install-so2 ALL cp ${CMAKE_CURRENT_BINARY_DIR}/lib_nuclear_data.so ${CMAKE_CURRENT_BINARY_DIR}/install/_nuclear_data.so
#                          DEPENDS _spinsys  
#                          COMMENT "Copying files to /install")
#
#
#add_custom_target(install-data ALL cp ${CMAKE_CURRENT_SOURCE_DIR}/data/* ${CMAKE_CURRENT_BINARY_DIR}/install/data
#                          DEPENDS _spinsys  
#                          COMMENT "Copying files to /install")
#
#
#