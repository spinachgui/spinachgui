cmake_minimum_required(VERSION 2.6)
project(popi)

############################################################
# Enable Testing

enable_testing()

############################################################
#Tell me what commands are being executed.
#set(CMAKE_VERBOSE_MAKEFILE TRUE)

# Make gcc output all the debug information it can! Enclose this in
#some sort of "if" if more than one compiler is ever supported
set(CMAKE_CXX_FLAGS "-D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ -DEIGEN_DONT_ALIGN")
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
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ggdb  -Wno-write-strings ")

############################################################
# Do automatic library detection

find_package(wxWidgets COMPONENTS gl adv aui REQUIRED)
find_package(Boost REQUIRED program_options filesystem unit_test_framework) 
#set(Boost_INCLUDE_DIRS /usr/shared/include)
set(Boost_LIBRARIES  -lboost_program_options -lboost_unit_test_framework -lboost_filesystem)

############################################################
#List of source files for all the targets

file(GLOB sharedSources shared/*.cpp)
file(GLOB formatSources shared/formats/*.cpp)
file(GLOB guiSources gui/*.cpp)
set(wxformbuilderSources ${CMAKE_CURRENT_BINARY_DIR}/auto/SpinachGUI.cpp)
set(xmlSources           ${CMAKE_CURRENT_BINARY_DIR}/auto/spinxml_schema.cpp)

############################################################
# Link and include directories
# If you include via absolute paths, you can move source files about
# more easily.
include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(SYSTEM ${wxWidgets_INCLUDE_DIRS})
include_directories(SYSTEM ${CMAKE_CURRENT_BINARY_DIR})

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(SYSTEM /usr/include/sigc++-2.0)
include_directories(SYSTEM /usr/lib/sigc++-2.0/include/)
include_directories(SYSTEM /usr/local/include/eigen3/)

############################################################
# Link the non graphical code into a library

add_library(spinxml STATIC ${xmlSources} ${sharedSources} ${formatSources})

############################################################
# GUI code build here

add_executable(SpinachGUI ${guiSources} ${wxformbuilderSources})
target_link_libraries(SpinachGUI ${Boost_LIBRARIES} spinxml xerces-c sigc-2.0  ${wxWidgets_LIBRARIES} GL)

############################################################
# Tests specified here

add_executable(InteractionRawTest "shared/test/interaction_test.cpp")
target_link_libraries(InteractionRawTest ${Boost_LIBRARIES} spinxml sigc-2.0)
add_test(InteractionRawTest ${EXECUTABLE_OUTPUT_PATH}/InteractionRawTest)

add_executable(OrientationTest "shared/test/orientation_test.cpp" )
target_link_libraries(OrientationTest spinxml ${Boost_LIBRARIES} spinxml sigc-2.0)
add_test(OrientationTest ${EXECUTABLE_OUTPUT_PATH}/OrientationTest)

add_executable(SpinTest "shared/test/spin_test.cpp" )
target_link_libraries(SpinTest spinxml ${Boost_LIBRARIES} sigc-2.0)
add_test(SpinTest ${EXECUTABLE_OUTPUT_PATH}/SpinTest)

add_executable(SpinSystemTest "shared/test/spinsys_test.cpp" )
target_link_libraries(SpinSystemTest spinxml ${Boost_LIBRARIES}  sigc-2.0)
add_test(SpinSystemTest ${EXECUTABLE_OUTPUT_PATH}/SpinSystemTest)

add_executable(FrameTest "shared/test/frame_test.cpp" )
target_link_libraries(FrameTest spinxml ${Boost_LIBRARIES}  sigc-2.0)
add_test(FrameTest ${EXECUTABLE_OUTPUT_PATH}/SpinSystemTest)

add_executable(FormatTest "shared/test/format_test.cpp" )
target_link_libraries(FormatTest spinxml ${Boost_LIBRARIES} sigc-2.0 xerces-c)
add_test(FrameTest ${EXECUTABLE_OUTPUT_PATH}/FormatTest)


############################################################
# Copy the dat directory
file(COPY data/ DESTINATION data/)
file(COPY examples/ DESTINATION examples/)