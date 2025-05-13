#include "glm/ext/matrix_clip_space.hpp"
#include "glm/trigonometric.hpp"
#include<iostream>

#include<catch2/catch_test_macros.hpp>
#include<glm/gtc/matrix_transform.hpp>


#include<student/prepareModel.hpp>
#include<student/fwd.hpp>
#include<student/gpu.hpp>

#define __FILENAME__ "vertexShader"
#include<tests/testCommon.hpp>
#include<tests/modelTestUtils.hpp>

using namespace tests;
using namespace tests::model;

SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - drawModel_vertexShader" << std::endl;


  InVertex inV;
  OutVertex outV;

  auto proj  = glm::perspective(glm::radians(60.f),1.f,1.f,100.f);
  auto view  = glm::lookAt(glm::vec3(100.f,120.f,-30.f),glm::vec3(0.f,0.f,10.f),glm::vec3(0.f,1.f,0.f));

  auto lightProj = glm::perspective(glm::radians(55.f),1.f,.2f,100.f);
  auto lightView = glm::lookAt(glm::vec3(300.f),glm::vec3(0.f),glm::vec3(0.f,1.f,0.f));

  auto model = glm::translate(glm::rotate(glm::scale(glm::mat4(1.f),glm::vec3(3.f,5.f,-3.f)),glm::radians(5.f),glm::vec3(1.f,2.f,1.f)),glm::vec3(10.f,30.f,-4.f));
  auto itm   = glm::transpose(glm::inverse(model));
  auto pos   = glm::vec3(1.f,2.f,-3.f);
  auto nor   = glm::vec3(glm::cos(1)*glm::cos(2),glm::sin(1),glm::sin(1)*glm::cos(2));
  auto coord = glm::vec2(0.3,0.8);

  inV.attributes[0].v3 = pos;
  inV.attributes[1].v3 = nor;
  inV.attributes[2].v2 = coord;


  Uniform uniforms[10000];//TODO FIX?

  ShaderInterface si;
  si.uniforms = uniforms;
  si.gl_DrawID = 13;

  uniforms[0].m4 = proj*view;
  uniforms[3].m4 = lightProj*lightView;
  uniforms[drawCallUniforms(si.gl_DrawID)+DrawCallUniform::MODEL    ].m4 = model;
  uniforms[drawCallUniforms(si.gl_DrawID)+DrawCallUniform::INV_MODEL].m4 = itm  ;


  drawModel_vertexShader(outV,inV,si);

  glm::vec4 egl_Position = proj*view*model*glm::vec4(pos,1.f);
  glm::vec3 epos         = glm::vec3(model*glm::vec4(pos,1.f));
  glm::vec3 enor         = glm::vec3(itm*glm::vec4(nor,0.f));
  glm::vec2 ecoord       = coord;
  glm::vec4 sPos         = lightProj*lightView*model*glm::vec4(pos,1.f);

  auto printInputs = [&](){
  };

  auto printInfo = [&](){
    std::cerr<<R".(

    Tento test zkouší, zda je funkce drawModel_vertexShader správně naimplementována.
    Tato funkce reprezentuje vertex shader, který je využíván při kreslení modelů.
    Cílem je ztransformovat vstupní vrchol pomocí matic na výstupní vrchol.
    Vstupní vrchol je tvořen třemi atributy: pozice, normála, texturovací koordináty.
    Vstupní vrchol má také informace o čísle vrcholu: gl_VertexID.

    inVertex.attributes[0].v3 = /* pozice vertexu v modelovém prostoru  */ )." << str(pos  ) << R".(;
    inVertex.attributes[1].v3 = /* normala vertexu v modelovém prostoru */ )." << str(nor  ) << R".(;
    inVertex.attributes[2].v2 = /* tex. coordinaty                      */ )." << str(coord) << R".(;

    Shader ma přístup ke čtyřem maticím:
     - matice kamery                          - jedna pro celý model
     - matice světla                          - pro shadow mapping, jedna pro celý model
     - modelová matice                        - různá pro každý DrawCommand
     - inverzní transponovaná modelová matice - pro normálové vektory

    si.uniforms[0].m4              // matice kamery - cameraProj * cameraView
    si.uniforms[3].m4              // matice světla - lightProj * lightView - pro shadow mapping
    si.uniforms[)."<<drawCallUniformOffset<<R".(+gl_Draw*)."<<drawCallNofUniforms<<R".(+)."<<DrawCallUniform::MODEL<<R".(].m4 // modelMatrix
    si.uniforms[)."<<drawCallUniformOffset<<R".(+gl_Draw*)."<<drawCallNofUniforms<<R".(+)."<<DrawCallUniform::INV_MODEL<<R".(].m4 // invModelMatrix
    si.gl_DrawId = )." << si.gl_DrawID << R".(

    Výstupem shaderu je:
     - outVertex.gl_Position      = ...; // pozice vertexu v clip-space kamery
     - outVertex.attributes[0].v3 = ...; // pozice vertexu ve world-space - pro výpočet osvětlení ve FS
     - outVertex.attributes[1].v3 = ...; // normala ve world-space - pro výpočet osvětlení ve FS
     - outVertex.attributes[2].v2 = ...; // texturovací souřadnice - pro čtení barvy povrchu z textury
     - outVertex.attributes[3].v4 = ...; // pozice vertexu v clip-space světla - pro stíny
    
    Pamatujte, pro transformaci pozice do clip-space je potřeba pronásobit vertex projekční, view i modelovou maticí.

    Normály by se měly násobit inverzní transponovanou maticí.
    Normály by měly mít homogenní složku = 0 protože to jsou vektory.
    Souřadnice do stínové mapy by měly být vynásobeny lightProjView a modelovou maticí.

    Něco se pokazilo...)."<<std::endl;
  };

  if(breakTest() || !equalVec4(egl_Position,outV.gl_Position)){
    printInfo();
    std::cerr << R".(
    Pozice gl_Position není správně.
    Měla být: )."<<str(egl_Position)<<R".(
    Ale  je : )."<<str(outV.gl_Position)<<R".(
    ).";
    REQUIRE(false);
  }

  if(breakTest() || !equalVec3(epos,outV.attributes[0].v3)){
    printInfo();
    std::cerr << R".(
    Pozice ve world space není správná.
    Měla být: )."<<str(epos)<<R".(
    Ale  je : )."<<str(outV.attributes[0].v3)<<R".(
    ).";
    REQUIRE(false);
  }

  if(breakTest() || !equalVec3(enor,outV.attributes[1].v3)){
    printInfo();
    std::cerr << R".(
    Normála ve world space není správná.
    Měla být: )."<<str(enor)<<R".(
    Ale  je : )."<<str(outV.attributes[1].v3)<<R".(
    ).";
    REQUIRE(false);
  }

  if(breakTest() || !equalVec2(ecoord,outV.attributes[2].v2)){
    printInfo();
    std::cerr << R".(
    Texturovací koordináty nejsou správné.
    Měly být: )."<<str(ecoord)<<R".(
    Ale  je : )."<<str(outV.attributes[2].v2)<<R".(
    ).";
    REQUIRE(false);
  }

  if(breakTest() || !equalVec4(sPos,outV.attributes[3].v4)){
    printInfo();
    std::cerr << R".(
    Pozice vertexu v prostoru světla je nesprávná.
    Měly být: )."<<str(sPos)<<R".(
    Ale  je : )."<<str(outV.attributes[3].v4)<<R".(
    ).";
    REQUIRE(false);
  }

}


