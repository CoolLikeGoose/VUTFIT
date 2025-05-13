#include<iostream>

#include<catch2/catch_test_macros.hpp>

#define __FILENAME__ "bindFramebufferTests"

#include<tests/testCommon.hpp>

using namespace tests;

void bindFramebufferTest(){
  MEMCB(100,100);
  mem.framebuffers[0].color.data = nullptr;
  mem.framebuffers[0].depth.data = nullptr;
  pushBindFramebufferCommand(cb,3);

  if(memCb->test(1,DumpDiff::DIFFERENT_BIND_FRAMEBUFFER))return;
  
  std::cerr << R".(
  TEST SELHAL!

  Tento test zkouší, zda funguje příkaz bindFramebuffer.
  Cílem příkazu bindFramebuffer je aktivovat správný framebuffer.
  Příkaz se podobá příkazu glBindFramebuffer z OpenGL.
  https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBindFramebuffer.xhtml

  Test selhal.
  V paměti není po vykonání příkazu správně nastaven activatedFramebuffer.)." << std::endl;

  memCb->printDetails();

  REQUIRE(false);
}

void multipleBindFramebufferTest(){
  MEMCB(100,100);
  mem.framebuffers[0].color.data = nullptr;
  mem.framebuffers[0].depth.data = nullptr;
  pushBindFramebufferCommand(cb,3);
  pushBindFramebufferCommand(cb,8);
  pushBindFramebufferCommand(cb,4);

  if(memCb->test(1,DumpDiff::DIFFERENT_BIND_FRAMEBUFFER))return;
  
  std::cerr << R".(
  TEST SELHAL!

  Tento test zkouší, zda funguje příkaz bindFramebuffer pokud se zavolá vícekrát.

  Test selhal.
  V paměti není po vykonání příkazu správně nastaven activatedFramebuffer.)." << std::endl;

  memCb->printDetails();

  REQUIRE(false);
}

SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - bind framebuffer" << std::endl;
  bindFramebufferTest();
  multipleBindFramebufferTest();
}
