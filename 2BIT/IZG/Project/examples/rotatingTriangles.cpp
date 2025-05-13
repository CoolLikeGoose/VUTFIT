/*!
 * @file
 * @brief This file contains implementation of rotating triangles.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <framework/programContext.hpp>

namespace rotatingTriangles{

/**
 * @brief This class represents 3D texture rendering method
 */
class Method: public ::Method{
  public:
    Method(GPUMemory&m,MethodConstructionData const*mcd);
    virtual ~Method();
    virtual void onDraw(SceneParam const&sceneParam) override;
    virtual void onUpdate(float dt) override;
    CommandBuffer commandBuffer;
    float         time = 0.f   ;///< elapsed time
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

  auto iTime = si.uniforms[0].v1;

  float ca = glm::cos(iTime*.1f);
  float sa = glm::sin(iTime*.1f);
  auto rot = glm::mat4(1);
  rot[0][0]=ca;
  rot[1][1]=ca;
  rot[0][1]=sa;
  rot[1][0]=-sa;
  float s = 0.2f;
  struct Vertex{
    glm::vec2 position;
    glm::vec3 color   ;
  };
  Vertex const verts[] = {
    {glm::vec2(-s,-s),glm::vec3(1,0,0)},
    {glm::vec2(+s,-s),glm::vec3(1,0,0)},
    {glm::vec2(-s,+s),glm::vec3(1,0,0)},
    {glm::vec2(-s,+s),glm::vec3(0,1,0)},
    {glm::vec2(+s,-s),glm::vec3(0,1,0)},
    {glm::vec2(+s,+s),glm::vec3(0,1,0)},
    {glm::vec2(-s  ,  -s),glm::vec3(0,0,1)},
    {glm::vec2(+s  ,  -s),glm::vec3(0,0,1)},
    {glm::vec2( 0  ,-2*s),glm::vec3(0,0,1)},
    {glm::vec2(-s  ,  -s),glm::vec3(1,0,1)},
    {glm::vec2(-s  ,  +s),glm::vec3(1,0,1)},
    {glm::vec2(-2*s,   0),glm::vec3(1,0,1)},
    {glm::vec2(+s  ,  -s),glm::vec3(1,1,0)},
    {glm::vec2(+s  ,  +s),glm::vec3(1,1,0)},
    {glm::vec2(+2*s,   0),glm::vec3(1,1,0)},
    {glm::vec2(-s  ,  +s),glm::vec3(0,1,1)},
    {glm::vec2(+s  ,  +s),glm::vec3(0,1,1)},
    {glm::vec2( 0  ,+2*s),glm::vec3(0,1,1)},
  };

  if(inVertex.gl_VertexID < sizeof(verts)/sizeof(Vertex)){
    auto const&v = verts[inVertex.gl_VertexID];
    outVertex.gl_Position      = rot*glm::vec4(v.position,0,1);
    outVertex.attributes[3].v4 = glm::vec4(v.color,1.f);
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
  pushBindProgramCommand(commandBuffer,0);
  pushDrawCommand (commandBuffer,100*3);
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
  mem.uniforms[0].v1 = time;
  izg_enqueue(mem,commandBuffer);
}

void Method::onUpdate(float dt){
  time += dt;
}

/**
 * @brief Descturctor
 */
Method::~Method(){}

EntryPoint main = [](){registerMethod<Method>("Rotating triangles");};

}
