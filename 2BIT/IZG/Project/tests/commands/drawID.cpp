#include<iostream>

#include<catch2/catch_test_macros.hpp>

#define __FILENAME__ "drawID"

#include<tests/testCommon.hpp>

using namespace tests;

void oneEqueue(){

  MEMCB(100,100);
  mem.framebuffers[0].color.data = nullptr;
  mem.framebuffers[0].depth.data = nullptr;

  mem.programs[0].vertexShader   = vertexShaderDump0  ;
  mem.programs[0].fragmentShader = fragmentShaderEmpty;
  mem.programs[1].vertexShader   = vertexShaderDump1  ;
  mem.programs[1].fragmentShader = fragmentShaderEmpty;

  pushBindProgramCommand(cb,0);
  pushDrawCommand(cb,3);
  pushBindProgramCommand(cb,1);
  pushDrawCommand(cb,3);
  pushBindProgramCommand(cb,0);
  pushDrawCommand(cb,3);
  pushBindProgramCommand(cb,1);
  pushDrawCommand(cb,3);
  pushBindProgramCommand(cb,0);
  pushDrawCommand(cb,3);
  pushBindProgramCommand(cb,1);
  pushDrawCommand(cb,3);
  pushBindProgramCommand(cb,0);
  pushDrawCommand(cb,3);

  if(memCb->test(1,DumpDiff::DIFFERENT_DRAWID))return;

  std::cerr << R".(
  TEST SELHAL!

  Tento test zkouší, zda kreslící příkazy mají správné gl_DrawID.
  Všechny kreslící příkazy v command bufferu by měly být číslovány počadovým číslem.
  Číslují se pouze kreslící příkazy.
  Pokud jsou v command bufferu například 4 kreslící příkazy proloženy
  jinými příkazy, budou mít tyto kreslící příkazy číslo 0,1,2,3,...

  gl_DrawID je ve struktuře ShaderInterface jejíž instance vstupuje do vertex shaderu.
  Podle gl_DrawID se obvykle vybírá nastavení pro danou část modelu (například textury, materiály, matice).)." << std::endl;

  memCb->printStatus();

  std::cerr<<R".(

  Správně číslujte kreslící příkazy!
  ).";
  REQUIRE(false);
}

void multipleEqueue(){

  MEMCB(100,100);
  mem.framebuffers[0].color.data = nullptr;
  mem.framebuffers[0].depth.data = nullptr;

  mem.programs[0].vertexShader   = vertexShaderDump0  ;
  mem.programs[0].fragmentShader = fragmentShaderEmpty;
  mem.programs[1].vertexShader   = vertexShaderDump1  ;
  mem.programs[1].fragmentShader = fragmentShaderEmpty;

  pushBindProgramCommand(cb,0);
  pushDrawCommand(cb,3);
  pushBindProgramCommand(cb,1);
  pushDrawCommand(cb,3);
  pushBindProgramCommand(cb,0);
  pushDrawCommand(cb,3);
  pushBindProgramCommand(cb,1);
  pushDrawCommand(cb,3);
  pushBindProgramCommand(cb,0);
  pushDrawCommand(cb,3);
  pushBindProgramCommand(cb,1);
  pushDrawCommand(cb,3);
  pushBindProgramCommand(cb,0);
  pushDrawCommand(cb,3);

  if(memCb->test(2,DumpDiff::DIFFERENT_DRAWID))return;

  std::cerr << R".(
  TEST SELHAL!

  Tento test zkouší, zda kreslící příkazy mají správné gl_DrawID.
  Příkaz izg_enqueue je v tomto testu zavolán několikrát.
  Při každém zavolání by se mělo gl_DrawID nastavit zpět na 0.
  )." << std::endl;

  memCb->printStatus();

  std::cerr<<R".(

  Správně číslujte kreslící příkazy!
  ).";
  REQUIRE(false);
}

SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - drawID" << std::endl;
  oneEqueue();
  multipleEqueue();
}
