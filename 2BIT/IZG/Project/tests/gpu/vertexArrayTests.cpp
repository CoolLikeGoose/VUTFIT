#include <catch2/catch_test_macros.hpp>
#define __FILENAME__ "vertexArrayTests"
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

#include <student/gpu.hpp>

#include <glm/gtc/matrix_transform.hpp>

using namespace tests;



void attributeTest(uint32_t offset = 0,uint32_t stride = sizeof(float)){
  std::cerr << "  (offset="<<offset<<",stride="<<stride<<")"<<std::endl;

  MEMCB(10,10);
  mem.framebuffers[0].color.data = nullptr;
  mem.framebuffers[0].depth.data = nullptr;

  std::vector<float> vert = {0.f,1.f,2.f,3.f,4.f,5.f,6.f,7.f,8.f,9.f,10.f,11.f,12.f,13.f};

  mem.buffers [0] =  vectorToBuffer(vert);
  auto&prg = mem.programs[8];
  prg.vertexShader   = vertexShaderDump0 ;
  prg.fragmentShader = fragmentShaderEmpty;

  auto&vao = mem.vertexArrays[4];
  vao.vertexAttrib[0].bufferID   = 0;
  vao.vertexAttrib[0].type       = AttributeType::FLOAT;
  vao.vertexAttrib[0].stride     = stride;
  vao.vertexAttrib[0].offset     = offset;

  auto elementSize = sizeof(decltype(vert)::value_type);
  auto offsetInElements = offset/elementSize;
  auto strideInElements = stride/elementSize;

  auto nofVerts = (vert.size()-offsetInElements) / strideInElements;
  nofVerts = (nofVerts/3)*3;

  pushBindProgramCommand(cb,8);
  pushBindVertexArrayCommand(cb,4);
  pushDrawCommand(cb,(uint32_t)nofVerts,0);

  if(memCb->test(1,DumpDiff::DIFFERENT_ATTRIBUTE))return;

  std::cerr << R".(

  Tento test kontroluje, jestli se do vertex shaderu posílají správná data z vertex bufferu.

  Uživatel si vytvořil buffer floatů a nastavil jeden vertex attribut.
  Poté pustil vykreslování a očekává, že se mu ve vstupním vrcholu (InVertex) do vertex shaderu objeví správná data.)." << std::endl;

  memCb->printStatus();

  memCb->printDetails();

  REQUIRE(false);
}

SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - vertex shader, attributes" << std::endl;
  attributeTest(0*sizeof(float),1*sizeof(float));
  attributeTest(3*sizeof(float),1*sizeof(float));
  attributeTest(0*sizeof(float),4*sizeof(float));
  attributeTest(2*sizeof(float),4*sizeof(float));
}


SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - vertex shader, attributes, offset, stride, multiple buffers" << std::endl;

  MEMCB(10,10);
  mem.framebuffers[0].color.data = nullptr;
  mem.framebuffers[0].depth.data = nullptr;

  uint8_t b0[400];
  uint8_t b1[400];

  memset(b0,0,sizeof(b0));
  memset(b1,0,sizeof(b1));

  ((float*)(b0+2))[0] = 100.f;
  ((float*)(b0+2))[1] = 101.f;
  ((float*)(b0+2))[2] = 102.f;
  ((float*)(b0+2))[3] = 103.f;
  ((float*)(b0+2))[4] = 104.f;
  ((float*)(b0+2))[5] = 105.f;
  ((float*)(b0+2))[6] = 106.f;

  ((float*)(b1+7))[ 0] = 10.f;
  ((float*)(b1+7))[ 1] = 11.f;
  ((float*)(b1+7))[ 2] = 12.f;
  ((float*)(b1+7))[ 3] = 13.f;
  ((float*)(b1+7))[ 4] =  0.f;
  ((float*)(b1+7))[ 5] = 14.f;
  ((float*)(b1+7))[ 6] = 15.f;
  ((float*)(b1+7))[ 7] = 16.f;
  ((float*)(b1+7))[ 8] = 17.f;
  ((float*)(b1+7))[ 9] =  0.f;
  ((float*)(b1+7))[10] = 18.f;
  ((float*)(b1+7))[11] = 19.f;
  ((float*)(b1+7))[12] = 20.f;
  ((float*)(b1+7))[13] = 21.f;

  auto&prg = mem.programs[2];
  prg.vertexShader   = vertexShaderDump0  ;
  prg.fragmentShader = fragmentShaderEmpty;
  mem.buffers[0].data = b0;
  mem.buffers[0].size = 400;
  mem.buffers[1].data = b1;
  mem.buffers[1].size = 400;

  auto&vao = mem.vertexArrays[3];
  vao.vertexAttrib[1].bufferID   = 0;
  vao.vertexAttrib[1].type       = AttributeType::VEC3;
  vao.vertexAttrib[1].stride     = sizeof(float)*2;
  vao.vertexAttrib[1].offset     = 2;

  vao.vertexAttrib[3].bufferID   = 1;
  vao.vertexAttrib[3].type       = AttributeType::VEC4;
  vao.vertexAttrib[3].stride     = sizeof(float)*5;
  vao.vertexAttrib[3].offset     = 7;

  pushBindProgramCommand(cb,2);
  pushBindVertexArrayCommand(cb,3);
  pushDrawCommand(cb,3,0);

  if(memCb->test(1,DumpDiff::DIFFERENT_ATTRIBUTE))return;

  std::cerr << R".(

  Tento test kontroluje, jestli se do vertex shaderu posílají správná data z vertex bufferu když se použije offset a krok a vícero bufferů.

  Nastala situace, kdy má uživatel dva buffery (b0, b1), ve kterých jsou uloženy vertex attributy.
  V buferu b1 jsou uloženy vec3 s offsetem 2 bajty a krokem pouze 8 (takže se komponenty překrývají).
  V buffer b0 jsou uloženy vec4 s offsetem 7 bajtů a krokem 20 (takže je mezi jednotlivými vec4 mezera).
  Uživatel využil 1. a 3. attribut.)." << std::endl;

  memCb->printStatus();

  memCb->printDetails();

  REQUIRE(false);
}


SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - vertex shader, attributes, offset, stride, indexing" << std::endl;

  MEMCB(10,10);
  mem.framebuffers[0].color.data = nullptr;
  mem.framebuffers[0].depth.data = nullptr;

  std::vector<glm::vec2> vert = {glm::vec2(-1.f,-1.f),glm::vec2(1.f,-1.f),glm::vec2(-1.f,1.f),glm::vec2(1.f,1.f)};

  std::vector<uint16_t> indices = {3,3,3,0,1,2,2,1,3};

  mem.buffers[0] = vectorToBuffer(vert);
  mem.buffers[1] = vectorToBuffer(indices);

  auto&prg = mem.programs[3];
  prg.vertexShader   = vertexShaderDump0  ;
  prg.fragmentShader = fragmentShaderEmpty;

  VertexArray&vao = mem.vertexArrays[8];
  vao.vertexAttrib[0].bufferID   = 0;
  vao.vertexAttrib[0].type       = AttributeType::VEC2;
  vao.vertexAttrib[0].stride     = sizeof(glm::vec2);
  vao.vertexAttrib[0].offset     = 0;

  vao.indexBufferID = 1;
  vao.indexType     = IndexType::UINT16;
  vao.indexOffset   = sizeof(uint16_t)*3;

  pushBindProgramCommand(cb,3);
  pushBindVertexArrayCommand(cb,8);
  pushDrawCommand(cb,6,0);

  if(memCb->test(1,DumpDiff::DIFFERENT_ATTRIBUTE))return;

  std::cerr << R".(
  Tento test kontroluje, jestli se do vertex shaderu posílají správná data z vertex bufferu když se použije indexing.
  Uživatel si vytvořil vertex buffer a index buffer a chtěl vykreslit čtverec pomocí 2 trojúhelníků.)." << std::endl;

  memCb->printStatus();

  memCb->printDetails();

  REQUIRE(false);
}

