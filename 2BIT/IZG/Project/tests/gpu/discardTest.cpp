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
#define __FILENAME__ "discradTest"
#include <tests/testCommon.hpp>

using namespace tests;

void fragmentShaderDiscard(OutFragment&o,InFragment const&i,ShaderInterface const&si){
  o.discard      = ((uint32_t)i.gl_FragCoord.x) % 2u == 0u;
  o.gl_FragColor = glm::vec4(1.f);
}

SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - discard test" << std::endl;

  auto res = glm::uvec2(100,100);
  MEMCB(res.x,res.y);

  auto backFaceCulling = true;

  auto&outVertices = memCb->outVertices;
  auto&inFragments = memCb->inFragments;
  outVertices.clear();
  outVertices.resize(3);
  outVertices[0].gl_Position = glm::vec4(-1.f,-1.f,0.f,1.f);
  outVertices[1].gl_Position = glm::vec4(+3.f,-1.f,0.f,1.f);
  outVertices[2].gl_Position = glm::vec4(-1.f,+3.f,0.f,1.f);
  inFragments.clear();

  auto&prg = mem.programs[4];
  prg.vertexShader   = vertexShaderInject;
  prg.fragmentShader = fragmentShaderDiscard;

  pushClearCommand(cb);
  pushBindProgramCommand(cb,4);
  pushDrawCommand(cb,3,backFaceCulling);

  memCb->test();

  bool success = true;

  uint32_t counter = 0;
  for(uint32_t y=0;y<res.y;++y)
    for(uint32_t x=0;x<res.y;++x){
      auto color = getColorF(memCb->studentMem.mem.framebuffers[0],x,y);
      if(color.x == 1.f && color.y == 1.f && color.z == 1.f)
        ++counter;
    }

  uint32_t expectedCount = res.x * res.y / 2;

  success &= counter == expectedCount;

  if(!breakTest()&&success)return;
  std::cerr << R".(
  Tento test zkouší, zda funguje discard flag v OutFragment struktuře.
  Pokud je discard flag nastaven, měl by se fragment zahodit a neměl by se nijak použít.

  Tento test selhal, protože to neplatí.

  Kreslí se trojúhelník přes celou obrazovku.
  A polovina fragmentů obdržela discard == true.

  Množství obarvených fragmentů by mělo odpovídat polovině pixelů ve framebufferu.
  )." << std::endl;

  std::cerr << "  Celkový počet fragmentů: " << res.x*res.y << std::endl;
  std::cerr << "  Očekávaný počet fragmentů, které obarvily framebuffer: " << expectedCount << std::endl;
  std::cerr << "  Počet fragmentů, které obarvily framebuffer: " << counter << std::endl;


  REQUIRE(false);
}
