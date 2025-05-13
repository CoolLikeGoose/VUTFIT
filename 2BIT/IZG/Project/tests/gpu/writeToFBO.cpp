#include <iostream>
#include <set>

#include <catch2/catch_test_macros.hpp>

#define __FILENAME__ "writeToFBO"
#include <tests/testCommon.hpp>
#include <tests/gpu/common.hpp>

using namespace tests;
using namespace dtl;

void fragmentShaderWhite(OutFragment&outFragment,InFragment const&inF,ShaderInterface const&u){
  outFragment.gl_FragColor = glm::vec4(1.f);
  fragmentShaderDump(outFragment,inF,u);
}

SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - fragment color should be written to framebuffer" << std::endl;

  auto res = glm::uvec2(100,100);

  MEMCB(res.x,res.y);

  auto&inFragments = memCb->inFragments;
  auto&outVertices = memCb->outVertices;

  initOutVertices();
  inFragments.clear();
  glm::vec3 ndc      [3];for(int i=0;i<3;++i)ndc[i] = glm::vec3(outVertices[i].gl_Position)/outVertices[i].gl_Position.w;
  glm::vec3 viewSpace[3];for(int i=0;i<3;++i)viewSpace[i] = glm::vec3((glm::vec2(ndc[i])*.5f+.5f)*glm::vec2(res),ndc[i].z);

  auto&prg = mem.programs[5];
  prg.vertexShader   = vertexShaderInject;
  prg.fragmentShader = fragmentShaderWhite;

  pushClearCommand(cb,glm::vec4(1,0,0,1));
  pushBindProgramCommand(cb,5);
  pushDrawCommand(cb,3);

  memCb->test();

  bool success = true;

  std::set<UV2>rasterized;
  for(auto const&f:inFragments)
    rasterized.insert({f.gl_FragCoord});

  std::set<UV2>shouldHaveColor;
  std::set<UV2>shouldBeEmpty;

  for(uint32_t y=0;y<res.y;++y)
    for(uint32_t x=0;x<res.x;++x){
      auto coord = glm::uvec2(x,y);
      auto color = glm::uvec3(getColorU(memCb->studentMem.mem.framebuffers[0],x,y));
      bool good;
      if(rasterized.find(coord) != rasterized.end()){
        good = color == glm::uvec3(255);
        if(!good)shouldHaveColor.insert(coord);
      }else{
        good = color == glm::uvec3(255,0,0);
        if(!good)shouldBeEmpty.insert(coord);
      }
      success &= good;
    }

  if(!breakTest()&&success)return;
  
  std::cerr << R".(
  Tento test zkouší, zda se barva fragmentu zapsala do framebufferu.

  Framebuffer má rozlišení: )."<<res.x<<"x"<<res.y<<R".(

  Rasterizuje se trojúhelník:
  A = vec2)."<<str(glm::vec2(viewSpace[0]))<<R".(;
  B = vec2)."<<str(glm::vec2(viewSpace[1]))<<R".(;
  C = vec2)."<<str(glm::vec2(viewSpace[2]))<<R".(;

  Ale byly nalezeny pixely ve framebufferu, které by měly být vybarvené, ale nejsou.
  Nebo byly nalezeny pixely ve framebufferu, který nejsou vybarvené, ale měly by být.)." << std::endl;

  std::cerr << R".(
  Tyto pixely by měly být obarvené, ale nebyly: {)."<<std::endl;
  uint32_t counter=0;
  for(auto const&v:shouldHaveColor){
    std::cerr << "    " << str(v.data) << std::endl;
    counter++;
    if(counter>=10)break;
  }
  std::cerr << "  }" << std::endl;
  counter=0;
  std::cerr << R".(
  Tyto pixely by neměly být obarvené, ale byly: {)."<<std::endl;
  for(auto const&v:shouldBeEmpty){
    std::cerr << "    " << str(v.data) << std::endl;
    counter++;
    if(counter>=10)break;
  }
  std::cerr << "  }" << std::endl;

  REQUIRE(false);
  
}

SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - fragment depth should be written to framebuffer" << std::endl;

  auto res = glm::uvec2(100,100);

  MEMCB(res.x,res.y);

  auto&inFragments = memCb->inFragments;
  auto&outVertices = memCb->outVertices;
  initOutVertices();
  inFragments.clear();
  glm::vec3 ndc      [3];for(int i=0;i<3;++i)ndc[i] = glm::vec3(outVertices[i].gl_Position)/outVertices[i].gl_Position.w;
  glm::vec3 viewSpace[3];for(int i=0;i<3;++i)viewSpace[i] = glm::vec3((glm::vec2(ndc[i])*.5f+.5f)*glm::vec2(res),ndc[i].z);

  auto&prg = mem.programs[7];
  prg.vertexShader   = vertexShaderInject;
  prg.fragmentShader = fragmentShaderWhite;

  pushClearCommand(cb);
  pushBindProgramCommand(cb,7);
  pushDrawCommand(cb,3);

  memCb->test();


  bool success = true;

  std::set<UV2>rasterized;
  for(auto const&f:inFragments)
    rasterized.insert({f.gl_FragCoord});

  std::set<UV2>shouldHaveDepth;
  std::set<UV2>shouldBeEmpty;

  auto&depth = memCb->studentMem.mem.framebuffers[0].depth;
  for(uint32_t y=0;y<res.y;++y)
    for(uint32_t x=0;x<res.x;++x){
      bool good = true;
      auto d = *(float*)getPixel(depth,x,y);
      if(rasterized.find(glm::uvec2(x,y)) != rasterized.end()){
        good = d == 0.f;
        if(!good)shouldHaveDepth.insert(glm::uvec2(x,y));
      }else{
        good = d != 0.f;
        if(!good)shouldBeEmpty.insert(glm::uvec2(x,y));
      }
      success &= good;
    }


  if(!breakTest()&&success)return;

  std::cerr << R".(
  Tento test zkouší, zda se houbka fragmentu zapsala do framebufferu.

  Framebuffer má rozlišení: )."<<res.x<<"x"<<res.y<<R".(

  Rasterizuje se trojúhelník:
  A = vec2)."<<str(viewSpace[0])<<R".(;
  B = vec2)."<<str(viewSpace[1])<<R".(;
  C = vec2)."<<str(viewSpace[2])<<R".(;

  Ale byly nalezeny pixely ve framebufferu, které by měly mít změněnou hloubku, ale nemají.
  Nebo byly nalezeny pixely ve framebufferu, který neměly změněnou hloubku, ale měly by mít.)." << std::endl;

  std::cerr << R".(
  Tyto pixely by měly mít změněnou hloubku, ale neměly: {)."<<std::endl;
  uint32_t counter=0;
  for(auto const&v:shouldHaveDepth){
    std::cerr << "    " << str(v.data) << std::endl;
    counter++;
    if(counter>=10)break;
  }
  std::cerr << "    }" << std::endl;
  counter=0;
  std::cerr << R".(
  Tyto pixely by neměly mít změněnou hloubu, ale měly: {)."<<std::endl;
  for(auto const&v:shouldBeEmpty){
    std::cerr << "    " << str(v.data) << std::endl;
    counter++;
    if(counter>=10)break;
  }
  std::cerr << "    }" << std::endl;

  REQUIRE(false);
  
}

