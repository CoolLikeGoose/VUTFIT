/*!
 * @file
 * @brief This file contains implementation of simple video player
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <framework/programContext.hpp>

namespace video{

/**
 * @brief This class represents texture rendering method
 */
class Method: public ::Method{
  public:
    Method(GPUMemory&m,MethodConstructionData const*);
    virtual ~Method();
    virtual void onDraw(SceneParam const&sceneParam) override;
    virtual void onUpdate(float dt) override;
    CommandBuffer commandBuffer;
    TextureData tex;///< texture
    float time = 0.f;
};

/**
 * @brief This function represents vertex shader of texture rendering method.
 *
 * @param outVertex output vertex
 * @param inVertex input vertex
 * @param uniforms uniform variables
 */
void vertexShader(OutVertex&outVertex,InVertex const&inVertex,ShaderInterface const&si){
  outVertex.gl_Position = glm::vec4(0.f,0.f,0.f,1.f);

  //fullscreen quad
  glm::vec2 const verts[]={
    glm::vec2( 0.f, 0.f),
    glm::vec2(+1.f, 0.f),
    glm::vec2( 0.f,+1.f),
    glm::vec2( 0.f,+1.f),
    glm::vec2(+1.f, 0.f),
    glm::vec2(+1.f,+1.f),
  };


  float time = si.uniforms[8].v1;

  float len=2.f;
  auto t = time/36.f;


  auto dim = glm::ivec2(20,28);

  int f=(int)(t*dim.x*dim.y);
  f = f % (int)(dim.x*dim.y);

  int x = f%dim.x;
  int y = f/dim.x;
  y = dim.y - y - 1;

  glm::vec2 pos = verts[inVertex.gl_VertexID];
  pos *= dim;
  pos.x-=x;
  pos.y-=y;

  outVertex.gl_Position = glm::vec4(pos*2.f-1.f,0.f,1.f);
  outVertex.attributes[0].v2 = verts[inVertex.gl_VertexID];
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
  tex = loadTexture(CMAKE_ROOT_DIR "/resources/images/video2.png");

  mem.textures[0] = tex.getTexture();
  mem.programs[0].vertexShader   = vertexShader  ; 
  mem.programs[0].fragmentShader = fragmentShader;
  mem.programs[0].vs2fs[0]       = AttributeType::VEC2;//tex coords

  pushClearCommand(commandBuffer,glm::vec4(0,0,0,1));
  pushBindProgramCommand(commandBuffer,0);
  pushDrawCommand (commandBuffer,6);
}

void Method::onDraw(SceneParam const&){
  mem.uniforms[8].v1 = time;
  izg_enqueue(mem,commandBuffer);
}

void Method::onUpdate(float dt){
  time += dt;
}

/**
 * @brief Descturctor
 */
Method::~Method(){}

EntryPoint main = [](){registerMethod<Method>("Video");};

}
