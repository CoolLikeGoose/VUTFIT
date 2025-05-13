/*!
 * @file
 * @brief This file contains implementation of 3D triangle rendering method.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <framework/programContext.hpp>

namespace texturedQuad{

/**
 * @brief This class represents texture rendering method
 */
class Method: public ::Method{
  public:
    Method(GPUMemory&m,MethodConstructionData const*);
    virtual ~Method();
    virtual void onDraw(SceneParam const&sceneParam) override;
    CommandBuffer commandBuffer;
    TextureData tex;///< texture
};

/**
 * @brief This function represents vertex shader of texture rendering method.
 *
 * @param outVertex output vertex
 * @param inVertex input vertex
 * @param uniforms uniform variables
 */
void vertexShader(OutVertex&outVertex,InVertex const&inVertex,ShaderInterface const&){
  outVertex.gl_Position = glm::vec4(0.f,0.f,0.f,1.f);

  //fullscreen quad
  glm::vec2 const verts[]={
    glm::vec2(-1.f,-1.f),
    glm::vec2(+1.f,-1.f),
    glm::vec2(-1.f,+1.f),
    glm::vec2(-1.f,+1.f),
    glm::vec2(+1.f,-1.f),
    glm::vec2(+1.f,+1.f),
  };

  outVertex.gl_Position = glm::vec4(verts[inVertex.gl_VertexID],0.f,1.f);
  outVertex.attributes[0].v2 = (glm::vec2(outVertex.gl_Position)+1.f)*.5f;
}

/**
 * @brief This functionrepresents fragment shader of texture rendering method.
 *
 * @param outFragment output fragment
 * @param inFragment input fragment
 * @param uniforms uniform variables
 */
void fragmentShader(OutFragment&outFragment,InFragment const&inFragment,ShaderInterface const&si){
  auto uv = inFragment.attributes[0].v2;
  outFragment.gl_FragColor = read_texture(si.textures[0],uv);
}

#ifndef CMAKE_ROOT_DIR
#define CMAKE_ROOT_DIR "."
#endif

/**
 * @brief Constructor
 */
Method::Method(GPUMemory&m,MethodConstructionData const*): ::Method(m){
  tex = loadTexture(CMAKE_ROOT_DIR "/resources/images/funnyProject.png");

  mem.textures[0] = tex.getTexture();
  mem.programs[0].vertexShader   = vertexShader  ; 
  mem.programs[0].fragmentShader = fragmentShader;
  mem.programs[0].vs2fs[0]       = AttributeType::VEC2;//tex coords

  pushClearCommand(commandBuffer,glm::vec4(0,0,0,1));
  pushBindProgramCommand(commandBuffer,0);
  pushDrawCommand (commandBuffer,6);
}

/**
 * @brief This function is called every frame and should render 3D triangle
 *
 * @param proj projection matrix
 * @param view view matrix
 * @param light light position
 * @param camera camera position
 */
void Method::onDraw(SceneParam const&){
  izg_enqueue(mem,commandBuffer);
}

/**
 * @brief Descturctor
 */
Method::~Method(){}

EntryPoint main = [](){registerMethod<Method>("funny project");};

}
