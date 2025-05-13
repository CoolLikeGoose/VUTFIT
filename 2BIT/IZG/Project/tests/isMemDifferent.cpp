#include <cstring>
#include<tests/isMemDifferent.hpp>

namespace tests{

enum class ImgDiffStatus{
  BYTESPERPIX = 0,
  CHANNELS       ,
  FORMAT         ,
  PITCH          ,
  CHANNELTYPES   ,
  HAS_DATA       ,
  DATA           ,
  SAME           ,
};


bool isNull(void const*ptr){
  return ptr == nullptr;
}

MemDiff isBufferDifferent(Buffer const&a,Buffer const&b){
  if(a.size != b.size)return MemDiffStatus::BUFFER_SIZE;
  if(isNull(a.data) != isNull(b.data))return MemDiffStatus::BUFFER_HAS_DATA;
  if(std::memcmp(a.data,b.data,a.size) != 0)return MemDiffStatus::BUFFER_DATA;
  return MemDiffStatus::SAME;
}

MemDiff isUniformDifferent(Uniform const&a,Uniform const&b){
  if(a.m4 != b.m4)return MemDiffStatus::UNIFORM;
  return MemDiffStatus::SAME;
}

MemDiff isProgramDifferent(Program const&a,Program const&b){
  if(a.vertexShader   != b.vertexShader  )return MemDiffStatus::PROGRAM_VS;
  if(a.fragmentShader != b.fragmentShader)return MemDiffStatus::PROGRAM_FS;
  for(uint32_t i=0;i<maxAttributes;++i){
    if(a.vs2fs[i] != b.vs2fs[i])return MemDiffStatus::PROGRAM_VS2FS;
  }
  return MemDiffStatus::SAME;
}

MemDiff isVertexArrayDifferent(VertexArray const&a,VertexArray const&b){
  if(a.indexBufferID != b.indexBufferID)return MemDiffStatus::VAO_HAS_INDEXING;
  if(a.indexOffset   != b.indexOffset  )return MemDiffStatus::VAO_INDEX_OFFSET;
  if(a.indexType     != b.indexType    )return MemDiffStatus::VAO_INDEX_TYPE  ;

  for(uint32_t i=0;i<maxAttributes;++i){
    auto const&aa=a.vertexAttrib[i];
    auto const&bb=b.vertexAttrib[i];
    if(aa.bufferID != bb.bufferID)return MemDiffStatus::VAO_ATT_BUFFER;
    if(aa.offset   != bb.offset  )return MemDiffStatus::VAO_ATT_OFFSET;
    if(aa.stride   != bb.stride  )return MemDiffStatus::VAO_ATT_STRIDE;
    if(aa.type     != bb.type    )return MemDiffStatus::VAO_ATT_TYPE  ;
  }
  return MemDiffStatus::SAME;
}

ImgDiffStatus isImageDifferent(Image const&a,Image const&b,uint32_t h){
  if(a.bytesPerPixel != b.bytesPerPixel)return ImgDiffStatus::BYTESPERPIX;
  if(a.channels      != b.channels     )return ImgDiffStatus::CHANNELS   ;
  if(a.format        != b.format       )return ImgDiffStatus::FORMAT     ;
  if(a.pitch         != b.pitch        )return ImgDiffStatus::PITCH      ;

  for(int i=0;i<4;++i)
    if(a.channelTypes[i] != b.channelTypes[i])
      return ImgDiffStatus::CHANNELTYPES;

  if(isNull(a.data) != isNull(b.data))return ImgDiffStatus::HAS_DATA;

  if(a.data)
    if(memcmp(a.data,b.data,a.pitch*h) != 0)return ImgDiffStatus::DATA;
  return ImgDiffStatus::SAME;
}

MemDiff isTextureDifferent(Texture const&a,Texture const&b){
  if(a.width  != b.width )return MemDiffStatus::TEX_WIDTH ;
  if(a.height != b.height)return MemDiffStatus::TEX_HEIGHT;
  auto imgStatus = isImageDifferent(a.img,b.img,a.height);
  if(imgStatus != ImgDiffStatus::SAME){
    auto v = (int)MemDiffStatus::TEX_BYTESPERPIX + (int)imgStatus;
    return (MemDiffStatus)v;
  }
  return MemDiffStatus::SAME;
}

MemDiff isFramebufferDifferent(Framebuffer const&a,Framebuffer const&b){
  if(a.width     != b.width    )return MemDiffStatus::FBO_WIDTH    ;
  if(a.height    != b.height   )return MemDiffStatus::FBO_HEIGHT   ;
  if(a.yReversed != b.yReversed)return MemDiffStatus::FBO_YREVERSED;
  auto colorStatus = isImageDifferent(a.color,b.color,a.height);
  if(colorStatus != ImgDiffStatus::SAME){
    auto v = (int)MemDiffStatus::FBO_COLOR_BYTESPERPIX + (int)colorStatus;
    return (MemDiffStatus)v;
  }
  auto depthStatus = isImageDifferent(a.depth,b.depth,a.height);
  if(depthStatus != ImgDiffStatus::SAME){
    auto v = (int)MemDiffStatus::FBO_DEPTH_BYTESPERPIX + (int)depthStatus;
    return (MemDiffStatus)v;
  }
  return MemDiffStatus::SAME;
}

MemDiff isMemDifferent(GPUMemory const&a,GPUMemory const&b){
  if(a.activatedFramebuffer != b.activatedFramebuffer)return MemDiffStatus::ACTIVATED_FRAMEBUFFER;
  if(a.activatedProgram     != b.activatedProgram    )return MemDiffStatus::ACTIVATED_PROGRAM    ;
  if(a.activatedVertexArray != b.activatedVertexArray)return MemDiffStatus::ACTIVATED_VERTEXARRAY;
  if(a.gl_DrawID            != b.gl_DrawID           )return MemDiffStatus::DRAWID               ;
  for(uint32_t i=0;i<a.maxBuffers;++i){
    auto s = isBufferDifferent(a.buffers[i],b.buffers[i]);
    if(s.status != MemDiffStatus::SAME)return {s.status,i};
  }
  for(uint32_t i=0;i<a.maxUniforms;++i){
    auto s = isUniformDifferent(a.uniforms[i],b.uniforms[i]);
    if(s.status != MemDiffStatus::SAME)return {s.status,i};
  }
  for(uint32_t i=0;i<a.maxPrograms;++i){
    auto s = isProgramDifferent(a.programs[i],b.programs[i]);
    if(s.status != MemDiffStatus::SAME)return {s.status,i};
  }
  for(uint32_t i=0;i<a.maxVertexArrays;++i){
    auto s = isVertexArrayDifferent(a.vertexArrays[i],b.vertexArrays[i]);
    if(s.status != MemDiffStatus::SAME)return {s.status,i};
  }
  for(uint32_t i=0;i<a.maxTextures;++i){
    auto s = isTextureDifferent(a.textures[i],b.textures[i]);
    if(s.status != MemDiffStatus::SAME)return {s.status,i};
  }
  for(uint32_t i=0;i<a.maxFramebuffers;++i){
    auto s = isFramebufferDifferent(a.framebuffers[i],b.framebuffers[i]);
    if(s.status != MemDiffStatus::SAME)return {s.status,i};
  }
  return MemDiffStatus::SAME;
}

}
