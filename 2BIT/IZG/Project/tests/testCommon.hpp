#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <memory>
#include <sstream>

#include <glm/glm.hpp>
#include <student/fwd.hpp>
#include <framework/colorPrinting.hpp>
#include <tests/memDeepCopy.hpp>
#include <tests/isMemDifferent.hpp>

namespace tests{

#ifndef __FILENAME__
#define __FILENAME__ __FILE__
#endif

#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define TEST_NAME __FILENAME__ ":" STRINGIZE(__LINE__)

std::string testCounter(bool first = false);

glm::uvec4 getColorU(Image       const&img  ,uint32_t x,uint32_t y);
glm:: vec4 getColorF(Image       const&img  ,uint32_t x,uint32_t y);
glm::uvec4 getColorU(Texture     const&tex  ,uint32_t x,uint32_t y);
glm:: vec4 getColorF(Texture     const&tex  ,uint32_t x,uint32_t y);
glm::uvec4 getColorU(Framebuffer const&frame,uint32_t x,uint32_t y);
glm:: vec4 getColorF(Framebuffer const&frame,uint32_t x,uint32_t y);
float      getDepth (Framebuffer const&frame,uint32_t x,uint32_t y);

void setColorU(Image       &tex  ,uint32_t x,uint32_t y,glm::uvec4 const&color);
void setColorF(Image       &tex  ,uint32_t x,uint32_t y,glm:: vec4 const&color);
void setColorU(Texture     &tex  ,uint32_t x,uint32_t y,glm::uvec4 const&color);
void setColorF(Texture     &tex  ,uint32_t x,uint32_t y,glm:: vec4 const&color);
void setColorU(Framebuffer &frame,uint32_t x,uint32_t y,glm::uvec4 const&color);
void setColorF(Framebuffer &frame,uint32_t x,uint32_t y,glm:: vec4 const&color);
void setDepth (Texture     &tex,  uint32_t x,uint32_t y,float            depth);
void setDepth (Framebuffer &frame,uint32_t x,uint32_t y,float            depth);

std::string getMemDifference(uint32_t padding,GPUMemory const&emp,GPUMemory const&student);

float const extern floatErr;

bool equalFloats(float const& a, float const& b,float err = floatErr);
bool equalVec2(glm::vec2 const&a,glm::vec2 const&b,float err = floatErr);
bool equalVec3(glm::vec3 const&a,glm::vec3 const&b,float err = floatErr);
bool equalVec4(glm::vec4 const&a,glm::vec4 const&b,float err = floatErr);

bool equalCounts(size_t a,size_t b,size_t err = 10);

bool greaterFloat(float a,float b,float err = floatErr);

bool lessFloat(float a,float b,float err = floatErr);

template<typename TYPE>
std::string typeToStr();

template<>inline std::string typeToStr<float   >(){return "float"   ;}
template<>inline std::string typeToStr<uint32_t>(){return "uint32_t";}
template<>inline std::string typeToStr<uint16_t>(){return "uint16_t";}
template<>inline std::string typeToStr<uint8_t >(){return "uint8_t" ;}

template<typename T>
std::string str(T const&a){
  std::stringstream ss;
  ss << a;
  return ss.str();
}



template<> std::string str(bool      const&a);
template<> std::string str(uint8_t  const&a);

template<> std::string str(glm::vec2 const&a);
template<> std::string str(glm::vec3 const&a);
template<> std::string str(glm::vec4 const&a);
template<> std::string str(glm::uvec2 const&a);
template<> std::string str(glm::uvec3 const&a);
template<> std::string str(glm::uvec4 const&a);
template<> std::string str(glm::ivec2 const&a);
template<> std::string str(glm::ivec3 const&a);
template<> std::string str(glm::ivec4 const&a);

template<> std::string str(glm::mat4 const&m);
template<> std::string str(IndexType const&i);
template<> std::string str(AttributeType const&a);
template<> std::string str(CommandType const&a);
std::string padding(size_t n=2);

void printModel(size_t p,Model const&m);
void printVertexAttrib(VertexAttrib const&a,uint32_t p);

uint8_t    floatColorToByte(float c);
glm::uvec4 floatColorToBytes(glm::vec4 const&col);


bool        sameColor(glm::uvec4 const&a,glm::uvec4 const&b,uint32_t channels);
std::string colorToStr(glm::uvec4 const&a,uint32_t channels);

std::vector<InVertex>computeExpectedInVertices(GPUMemory const&mem,DrawCommand const&cmd);

bool operator==(VertexAttrib const&a,VertexAttrib const&b);
bool operator==(InVertex const&a,InVertex const&b);

enum class DumpDiff{
  NOF_COMMANDS_DIFFER       ,
  DIFFERENT_COMMAND_TYPES   ,
  DIFFERENT_MEMORY          ,
  DIFFERENT_CLEAR           ,
  DIFFERENT_SET_DRAW_ID     ,
  DIFFERENT_BIND_FRAMEBUFFER,
  DIFFERENT_BIND_PROGRAM    ,
  DIFFERENT_BIND_VERTEXARRAY,
  DIFFERENT_COPY_DATA       ,
  DIFFERENT_NOF_VERTICES    ,
  DIFFERENT_DRAWID          ,
  DIFFERENT_VERTEXID        ,
  DIFFERENT_ATTRIBUTE       ,
  DIFFERENT_SHADERINTERFACE ,
  SAME                      ,
};

struct DrawDump;

using DrawCommandsDump = std::vector<std::shared_ptr<DrawDump>>;

struct AllocatedFramebuffer{
  std::vector<uint8_t>colorBacking;
  std::vector<float  >depthBacking;
  Framebuffer frame;
};

AllocatedFramebuffer createFramebuffer(uint32_t height,uint32_t width,bool flipped = false);

struct MemCb{
  MemCb();
  MemCb(uint32_t w,uint32_t h);
  AllocatedMem         expectedMem;
  AllocatedMem         studentMem ;
  GPUMemory            mem     ;
  CommandBuffer        cb      ;
  AllocatedFramebuffer aframe  ;
  DumpDiff             status  ;
  MemDiff              memStatus;
  GPUMemory*           inFlightMem = nullptr;

