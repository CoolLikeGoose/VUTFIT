#include <iostream>

#include <catch2/catch_test_macros.hpp>

#define __FILENAME__ "perspectiveDivision"
#include <tests/testCommon.hpp>
#include <tests/gpu/common.hpp>

using namespace tests;
using namespace dtl;

SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - perspective division" << std::endl;
  auto res = glm::uvec2(100,100);
  MEMCB(res.x,res.y);
  auto backFaceCulling = true;

  float f0 = 0.1f;
  float f1 = 0.3f;
  float f2 = 4.0f;
  initOutVertices();
  auto&outVertices = memCb->outVertices;
  auto&inFragments = memCb->inFragments;
  outVertices[0].gl_Position*=f0;
  outVertices[1].gl_Position*=f1;
  outVertices[2].gl_Position*=f2;
  inFragments.clear();
  glm::vec3 ndc      [3];for(int i=0;i<3;++i)ndc[i] = glm::vec3(outVertices[i].gl_Position)/outVertices[i].gl_Position.w;
  glm::vec3 viewSpace[3];for(int i=0;i<3;++i)viewSpace[i] = glm::vec3((glm::vec2(ndc[i])*.5f+.5f)*glm::vec2(res),ndc[i].z);

  auto&prg = mem.programs[4];
  prg.vertexShader   = vertexShaderInject;
  prg.fragmentShader = fragmentShaderDump;

  pushClearCommand(cb);
  pushBindProgramCommand(cb,4);
  pushDrawCommand(cb,3,backFaceCulling);

  memCb->test();

  bool success = true;

  auto expectedCount = computeNofRasterizedFragments(viewSpace[0],viewSpace[1],viewSpace[2],res,backFaceCulling);
  success &= equalCounts(inFragments.size(),expectedCount.x,expectedCount.y);

  if(!breakTest()&&success)return;
  std::cerr << R".(
  Tento test zkouší, zda se provádí perspektivní dělení.
  Z vertex shaderu vycházejí vrcholy, které mají homogenní složku jinou od 1.
  
  Framebuffer má rozlišení: )."<<res.x<<"x"<<res.y<<R".(

  Rasterizuje se trojúhelník, který má souřadnice v clip space:
  A.gl_Position = vec4)."<<str(outVertices[0].gl_Position)<<R".(;
  B.gl_Position = vec4)."<<str(outVertices[1].gl_Position)<<R".(;
  C.gl_Position = vec4)."<<str(outVertices[2].gl_Position)<<R".(;

  Což odpovídá rasterizaci trojúhelníků v NDC (po perspektivním dělení):
  A = vec3)."<<str(ndc[0])<<R".(;
  B = vec3)."<<str(ndc[1])<<R".(;
  C = vec3)."<<str(ndc[2])<<R".(;

  Což odpovídá rasterizaci trojúhelníků ve view-space:
  A = vec3)."<<str(viewSpace[0])<<R".(;
  B = vec3)."<<str(viewSpace[1])<<R".(;
  C = vec3)."<<str(viewSpace[2])<<R".(;

  počet vyrasterizovaných fragmentu: )."<<inFragments.size()<<R".(
  přibližný počet fragmentů, který se měl vyrasterizovat: )."<<expectedCount.x<<R".(
  s odchylkou: )."<<expectedCount.y<<std::endl;

  REQUIRE(false);
}
