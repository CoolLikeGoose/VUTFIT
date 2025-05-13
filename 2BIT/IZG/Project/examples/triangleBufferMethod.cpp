/*!
 * @file
 * @brief This file contains implementation of rendering method that renders triangle stored in buffer.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <framework/programContext.hpp>

namespace triangleBufferMethod{

/**
 * @brief This class represents triangle buffer rendering method
 */
class Method: public ::Method{
  public:
    Method(GPUMemory&m,MethodConstructionData const*);
    virtual ~Method(){}
    virtual void onDraw(SceneParam const&sceneParam) override;
    CommandBuffer commandBuffer;///< command buffer
    std::vector<float>buffer;///< vertex buffer
};

/**
 * @brief This function is vertex shader of triangle buffer method
 *
 * @param outVertex output vertex
 * @param inVertex input vertex
 * @param uniforms uniform variables
 */
void vertexShader(OutVertex&outVertex,InVertex const&inVertex,ShaderInterface const&){
  outVertex.gl_Position = glm::vec4(inVertex.attributes[0].v2,0.f,1.f);
}

/**
 * @brief This function is fragment shader of triangle buffer method
 *
 * @param outFragment output fragment
 * @param inFragment input fragment 
 * @param uniforms uniform variables
 */
void fragmentShader(OutFragment&outFragment,InFragment const&,ShaderInterface const&){
  outFragment.gl_FragColor = glm::vec4(0.f,1.f,0.f,1.f);
}


Method::Method(GPUMemory&m,MethodConstructionData const*): ::Method(m){
  buffer = {
    -.5f,-.5f,
    -.1f,-.5f,
    -.5f,-.1f,

    +.1f,-.5f,
    +.5f,-.5f,
    +.5f,-.1f,

    -.5f,+.1f,
    -.1f,+.5f,
    -.5f,+.5f,

    +.1f,+.5f,
    +.5f,+.1f,
    +.5f,+.5f,
  };

  mem.buffers[0].data = buffer.data();
  mem.buffers[0].size = buffer.size() * sizeof(decltype(buffer)::value_type);
  mem.programs[0].vertexShader   = vertexShader;
  mem.programs[0].fragmentShader = fragmentShader;

  mem.vertexArrays[0].vertexAttrib[0].bufferID = 0                  ;
  mem.vertexArrays[0].vertexAttrib[0].type     = AttributeType::VEC2;
  mem.vertexArrays[0].vertexAttrib[0].stride   = sizeof(float)*2    ;
  mem.vertexArrays[0].vertexAttrib[0].offset   = 0                  ;

  pushClearCommand          (commandBuffer,glm::vec4(0));
  pushBindProgramCommand    (commandBuffer,0           );
  pushBindVertexArrayCommand(commandBuffer,0           );
  pushDrawCommand           (commandBuffer,3*4         );
}


void Method::onDraw(SceneParam const&){
  izg_enqueue(mem,commandBuffer);
}

EntryPoint main = [](){registerMethod<Method>("triangleBuffer");};

}
