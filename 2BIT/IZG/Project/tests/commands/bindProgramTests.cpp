#include<iostream>

#include<catch2/catch_test_macros.hpp>

#define __FILENAME__ "bindProgramTests"

#include<tests/testCommon.hpp>

using namespace tests;

void bindProgramTest(){
  MEMCB(100,100);
  mem.framebuffers[0].color.data = nullptr;
  mem.framebuffers[0].depth.data = nullptr;
  pushBindProgramCommand(cb,3);

  if(memCb->test(1,DumpDiff::DIFFERENT_BIND_PROGRAM))return;
  
  std::cerr << R".(
  TEST SELHAL!

  Tento test zkouší, zda funguje příkaz bindProgram.
  Cílem příkazu bindProgram je aktivovat správný shader program.
  Příkaz se podobá příkazu glUseProgram z OpenGL.
  https://registry.khronos.org/OpenGL-Refpages/gl4/html/glUseProgram.xhtml

  Test selhal.
  V paměti není po vykonání příkazu správně nastaven activatedProgram.
  )." << std::endl;

  memCb->printDetails();

  REQUIRE(false);
}

void multipleBindProgramTest(){
  MEMCB(100,100);
  mem.framebuffers[0].color.data = nullptr;
  mem.framebuffers[0].depth.data = nullptr;
  pushBindProgramCommand(cb,3);
  pushBindProgramCommand(cb,2);
  pushBindProgramCommand(cb,8);

  if(memCb->test(1,DumpDiff::DIFFERENT_BIND_PROGRAM))return;
  
  std::cerr << R".(
  TEST SELHAL!

  Tento test zkouší využít příkaz bindProgram několikrát.

  Test selhal.
  V paměti není po vykonání příkazů správně nastaven activatedProgram.
  )." << std::endl;

  memCb->printDetails();

  REQUIRE(false);
}

SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - bind program" << std::endl;
  bindProgramTest();
  multipleBindProgramTest();
}
