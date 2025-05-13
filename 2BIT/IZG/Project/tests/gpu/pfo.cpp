#include <iostream>

#include <catch2/catch_test_macros.hpp>

#define __FILENAME__ "pfo"
#include <tests/testCommon.hpp>

using namespace tests;

void fragmentShaderDepthTest(OutFragment&outF,InFragment const&inF,ShaderInterface const&){
  outF.gl_FragColor = glm::vec4(inF.attributes[0].v4);
}

SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - depth test" << std::endl;


  auto res = glm::uvec2(100,100);
  MEMCB(res.x,res.y);

  auto&inFragments = memCb->inFragments;
  auto&outVertices = memCb->outVertices;

  outVertices.clear();
  outVertices.resize(9);
  outVertices[0].gl_Position = glm::vec4(-1,-1,0,1);
  outVertices[1].gl_Position = glm::vec4(+4,-1,0,1);
  outVertices[2].gl_Position = glm::vec4(-1,+4,0,1);
  outVertices[3].gl_Position = glm::vec4(-1,-1,-.1,1);
  outVertices[4].gl_Position = glm::vec4(+0,-1,-.1,1);
  outVertices[5].gl_Position = glm::vec4(-1,+0,-.1,1);
  outVertices[6].gl_Position = glm::vec4(-1,-1,+.1,1);
  outVertices[7].gl_Position = glm::vec4(+0,-1,+.1,1);
  outVertices[8].gl_Position = glm::vec4(-1,+0,+.1,1);

  outVertices[0].attributes[0].v4 = glm::vec4(1,0,0,1);
  outVertices[1].attributes[0].v4 = glm::vec4(1,0,0,1);
  outVertices[2].attributes[0].v4 = glm::vec4(1,0,0,1);
  outVertices[3].attributes[0].v4 = glm::vec4(0,1,0,1);
  outVertices[4].attributes[0].v4 = glm::vec4(0,1,0,1);
  outVertices[5].attributes[0].v4 = glm::vec4(0,1,0,1);
  outVertices[6].attributes[0].v4 = glm::vec4(0,0,1,1);
  outVertices[7].attributes[0].v4 = glm::vec4(0,0,1,1);
  outVertices[8].attributes[0].v4 = glm::vec4(0,0,1,1);

  inFragments.clear();
  glm::vec3 ndc      [3];for(int i=0;i<3;++i)ndc[i] = glm::vec3(outVertices[i].gl_Position)/outVertices[i].gl_Position.w;
  glm::vec3 viewSpace[3];for(int i=0;i<3;++i)viewSpace[i] = glm::vec3((glm::vec2(ndc[i])*.5f+.5f)*glm::vec2(res),ndc[i].z);

  auto&prg = mem.programs[3];
  prg.vertexShader   = vertexShaderInject;
  prg.fragmentShader = fragmentShaderDepthTest;
  prg.vs2fs[0]       = AttributeType::VEC4;

  pushClearCommand(cb);
  pushBindProgramCommand(cb,3);
  pushDrawCommand(cb,9);

  memCb->test();

  bool success = true;

  auto const&fbo= memCb->studentMem.mem.framebuffers[0];
  auto col0 = getColorU(fbo,0,0);
  auto col1 = getColorU(fbo,fbo.width-1,fbo.height-1);

  success &= col0[0] == 0;
  success &= col0[1] == 255;
  success &= col0[2] == 0  ;

  success &= col1[0] == 255;
  success &= col1[1] == 0  ;
  success &= col1[2] == 0  ;


  if(!breakTest()&&success)return;
  
  std::cerr << R".(Tento test kontroluje, jetli funguje depth test.
  Fragmenty, které mají menší hloubku, než je ta, která je poznačená ve framebufferu
  by měly změnit barvu a hloubu ve framebuffer.

  Ale něco je špatně...

  Nápověda:

  if(fragDepth < frame.depth[pix]){
    frame.depth[pix] = fragDepth;
    frame.color[pix] = fragColor;
  }

  ).";
  REQUIRE(false);
  
}

SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - blending" << std::endl;

  auto res = glm::uvec2(100,100);

  MEMCB(res.x,res.y);

  auto&inFragments = memCb->inFragments;
  auto&outVertices = memCb->outVertices;

  auto color      = glm::vec4(.3,.4,.2,0.7);
  auto frameColor = glm::vec4(.5f,.3f,.2f,1.f);

  outVertices.clear();
  outVertices.resize(3);
  outVertices[0].gl_Position = glm::vec4(-1,-1,0,1);
  outVertices[1].gl_Position = glm::vec4(+4,-1,0,1);
  outVertices[2].gl_Position = glm::vec4(-1,+4,0,1);

  outVertices[0].attributes[0].v4 = color;
  outVertices[1].attributes[0].v4 = color;
  outVertices[2].attributes[0].v4 = color;

  inFragments.clear();

  auto&prg = mem.programs[9];
  prg.vertexShader   = vertexShaderInject;
  prg.fragmentShader = fragmentShaderDepthTest;
  prg.vs2fs[0]       = AttributeType::VEC4;

  pushClearCommand(cb,frameColor);
  pushBindProgramCommand(cb,9);
  pushDrawCommand(cb,3);

  memCb->test();

  bool success = true;

  auto samplePos = glm::uvec2(10,10);

  auto finalColor    = getColorU(memCb->studentMem.mem.framebuffers[0],samplePos.x,samplePos.y);
  auto expectedColor = alphaMix(frameColor,color);

  success &= finalColor.r == expectedColor.r;
  success &= finalColor.g == expectedColor.g;
  success &= finalColor.b == expectedColor.b;

  if(!breakTest()&&success)return;
  
  std::cerr << R".(Tento test kontroluje, jetli funguje blending (přimíchávání barev).

  Fragmenty, které mají 4. komponentu barvy jinou od 1 by se měly přimíchat
  ke stávající barvě framebufferu.
  4. komponenta barvy je alpha - "neprůhlednost".
  0 - absolutně průhledné, 1 - absolutně neprůhledné.
  Přimíchání spočívá v tom, že se vyváhuje barva fragmentu s barvou
  ve framebufferu pomocí alpha hodnoty.

  Ale něco je špatně...

  Barva framebufferu byla nastavena na hodnotu:)."<< str(frameColor)<<std::endl;
  std::cerr << R".(
  Rasterizuje se trojúhelník přes celou obrazovku a má barvu:)."<<str(color)<<std::endl;
  std::cerr << R".(
  Výsledná barva:)."<<str(finalColor)<<R".( se liší od barvy,
  co by tam měla být:)."<<str(expectedColor)<<std::endl;

  memCb->printHelp(R".(
  // obě barvy jsou v rozsahu [0,1]
  vec4 fragColor       ; // barva fragmentu
  vec4 framebufferColor; // barva ve framebufferu
  float a = fragColor.a; // nepruhlenost

  // finalni barva
  vec3 finalColor = fragColor.rgb * a + framebufferColor.rgb * (1-a);
  // je potřeba ji převést na správný formát

  Blending se váže k OpenGL blendingu a blendovacímu módu:
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  ).");
  REQUIRE(false);
  
}
