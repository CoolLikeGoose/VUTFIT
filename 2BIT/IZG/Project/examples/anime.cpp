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

namespace animeMethod{

class Method: public ::Method{
  public:
    Method(GPUMemory&m,MethodConstructionData const*cd = nullptr);
    virtual ~Method(){}
    virtual void onDraw(SceneParam const&sceneParam) override;
    virtual void onUpdate(float dt) override;
    float time = 0;

    ModelData     modelData;
    Model         model;

    CommandBuffer modelCB;
    CommandBuffer drawCB;
};

using namespace glm;

#ifndef CMAKE_ROOT_DIR
#define CMAKE_ROOT_DIR "."
#endif

Method::Method(GPUMemory&m,MethodConstructionData const*): ::Method(m){

  modelData.load(CMAKE_ROOT_DIR "/resources/models/young_priestess/scene.gltf");
  model = modelData.getModel();
  prepareModel(mem,modelCB,model);

  auto&prg1 = m.programs[0];
  prg1.vertexShader   = drawModel_vertexShader;//scene_vs;
  prg1.fragmentShader = drawModel_fragmentShader;//scene_fs;
  prg1.vs2fs[0] = AttributeType::VEC3;
  prg1.vs2fs[1] = AttributeType::VEC3;
  prg1.vs2fs[2] = AttributeType::VEC2;
  prg1.vs2fs[3] = AttributeType::VEC4;

  //draw scene
  pushBindFramebufferCommand(drawCB,0);
  pushBindProgramCommand    (drawCB,0);
  pushClearCommand          (drawCB,glm::vec4(0xd4,0x6d,0x63,0xff)/255.f);
  pushSetDrawIdCommand      (drawCB,0);
  pushSubCommand            (drawCB,&modelCB);

}

void Method::onUpdate(float dt){
  time += dt;
}

void Method::onDraw(SceneParam const&sceneParam){

  auto view = glm::lookAt(glm::vec3(140.f*cos(time),-50.f,140.f*sin(time)),glm::vec3(0.f,-50.f,0.f),glm::vec3(0,1,0));

  mem.uniforms[0].m4 = sceneParam.proj*view;
  mem.uniforms[1].v3 = glm::vec3(10.f);
  mem.uniforms[2].v3 = glm::vec3(glm::inverse(view)*glm::vec4(0.f,0.f,0.f,1.f));//sceneParam.camera;
  mem.uniforms[6].i1 = -1;
  mem.uniforms[7].v3 = glm::vec3(1.f,1.f,1.0f);
  mem.uniforms[8].v3 = glm::vec3(0.f);
  izg_enqueue(mem,drawCB);
}

EntryPoint main = [](){registerMethod<Method>("anime - somebody suggest I should include it...");};

}
