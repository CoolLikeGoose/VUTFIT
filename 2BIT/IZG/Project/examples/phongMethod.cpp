/*!
 * @file
 * @brief This file contains implementation of phong rendering method
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <framework/bunny.hpp>
#include <framework/programContext.hpp>
#include <framework/bunny.hpp>

namespace phongMethod{

/**
 * @brief This class holds all variables of phong method.
 */
class Method: public ::Method{
  public:
    Method(GPUMemory&m,MethodConstructionData const*);
    virtual ~Method();
    virtual void onDraw(SceneParam const&sceneParam) override;
    CommandBuffer commandBuffer;
};


//! [PhongMethod]
/**
 * @brief This function represents vertex shader of phong method.
 *
 * @param outVertex output vertex
 * @param inVertex input vertex
 * @param uniforms uniform variables
 */
void vertexShader(OutVertex&outVertex,InVertex const&inVertex,ShaderInterface const&si){
  auto const pos = glm::vec4(inVertex.attributes[0].v3,1.f);
  auto const&nor = inVertex.attributes[1].v3;
  auto const&viewMatrix       = si.uniforms[0].m4;
  auto const&projectionMatrix = si.uniforms[1].m4;

  auto mvp = projectionMatrix*viewMatrix;

  outVertex.gl_Position = mvp * pos;
  outVertex.attributes[0].v3 = pos;
  outVertex.attributes[1].v3 = nor;
}

/**
 * @brief This function represents fragment shader of phong method.
 *
 * @param outFragment output fragment
 * @param inFragment input fragment
 * @param uniforms uniform variables
 */
void fragmentShader(OutFragment&outFragment,InFragment const&inFragment,ShaderInterface const&si){
  auto const& light          = si.uniforms[2].v3;
  auto const& cameraPosition = si.uniforms[3].v3;
  auto const& vpos           = inFragment.attributes[0].v3;
  auto const& vnor           = inFragment.attributes[1].v3;
  auto vvnor = glm::normalize(vnor);

  auto l = glm::normalize(light-vpos);
  float diffuseFactor                    = glm::dot(l, vvnor);
  if (diffuseFactor < 0.f) diffuseFactor = 0.f;

  auto v = glm::normalize(cameraPosition-vpos);
  auto r = -glm::reflect(v,vvnor);
  float specularFactor                     = glm::dot(r, l);
  if (specularFactor < 0.f) specularFactor = 0.f;
  float const shininess                    = 40.f;

  if (diffuseFactor < 0)
    specularFactor = 0;
  else
    specularFactor = powf(specularFactor, shininess);

  float t = vvnor[1];
  if(t<0.f)t=0.f;
  t*=t;
  auto materialDiffuseColor = glm::mix(glm::vec3(0.f,1.f,0.f),glm::vec3(1.f,1.f,1.f),t);
  
  float const nofStripes = 10;
  float factor = 1.f / nofStripes * 2.f;

  auto xs = static_cast<float>(glm::mod(vpos.x+glm::sin(vpos.y*10.f)*.1f,factor)/factor > 0.5);

  materialDiffuseColor = glm::mix(glm::mix(glm::vec3(0.f,.5f,0.f),glm::vec3(1.f,1.f,0.f),xs),glm::vec3(1.f),t);

  auto materialSpecularColor = glm::vec3(1.f);

  auto diffuseColor  = materialDiffuseColor  * diffuseFactor;
  auto specularColor = materialSpecularColor * specularFactor;

  auto const color = glm::min(diffuseColor + specularColor,glm::vec3(1.f));
  outFragment.gl_FragColor = glm::vec4(color,1.f);
}

/**
 * @brief Constructoro f phong method
 */
Method::Method(GPUMemory&m,MethodConstructionData const*): ::Method(m){
  mem.buffers[0].data = (void const*)bunnyVertices;
  mem.buffers[0].size = sizeof(bunnyVertices);
  mem.buffers[1].data = (void const*)bunnyIndices;
  mem.buffers[1].size = sizeof(bunnyIndices);
  mem.programs[0].vertexShader   = vertexShader;
  mem.programs[0].fragmentShader = fragmentShader;
  mem.programs[0].vs2fs[0]       = AttributeType::VEC3;
  mem.programs[0].vs2fs[1]       = AttributeType::VEC3;

  mem.vertexArrays[0].vertexAttrib[0].bufferID   = 0                  ;
  mem.vertexArrays[0].vertexAttrib[0].type       = AttributeType::VEC3;
  mem.vertexArrays[0].vertexAttrib[0].stride     = sizeof(BunnyVertex);
  mem.vertexArrays[0].vertexAttrib[0].offset     = 0                  ;
  mem.vertexArrays[0].vertexAttrib[1].bufferID   = 0                  ;
  mem.vertexArrays[0].vertexAttrib[1].type       = AttributeType::VEC3;
  mem.vertexArrays[0].vertexAttrib[1].stride     = sizeof(BunnyVertex);
  mem.vertexArrays[0].vertexAttrib[1].offset     = sizeof(glm::vec3)  ;
  mem.vertexArrays[0].indexBufferID = 1                ;
  mem.vertexArrays[0].indexOffset   = 0                ;
  mem.vertexArrays[0].indexType     = IndexType::UINT32;

  pushClearCommand(commandBuffer,glm::vec4(.5,.5,.5,1));
  pushBindProgramCommand(commandBuffer,0);
  pushBindVertexArrayCommand(commandBuffer,0);
  pushDrawCommand (commandBuffer,sizeof(bunnyIndices)/sizeof(VertexIndex));
}


/**
 * @brief This function draws phong method.
 *
 * @param proj projection matrix
 * @param view view matrix
 * @param light light position
 * @param camera camera position
 */
void Method::onDraw(SceneParam const&sceneParam){
  mem.uniforms[0].m4 = sceneParam.view  ;
  mem.uniforms[1].m4 = sceneParam.proj  ;
  mem.uniforms[2].v3 = sceneParam.light ;
  mem.uniforms[3].v3 = sceneParam.camera;
  
  izg_enqueue(mem,commandBuffer);
}

//! [PhongMethod]

/**
 * @brief Destructor of phong method.
 */
Method::~Method(){
}

EntryPoint main = [](){registerMethod<Method>("phong bunny");};
}
