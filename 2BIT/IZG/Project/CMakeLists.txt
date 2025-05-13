cmake_minimum_required(VERSION 3.12.0)

PROJECT(izgProject)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_C_STANDARD 99)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1 ) 

set(STUDENT_SOURCES
  student/fwd.hpp
  student/fwd.cpp
  student/gpu.hpp
  student/gpu.cpp
  student/prepareModel.hpp
  student/prepareModel.cpp
  )

set(FRAMEWORK_SOURCES
  framework/arguments.hpp
  framework/arguments.cpp
  framework/main.cpp
  framework/window.hpp
  framework/window.cpp
  framework/method.hpp
  framework/programContext.hpp
  framework/programContext.cpp
  framework/application.cpp
  framework/application.hpp
  framework/timer.hpp
  framework/bunny.hpp
  framework/bunny.cpp
  framework/textureData.hpp
  framework/textureData.cpp
  framework/model.hpp
  framework/model.cpp
  framework/systemSpecific.hpp
  framework/systemSpecific.cpp
  framework/colorPrinting.hpp
  framework/colorPrinting.cpp
  )

set(EXAMPLES_SOURCES
  examples/emptyMethod.cpp
  examples/triangleMethod.cpp
  examples/rotatingTriangles.cpp
  examples/triangleClip1Method.cpp
  examples/triangleClip2Method.cpp
  examples/triangle3DMethod.cpp
  examples/triangleBufferMethod.cpp
  examples/czFlagMethod.cpp
  examples/angry.cpp
  examples/stairs.cpp
  examples/phongMethod.cpp
  examples/texturedQuadMethod.cpp
  examples/video.cpp
  examples/skFlagMethod.cpp
  examples/edgeDetect.cpp
  examples/shadowMapping.cpp
  examples/anime.cpp
  examples/modelMethod.cpp
  examples/modelMethod.hpp
  examples/shadowModel.hpp
  examples/shadowModel.cpp
  )

set(LIBS_SOURCES
  libs/tiny_gltf/tiny_gltf.h
  libs/stb_image/stb_image.h
  libs/stb_image/stb_image_write.h
  libs/json/json.hpp
  libs/impl.cpp
  )

set(TESTS_SOURCES 
  tests/testCommon.hpp
  tests/testCommon.cpp
  tests/memDeepCopy.hpp
  tests/memDeepCopy.cpp
  tests/gpuMemToStr.hpp
  tests/gpuMemToStr.cpp
  tests/commandBufferToStr.hpp
  tests/commandBufferToStr.cpp
  tests/modelTestUtils.hpp
  tests/modelTestUtils.cpp

  tests/renderMethodFrame.hpp
  tests/renderMethodFrame.cpp
  tests/takeScreenShot.hpp
  tests/takeScreenShot.cpp
  tests/conformanceTests.hpp
  tests/conformanceTests.cpp
  tests/performanceTest.hpp
  tests/performanceTest.cpp

  tests/isMemDifferent.hpp
  tests/isMemDifferent.cpp

  # Commands tests
  tests/commands/clear.cpp
  tests/commands/bindFramebufferTests.cpp
  tests/commands/bindProgramTests.cpp
  tests/commands/bindVertexArrayTests.cpp
  tests/commands/framebufferTests.cpp
  tests/commands/drawTests.cpp
  tests/commands/drawID.cpp
  tests/commands/setDrawId.cpp
  tests/commands/subCommandTests.cpp
  tests/commands/commandTests.cpp

  # GPU tests
  tests/gpu/common.hpp
  tests/gpu/common.cpp
  tests/gpu/gl_VertexID.cpp
  tests/gpu/vs_interface.cpp
  tests/gpu/vertexArrayTests.cpp
  tests/gpu/rasterization.cpp
  tests/gpu/perspectiveDivision.cpp
  tests/gpu/gl_FragCoord.cpp
  tests/gpu/barycentrics.cpp
  tests/gpu/discardTest.cpp
  tests/gpu/writeToFBO.cpp
  tests/gpu/pfo.cpp
  tests/gpu/clippingTests.cpp

  # MODEL tests
  tests/traverseModelTests.cpp
  tests/drawModelTests.cpp
  tests/vertexShader.cpp
  tests/fragmentShader.cpp
  tests/finalImageTest.cpp

  tests/createModel.hpp
  tests/createModel.cpp
  tests/getExpectedPreparedModel.hpp
  tests/getExpectedPreparedModel.cpp
  )

source_group("student"   FILES ${STUDENT_SOURCES})
source_group("framework" FILES ${FRAMEWORK_SOURCES})
source_group("examples"  FILES ${EXAMPLES_SOURCES})
source_group("libs"      FILES ${LIBS_SOURCES})
source_group("tests"     FILES ${TESTS_SOURCES})

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE libs/glm-0.9.9.8)

add_subdirectory(libs/MealyMachine)
add_subdirectory(libs/TxtUtils)
add_subdirectory(libs/ArgumentViewer)
add_subdirectory(libs/BasicCamera)
add_subdirectory(libs/Catch2-3.3.1)

option(SDL_SHARED "" OFF)
option(SDL_STATIC "" ON)
add_subdirectory(libs/SDL-release-2.30.2)

add_executable(${PROJECT_NAME} ${STUDENT_SOURCES} ${FRAMEWORK_SOURCES} ${EXAMPLES_SOURCES} ${LIBS_SOURCES} ${TESTS_SOURCES})

target_link_libraries(${PROJECT_NAME} 
  SDL2::SDL2
  SDL2::SDL2main
  ArgumentViewer::ArgumentViewer
  BasicCamera::BasicCamera
  Catch2::Catch2
  )

if (CMAKE_CROSSCOMPILING)
  target_link_libraries(${PROJECT_NAME}  
    -lmingw32
    -static-libgcc 
    -static-libstdc++
    )
endif()


target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/json)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/stb_image)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

option(CLEAR_CMAKE_ROOT_DIR "if this is set, #define CMAKE_ROOT_DIR will be .")

if(NOT CLEAR_CMAKE_ROOT_DIR)
  target_compile_definitions(${PROJECT_NAME} PUBLIC CMAKE_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
else()
  target_compile_definitions(${PROJECT_NAME} PUBLIC CMAKE_ROOT_DIR=".")
endif()
