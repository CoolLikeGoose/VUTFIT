/*!
 * @file
 * @brief This file contains implementation of model visualizer
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <framework/model.hpp>
#include <framework/programContext.hpp>
#include <student/prepareModel.hpp>
#include <examples/modelMethod.hpp>

namespace modelMethod{

/**
 * @brief Constructor
 */
Method::Method(GPUMemory&m,MethodConstructionData const*): ::Method(m){
  modelData.load(ProgramContext::get().args.modelFile);
  model = modelData.getModel();

  prepareModel(mem,modelCB,model);

  mem.programs[0].vertexShader   = drawModel_vertexShader;
  mem.programs[0].fragmentShader = drawModel_fragmentShader;
  mem.programs[0].vs2fs[0]       = AttributeType::VEC3;
  mem.programs[0].vs2fs[1]       = AttributeType::VEC3;
  mem.programs[0].vs2fs[2]       = AttributeType::VEC2;

  pushClearCommand(drawCB,glm::vec4(0.1,0.15,0.1,1.),10e10f);
  pushBindProgramCommand(drawCB,0);
  pushSubCommand(drawCB,&modelCB);

}


/**
 * @brief This function is called every frame and should render a model
 *
 * @param proj projection matrix
 * @param view view matrix
 * @param light light position
 * @param camera camera position
 */
void Method::onDraw(SceneParam const&sceneParam){
  mem.uniforms[0].m4 = sceneParam.proj * sceneParam.view;
  mem.uniforms[1].v3 = sceneParam.light;
  mem.uniforms[2].v3 = sceneParam.camera;
  mem.uniforms[6].i1 = -1;
  mem.uniforms[7].v3 = glm::vec3(0.2f);
  mem.uniforms[8].v3 = glm::vec3(1.f);

  izg_enqueue(mem,drawCB);
}

EntryPoint main = [](){registerMethod<Method>("model loader");};

}