  DrawCommandsDump       commands;
  DrawCommandsDump       student ;
  DrawCommandsDump       expected;
  std::vector<OutVertex >outVertices;
  std::vector<InFragment>inFragments;
  void test(uint32_t n=1);
  bool test(uint32_t n,DumpDiff whatToTest,bool first = false);
  bool ok(DumpDiff whatToTest);
  void printDetails();
  void printStatus();
  void printHelp(std::string const&str);

  DrawCommandsDump getDrawCommandsDump(uint32_t n,GPUMemory&mem,CommandBuffer const&cb,void(*fce)(GPUMemory&,CommandBuffer const&));
  
};

extern std::shared_ptr<MemCb>memCb;


#define MEMCB(w,h)                                                                                   \
  memCb = std::make_shared<MemCb>(w,h);\
  GPUMemory    &mem                        = memCb->mem                                             ;\
  CommandBuffer&cb                         = memCb->cb                                              ;\
  mem.framebuffers[mem.defaultFramebuffer] = memCb->aframe.frame                                    ;\
  auto channels                            = mem.framebuffers[mem.defaultFramebuffer].color.channels
  
  
template<typename T>
Buffer vectorToBuffer(std::vector<T>const&d){
  Buffer res;
  res.data = d.data();
  res.size = sizeof(T)*d.size();
  return res;
}

struct DrawDump{
  DrawDump(){}
  AllocatedMem         memCopy   ;
  ShaderInterface      si        ;
  std::vector<InVertex>inVertices;
};

struct FragmentShaderDump{
  FragmentShaderDump();
  void*fs = nullptr;
  std::vector<InFragment>inFragments;
  //Uniform unif[GPUMemory::maxUniforms];
  //Texture texs[GPUMemory::maxTextures];
};

std::string commandDumpToStr(size_t p,GPUMemory const&mem);
bool hasCommandDumpRightDrawID();
std::string commandDumpDrawIDToStr(size_t p);

void vertexShaderDump0(OutVertex&,InVertex const&i,ShaderInterface const&u);
void vertexShaderDump1(OutVertex&,InVertex const&i,ShaderInterface const&u);
void vertexShaderDump2(OutVertex&,InVertex const&i,ShaderInterface const&u);
void vertexShaderDump3(OutVertex&,InVertex const&i,ShaderInterface const&u);

void vertexShaderInject(OutVertex&outVertex,InVertex const&inVertex,ShaderInterface const&);
void fragmentShaderDump(OutFragment&,InFragment const&inF,ShaderInterface const&);
void vertexShaderEmpty(OutVertex&,InVertex const&inF,ShaderInterface const&);
void fragmentShaderEmpty(OutFragment&,InFragment const&inF,ShaderInterface const&);
void clearFrame(Framebuffer&frame,glm::vec3 const&c = glm::vec3(0.f),float d=1.f);
void clearFrame(Framebuffer&frame,glm::uvec3 const&c = glm::uvec3(0),float d=1.f);
glm::uvec3 alphaMix(glm::vec4 const&frameColor,glm::vec4 const&fragColor);


bool breakTest(bool first = false);


}
