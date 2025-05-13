#include<iostream>

#include<catch2/catch_test_macros.hpp>

#define __FILENAME__ "setDrawId"

#include<tests/testCommon.hpp>

using namespace tests;

void setDrawId(){

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
  pushSetDrawIdCommand(cb,30);
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
  Využívá se příkaz SetDrawIdCommand, který umožňuje nastavit gl_DrawID v průběhu.

  gl_DrawID je ve struktuře ShaderInterface jejíž instance vstupuje do vertex shaderu.
  Podle gl_DrawID se obvykle vybírá nastavení pro danou část modelu (například textury, materiály, matice).)." << std::endl;

  memCb->printStatus();

  std::cerr<<R".(

  Správně číslujte kreslící příkazy!
  ).";
  REQUIRE(false);
}

SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - setDrawId" << std::endl;
  setDrawId();
}
