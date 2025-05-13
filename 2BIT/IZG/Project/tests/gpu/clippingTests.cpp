#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <string.h>

#include <algorithm>
#include <numeric>

#include <student/gpu.hpp>
#include <framework/method.hpp>
#define __FILENAME__ "clippingTests"
#include <tests/testCommon.hpp>

using namespace tests;

#include <glm/gtc/matrix_transform.hpp>

SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - clipping - CW triangle behind near plane" << std::endl;

  uint32_t w = 100;
  uint32_t h = 100;
  MEMCB(w,h);

  auto&inFragments = memCb->inFragments;
  auto&outVertices = memCb->outVertices;

  OutVertex a;
  outVertices.clear();
  inFragments.clear();

  outVertices.push_back({{},glm::vec4(-1.f,-1.f,-7.f,-1.f)});
  outVertices.push_back({{},glm::vec4(+1.f,-1.f,-7.f,-1.f)});
  outVertices.push_back({{},glm::vec4(-1.f,+1.f,-7.f,-1.f)});

  outVertices.push_back({{},glm::vec4(-1.f,+1.f,+0.f,+1.f)});
  outVertices.push_back({{},glm::vec4(+1.f,-1.f,+0.f,+1.f)});
  outVertices.push_back({{},glm::vec4(+1.f,+1.f,+0.f,+1.f)});

  auto&prg = mem.programs[3];
  prg.vertexShader = vertexShaderInject;
  prg.fragmentShader = fragmentShaderDump;

  pushClearCommand(cb);
  pushBindProgramCommand(cb,3);
  pushDrawCommand(cb,(uint32_t)outVertices.size());

  memCb->test();

  auto studentCount = inFragments.size();
  auto expectedCount = w*h/2;
  auto tol = w+h+h;

  if(!breakTest()&&equalCounts(expectedCount,studentCount,tol))return;

  std::cerr << "  Test selhal!" << std::endl;
  std::cerr << "  Očekávaný počet fragmentů: " << expectedCount << std::endl;
  std::cerr << "  Váš počet: " << studentCount << std::endl;
  std::cerr << "  Tolerance: " << tol << std::endl;

  REQUIRE(false);
}


SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - clipping - CCW triangle behind near plane" << std::endl;

  uint32_t w = 100;
  uint32_t h = 100;

  MEMCB(w,h);

  auto&inFragments = memCb->inFragments;
  auto&outVertices = memCb->outVertices;

  OutVertex a;
  outVertices.clear();
  inFragments.clear();
  //near 1, far 2
  outVertices.push_back({{},glm::vec4(-1.f,-1.f,-7.f,1.f)});
  outVertices.push_back({{},glm::vec4(-1.f,+1.f,-7.f,1.f)});
  outVertices.push_back({{},glm::vec4(+1.f,-1.f,-7.f,1.f)});

  outVertices.push_back({{},glm::vec4(+1.f,-1.f,0.f,1.f)});
  outVertices.push_back({{},glm::vec4(-1.f,+1.f,0.f,1.f)});
  outVertices.push_back({{},glm::vec4(+1.f,+1.f,0.f,1.f)});

  auto&prg = mem.programs[2];
  prg.vertexShader   = vertexShaderInject;
  prg.fragmentShader = fragmentShaderDump;

  pushClearCommand(cb);
  pushBindProgramCommand(cb,2);
  pushDrawCommand(cb,(uint32_t)outVertices.size());

  memCb->test();

  auto studentCount = inFragments.size();
  auto expectedCount = w*h/2;
  auto tol = w+h+h;

  if(!breakTest()&&equalCounts(expectedCount,studentCount,tol))return;

  std::cerr << "  Test selhal!" << std::endl;
  std::cerr << "  Očekávaný počet fragmentů: " << expectedCount << std::endl;
  std::cerr << "  Váš počet: " << studentCount << std::endl;
  std::cerr << "  Tolerance: " << tol << std::endl;

  REQUIRE(false);
}


SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - clipping - 1 vertex behind near plane" << std::endl;

  uint32_t w = 100;
  uint32_t h = 100;

  MEMCB(w,h);

  auto&inFragments = memCb->inFragments;
  auto&outVertices = memCb->outVertices;

  OutVertex a;
  outVertices.clear();
  inFragments.clear();
  
  float nnear = 1.f;
  float ffar  = 2.f;
  float e = - (ffar + nnear) / (ffar - nnear);
  float f = - 2.f * ffar * nnear / (ffar - nnear);
  float z = 2.f/3.f;

  outVertices.push_back({{},glm::vec4(-2.f,-2.f,+2.f,+2.f)});
  outVertices.push_back({{},glm::vec4(+2.f,-2.f,+2.f,+2.f)});
  outVertices.push_back({{},glm::vec4(-z  ,+z  ,-z*e+f,+z)});


  auto&prg = mem.programs[4];
  prg.vertexShader = vertexShaderInject;
  prg.fragmentShader = fragmentShaderDump;


  pushClearCommand(cb);
  pushBindProgramCommand(cb,4);
  pushDrawCommand(cb,3);

  memCb->test();

  uint32_t const expectedCount = 100*100/8*3;
  uint32_t const err           = 2*100;

  if(expectedCount < inFragments.size())
    REQUIRE(inFragments.size() <= expectedCount + err);
  else
    REQUIRE(inFragments.size() >= expectedCount - err);
}

SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - clipping - 2 vertices behind near plane" << std::endl;

  uint32_t w = 100;
  uint32_t h = 100;

  MEMCB(w,h);

  auto&inFragments = memCb->inFragments;
  auto&outVertices = memCb->outVertices;

  OutVertex a;
  outVertices.clear();
  inFragments.clear();
  
  float nnear = 1.f;
  float ffar  = 2.f;
  float e = - (ffar + nnear) / (ffar - nnear);
  float f = - 2.f * ffar * nnear / (ffar - nnear);
  float z = 2.f/3.f;

  outVertices.push_back({{},glm::vec4(-z,-z,-z*e+f,+z)});
  outVertices.push_back({{},glm::vec4(+z,-z,-z*e+f,+z)});
  outVertices.push_back({{},glm::vec4(-2,+2,+2,+2)});

  auto&prg = mem.programs[5];
  prg.vertexShader = vertexShaderInject;
  prg.fragmentShader = fragmentShaderDump;


  pushClearCommand(cb);
  pushBindProgramCommand(cb,5);
  pushDrawCommand(cb,3);

  memCb->test();

  uint32_t const expectedCount = 100*100/8*1;
  uint32_t const err           = 2*100;

  if(expectedCount < inFragments.size())
    REQUIRE(inFragments.size() <= expectedCount + err);
  else
    REQUIRE(inFragments.size() >= expectedCount - err);
}

