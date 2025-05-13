/*!
 * @file
 * @brief This file contains implementation of czech flag rendering method
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <framework/programContext.hpp>
#include <vector>

namespace stairsMethod{

/**
 * @brief Czech flag rendering method
 */
class Method: public ::Method{
  public:
    Method(GPUMemory&m,MethodConstructionData const*);
    virtual ~Method(){}
    virtual void onDraw(SceneParam const&sceneParam) override;
    virtual void onUpdate(float dt) override;
    CommandBuffer           commandBuffer       ;///< command buffer
    float                   time = 0.f          ;///< elapsed time
};

using namespace glm;

void box(vec4&gl_Position,uint32_t&vMat,int offset,vec3 tt,int m,mat4 mvp,uint32_t gl_VertexID){
  uint32_t const indices[] = {
    0u,1u,2u,2u,1u,3u,
    4u,5u,6u,6u,5u,7u,
    0u,4u,2u,2u,4u,6u,
    1u,5u,3u,3u,5u,7u,
    0u,1u,4u,4u,1u,5u,
    2u,3u,6u,6u,3u,7u
  };
  int vId = gl_VertexID-offset;
  if(vId>=(int)(sizeof(indices)/sizeof(uint32_t))||vId<0)return;

  vMat = m;

  vec3 pos;
  for(uint i=0u;i<3u;++i)
    pos[i] = float((indices[vId]>>i)&1u);

  gl_Position = mvp*vec4(vec3(pos)*vec3(10,1,1)+tt,1.f);
}

/**
 * @brief Czech flag vertex shader
 *
 * @param outVertex out vertex
 * @param inVertex in vertex
 * @param uniforms uniform variables
 */
void vertexShader(OutVertex&outVertex,InVertex const&inVertex,ShaderInterface const&si){
  auto const& vp   = si.uniforms[0].m4;

  auto&vMat   = outVertex.attributes[0].u1;

  auto gl_VertexID = inVertex.gl_VertexID;
  auto iTime = si.uniforms[1].v1;

  outVertex.gl_Position = vec4(0.f,0.f,0.f,1.f);
  int N=int((sin(iTime)*0.5+.5f)*30+1);
  for(int i=0;i<N;++i){
    mat4 rr = mat4(1);
    float c=cos(radians(10.f)*i);
    float s=sin(radians(10.f)*i);
    rr[0] = vec4(c,0,s,0);
    rr[2] = vec4(-s,0,c,0);
    
    box(outVertex.gl_Position,vMat,36*i,vec3(0,i-15,0),i,vp*rr,gl_VertexID);
  }

}

/**
 * @brief Czech flag fragment shader
 *
 * @param outFragment output fragment
 * @param inFragment input fragment
 * @param uniforms uniform variables
 */
void fragmentShader(OutFragment&outFragment,InFragment const&inFragment,ShaderInterface const&){
  auto&vMat   = inFragment.attributes[0].u1;
  float t= float(vMat)/float(30);

  outFragment.gl_FragColor = vec4(vec3(cos(40*t),sin(30*t),cos(10*t))*.5f+.5f,1.f);
}

Method::Method(GPUMemory&m,MethodConstructionData const*): ::Method(m){

  mem.programs[0].vertexShader   = vertexShader;
  mem.programs[0].fragmentShader = fragmentShader;
  mem.programs[0].vs2fs[0]       = AttributeType::UINT;

  pushClearCommand(commandBuffer,glm::vec4(.1,.1,.1,1));
  pushBindProgramCommand(commandBuffer,0);
  pushDrawCommand (commandBuffer,6*6*100);
}

void Method::onUpdate(float dt){
  time += dt;
}

void Method::onDraw(SceneParam const&sceneParam){
  mem.uniforms[0].m4 = sceneParam.proj*sceneParam.view;
  mem.uniforms[1].v1 = time;
  izg_enqueue(mem,commandBuffer);
}

EntryPoint main = [](){registerMethod<Method>("stairs");};

}
