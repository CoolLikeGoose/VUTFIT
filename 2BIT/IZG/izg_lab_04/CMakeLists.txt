CMAKE_MINIMUM_REQUIRED(VERSION 3.15)
PROJECT( izg04 )

 #cmake_policy(SET CMP0144 NEW)

include(${CMAKE_SOURCE_DIR}/CMake/CMakeMarcos.cmake)

findOsgLib()
#findImgui()

CreateMainTarget("izg04")

CreateTestsTarget()  
