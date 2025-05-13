#include <iostream>

#include <catch2/catch_test_macros.hpp>

#define __FILENAME__ "gl_VertexID"
#include<tests/testCommon.hpp>

using namespace tests;

SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - vertex shader, gl_VertexID (no indexing)" << std::endl;

  MEMCB(10,10);
  mem.framebuffers[0].color.data = nullptr;
  mem.framebuffers[0].depth.data = nullptr;

  auto&prg = mem.programs[7];
  prg.vertexShader   = vertexShaderDump0;
  prg.fragmentShader = fragmentShaderEmpty;

  pushBindProgramCommand(cb,7);
  pushDrawCommand(cb,9);

  if(memCb->test(1,DumpDiff::DIFFERENT_VERTEXID))return;
  std::cerr << R".(
  Vrcholy chodí do vertex shaderu číslované.
  Číslo vrcholu je uloženo v položce gl_VertexID ve struktuře InVertex.
  Když není zapnuté indexování (v tomto testu není), tak je číslo vrcholu rovno číslu zavolání vertex shaderu.

  Proměnná gl_VertexID odpovída proměnné gl_VertexId z GLSL.)."<<std::endl;

  memCb->printStatus();

  memCb->printDetails();

  memCb->printHelp(R".(
  void draw(DrawCommand const&cmd){
    for(... cmd.nofVertices ... ){//smyčka přes vrcholy
      InVertex  inVertex; // vrchol, co vstupuje do vertex shader
      OutVertex outVertex;// vrchol, co leze ven z vertex shaderu 
      inVertex.gl_VertexID = ??;
      ShaderInterface si;
      ctx.prg.vertexShader(outVertex,inVertex,si);
    }
  }).");

  REQUIRE(false);
}


template<typename T = uint32_t>
void indexTest(){
  auto bits = sizeof(T)*8;
  std::cerr << "  ("<<bits<<"bit indexing)" << std::endl;

  std::vector<T> indices = {0,1,2,2,1,3};

  MEMCB(10,10);
  mem.framebuffers[0].color.data = nullptr;
  mem.framebuffers[0].depth.data = nullptr;

  auto&prg = mem.programs[4];
  prg.vertexShader   = vertexShaderDump0;
  prg.fragmentShader = fragmentShaderEmpty;
  mem.buffers[0] = vectorToBuffer(indices);

  auto&vao = mem.vertexArrays[7];
  vao.indexBufferID  = 0;
  vao.indexOffset    = 0;
  vao.indexType      = (IndexType)sizeof(T);

  pushBindProgramCommand(cb,4);
  pushBindVertexArrayCommand(cb,7);
  pushDrawCommand(cb,(uint32_t)indices.size(),0);

  if(memCb->test(1,DumpDiff::DIFFERENT_VERTEXID))return;

  std::cerr << R".(
  TEST SELHAL!
  ).";

  memCb->printStatus();

  memCb->printDetails();

  std::cerr << R".(

  Test kreslí )."<<indices.size()<<R".( vrcholů.
  Tyto vrcholy by měly mít správné pořadové číslo (gl_VertexID), které je rovno hodnotě z indexačního bufferu.

  Vrcholy chodí do vertex shaderu číslované.
  Číslo vrcholu je uloženo v položce gl_VertexID ve struktuře InVertex.
  Když je zapnuté indexování (v tomto testu je), tak je číslo vrcholu rovno hodnotě v indexačním bufferu.
  V tomto testu se předpokládá )."<<bits<<R".( bitový index.
  Číslo bufferu s indexy je v proměnné vao.indexBufferID.
  Typ indexu (8b,16b,32b) je v proměnné vao.indexType.
  Offset indexů je v proměnné vao.indexOffset.

  Nezapomeňte správně adresovat indexační buffer.
  Jedná se o tzn. "pointer arithmetic".

  Musíte si uvědomit, že vao.indexBuffer ukazuje do tabulky všech bufferů v GPUMemory.
  Indexační buffer obsahuje indexy, které mají všechny stejnou velikost, která může být 8b, 16b nebo 32 bitů.

  Položka z indexačního bufferu je vybrána číslem invokace vertex shaderu.

  Na buffer je možné nahlížet jako na pole:
  uint)."<<bits<<R".(_t*pole;

  číslo vrcholu gl_VertexID je potom rovno pole[číslo invokace vertex shaderu].

  Indexing je vypnutý, pokud je vao.indexBufferID < 0.)." << std::endl;

  memCb->printHelp(R".(
  uint32_t computeVertexID(GPUMemory&mem,VertexArray const&vao,uint32_t shaderInvocation){
    if(vao.indexBufferID<0)return shaderInvocation;

    void*indexBuffer = mem.buffers[vao.indexBufferID];

    if(vao.indexType == IndexType::UIN)."+str(bits)+R".(){
      uint)."+str(bits)+R".(_t*ind = (uint)."+str(bits)+R".(_t*)(vao.indexBuffer+vao.indexOffset);
      return ind[shaderInvocation];
    }
    ...
  }

  void draw(GPUMemory&mem,DrawCommand const&cmd){

    for(...){//smyčka přes vrcholy
      ...
      InVertex  inVertex; // vrchol, co vstupuje do vertex shader
      inVertex.gl_VertexID = computeVertexID(mem,cmd.vao,...);
      ...
    }

  }
  ).");

  REQUIRE(false);
}

SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - vertex shader, gl_VertexID (indexing)" << std::endl;
  indexTest<uint32_t>();
  indexTest<uint16_t>();
  indexTest<uint8_t>();
}
