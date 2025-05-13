#include <iostream>
#include <map>
#include <set>

#include <catch2/catch_test_macros.hpp>

#define __FILENAME__ "gl_FragCoord"
#include <tests/testCommon.hpp>
#include <tests/gpu/common.hpp>

using namespace tests;
using namespace dtl;

SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - gl_FragCoord.xy should be inside triangle" << std::endl;

  auto res = glm::uvec2(100,100);
  MEMCB(res.x,res.y);

  auto&inFragments = memCb->inFragments;
  auto&outVertices = memCb->outVertices;
  initOutVertices();
  inFragments.clear();

  auto&prg = mem.programs[7];
  prg.vertexShader   = vertexShaderInject;
  prg.fragmentShader = fragmentShaderDump;

  pushClearCommand(cb);
  pushBindProgramCommand(cb,7);
  pushDrawCommand(cb,3);

  memCb->test();

  uint32_t expectedCount = res.x*res.y/2;
  uint32_t err = res.x;

  bool success = true;

  success &= equalCounts(inFragments.size(),expectedCount,err);

  glm::vec3 ndc      [3];for(int i=0;i<3;++i)ndc[i]       = glm::vec3(outVertices[i].gl_Position)/outVertices[i].gl_Position.w;
  glm::vec3 viewSpace[3];for(int i=0;i<3;++i)viewSpace[i] = glm::vec3((glm::vec2(ndc[i])*.5f+.5f)*glm::vec2(res),ndc[i].z);

  std::map<V2,size_t>fragPos;

  std::set<V2>duplicatedPositions;
  for(auto const&f:inFragments){
    success &= f.gl_FragCoord.x >   0.f;
    success &= f.gl_FragCoord.y <  100.f;
    success &= lessFloat(f.gl_FragCoord.x + f.gl_FragCoord.y,(float)res.y);

    auto pos = glm::vec2(f.gl_FragCoord);
    auto it = fragPos.find(pos);
    if(it == fragPos.end())
      fragPos[pos] = 0;

    fragPos[pos]++;

    if(fragPos.at(pos)>1)duplicatedPositions.insert(pos);
  }

  success &= duplicatedPositions.empty();
  
  if(!breakTest()&&success)return;
  
  std::cerr << R".(
  Tento test zkouší, zda je pozice fragmentů gl_FragCoord.xy vrámci okna a trojúhelníku

  Framebuffer má rozlišení: )."<<res.x<<"x"<<res.y<<R".(

  Rasterizuje se trojúhelník, který pokrývá polovinu obrazovky:
  A.gl_Position = vec4)."<<str(outVertices.at(0).gl_Position)<<R".(;
  B.gl_Position = vec4)."<<str(outVertices.at(1).gl_Position)<<R".(;
  C.gl_Position = vec4)."<<str(outVertices.at(2).gl_Position)<<R".(;

  Což odpovídá rasterizaci trojúhelníka ve view-space:
  A = vec3)."<<str(viewSpace[0])<<R".(;
  B = vec3)."<<str(viewSpace[1])<<R".(;
  C = vec3)."<<str(viewSpace[2])<<R".(;
   ________
  |*      |
  |***    |
  |*****  |
  |*******|

  Očekává se, že gl_FragCoord.xy každého fragmentu bude v rozmezí

  x > 0 // levá hranice okna
  y > 0 // dolní hranice okna
  y < )."<<res.x<<R".( // horní hranice okna
  x < )."<<res.y<<R".( // pravá hranice okna
  x+y <= )."<<res.y<<R".( // přepona trojúhelníku

  Pozice fragmentu gl_FragCoord.xy by měla odpovídat pozici pixelu + 0.5.
  Fragment pro levý dolní pixel obrazovky bude mít souřadnice (0.5,0.5).
  Fragment pro pravý horní pixel obrazovky bude mít souřadnice ()."<<res.x-1.f+0.5f<<R".(,)."<<res.y-1.f+0.5f<<R".().

  Byly nalezeny fragmenty, které mají špatné gl_FragCoord.xy

  ).";
  if(!duplicatedPositions.empty()){
  std::cerr << R".(
  Byly nalezeny fragmenty, které mají stejné pozice, což nesmí nastat.
  Jsou to třeba tyto fragmenty:)." << std::endl;
    for(auto const&d:duplicatedPositions){
      std::cerr << "    gl_Position.xy == " << str(d.data) << " se vyskytl: " << fragPos.at(d) << "x."<<std::endl;
    }
  }
  REQUIRE(false);
  

}
