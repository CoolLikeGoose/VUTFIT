/*!
 * @file
 * @brief This file contains implementation of basic shadow mapping algorithm.
 * More information can be found here
 * <a href="https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping">shadow mapping</a>
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <framework/programContext.hpp>


namespace shadowMappingMethod{

using namespace glm;

//! [ShadowMapping]
/**
 * @brief Empty rendering method
 */
class Method: public ::Method{
  public:
    Method(GPUMemory&m,MethodConstructionData const*);
    virtual ~Method(){}
    virtual void onDraw(SceneParam const&sceneParam) override;
    virtual void onUpdate(float dt) override;
    float time = 0;
    CommandBuffer commandBuffer;
    TextureData shadowMap;
};

// struktura reprezentujici vertex
struct Vertex{
  vec3 pos; // pozice
  vec3 col; // barva
};

// vertex sceny - dva čtverce, jeden zeleny a druhy červený
Vertex const vertices[] = {
  {vec3(-8,8,-8),vec3(1,0,0)},
  {vec3(+8,8,-8),vec3(1,0,0)},
  {vec3(-8,8,+8),vec3(1,0,0)},
  {vec3(-8,8,+8),vec3(1,0,0)},
  {vec3(+8,8,-8),vec3(1,0,0)},
  {vec3(+8,8,+8),vec3(1,0,0)},

  {vec3(-100,0,-100),vec3(0,1,0)},
  {vec3(+100,0,-100),vec3(0,1,0)},
  {vec3(-100,0,+100),vec3(0,1,0)},
  {vec3(-100,0,+100),vec3(0,1,0)},
  {vec3(+100,0,-100),vec3(0,1,0)},
  {vec3(+100,0,+100),vec3(0,1,0)},

};

// vertex shader pro vytvoření shadow mapy
void createShadowMap_vs(OutVertex&outVertex,InVertex const&inVertex,ShaderInterface const&si){
  auto gl_VertexID = inVertex.gl_VertexID;

  // light view matice
  auto view = si.uniforms[2].m4;
  // light projekční matice 
  auto proj = si.uniforms[3].m4;

  // vypočet pozice vrcholu v clip-space
  outVertex.gl_Position = view*vec4(vertices[gl_VertexID].pos,1.f);
  outVertex.gl_Position.z -= .5f; // bias (proti self shadowingu)
  outVertex.gl_Position = proj * outVertex.gl_Position;
}

// nepotřebujeme fragment shader, stačí nám hloubka
void createShadowMap_fs(OutFragment&,InFragment const&,ShaderInterface const&){
}

// vertex shader pro výpočet stínu
void scene_vs(OutVertex&outVertex,InVertex const&inVertex,ShaderInterface const&si){
  // číslo vrcholu
  auto gl_VertexID = inVertex.gl_VertexID;

  // view matice kamery
  auto view = si.uniforms[0].m4;
  // projekční matice kamery
  auto proj = si.uniforms[1].m4;
  // view matice světla
  auto lightView = si.uniforms[2].m4;
  // projekční matice světla
  auto lightProj = si.uniforms[3].m4;
  // bias matice světla
  auto lightBias = si.uniforms[4].m4;

  // pozice vertexu ve world-space
  auto vertex = vec4(vertices[gl_VertexID].pos,1.f);

  // zápis barvy
  outVertex.attributes[0].v3 = vertices[gl_VertexID].col;
  // zápis pozice vertexu v clip-space světla, tady jsou uvedeny všechny matice explicitně
  outVertex.attributes[1].v4 = lightBias*lightProj*lightView*vertex;
  // zápis pozice vertexu v clip-space kamery
  outVertex.gl_Position = proj*view*vertex;
}

// fragment shader pro výpočet stínu
void scene_fs(OutFragment&outFragment,InFragment const&inFragment,ShaderInterface const&si){
  // barva
  auto color     = inFragment.attributes[0].v3;
  // pozice fragmentu v clip-space světla
  auto shadowPos = inFragment.attributes[1].v4;

  // perspektivní dělení
  shadowPos/=shadowPos.w;

  // vyčtení hloubky ze stínové mapy
  auto sm = read_textureClamp(si.textures[1],glm::vec2(shadowPos)).r;

  // je hloubka fragmentu větší než to, co je ve stínové mapě?
  auto isShadow = (float)(shadowPos.z > sm);

  // útlum barvy stínem
  color *= (1.f - .5f*isShadow);
  
  // zápis barvy
  outFragment.gl_FragColor = vec4(color,1.f);
}


Method::Method(GPUMemory&m,MethodConstructionData const*): ::Method(m){
  // vytvoření stínové mapy (data)
  shadowMap = TextureData(m.framebuffers[0].width,m.framebuffers[0].height,1,Image::FLOAT32);

  // program pro vytvoření stínové mapy
  auto&prg0 = m.programs[0];
  prg0.vertexShader   = createShadowMap_vs;
  prg0.fragmentShader = createShadowMap_fs;

  // program pro vykreslení scény se stíny
  auto&prg1 = m.programs[1];
  prg1.vertexShader   = scene_vs;
  prg1.fragmentShader = scene_fs;
  prg1.vs2fs[0] = AttributeType::VEC3;
  prg1.vs2fs[1] = AttributeType::VEC4;

  // framebuffer pro vykreslování stínové mapy
  m.textures[1] = shadowMap.getTexture();
  m.framebuffers[1].depth  = m.textures[1].img;
  m.framebuffers[1].width  = m.textures[1].width;
  m.framebuffers[1].height = m.textures[1].height;

  // vykreslení stínové mapy
  pushBindFramebufferCommand(commandBuffer,1);
  pushBindProgramCommand(commandBuffer,0);
  pushClearCommand(commandBuffer,glm::vec4(0,0,0,1));
  pushDrawCommand(commandBuffer,12);

  // vykreslení scény
  pushBindFramebufferCommand(commandBuffer,0);
  pushBindProgramCommand(commandBuffer,1);
  pushClearCommand(commandBuffer,glm::vec4(0,0,0,1));
  pushDrawCommand(commandBuffer,12);
}

// časovač
void Method::onUpdate(float dt){
  time += dt;
}

void Method::onDraw(SceneParam const&sceneParam){
  // výpočet matic
  auto lightView = glm::lookAt(glm::vec3(100*glm::cos(time),100,100*glm::sin(time)),glm::vec3(0,0,0),glm::vec3(0,1,0));
  auto lightProj = glm::ortho(-100.f,+100.f,-100.f,+100.f,0.f,1000.f);
  auto lightBias = glm::scale(glm::mat4(1.f),glm::vec3(.5f,.5f,1.f))*glm::translate(glm::mat4(1.f),glm::vec3(1,1,0));

  // nastavení uniformních proměnných
  mem.uniforms[0].m4 = sceneParam.view;
  mem.uniforms[1].m4 = sceneParam.proj;
  mem.uniforms[2].m4 = lightView;
  mem.uniforms[3].m4 = lightProj;
  mem.uniforms[4].m4 = lightBias;
  mem.uniforms[5].i1 = -1;
  mem.uniforms[7].v3 = glm::vec3(0.2f);
  mem.uniforms[8].v3 = glm::vec3(1.f);
  izg_enqueue(mem,commandBuffer);
}
//! [ShadowMapping]

EntryPoint main = [](){registerMethod<Method>("shadowMapping");};

}
