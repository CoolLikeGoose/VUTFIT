#include <catch2/catch_test_macros.hpp>

#define __FILENAME__ "commandTests"

#include <tests/testCommon.hpp>
#include <tests/commandBufferToStr.hpp>
#include <tests/gpuMemToStr.hpp>

#include <iostream>
#include <string.h>
#include <sstream>
#include<functional>
#include<map>

#include <algorithm>
#include <numeric>
#include <iostream>

#include <student/gpu.hpp>

using namespace tests;



SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - mixed clear and draw commands" << std::endl;

  MEMCB(100,100);
  mem.framebuffers[0].color.data = nullptr;
  mem.framebuffers[0].depth.data = nullptr;

  mem.programs[0].  vertexShader   =   vertexShaderDump0;
  mem.programs[0].fragmentShader   = fragmentShaderEmpty;
  mem.programs[1].  vertexShader   =   vertexShaderDump1;
  mem.programs[1].fragmentShader   = fragmentShaderEmpty;
  mem.programs[2].  vertexShader   =   vertexShaderDump2;
  mem.programs[2].fragmentShader   = fragmentShaderEmpty;
  mem.programs[3].  vertexShader   =   vertexShaderDump3;
  mem.programs[3].fragmentShader   = fragmentShaderEmpty;

  pushClearCommand(cb,glm::vec4(.1,.1,.1,1),3,true,true);
  pushBindProgramCommand(cb,0);
  pushDrawCommand(cb,3);
  pushClearCommand(cb,glm::vec4(.1,.1,.1,1),4,false,true);
  pushBindProgramCommand(cb,1);
  pushDrawCommand(cb,6);
  pushBindProgramCommand(cb,2);
  pushDrawCommand(cb,9);
  pushClearCommand(cb,glm::vec4(.1,.1,.1,1),5,false,true);
  pushBindProgramCommand(cb,3);
  pushDrawCommand(cb,12);

  if(memCb->test(1,DumpDiff::DIFFERENT_CLEAR))return;

  std::cerr << R".(
  TEST SELHAL!

  Tento test zkouší command buffer s několik promíchanými příkazy.)." << std::endl;

  memCb->printStatus();

  std::cerr<<R".(

  Správně zpracovávejte příkazy v command bufferu!
  ).";
  REQUIRE(false);
}


