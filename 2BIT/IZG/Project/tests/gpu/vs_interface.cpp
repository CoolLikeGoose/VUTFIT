#include <iostream>

#include <catch2/catch_test_macros.hpp>

#define __FILENAME__ "vs_interface"
#include<tests/testCommon.hpp>

using namespace tests;

SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - vertex shader, shader interface" << std::endl;

  MEMCB(10,10);
  mem.framebuffers[0].color.data = nullptr;
  mem.framebuffers[0].depth.data = nullptr;

  auto&prg = mem.programs[9];
  prg.vertexShader   = vertexShaderDump0  ;
  prg.fragmentShader = fragmentShaderEmpty;

  pushBindProgramCommand(cb,9);
  pushDrawCommand(cb,3);

  if(memCb->test(1,DumpDiff::DIFFERENT_SHADERINTERFACE))return;

  std::cerr << R".(
  TEST SELHAL

  Vertex Shader by měl obdržet správný ShaderInterface.
  Shader interface obsahuje uniformní proměnné a textury.
  Obě jsou uloženy v paměti grafické karty GPUMemory.
  ).";
  REQUIRE(false);
}
