cmake_minimum_required(VERSION 2.4.6)
include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
set(ROS_BUILD_TYPE Debug)

rosbuild_init()

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#uncomment if you have defined messages
#rosbuild_genmsg()
#uncomment if you have defined services
#rosbuild_gensrv()

rosbuild_add_library(irdetect src/blob.cpp src/graylevels.cpp src/contrast.cpp src/histogram.cpp src/diffusion.cpp src/meanvalue.cpp src/importantvalue.cpp src/blobfinal.cpp)
rosbuild_add_executable(testirdetect src/IRdetect.cpp)
target_link_libraries(testirdetect irdetect)

#test false color conversion
#rosbuild_add_library(falsecolor src/false_color.cpp)
#rosbuild_add_executable(testfalsecolor src/test/testfalsecolor.cpp)
#target_link_libraries(testfalsecolor falsecolor)



