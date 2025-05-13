#include <iostream>

#include <catch2/catch_test_macros.hpp>

#define __FILENAME__ "barycentrics"
#include <tests/testCommon.hpp>
#include <tests/gpu/common.hpp>

using namespace tests;
using namespace dtl;

SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - depth interpolation" << std::endl;

  uint32_t w = 100;
  uint32_t h = 100;
  MEMCB(w,h);
  auto&inFragments = memCb->inFragments;
  auto&outVertices = memCb->outVertices;

  outVertices.clear();
  inFragments.clear();

  float hc[3] = {1,2,.5};
  float zz[3] = {.9f,.4f,.8f};
  outVertices.resize(3);

  outVertices[0].gl_Position=glm::vec4(-hc[0],-hc[0],zz[0],hc[0]);
  outVertices[1].gl_Position=glm::vec4(+hc[1],-hc[1],zz[1],hc[1]);
  outVertices[2].gl_Position=glm::vec4(-hc[2],+hc[2],zz[2],hc[2]);


  auto&prg = mem.programs[4];
  mem.programs[4].vertexShader   = vertexShaderInject;
  mem.programs[4].fragmentShader = fragmentShaderDump;

  pushClearCommand(cb);
  pushBindProgramCommand(cb,4);
  pushDrawCommand(cb,3);

  memCb->test();

  bool success = true;

  uint32_t expectedCount = w*h/2;
  uint32_t err = w;
  success &= equalCounts(inFragments.size(),expectedCount,err);

  std::vector<InFragment>badFragments;

  for(auto const&f:inFragments){
    auto l = barycentrics(outVertices[0],outVertices[1],outVertices[2],f.gl_FragCoord,w,h);

    auto att = 
      outVertices[0].gl_Position.z/outVertices[0].gl_Position.w * l[0]+
      outVertices[1].gl_Position.z/outVertices[1].gl_Position.w * l[1]+
      outVertices[2].gl_Position.z/outVertices[2].gl_Position.w * l[2];

    bool good = equalFloats(f.gl_FragCoord.z,att); 

    if(!good)
      badFragments.push_back(f);

    success &= good;
  }

  glm::vec3 ndc      [3];for(int i=0;i<3;++i)ndc[i] = glm::vec3(outVertices[i].gl_Position)/outVertices[i].gl_Position.w;
  glm::vec3 viewSpace[3];for(int i=0;i<3;++i)viewSpace[i] = glm::vec3((glm::vec2(ndc[i])*.5f+.5f)*glm::vec2(w,h),ndc[i].z);

  if(!breakTest()&&success)return;
  std::cerr << R".(
  Tento test zkouší, zda se správně interpoluje hloubka fragmentů.

  Framebuffer má rozlišení: )."<<w<<"x"<<h<<R".(

  Rasterizuje se trojúhelník, který má souřadnice v clip space:
  A.gl_Position = vec4)."<<str(outVertices[0].gl_Position)<<R".(;
  B.gl_Position = vec4)."<<str(outVertices[1].gl_Position)<<R".(;
  C.gl_Position = vec4)."<<str(outVertices[2].gl_Position)<<R".(;

  Což odpovídá rasterizaci trojúhelníků v NDC:
  A = vec3)."<<str(ndc[0])<<R".(;
  B = vec3)."<<str(ndc[1])<<R".(;
  C = vec3)."<<str(ndc[2])<<R".(;

  Což odpovídá rasterizaci trojúhelníků ve view-space:
  A = vec3)."<<str(viewSpace[0])<<R".(;
  B = vec3)."<<str(viewSpace[1])<<R".(;
  C = vec3)."<<str(viewSpace[2])<<R".(;

  Hloubka by měla být interpolována pomocí barycentrických souřadnic, BEZ perspektviní korekce.

  To znamená, že pokud jsou barycentrické souřadnice l0,l1,l2 spočítané z obsahů ve 2D, pak
  by výsledná hloubka měla být:

  A.z*l0 + B.z*l1 + C.z*l2

  Byly nalezeny fragmenty, které mají špatně interpolovanou hloubu.

  Například tyto fragmenty jsou špatně:)."<<std::endl;

  uint32_t counter = 0;
  for(auto const&b:badFragments){
    counter++;

    auto l = barycentrics(outVertices[0],outVertices[1],outVertices[2],b.gl_FragCoord,w,h);

    auto att = 
      outVertices[0].gl_Position.z/outVertices[0].gl_Position.w * l[0]+
      outVertices[1].gl_Position.z/outVertices[1].gl_Position.w * l[1]+
      outVertices[2].gl_Position.z/outVertices[2].gl_Position.w * l[2];
    std::cerr << "    gl_FragCoord.xy == " << str(glm::vec2(b.gl_FragCoord)) << " ma hloubku == " << str(b.gl_FragCoord.z) << ", ale měl mít hloubku: " << str(att) << std::endl;

    if(counter>=10)break;
  }

  REQUIRE(false);
}



SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - vertex attributes interpolated to fragment attributes" << std::endl;

  uint32_t w = 100;
  uint32_t h = 100;

  MEMCB(w,h);

  auto&inFragments = memCb->inFragments;
  auto&outVertices = memCb->outVertices;

  outVertices.clear();
  inFragments.clear();

  initOutVertices();
  outVertices[0].attributes[0].v3 = glm::vec3(1,0,0);
  outVertices[1].attributes[0].v3 = glm::vec3(0,1,0);
  outVertices[2].attributes[0].v3 = glm::vec3(0,0,1);

  auto&prg = mem.programs[6];
  prg.vertexShader   = vertexShaderInject;
  prg.fragmentShader = fragmentShaderDump;
  prg.vs2fs[0]       = AttributeType::VEC3;

  pushClearCommand(cb);
  pushBindProgramCommand(cb,6);
  pushDrawCommand(cb,3);

  memCb->test();

  bool success = true;

  uint32_t expectedCount = w*h/2;
  uint32_t err = w;
  success &= equalCounts(inFragments.size(),expectedCount,err);

  std::vector<InFragment>badFragments;

  for(auto const&f:inFragments){
    auto l = barycentrics(outVertices[0],outVertices[1],outVertices[2],f.gl_FragCoord,w,h);

    auto att = 
      outVertices[0].attributes[0].v3 * l[0]+
      outVertices[1].attributes[0].v3 * l[1]+
      outVertices[2].attributes[0].v3 * l[2];

    bool good = equalVec3(f.attributes[0].v3,att); 

    if(!good)
      badFragments.push_back(f);

    success &= good;
  }

  if(!breakTest()&&success)return;
  
  std::cerr << R".(
  Tento test zkouší, zda se interpolují vertex attributy do fragment attributů.

  Framebuffer má rozlišení: )."<<w<<"x"<<h<<R".(

  Rasterizuje se trojúhelník, který pokrývá polovinu obrazovky:
  A.gl_Position = vec4(-1,-1,-1,+1);
  B.gl_Position = vec4(+1,-1,-1,+1);
  C.gl_Position = vec4(-1,+1,-1,+1);

   ________
  |*      |
  |***    |
  |*****  |
  |*******|
  
  Vrcholy obsahují barvu v nultém atributu
  A.attributes[0].v3 = vec3(1,0,0);
  B.attributes[0].v3 = vec3(0,1,0);
  C.attributes[0].v3 = vec3(0,0,1);

  Vyrasterizované fragmenty by měly mít správně interpolovanou barvu pomocí barycentrických souřadnic.

  Barycentrické souřadnice se spočítají pomocí obsahů trojúhelníků.

  Byly nalezeny fragmenty, které mají špatně interpolovanou barvu.

  Například tyto fragmenty jsou špatně:)."<<std::endl;

  uint32_t counter = 0;
  for(auto const&b:badFragments){
    counter++;

    auto const&fragCoord = b.gl_FragCoord;

    auto l = barycentricsPerspective(outVertices[0],outVertices[1],outVertices[2],b.gl_FragCoord,w,h);

    auto att = 
      outVertices[0].attributes[0].v3 * l[0]+
      outVertices[1].attributes[0].v3 * l[1]+
      outVertices[2].attributes[0].v3 * l[2];

    std::cerr << "    gl_FragCoord.xy == " << str(glm::vec2(fragCoord)) << " ma barvu attributes[0].v3 == " << str(b.attributes[0].v3) << ", ale měl mít barvu: " << str(att) << std::endl;

    if(counter>=10)break;
  }

  REQUIRE(false);
  
}



SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - perspective correct interpolation of vertex attributes to fragment attributes" << std::endl;

  uint32_t w = 100;
  uint32_t h = 100;

  MEMCB(w,h);

  auto&inFragments = memCb->inFragments;
  auto&outVertices = memCb->outVertices;

  outVertices.clear();
  inFragments.clear();

  float hc[3] = {1,2,.5};
  float zz[3] = {.9f,.4f,.8f};
  outVertices.resize(3);

  outVertices[0].gl_Position=glm::vec4(-hc[0],-hc[0],zz[0],hc[0]);
  outVertices[1].gl_Position=glm::vec4(+hc[1],-hc[1],zz[1],hc[1]);
  outVertices[2].gl_Position=glm::vec4(-hc[2],+hc[2],zz[2],hc[2]);
  outVertices[0].attributes[0].v3 = glm::vec3(1,0,0);
  outVertices[1].attributes[0].v3 = glm::vec3(0,1,0);
  outVertices[2].attributes[0].v3 = glm::vec3(0,0,1);


  auto&prg = mem.programs[1];
  prg.vertexShader   = vertexShaderInject;
  prg.fragmentShader = fragmentShaderDump;
  prg.vs2fs[0]       = AttributeType::VEC3;

  pushClearCommand(cb);
  pushBindProgramCommand(cb,1);
  pushDrawCommand(cb,3);

  memCb->test();

  bool success = true;

  uint32_t expectedCount = w*h/2;
  uint32_t err = w;
  success &= equalCounts(inFragments.size(),expectedCount,err);

  std::vector<InFragment>badFragments;

  for(auto const&f:inFragments){
    auto l = barycentricsPerspective(outVertices[0],outVertices[1],outVertices[2],f.gl_FragCoord,w,h);

    auto att = 
      outVertices[0].attributes[0].v3 * l[0]+
      outVertices[1].attributes[0].v3 * l[1]+
      outVertices[2].attributes[0].v3 * l[2];

    bool good = equalVec3(f.attributes[0].v3,att); 

    if(!good)
      badFragments.push_back(f);

    success &= good;
  }

  glm::vec3 ndc      [3];for(int i=0;i<3;++i)ndc[i] = glm::vec3(outVertices[i].gl_Position)/outVertices[i].gl_Position.w;
  glm::vec3 viewSpace[3];for(int i=0;i<3;++i)viewSpace[i] = glm::vec3((glm::vec2(ndc[i])*.5f+.5f)*glm::vec2(w,h),ndc[i].z);

  if(!breakTest()&&success)return;
  
  std::cerr << R".(
  Tento test zkouší, zda se perspektivně správně interpoluje vertex attributy.

  Framebuffer má rozlišení: )."<<w<<"x"<<h<<R".(

  Rasterizuje se trojúhelník, který má souřadnice v clip space:
  A.gl_Position = vec4)."<<str(outVertices[0].gl_Position)<<R".(;
  B.gl_Position = vec4)."<<str(outVertices[1].gl_Position)<<R".(;
  C.gl_Position = vec4)."<<str(outVertices[2].gl_Position)<<R".(;

  Což odpovídá rasterizaci trojúhelníků v NDC:
  A = vec3)."<<str(ndc[0])<<R".(;
  B = vec3)."<<str(ndc[1])<<R".(;
  C = vec3)."<<str(ndc[2])<<R".(;

  Což odpovídá rasterizaci trojúhelníků ve view-space:
  A = vec3)."<<str(viewSpace[0])<<R".(;
  B = vec3)."<<str(viewSpace[1])<<R".(;
  C = vec3)."<<str(viewSpace[2])<<R".(;

  Vrcholy obsahují barvu v nultém atributu
  A.attributes[0].v3 = vec3(1,0,0);
  B.attributes[0].v3 = vec3(0,1,0);
  C.attributes[0].v3 = vec3(0,0,1);

  Atributy by měly být interpolovány pomocí barycentrických souřadnic s perspektviní korekcí.

  To znamená, že pokud jsou barycentrické souřadnice l0,l1,l2 spočítané z obsahů ve 2D, pak
  by výsledná barva měla být:

  (A.color*l0/h0 + B.color*l1/h1 + C.color*l2/h2) / (l0/h0 + l1/h0 + l2/h2)

  Byly nalezeny fragmenty, které mají špatně interpolovanou barvu.

  Například tyto fragmenty jsou špatně:)."<<std::endl;

  uint32_t counter = 0;
  for(auto const&f:badFragments){
    counter++;

    auto l = barycentricsPerspective(outVertices[0],outVertices[1],outVertices[2],f.gl_FragCoord,w,h);

    auto att = 
      outVertices[0].attributes[0].v3 * l[0]+
      outVertices[1].attributes[0].v3 * l[1]+
      outVertices[2].attributes[0].v3 * l[2];

    std::cerr << "    gl_FragCoord.xy == " << str(glm::vec2(f.gl_FragCoord)) << " ma barvu == " << str(f.attributes[0].v3) << ", ale měl mít barvu: " << str(att) << std::endl;

    if(counter>=10)break;
  }

  REQUIRE(false);
  
}
