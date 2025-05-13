#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <string.h>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
#include <cstring>

#include <glm/gtc/matrix_transform.hpp>

#include <student/gpu.hpp>

#define __FILENAME__ "framebufferTests"

#include <tests/testCommon.hpp>

using namespace tests;

SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - framebuffer test" << std::endl;

  MEMCB(100,100);

  auto secondFramebuffer = createFramebuffer(50,50);

  mem.framebuffers[1] = secondFramebuffer.frame;

  //auto&prg = mem.programs[4];
  //prg.vertexShader   = vertexShaderInject;
  //prg.fragmentShader = fragmentShaderDump;

  auto eColor0 = glm::vec4(1.f,0.f,0.f,1.f);
  auto eDepth0 = 1000.f;
  auto eColor1 = glm::vec4(0.f,0.f,1.f,1.f);
  auto eDepth1 =  100.f;

  pushBindFramebufferCommand(cb,1);
  pushClearCommand(cb,eColor1,eDepth1);
  pushBindFramebufferCommand(cb,0);
  pushClearCommand(cb,eColor0,eDepth0);

  if(memCb->test(1,DumpDiff::DIFFERENT_CLEAR))return;

  //bool success = true;

  //auto channels0 = mem.framebuffers[0].color.channels;
  //auto channels1 = mem.framebuffers[1].color.channels;

  //auto sColor0 = getColorF(mem.framebuffers[0],40,30);
  //auto sDepth0 = getDepth (mem.framebuffers[0],40,30);
  //auto sColor1 = getColorF(mem.framebuffers[1],30,30);
  //auto sDepth1 = getDepth (mem.framebuffers[1],30,30);


  //success &= sameColor(eColor0,sColor0,channels0);
  //success &= sameColor(eColor1,sColor1,channels1);
  //success &= eDepth0 == sDepth0;
  //success &= eDepth1 == sDepth1;

  //if(!breakTest()&&success)return;
  std::cerr << R".(
  Tento test zkouší, zda je možné využívat vícero framebufferů.
  Struktura GPUMemory obsahuje identifikátor framebuffer - activatedFramebuffer.
  Pomocí něj je možné zvolit framebuffer, který se využívá.

  Příkaz BindFramebufferCommand slouží pro nastavení daného indentifikátoru.

  Příkazy, který framebuffer využívájí (kreslení, čistění) se provádí nad
  vybraným framebufferem.

  Tento test selhal, protože to neplatí.

  Tento test využívá dva framebuffery.

  Test slehal, protože jsou ve framebufferech špatné hodnoty.)." << std::endl;
  //std::cerr << "  Framebuffer[0]:"<< std::endl;
  //std::cerr << "    očekávaná barva  : " << colorToStr(eColor0,channels0) << std::endl;
  //std::cerr << "    očekávaná hloubka: " <<            eDepth0            << std::endl;
  //std::cerr << "    vaše barva       : " << colorToStr(sColor0,channels0) << std::endl;
  //std::cerr << "    vaše hloubka     : " <<            sDepth0            << std::endl;
  //std::cerr << "  Framebuffer[1]:"<< std::endl;
  //std::cerr << "    očekávaná barva  : " << colorToStr(eColor1,channels1) << std::endl;
  //std::cerr << "    očekávaná hloubka: " <<            eDepth1            << std::endl;
  //std::cerr << "    vaše barva       : " << colorToStr(sColor1,channels1) << std::endl;
  //std::cerr << "    vaše hloubka     : " <<            sDepth1            << std::endl;


  memCb->printDetails();


  REQUIRE(false);
}
