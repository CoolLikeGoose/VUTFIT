#include <framework/programContext.hpp>

namespace edgeDetectMethod{

/**
 * @brief Empty rendering method
 */
class Method: public ::Method{
  public:
    Method(GPUMemory&m,MethodConstructionData const*);
    virtual ~Method();
    virtual void onDraw(SceneParam const&sceneParam) override;
    CommandBuffer commandBuffer;
    TextureData colorBuffer;
    TextureData depthBuffer;
};

void box_vs(OutVertex&outVertex,InVertex const&inVertex,ShaderInterface const&si){
  outVertex.gl_Position = glm::vec4(0.f,0.f,0.f,1.f);

  glm::mat4 view = si.uniforms[3].m4;
  glm::mat4 proj = si.uniforms[2].m4;
 
  const uint32_t indices[] = {
    0u,1u,2u,2u,1u,3u,
    4u,5u,6u,6u,5u,7u,
    0u,4u,2u,2u,4u,6u,
    1u,5u,3u,3u,5u,7u,
    0u,1u,4u,4u,1u,5u,
    2u,3u,6u,6u,3u,7u
  };

  if(inVertex.gl_VertexID>=sizeof(indices)/sizeof(indices[0]))return;

  glm::vec3 pos;
  for(uint32_t i=0u;i<3u;++i)
    pos[i] = float((indices[inVertex.gl_VertexID]>>i)&1u);

  outVertex.gl_Position = proj*view*glm::vec4(pos*2.0f-1.f,1.f);
}

void box_fs(OutFragment&outFragment,InFragment const&inFragment,ShaderInterface const&){
  outFragment.gl_FragColor = inFragment.attributes[3].v4;
}

void vfx_vs(OutVertex&outVertex,InVertex const&inVertex,ShaderInterface const&si){
  outVertex.gl_Position = glm::vec4(0.f,0.f,0.f,1.f);

  float xo[] = {0,1,0,0,1,1};
  float yo[] = {0,0,1,1,0,1};
  auto uv = glm::vec2(xo[inVertex.gl_VertexID],yo[inVertex.gl_VertexID]);
  outVertex.gl_Position = glm::vec4(uv*2.f-1.f,0.f,1.f);
  
  outVertex.attributes[0].v2 = uv;
}

void vfx_fs(OutFragment&outFragment,InFragment const&inFragment,ShaderInterface const&si){
  auto uv = inFragment.attributes[0].v2;
  float const xSobel[] = {
    -1,0,1,
    -2,0,2,
    -1,0,1,
  };
  float const ySobel[] = {
    -1,-2,-1,
     0, 0, 0,
     1, 2, 1,
  };

  float xs = 0,ys = 0;
  for(int y=-1;y<=1;++y)
    for(int x=-1;x<=1;++x){
      auto color = read_texture(si.textures[0],uv+glm::vec2(x,y)/500.f).r;
      xs += color * xSobel[(y+1)*3+x+1];
      ys += color * ySobel[(y+1)*3+x+1];
    }

  float sobel = glm::sqrt(xs*xs+ys*ys);


  outFragment.gl_FragColor = glm::vec4(uv,0.f,1.f);
  outFragment.gl_FragColor = glm::vec4(glm::vec3(sobel),1.f);
}

Method::Method(GPUMemory&m,MethodConstructionData const*): ::Method(m){
  colorBuffer = TextureData(m.framebuffers[0].width,m.framebuffers[0].height,3,Image::UINT8  );
  depthBuffer = TextureData(m.framebuffers[0].width,m.framebuffers[0].height,1,Image::FLOAT32);

  m.textures[0] = colorBuffer.getTexture();
  m.textures[1] = depthBuffer.getTexture();
  m.framebuffers[1].color  = m.textures[0].img;
  m.framebuffers[1].depth  = m.textures[1].img;
  m.framebuffers[1].width  = m.textures[0].width;
  m.framebuffers[1].height = m.textures[0].height;

  pushBindFramebufferCommand(commandBuffer,1);
  pushClearCommand(commandBuffer,glm::vec4(.0,.0,0,1));
  mem.programs[0].vertexShader   = box_vs;
  mem.programs[0].fragmentShader = box_fs;
  mem.programs[0].vs2fs[3]       = AttributeType::VEC4;
  pushBindProgramCommand(commandBuffer,0);
  pushDrawCommand(commandBuffer,6*2*3);

  pushBindFramebufferCommand(commandBuffer,0);
  pushClearCommand(commandBuffer,glm::vec4(.0,.0,0,1));
  mem.programs[1].vertexShader   = vfx_vs;
  mem.programs[1].fragmentShader = vfx_fs;
  mem.programs[1].vs2fs[0]       = AttributeType::VEC2;
  pushBindProgramCommand(commandBuffer,1);
  pushDrawCommand(commandBuffer,6);
}

Method::~Method(){
}

void Method::onDraw(SceneParam const&sceneParam){
  mem.uniforms[3].m4 = sceneParam.view;
  mem.uniforms[2].m4 = sceneParam.proj;
  izg_enqueue(mem,commandBuffer);
}

EntryPoint main = [](){registerMethod<Method>("edge detect");};

}
