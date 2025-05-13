#pragma once

#include<student/fwd.hpp>
#include<list>

namespace tests{


struct MemBacking{
  uint8_t  *ptr     = nullptr;
  size_t    size    = 0      ;
  uint8_t  *backing = nullptr;
  struct Object{
    enum Type{
      BUFFER,
      TEXTURE,
      FRAMEBUFFER_COLOR,
      FRAMEBUFFER_DEPTH,
    }; 
    Type     type   = BUFFER;
    uint32_t id     = 0     ;
    size_t   offset = 0     ;
    Object(){}
    Object(Type t,uint32_t i):type(t),id(i){}
  };
  std::vector<Object>objects;
  ~MemBacking(){
    ptr  = nullptr;
    size = 0      ;
    if(!backing)return;
    delete[]backing;
    backing = nullptr;
  }
  bool operator<(MemBacking const&o);
  bool operator>(MemBacking const&o);
};

MemBacking merge(MemBacking a,MemBacking b);

struct AllocatedMem{
  GPUMemory mem;
  std::list<MemBacking>backings;
  void allocate();
 private:
  void insertPointers();
  void allocateBacking();
  void putBackingsIntoMem();
  void putBackingIntoMem(MemBacking const&b);
  void putObjectBackingIntoMem(uint8_t*backing,MemBacking::Object const&o);
  void insertBuffersPointers();
  void insertTexturesPointers();
  void insertFramebuffersPointers();
  void insertBufferPointer(Buffer const&buf,uint32_t id);
  void insertImagePointer(Image const&img,uint32_t height,MemBacking::Object::Type type,uint32_t id);
  void insertBacking(MemBacking b);
};

AllocatedMem deepCopy(GPUMemory const&mem);

void shallowCopy(GPUMemory  &d,GPUMemory   const&s);
void shallowCopy(Program    &d,Program     const&s);
void shallowCopy(VertexArray&d,VertexArray const&s);
void shallowCopy(Texture    &d,Texture     const&s);
void shallowCopy(Framebuffer&d,Framebuffer const&s);
void shallowCopy(Image      &d,Image       const&s);

void copyData(GPUMemory  &d,GPUMemory   const&s);
void copyData(Buffer     &d,Buffer      const&s);
void copyData(Texture    &d,Texture     const&s);
void copyData(Framebuffer&d,Framebuffer const&s);
void copyData(Image      &d,Image       const&s,uint32_t h);

}
