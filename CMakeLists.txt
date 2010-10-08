cmake_minimum_required(VERSION 2.6)
project(popi)

############################################################
# Enable Testing

enable_testing()

############################################################
#Tell me what commands are being executed.
set(CMAKE_VERBOSE_MAKEFILE FALSE)

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
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ggdb  -Wno-write-strings ")


find_package(Boost REQUIRED program_options unit_test_framework)
set(CMAKE_CXX_COMPILER "g++")

############################################################
#List of source files for all the targets

file(GLOB sharedSources shared/*.cpp)

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

add_library(spinxml STATIC       ${sharedSources})

############################################################
# Tests specified here

add_executable(InteractionRawTest "shared/test/interactions_test.cpp")
target_link_libraries(InteractionRawTest ${Boost_LIBRARIES} spinxml)
add_test(InteractionRawTest ${EXECUTABLE_OUTPUT_PATH}/InteractionRawTest)

add_executable(OrientationTest "shared/test/orientation_test.cpp" )
target_link_libraries(OrientationTest spinxml ${Boost_LIBRARIES} spinxml)
add_test(OrientationTest ${EXECUTABLE_OUTPUT_PATH}/OrientationTest)

add_executable(SpinTest "shared/test/spin_test.cpp" )
target_link_libraries(SpinTest spinxml ${Boost_LIBRARIES} spinxml)
add_test(SpinTest ${EXECUTABLE_OUTPUT_PATH}/SpinTest)

