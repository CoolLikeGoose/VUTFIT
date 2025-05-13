#include<iostream>

#include<catch2/catch_test_macros.hpp>

#define __FILENAME__ "drawTests"

#include<tests/testCommon.hpp>

using namespace tests;

void nofVerticesTest(uint32_t N,uint32_t prg){
  MEMCB(100,100);

  mem.framebuffers[0].depth.data = nullptr;
  mem.framebuffers[0].color.data = nullptr;

  mem.programs[0].vertexShader   = vertexShaderDump0  ;
  mem.programs[0].fragmentShader = fragmentShaderEmpty;
  mem.programs[1].vertexShader   = vertexShaderDump1  ;
  mem.programs[1].fragmentShader = fragmentShaderEmpty;
  mem.programs[2].vertexShader   = vertexShaderDump2  ;
  mem.programs[2].fragmentShader = fragmentShaderEmpty;
  mem.programs[3].vertexShader   = vertexShaderDump3  ;
  mem.programs[3].fragmentShader = fragmentShaderEmpty;

  pushBindProgramCommand(cb,prg);
  pushDrawCommand(cb,N);

  if(memCb->test(1,DumpDiff::DIFFERENT_NOF_VERTICES))return;

  std::cerr << R".(
  TEST SELHAL!

  Tento test počítá, kolikrát jste pustili vertex shader.

  izg_enqueue umožňuje také kreslit.
  V takovém případě obsahuje command buffer kreslící příkazy.

  Příkaz pro kreslení je uvedený v souboru student/fwd.hpp vypadá takto:)." << std::endl;

  print(R".(
  struct DrawCommand{
    uint32_t    nofVertices     = 0    ;
    bool        backfaceCulling = false;
  };
  ).",Color::GRAY);

  std::cerr << R".(
  V tomto testu je důležitá komponenta programID, což je číslo programu
  v paměti grafické karty, který by se měl volat.)." << std::endl;

  memCb->printStatus();

  memCb->printDetails();

  std::cerr<<R".(
  Zavolejte správný vertex shader správný počet krát!)." << std::endl;

  memCb->printHelp(R".(
  void izg_enqueue(GPUMemory&mem,CommandBuffer const&cb){
    for(uint32_t i=0;i<cb.nofCommands;++i){
      if(cb.commands[i].type == CommandType::DRAW){
        DrawCommand cmd = cb.commands[i].commandData.drawCommand;
        VertexShader vs = mem.programs[cmd.programID].vertexShader;
        for(uint32_t n=0;n<cmd.nofVertices;++n){
          InVertex inVertex;
          OuntVertex outVertex;
          ShaderInterface si;
          vs(outVertex,inVertex,si);
        }
      }
    }
  }
  ).");

  REQUIRE(false);
}

SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - draw commands" << std::endl;
  nofVerticesTest(3,2);
  nofVerticesTest(6,0);
  nofVerticesTest(9,1);
  nofVerticesTest(1800,0);
}
