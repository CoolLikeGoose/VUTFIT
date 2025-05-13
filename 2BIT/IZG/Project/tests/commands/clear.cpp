#include<iostream>

#include<catch2/catch_test_macros.hpp>

#define __FILENAME__ "clear"

#include<tests/testCommon.hpp>
#include<tests/commandBufferToStr.hpp>

using namespace tests;

void clearColorTest(){
  MEMCB(20,20);

  auto pix        = glm::uvec2(10,10);
  auto clearColor = glm::vec4(1,0,0,1);
  auto clearDepth = 10e9f;
  auto initDepth  = 4.f;


  setColorU(mem.framebuffers[mem.defaultFramebuffer],pix.x,pix.y,glm::uvec4(1,2,3,4));
  setDepth (mem.framebuffers[mem.defaultFramebuffer],pix.x,pix.y,initDepth);

  pushClearCommand(cb,clearColor,clearDepth,true,false);

  if(memCb->test(1,DumpDiff::DIFFERENT_CLEAR,true))return;

  std::cerr << R".(
  TEST SELHAL!

  Vašim úkolem v tomto projektu je naprogramovat funkcionalitu grafické karty.
  Funkcionalita grafické karty je reprezentována funkcí:
  izg_enqueue(GPUMemory const& mem,CommnadBuffer const& cmdBuffer)
  v souboru student/gpu.cpp.
  
  Mezi funkcionality grafické karty patří kreslení, čistění obrazovky a další.

  S realnými gpu se komunikuje skrze fronty přikazů, po kterých se posílají úlohy.
  Tyto úlohy bývají zabalené do balíčků práce (tzn. command buffer).
  Podobně je to například v API Vulkan:
  https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkCommandBuffer.html

  V tomto projektu je to simulováno obdobně - funkce izg_enqueue obdrží paměť grafické karty a command buffer.
  Úkolem funkce izg_enqueue je odsimulovat balíček práce nad pamětí grafické karty.

  Paměť grafické karty se skládá z programů, bufferů (lineární paměti), textur, framebufferů, vertex array objektů a dalších částí.

  Tento test zkouší, zda váš kód čistí framebuffer v paměti grafické karty.

  Paměť grafické karty je ve struktuře GPUMemory.

  Příkaz pro čištění framebufferu uvedený v souboru student/fwd.hpp vypadá takto:
  ).";
  print(R".(
  struct ClearCommand{
    glm::vec4   color      = glm::vec4(0);
    float       depth      = 1e10        ;
    bool        clearColor = true        ;
    bool        clearDepth = true        ;
  };
  ).",Color::GRAY);
  std::cerr << R".(
  V tomto testu vypadá command buffer takto:
  )." << std::endl;

  print(commandBufferToStr(2,cb),Color::GRAY);

  std::cerr << R".(
  Jak můžete vidět, obsahuje jen jeden příkaz - vyčištění obrazovky.

  Můžete vidět barvu, na kterou se framebuffer čistí (r,g,b,a), hloubku (depth)
  a zda je čištění framebuffer povoleno.
  Framebuffer se skládá ze dvou částí: color buffer a depth buffer - paměť barvy a hloubky.
  Pokud je příznak clearColor == false, neprovede se čistění color bufferu.
  Pokud je příznak clearDepth == false, neprovede se čistění depth bufferu.

  Barvy jsou v ClearCommand uvedeny ve floatech v rozsahu: [0-1].
  Při zápisu do framebufferu je nutné je převést na uint8_t rozsahu: [0,255];
  )." << std::endl;

  std::cerr << R".(

  Test selhal, protože hodnoty ve framebufferu jsou po provedení příkazu nesprávné.)." << std::endl;

  memCb->printDetails();

  std::cerr <<R".(

  Tak správně vyčistěte framebuffer!
  Upravte funkci izg_enqueue v souboru student/gpu.cpp)." << std::endl;

  memCb->printHelp(R".(
  void izg_enqueue(GPUMemory&mem,CommandBuffer const&cb){
    for(uint32_t i=0;i<cb.nofCommands;++i){
      if(cb.commands[i].type == CommandType::CLEAR){
        clear(mem,cb.commands[i].commandData.clearCommand);
      }
    }
  }
  ).");

  REQUIRE(false);
}

void clearDepthTest(){
  MEMCB(20,20);

  auto pix        = glm::uvec2(10,10);
  auto clearColor = glm::vec4(1,1,0,1);
  auto clearDepth = 10e9f;
  auto initDepth  = 4.f;
  auto initColor  = glm::uvec4(1,2,3,4);
  setColorU(mem.framebuffers[mem.defaultFramebuffer],pix.x,pix.y,initColor);
  setDepth (mem.framebuffers[mem.defaultFramebuffer],pix.x,pix.y,initDepth);
  pushClearCommand(cb,clearColor,clearDepth,false,true);

  if(memCb->test(1,DumpDiff::DIFFERENT_CLEAR))return;
  
  std::cerr << R".(
  TEST SELHAL!

  V tomto testu se ve framebufferu čistí pouze hloubka.

  Test selhal, protože hodnoty ve framebufferu jsou špatně.)." << std::endl;

  memCb->printDetails();

  REQUIRE(false);
}

