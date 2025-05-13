/*!
 * @file
 * @brief This file contains implementation of czech flag rendering method
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <framework/programContext.hpp>
#include <vector>

namespace angryMethod{

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

/**
 * @brief Czech flag vertex shader
 *
 * @param outVertex out vertex
 * @param inVertex in vertex
 * @param uniforms uniform variables
 */
void vertexShader(OutVertex&outVertex,InVertex const&inVertex,ShaderInterface const&si){
  auto const& vp   = si.uniforms[0].m4;

  auto&vCoord = outVertex.attributes[0].v2;
  auto&vPos   = outVertex.attributes[1].v3;
  auto&voff   = outVertex.attributes[2].u1;

  auto gl_VertexID = inVertex.gl_VertexID;
  
  const vec2 vertices[] = {
    vec2(-1.f,-1.f),
    vec2(+1.f,-1.f),
    vec2(-1.f,+1.f),
    vec2(-1.f,+1.f),
    vec2(+1.f,-1.f),
    vec2(+1.f,+1.f)
  };
  int vid = gl_VertexID % 6;
  int fid =  gl_VertexID / 6;
  voff = fid;
  vec2 pos =  vertices[vid];
  vPos = vec3(pos,0);
  outVertex.gl_Position = vp * vec4(pos+vec2(fid%10*2-10,fid/10*3-40),0.f,1.f);
  vCoord = pos*0.5f+0.5f;
}

vec4 angryTexture(vec2 uv,uint32_t voff,float iTime){
  float dist = distance(uv,vec2(.5f,.5f));
  
  vec2 mrk = vec2(1,1+100*pow(abs(sin(iTime*2+voff)),60.f));
  vec2 mrk2 = vec2(1,1+100*pow(abs(sin(iTime*3+2+voff)),60.f));
  vec2 ee1 = vec2(.75f,.7f);
  vec2 ee2 = vec2(.25f,.7f);  
  float e1 = distance((uv-ee1)*mrk+ee1,ee1);
  float e2 = distance((uv-ee2)*mrk2+ee2,ee2);
  
  vec2 mmp1 = vec2(.5f,.23f);
  vec2 mmp2 = vec2(.5f,.23f+0.06*sin(iTime+voff*30)); 
  vec2 mm1 =   vec2(0.7,1);
  float m1 = distance((uv-mmp1)*mm1+mmp1,mmp1);
  float m2 = distance((uv-mmp2)*mm1+mmp2,mmp2); 
  if(e1<0.02+0.06*sin(voff))return vec4(0,0,0,1);
  if(e2<0.02+0.06*sin(voff))return vec4(0,0,0,1);
  if(e1<0.1)return vec4(1);
  if(e2<0.1)return vec4(1);
  if(m1<0.15+0.06*sin(voff) && m2> 0.16+0.06*sin(voff))return vec4(0,0,0,1);
  vec3 red    = vec3(cos(voff*30),sin(voff*20),cos(voff*100))*.5f+.5f;
 
  return vec4(red,float(dist < .5f));
}

/**
 * @brief Czech flag fragment shader
 *
 * @param outFragment output fragment
 * @param inFragment input fragment
 * @param uniforms uniform variables
 */
void fragmentShader(OutFragment&outFragment,InFragment const&inFragment,ShaderInterface const&si){
  auto const& vCoord = inFragment.attributes[0].v2;
  auto const& voff = inFragment.attributes[2].u1;
  auto iTime = si.uniforms[1].v1;
  outFragment.gl_FragColor= angryTexture(vCoord,voff,iTime);
}

Method::Method(GPUMemory&m,MethodConstructionData const*): ::Method(m){
  auto&prg = mem.programs[0];
  prg.vertexShader   = vertexShader;
  prg.fragmentShader = fragmentShader;
  prg.vs2fs[0]       = AttributeType::VEC2;
  prg.vs2fs[1]       = AttributeType::VEC3;
  prg.vs2fs[2]       = AttributeType::UINT;

  pushClearCommand      (commandBuffer,glm::vec4(.1,.1,.1,1));
  pushBindProgramCommand(commandBuffer,0);
  pushDrawCommand       (commandBuffer,6*545);
}

void Method::onUpdate(float dt){
  time += dt;
}

void Method::onDraw(SceneParam const&sceneParam){
  mem.uniforms[0].m4 = sceneParam.proj*sceneParam.view;
  mem.uniforms[1].v1 = time;
  izg_enqueue(mem,commandBuffer);
}

EntryPoint main = [](){registerMethod<Method>("students when they see the izg project");};

}
