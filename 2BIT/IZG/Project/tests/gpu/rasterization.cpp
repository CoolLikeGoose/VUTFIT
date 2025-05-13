#include <iostream>

#include <catch2/catch_test_macros.hpp>

#define __FILENAME__ "rasterization"
#include <tests/testCommon.hpp>
#include <tests/gpu/common.hpp>
using namespace dtl;

using namespace tests;

void triangleRasterizationTest(glm::vec4 const&a,glm::vec4 const&b,glm::vec4 const&c,glm::uvec2 const&res,bool backFaceCulling){
  MEMCB(res.x,res.y);

  auto&inFragments = memCb->inFragments;
  inFragments.clear();
  auto&outVertices = memCb->outVertices;
  outVertices.clear();
  outVertices.resize(3);
  outVertices[0].gl_Position=a;
  outVertices[1].gl_Position=b;
  outVertices[2].gl_Position=c;
  glm::vec3 ndc      [3];for(int i=0;i<3;++i)ndc[i] = glm::vec3(outVertices[i].gl_Position)/outVertices[i].gl_Position.w;
  glm::vec3 viewSpace[3];for(int i=0;i<3;++i)viewSpace[i] = glm::vec3((glm::vec2(ndc[i])*.5f+.5f)*glm::vec2(res),ndc[i].z);

  std::cerr << "  rasterizace trojúhelníku: A=" << str(glm::vec2(viewSpace[0])) << " B=" <<str(glm::vec2(viewSpace[1])) << " C=" << str(glm::vec2(viewSpace[2]));
  std::cerr << " při rozlišení: " << str(res) << " a backFaceCulling: " << str(backFaceCulling) << " dopadla: ";


  auto&prg = mem.programs[9];
  prg.vertexShader   = vertexShaderInject;
  prg.fragmentShader = fragmentShaderDump;

  pushBindProgramCommand(cb,9);
  pushDrawCommand(cb,3,backFaceCulling);

  memCb->test();

  bool success = true;
  
  auto expectedCount = computeNofRasterizedFragments(viewSpace[0],viewSpace[1],viewSpace[2],res,backFaceCulling);
  success &= equalCounts(inFragments.size(),expectedCount.x,expectedCount.y);

  if(!breakTest()&&success){std::cerr << "DOBŘE!" << std::endl;return;}
  
  std::cerr << "ŠPATNĚ!" << std::endl;
  std::cerr << R".(
  Tento test zkouší vyrasterizovat trojúhelník a počítá množství vyresterizovaných fragmentů.

  Pro správné fungování je potřeba sestavit trojúhelník, provést perspektivní dělení, provést view-port transformaci.

  Vrchol co vyleze z vertex shaderu (x,y,z,w)
  Po perspektivním dělení (x/w,y/w,z/w), mělo by to být v rozsahu [-1,-1,-1] -> [+1,+1,-1]
  Po view-port trasnformaci ((x/w*.5+.5)*w,(y/w*.5+.5)*h,...) , mělo by to být v rozsahu [0,0] -> [w,h].

  Co se v tomto testu dělo?
  Framebuffer má velikost: )."<<res.x<<"x"<<res.y<<R".(

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

  Pozor na backFaceCulling, pokud je zapnutý (drawCommand.backFaceCulling==true),
  trojúhelníky, které jsou specifikovány po směru hodinových ručiček nejsou vykresleny.
  V tomto testu byl backFaceCulling=)."<<str(backFaceCulling)<<R".(

  počet vyrasterizovaných fragmentu: )."<<inFragments.size()<<R".(
  přibližný počet fragmentů, který se měl vyrasterizovat: )."<<expectedCount.x<<R".(
  s odchylkou: )."<<expectedCount.y<<std::endl;

  memCb->printHelp(R".(
  struct Triangle{
    OutVertex points[3];
  };
  
  void loadTriangle(Triangle&triangle,Program const&ptr,VertexArray const&vao,uint32_t tId){
    for(...){ // smyčka přes vrcholy trojúhelníku
      InVertex inVertex;
      loadVertex(inVertex,vao,tId+i);
      prg.vertexShader(triangle.points[i],inVertex,...);
    }
  }

  void rasterize(Framebuffer&frame,Triangle const&triangle,Program const&prg){
    ...
    // spočítat hranice trojúhelníku
    // 
   
    if(!backFaceCulling){
      // pokud je vypnuty back face culling, zobrazují se i odvracene trojúhelníky
      // prohodí se, co je uvnitř a venku
    }

    for() // smyčka přes pixely
      for(){
        if(pixel in triangle){
          InFragment inFragment;
          OutFragment outFragment;
          prg.fragmentShader(outFragment,inFragment,...);
          ...
        }
      }
  }

  void izg_enqueue(GPUMemory& mem,CommandBuffer const& cb){

    for( ... commands ...)
    if DRAWCALL
    for( t...){//smyčka přeš trojúhelníky
      Triangle triangle;
      loadTriangle(triangle,mem.vertexArray...,t);

      perspectiveDivision(triangle);

      viewportTransformation(triangle,mem.framebuffers... width,height);

      rasterize(mem.framebuffers...,triangle,mem.programs...);
    }

  }).");

  REQUIRE(false);
}

SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - rasterization should produce correct number of fragments" << std::endl;

  std::vector<glm::uvec2>resolutions = {
    glm::uvec2(100,100),
    glm::uvec2(200,100),
    glm::uvec2(100,200),
    glm::uvec2(337,113),
  };

  struct Triangle{
    Triangle(){}
    Triangle(glm::vec4 const&aa,glm::vec4 const&bb,glm::vec4 const&cc):a(aa),b(bb),c(cc){}
    glm::vec4 a=glm::vec4(1.f);
    glm::vec4 b=glm::vec4(1.f);
    glm::vec4 c=glm::vec4(1.f);
  };

  std::vector<Triangle>triangles = {
    {glm::vec4(-1.0,-1.0,-1,1),glm::vec4(+1.0,+0.0,-1,1),glm::vec4(+0.0,+1.0,-1,1)},
    {glm::vec4(-0.5,-0.6,-1,1),glm::vec4(+0.3,-0.1,-1,1),glm::vec4(-0.6,+0.9,-1,1)},
    {glm::vec4(-1.0,-1.0,-1,1),glm::vec4(+1.0,-1.0,-1,1),glm::vec4(-1.0,+1.0,-1,1)},
    {glm::vec4(+0.0,+0.0,-1,1),glm::vec4(+1.0,+0.0,-1,1),glm::vec4(+0.0,+1.0,-1,1)},
    {glm::vec4(-2.0,-1.0,-1,1),glm::vec4(+1.0,-1.0,-1,1),glm::vec4(-2.0,+1.0,-1,1)},
    {glm::vec4(-1.0,-1.0,-1,1),glm::vec4(+2.0,-1.0,-1,1),glm::vec4(-1.0,+1.0,-1,1)},
    {glm::vec4(-1.0,-2.0,-1,1),glm::vec4(+1.0,-2.0,-1,1),glm::vec4(-1.0,+1.0,-1,1)},
    {glm::vec4(+0.0,+0.0,-1,1),glm::vec4(+0.0,+0.0,-1,1),glm::vec4(+0.0,+0.0,-1,1)},
    {glm::vec4(+0.0,+0.0,-1,1),glm::vec4(+0.0,+0.0,-1,1),glm::vec4(+1.0,+1.0,-1,1)},
    {glm::vec4(+0.0,+0.0,-1,1),glm::vec4(+0.0,+0.0,-1,1),glm::vec4(-1.0,+1.0,-1,1)},
    {glm::vec4(+0.0,+0.0,-1,1),glm::vec4(+0.0,+0.0,-1,1),glm::vec4(-1.0,-1.0,-1,1)},
    {glm::vec4(+0.0,+0.0,-1,1),glm::vec4(+0.0,+0.0,-1,1),glm::vec4(+1.0,-1.0,-1,1)},
    {glm::vec4(-0.4,-1.3,-1,1),glm::vec4(+1.2,+0.2,-1,1),glm::vec4(-0.9,+1.4,-1,1)},
    {glm::vec4(-2.0,-2.0,-1,1),glm::vec4(+8.0,-2.0,-1,1),glm::vec4(-2.0,+8.0,-1,1)},
  };

  for(int b=0;b<2;++b)//backfaceculling false/true
    for(int f=0;f<2;++f)//CCW/CW
      for(auto const&t:triangles)
        for(auto const&r:resolutions){
          if(f==0)//flip CCW to CW
            triangleRasterizationTest(t.a,t.b,t.c,r,(bool)b);
          else
            triangleRasterizationTest(t.a,t.c,t.b,r,(bool)b);
        }
}

