/*!
 * @file
 * @brief This file contains implementation of 3D triangle rendering method.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <framework/programContext.hpp>

namespace triangle3DMethod{

/**
 * @brief This class represents 3D texture rendering method
 */
class Method: public ::Method{
  public:
    Method(GPUMemory&m,MethodConstructionData const*mcd);
    virtual ~Method();
    virtual void onDraw(SceneParam const&sceneParam) override;
    CommandBuffer commandBuffer;
};


/**
 * @brief This function represents vertex shader of 3D triangle rendering method.
 *
 * @param outVertex output vertex
 * @param inVertex input vertex
 * @param uniforms uniform variables
 */
void vertexShader(OutVertex&outVertex,InVertex const&inVertex,ShaderInterface const&si){
  outVertex.gl_Position = glm::vec4(0.f,0.f,0.f,1.f);

  glm::mat4 viewMatrix       = si.uniforms[3].m4;
  glm::mat4 projectionMatrix = si.uniforms[2].m4;
 
  glm::mat4 mvp = projectionMatrix * viewMatrix;

  if(inVertex.gl_VertexID == 0){
    outVertex.gl_Position = mvp*glm::vec4(-1.f,-1.f,0.f,1.f);
    outVertex.attributes[3].v4 = glm::vec4( 1.f, 0.f,0.f,1.f);
  }
  if(inVertex.gl_VertexID == 1){
    outVertex.gl_Position      = mvp*glm::vec4(1.f,-1.f,0.f,1.f);
    outVertex.attributes[3].v4 = glm::vec4(0.f, 1.f,0.f,1.f);
  }
  if(inVertex.gl_VertexID == 2){
    outVertex.gl_Position      = mvp*glm::vec4(-1.f,+1.f,0.f,1.f);
    outVertex.attributes[3].v4 = glm::vec4( 0.f, 0.f,1.f,1.f);
  }

}

/**
 * @brief This functionrepresents fragment shader of 3D triangle rendering method.
 *
 * @param outFragment output fragment
 * @param inFragment input fragment
 * @param uniforms uniform variables
 */
void fragmentShader(OutFragment&outFragment,InFragment const&inFragment,ShaderInterface const&){
  outFragment.gl_FragColor = inFragment.attributes[3].v4;
}

/**
 * @brief Constructor
 */
Method::Method(GPUMemory&m,MethodConstructionData const*): ::Method(m){
  mem.programs[0].vertexShader   = vertexShader;
  mem.programs[0].fragmentShader = fragmentShader;
  mem.programs[0].vs2fs[3]       = AttributeType::VEC4;

  pushClearCommand(commandBuffer,glm::vec4(.1,.1,.1,1));
  pushDrawCommand (commandBuffer,3);
}

/**
 * @brief This function is called every frame and should render 3D triangle
 *
 * @param proj projection matrix
 * @param view view matrix
 * @param light light position
 * @param camera camera position
 */
void Method::onDraw(SceneParam const&sceneParam){
  mem.uniforms[3].m4 = sceneParam.view;
  mem.uniforms[2].m4 = sceneParam.proj;
  izg_enqueue(mem,commandBuffer);
}

/**
 * @brief Descturctor
 */
Method::~Method(){}

EntryPoint main = [](){registerMethod<Method>("triangle3D");};

}
