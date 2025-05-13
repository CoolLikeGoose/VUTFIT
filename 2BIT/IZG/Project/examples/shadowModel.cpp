#include <glm/ext/quaternion_common.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <tests/modelTestUtils.hpp>
using namespace tests::model;

#include <framework/programContext.hpp>
#include <framework/model.hpp>

#include <student/prepareModel.hpp>

#include <examples/shadowModel.hpp>

namespace shadowModelMethod{

using namespace glm;

void createShadowMap_vs(OutVertex&outVertex,InVertex const&inVertex,ShaderInterface const&si){
  auto gl_VertexID = inVertex.gl_VertexID;

  auto const&vp    = si.uniforms[4].m4;
  auto const&model = si.uniforms[drawCallUniforms(si.gl_DrawID)+MODEL    ].m4;
  
  outVertex.gl_Position = vp*model*vec4(inVertex.attributes[0].v3,1.f);
}

void createShadowMap_fs(OutFragment&,InFragment const&,ShaderInterface const&){
}

#ifndef CMAKE_ROOT_DIR
#define CMAKE_ROOT_DIR "."
#endif

Method::Method(GPUMemory&m,MethodConstructionData const*): ::Method(m){
  shadowMap = TextureData(1024,1024,1,Image::FLOAT32);

  modelData.load(CMAKE_ROOT_DIR "/resources/models/izg_tf2.glb");
  model = modelData.getModel();
  prepareModel(mem,modelCB,model);

  auto&prg0 = m.programs[0];
  prg0.vertexShader   = createShadowMap_vs;
  prg0.fragmentShader = createShadowMap_fs;

  auto&prg1 = m.programs[1];
  prg1.vertexShader   = drawModel_vertexShader;//scene_vs;
  prg1.fragmentShader = drawModel_fragmentShader;//scene_fs;
  prg1.vs2fs[0] = AttributeType::VEC3;
  prg1.vs2fs[1] = AttributeType::VEC3;
  prg1.vs2fs[2] = AttributeType::VEC2;
  prg1.vs2fs[3] = AttributeType::VEC4;

  m.textures[shadowMapId] = shadowMap.getTexture();
  m.framebuffers[1].depth  = m.textures[shadowMapId].img;
  m.framebuffers[1].width  = m.textures[shadowMapId].width;
  m.framebuffers[1].height = m.textures[shadowMapId].height;

  //draw shadow map
  pushBindFramebufferCommand(drawCB,1);
  pushBindProgramCommand    (drawCB,0);
  pushClearCommand          (drawCB);
  pushSubCommand            (drawCB,&modelCB);

  //draw scene
  pushBindFramebufferCommand(drawCB,0);
  pushBindProgramCommand    (drawCB,1);
  pushClearCommand          (drawCB,glm::vec4(0.4f,0.f,.2f,1.f));
  pushSetDrawIdCommand      (drawCB,0);
  pushSubCommand            (drawCB,&modelCB);

  lightProj = glm::ortho(-100.f,+100.f,-100.f,+100.f,0.f,1000.f);
  lightBias = glm::scale(glm::vec3(.5f,.5f,1.f))*glm::translate(glm::vec3(1,1,0));
}

void Method::onUpdate(float dt){
  time += dt;
}

void Method::onDraw(SceneParam const&sceneParam){
  float atime = 2;
  auto lightPos = glm::vec3(100*glm::cos(atime),100,100*glm::sin(atime));
  auto lightView = glm::lookAt(lightPos,glm::vec3(0,0,0),glm::vec3(0,1,0));

  auto view = glm::lookAt(glm::vec3(69.f,51.f,127.f),glm::vec3(0.f),glm::vec3(0,1,0));

  mem.uniforms[0].m4 = sceneParam.proj*view;//sceneParam.view;
  mem.uniforms[1].v3 = lightPos;
  mem.uniforms[2].v3 = glm::vec3(glm::inverse(view)*glm::vec4(0.f,0.f,0.f,1.f));//sceneParam.camera;
  mem.uniforms[3].m4 = lightBias*lightProj*lightView;
  mem.uniforms[4].m4 = lightProj*glm::translate(glm::vec3(0.f,0.f,-0.5f))*lightView;
  mem.uniforms[6].i1 = shadowMapId;
  mem.uniforms[7].v3 = glm::vec3(0.4f,0.f,0.2f);
  mem.uniforms[8].v3 = glm::vec3(1.f);
  izg_enqueue(mem,drawCB);
}

EntryPoint main = [](){registerMethod<Method>("shadowModel");};

}
