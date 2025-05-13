#include<tests/memDeepCopy.hpp>
#include<cstring>

namespace tests{

void AllocatedMem::allocate(){
  insertPointers();
  allocateBacking();
  putBackingsIntoMem();
}

void AllocatedMem::insertPointers(){
  insertBuffersPointers();
  insertTexturesPointers();
  insertFramebuffersPointers();
}

void AllocatedMem::allocateBacking(){
  for(auto&b:backings)
    b.backing = new uint8_t[b.size];
}

void AllocatedMem::putBackingsIntoMem(){
  for(auto const&b:backings)
    putBackingIntoMem(b);
}

void AllocatedMem::putBackingIntoMem(MemBacking const&b){
  for(auto const&o:b.objects)
    putObjectBackingIntoMem(b.backing,o);
}

void AllocatedMem::putObjectBackingIntoMem(uint8_t*backing,MemBacking::Object const&o){
  auto ptr = backing + o.offset;
  switch(o.type){
    case MemBacking::Object::BUFFER           :mem.buffers     [o.id]      .data = ptr;break;
    case MemBacking::Object::TEXTURE          :mem.textures    [o.id].img  .data = ptr;break;
    case MemBacking::Object::FRAMEBUFFER_COLOR:mem.framebuffers[o.id].color.data = ptr;break;
    case MemBacking::Object::FRAMEBUFFER_DEPTH:mem.framebuffers[o.id].depth.data = ptr;break;
  }
}

void AllocatedMem::insertBuffersPointers(){
  for(uint32_t i=0;i<mem.maxBuffers;++i){
    auto const&b=mem.buffers[i];
    insertBufferPointer(b,i);
  }
}

void AllocatedMem::insertTexturesPointers(){
  for(uint32_t i=0;i<mem.maxTextures;++i){
    auto const&t=mem.textures[i];
    insertImagePointer(t.img,t.height,MemBacking::Object::TEXTURE,i);
  }
}

void AllocatedMem::insertFramebuffersPointers(){
  for(uint32_t i=0;i<mem.maxFramebuffers;++i){
    auto const&f=mem.framebuffers[i];
    insertImagePointer(f.color,f.height,MemBacking::Object::FRAMEBUFFER_COLOR,i);
    insertImagePointer(f.depth,f.height,MemBacking::Object::FRAMEBUFFER_DEPTH,i);
  }
}

void AllocatedMem::insertBufferPointer(Buffer const&buf,uint32_t id){
  if(!buf.data)return;
  MemBacking ba;
  ba.ptr  = (uint8_t*)buf.data;
  ba.size = buf.size;
  ba.objects.emplace_back(MemBacking::Object::BUFFER,id);
  insertBacking(ba);
}

void AllocatedMem::insertImagePointer(
    Image                    const&img   ,
    uint32_t                       height,
    MemBacking::Object::Type       type  ,
    uint32_t                       id    ){
  if(!img.data)return;
  MemBacking ba;
  ba.ptr  = (uint8_t*)img.data;
  ba.size = height*img.pitch;
  ba.objects.emplace_back(type,id);
  insertBacking(ba);
}

void AllocatedMem::insertBacking(MemBacking b){
  std::list<MemBacking>finalList;
  for(auto it = backings.begin();it != backings.end();++it){
    if(b < *it){
      finalList.push_back(b);
      finalList.push_back(*it);
    }else if(b > *it){
      finalList.push_back(*it);
    }else
      b = merge(b,*it);
  }
  finalList.push_back(b);

  backings = finalList;
}

MemBacking merge(MemBacking a,MemBacking b){
  auto&l = (a.ptr < b.ptr)?a:b;
  auto&r = (a.ptr < b.ptr)?b:a;

  l.size      = std::max(l.size,(size_t)(r.ptr+r.size-l.ptr));
  auto offset = (size_t)(r.ptr-l.ptr);

  for(auto &o:r.objects)
    o.offset += offset;

  for(auto const&o:r.objects)
    l.objects.push_back(o);

  return l;
}

bool MemBacking::operator< (MemBacking const&o){
  return ptr+size < o.ptr;
}

bool MemBacking::operator>(MemBacking const&o){
  // it is open interval from the right [ptr,ptr+size)
  // [10,11), just one byte 
  return ptr >= o.ptr + o.size;
}

AllocatedMem deepCopy(GPUMemory const&mem){
  AllocatedMem amem;
  shallowCopy(amem.mem,mem);
  amem.allocate();
  copyData(amem.mem,mem);
  return amem;
}


void shallowCopy(GPUMemory&dst,GPUMemory const&src){
  dst.activatedFramebuffer = src.activatedFramebuffer;
  dst.activatedProgram     = src.activatedProgram    ;
  dst.activatedVertexArray = src.activatedVertexArray;
  for(uint32_t i=0;i<src.maxBuffers;++i)
    dst.buffers[i] = src.buffers[i];

  for(uint32_t i=0;i<src.maxPrograms;++i)
    shallowCopy(dst.programs[i],src.programs[i]);

  for(uint32_t i=0;i<src.maxUniforms;++i)
    dst.uniforms[i].m4 = src.uniforms[i].m4;

  for(uint32_t i=0;i<src.maxTextures;++i)
    shallowCopy(dst.textures[i],src.textures[i]);

  for(uint32_t i=0;i<src.maxVertexArrays;++i)
    shallowCopy(dst.vertexArrays[i],src.vertexArrays[i]);

  for(uint32_t i=0;i<src.maxFramebuffers;++i)
    shallowCopy(dst.framebuffers[i],src.framebuffers[i]);
}

void copyData(GPUMemory &d,GPUMemory const&s){
  for(uint32_t i=0;i<s.maxBuffers;++i)
    copyData(d.buffers[i],s.buffers[i]);

  for(uint32_t i=0;i<s.maxTextures;++i)
    copyData(d.textures[i],s.textures[i]);

  for(uint32_t i=0;i<s.maxFramebuffers;++i)
    copyData(d.framebuffers[i],s.framebuffers[i]);
}

void copyData(Buffer &d,Buffer const&s){
  std::memcpy((void*)d.data,s.data,s.size);
}

void copyData(Texture &d,Texture const&s){
  copyData(d.img,s.img,s.height);
}

void copyData(Framebuffer&d,Framebuffer const&s){
  copyData(d.color,s.color,s.height);
  copyData(d.depth,s.depth,s.height);
}

void copyData(Image &d,Image const&s,uint32_t h){
  if(!d.data || !s.data)return;
  std::memcpy((void*)d.data,s.data,s.pitch * h);
}


void shallowCopy(Program&d,Program const&s){
  d = s;
  for(uint32_t a=0;a<maxAttributes;++a)
    d.vs2fs[a] = s.vs2fs[a];
}

void shallowCopy(VertexArray&d,VertexArray const&s){
  d = s;
  for(uint32_t a=0;a<maxAttributes;++a)
    d.vertexAttrib[a] = s.vertexAttrib[a];
}

void shallowCopy(Texture&d,Texture const&s){
  d = s;
  shallowCopy(d.img,s.img);
}

void shallowCopy(Framebuffer&d,Framebuffer const&s){
  d = s;
  shallowCopy(d.color,s.color);
  shallowCopy(d.depth,s.depth);
}

void shallowCopy(Image&d,Image const&s){
  d = s;
  for(int i=0;i<4;++i)
    d.channelTypes[i] = s.channelTypes[i];
}


}