void clearColorThenClearDepthTest(){
  MEMCB(100,100);

  auto pix        = glm::uvec2(10,10);
  auto clearColor = glm::vec4(1,0,1,1);
  auto clearDepth = 10e7f;
  auto initDepth  = 4.f;
  auto initColor  = glm::uvec4(1,2,3,4);
  setColorU(mem.framebuffers[mem.defaultFramebuffer],pix.x,pix.y,initColor);
  setDepth (mem.framebuffers[mem.defaultFramebuffer],pix.x,pix.y,initDepth);
  pushClearCommand(cb,clearColor,7,true,false);
  pushClearCommand(cb,glm::vec4(4),clearDepth,false,true);

  if(memCb->test(1,DumpDiff::DIFFERENT_CLEAR))return;

  std::cerr << R".(
  TEST SELHAL!

  V tomto testu obsahuje command buffer dva příkazy pro čištění framebufferu.

  Test selhal, protože hodnoty ve framebufferu jsou špatně.)." << std::endl;

  memCb->printDetails();
  
  REQUIRE(false);
}

void multipleClearTest(){
  MEMCB(20,20);

  auto pix        = glm::uvec2(10,10);
  auto clearColor = glm::vec4(1,0,1,1);
  auto clearDepth = 10e7f;
  auto initDepth  = 4.f;
  auto initColor  = glm::uvec4(1,2,3,4);
  setColorU(mem.framebuffers[mem.defaultFramebuffer],pix.x,pix.y,initColor);
  setDepth (mem.framebuffers[mem.defaultFramebuffer],pix.x,pix.y,initDepth);
  pushClearCommand(cb,clearColor,clearDepth,false,false);
  pushClearCommand(cb,glm::vec4(1,1,1,1),7,true,false);
  pushClearCommand(cb,glm::vec4(0,1,1,1),2,true,true );
  pushClearCommand(cb,glm::vec4(0,0,0,1),3,true,false);
  pushClearCommand(cb,clearColor,clearDepth,true,true);

  if(memCb->test(1,DumpDiff::DIFFERENT_CLEAR))return;

  std::cerr << R".(
  TEST SELHAL!

  V tomto testu obsahuje command buffer vícero čistítích příkazů.

  Test selhal, protože hodnoty ve framebufferu jsou špatně.)." << std::endl;

  memCb->printDetails();

  REQUIRE(false);
  
}



SCENARIO(TEST_NAME){
  std::cerr << testCounter(true) << " - clear commands" << std::endl;
  clearColorTest();
  clearDepthTest();
  clearColorThenClearDepthTest();
  multipleClearTest();
}

void clearColorEmptyDepth(){
  MEMCB(20,20);

  mem.framebuffers[0].depth = Image{};

  auto pix        = glm::uvec2(10,10);
  auto clearColor = glm::vec4(1,0,1,1);
  auto clearDepth = 10e7f;
  auto initColor  = glm::uvec4(1,2,3,4);
  setColorU(mem.framebuffers[mem.defaultFramebuffer],pix.x,pix.y,initColor);
  pushClearCommand(cb,clearColor,clearDepth,true,true);

  if(memCb->test(1,DumpDiff::DIFFERENT_CLEAR))return;

  std::cerr << R".(
  TEST SELHAL!

  V tomto testu obsahuje command buffer čištění barvy i hloubky.
  Framebuffer však obsahuje pouze barevný buffer.
  Proto by měl vyčistit jen barevný buffer.

  Test selhal, protože hodnoty ve framebufferu jsou špatně.)." << std::endl;

  memCb->printDetails();

  REQUIRE(false);
}

void clearDepthEmptyColor(){
  MEMCB(20,20);

  mem.framebuffers[0].color = Image{};

  auto pix        = glm::uvec2(10,10);
  auto clearColor = glm::vec4(1,0,1,1);
  auto clearDepth = 10e7f;
  auto initDepth  = 4.f;
  auto initColor  = glm::uvec4(1,2,3,4);
  setDepth(mem.framebuffers[mem.defaultFramebuffer],pix.x,pix.y,initDepth);
  pushClearCommand(cb,clearColor,clearDepth,true,true);

  if(memCb->test(1,DumpDiff::DIFFERENT_CLEAR))return;

  std::cerr << R".(
  TEST SELHAL!

  V tomto testu obsahuje command buffer čištění barvy i hloubky.
  Framebuffer však obsahuje pouze barevný buffer.
  Proto by měl vyčistit jen barevný buffer.

  Test selhal, protože hodnoty ve framebufferu jsou špatně.)." << std::endl;

  memCb->printDetails();

  REQUIRE(false);
}

SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - clear commands, partial framebuffer" << std::endl;
  clearColorEmptyDepth();
  clearDepthEmptyColor();
}


