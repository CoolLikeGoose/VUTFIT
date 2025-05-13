#include<sstream>
#include<map>
#include<iomanip>

#include<tests/gpuMemToStr.hpp>
#include<tests/commandBufferToStr.hpp>
#include<tests/testCommon.hpp>

namespace tests{

enum class BufferType{
  FLOAT,
  U8   ,
  U16  ,
  U32  ,
  MIXED,
};

std::string getBufferName(BufferType t,uint32_t i){
  std::stringstream ss;
  switch(t){
    case BufferType::FLOAT:ss << "vertexBuffer" ;break;
    case BufferType::MIXED:ss << "generalBuffer";break;
    default               :ss << "indexBuffer"  ;break;
  }
  ss << i;
  return ss.str();
}

template<typename ELEMENT_TYPE>
std::string declareBufferImpl(size_t p,Buffer const&b,std::string const&name){
  std::stringstream ss;
  size_t n = b.size/sizeof(ELEMENT_TYPE);
  ss << padding(p) << typeToStr<ELEMENT_TYPE>() << " "<<name<< "["<<n<<"] = {" << std::endl;
  for(size_t i=0;i<n;++i)
    ss << padding(p+2) << str<ELEMENT_TYPE>(((ELEMENT_TYPE*)b.data)[i]) << "," << std::endl;
  ss << padding(p) << "};" << std::endl;
  return ss.str();
}

std::string declareGeneraBufferImpl(size_t p,Buffer const&b,std::string const&name){
  std::stringstream ss;
  ss << padding(p) << typeToStr<uint8_t>() << " " << name << "["<<b.size<<"] = {...};" << std::endl;
  return ss.str();
}

std::string declareBuffer(size_t p,uint32_t i,Buffer const&b,BufferType type){
  auto const name = getBufferName(type,i);
  if(type == BufferType::FLOAT)return declareBufferImpl<float   >(p,b,name);
  if(type == BufferType::U8   )return declareBufferImpl<uint8_t >(p,b,name);
  if(type == BufferType::U16  )return declareBufferImpl<uint16_t>(p,b,name);
  if(type == BufferType::U32  )return declareBufferImpl<uint32_t>(p,b,name);
  if(type == BufferType::MIXED)return declareGeneraBufferImpl    (p,b,name);
  return "";
}

void insertTypeToBufferTypes(std::map<uint32_t,BufferType>&bufferTypes,uint32_t b,BufferType t){
  if(bufferTypes.count(b)){
    if(bufferTypes[b] != t)
      bufferTypes[b] = BufferType::MIXED;
  }else
    bufferTypes[b] = t;
}

void insertIndexBufferToBufferTypes(std::map<uint32_t,BufferType>&bufferTypes,VertexArray const&vao){
  if(vao.indexBufferID < 0)return;
  if(vao.indexType == IndexType::UINT32)insertTypeToBufferTypes(bufferTypes,vao.indexBufferID,BufferType::U32);
  if(vao.indexType == IndexType::UINT16)insertTypeToBufferTypes(bufferTypes,vao.indexBufferID,BufferType::U16);
  if(vao.indexType == IndexType::UINT8 )insertTypeToBufferTypes(bufferTypes,vao.indexBufferID,BufferType::U8 );
}

bool offsetIsNotAlignedWithType(VertexAttrib const&attrib){
  return attrib.offset % (uint32_t)attrib.type != 0;
}

void insertAttribBufferToBufferTypes(std::map<uint32_t,BufferType>&bufferTypes,VertexAttrib const&attrib){
  if(attrib.bufferID < 0)return;
  auto type = BufferType::FLOAT;
  if(offsetIsNotAlignedWithType(attrib))type = BufferType::MIXED;
  insertTypeToBufferTypes(bufferTypes,attrib.bufferID,type);
}

void insertAttribBuffersToBufferTypes(std::map<uint32_t,BufferType>&bufferTypes,VertexArray const&vao){
  for(uint32_t i=0;i<maxAttributes;++i)
    insertAttribBufferToBufferTypes(bufferTypes,vao.vertexAttrib[i]);
}

void insertBuffersMentionedInVertexArraysToBufferTypes(GPUMemory const&mem,std::map<uint32_t,BufferType>&bufferTypes){
  for(uint32_t i=0;i<mem.maxVertexArrays;++i){
    auto const&vao = mem.vertexArrays[i];
    insertIndexBufferToBufferTypes(bufferTypes,vao);
    insertAttribBuffersToBufferTypes(bufferTypes,vao);
  }
}

void appendBuffersNotMentionedInCommandBufferToBufferTypes(std::map<uint32_t,BufferType>&bufferTypes,GPUMemory const&mem){
  for(uint32_t i=0;i<mem.maxBuffers;++i){
    if(!mem.buffers[i].data)continue;
    if(bufferTypes.count(i))continue;
    insertTypeToBufferTypes(bufferTypes,i,BufferType::MIXED);
  }
}

std::map<uint32_t,BufferType>createBufferTypes(GPUMemory const&mem){
  std::map<uint32_t,BufferType>bufferTypes;

  insertBuffersMentionedInVertexArraysToBufferTypes    (mem,bufferTypes);
  appendBuffersNotMentionedInCommandBufferToBufferTypes(bufferTypes,mem);

  return bufferTypes;
}

std::string declareBuffers(size_t p,GPUMemory const&mem){
  std::stringstream ss;

  auto bufferTypes = createBufferTypes(mem);

  for(auto const&bt:bufferTypes)
    ss << declareBuffer(p,bt.first,mem.buffers[bt.first],bt.second);
  return ss.str();
}

std::string gpuMemBuffersToString(size_t p,GPUMemory const&mem){
  std::stringstream ss;

  auto bufferTypes = createBufferTypes(mem);
  for(auto const&b:bufferTypes){
    ss << padding(p) << "mem.buffers["<<b.first<<"].data = " << getBufferName(b.second,b.first) << ";" << std::endl;
    ss << padding(p) << "mem.buffers["<<b.first<<"].size = " << mem.buffers[b.first].size << ";" << std::endl;
  }

  return ss.str();
}

std::string imageFormatToString(Image::Format const&f){
  if(f == Image::FLOAT32)return "FLOAT32";
  if(f == Image::UINT8  )return "UINT8"  ;
  return "";
}

std::string channelTypeToString(Image::Channel c){
  switch(c){
    case Image::RED  :return "RED"  ;
    case Image::GREEN:return "GREEN";
    case Image::BLUE :return "BLUE" ;
    case Image::ALPHA:return "ALPHA";
  }
  return "";
}

std::string channelTypesToString(Image::Channel const*c){
  std::stringstream ss;
  ss << "{";
  for(int i=0;i<4;++i){
    if(i>0)ss << ",";
    ss << channelTypeToString(c[i]);
  }
  ss << "}";
  return ss.str();
}

std::string imageToString(size_t p,Image const&img,uint32_t w,uint32_t h){
  std::stringstream ss;
  ss << padding(p) << "bytesPerPixel = " << img.bytesPerPixel                      << ";" << std::endl;
  ss << padding(p) << "channels      = " << img.channels                           << ";" << std::endl;
  ss << padding(p) << "pitch         = " << img.pitch                              << ";" << std::endl;
  ss << padding(p) << "format        = " << imageFormatToString (img.format      ) << ";" << std::endl;
  ss << padding(p) << "channelTypes  = " << channelTypesToString(img.channelTypes) << ";" << std::endl;
  if(w == 0 || h == 0){
    ss << padding(p) << "data  = " << img.data << ";" << std::endl;
  }else{
    uint32_t counter = 0;
    uint32_t const maxPixels = 20;
    for(uint32_t y=0;y<h;++y){
      for(uint32_t x=0;x<w;++x){
        if(counter>=maxPixels)continue;
        ss << padding(p) << "data[x="<<x<<",y="<<y<<"] = ";
        auto sss = (uint8_t*)img.data+y*img.pitch+x*img.bytesPerPixel;
        if(img.format == Image::UINT8){
          for(uint32_t k=0;k<img.bytesPerPixel;++k){
            ss << std::setw(2) << std::setfill('0') << std::hex << (int)sss[k];
          }
        }
        if(img.format == Image::FLOAT32){
          for(uint32_t c=0;c<img.channels;++c)
            ss << ((float*)sss)[c] << " ";
        }
        ss << ";" << std::endl;
        counter++;
      }
    }
    ss << padding(p) << "..." << std::endl;
  }

  return ss.str();
}

std::string gpuMemFramebufferToString(size_t p,GPUMemory const&mem){
  std::stringstream ss;
  auto const&fs = mem.framebuffers;
  for(uint32_t i=0;i<mem.maxFramebuffers;++i){
    if(!fs[i].color.data && !fs[i].depth.data)continue;
    ss << padding(p) << "mem.framebuffers["<<i<<"].width     = " << fs[i].width     << ";" << std::endl;
    ss << padding(p) << "mem.framebuffers["<<i<<"].height    = " << fs[i].height    << ";" << std::endl;
    ss << padding(p) << "mem.framebuffers["<<i<<"].yReversed = " << fs[i].yReversed << ";" << std::endl;
    ss << padding(p) << "mem.framebuffers["<<i<<"].color:" << std::endl;
    ss << imageToString(p+2,fs[i].color,fs[i].width,fs[i].height);
    ss << padding(p) << "mem.framebuffers["<<i<<"].depth:" << std::endl;
    ss << imageToString(p+2,fs[i].depth,fs[i].width,fs[i].height);
  }
  return ss.str();
}


std::string gpuMemProgramsToString(size_t p,GPUMemory const&mem){
  std::stringstream ss;
  for(uint32_t i=0;i<mem.maxPrograms;++i)
    ss << programToString(p,mem.programs[i],i);
  return ss.str();
}

std::string programToString(size_t p,Program const&prg,uint32_t id){
  std::stringstream ss;
  ss << programVertexShaderToString  (p,prg,id);
  ss << programFragmentShaderToString(p,prg,id);
  ss << programVS2FSToString         (p,prg,id);
  return ss.str();
}

std::string programVertexShaderToString(size_t p,Program const&prg,uint32_t id){
  if(!prg.vertexShader)return "";
  std::stringstream ss;
  ss << padding(p) << "mem.programs["<<id<<"].vertexShader   = function;"<< std::endl;
  return ss.str();
}

std::string programVS2FSToString(size_t p,Program const&prg,uint32_t id){
  std::stringstream ss;
  for(uint32_t j=0;j<maxAttributes;++j){
    if(prg.vs2fs[j] == AttributeType::EMPTY)continue;
    ss << padding(p) << "mem.programs["<<id<<"].vs2fs["<<j<<"] = "<<str(prg.vs2fs[j])<<";" << std::endl;
  }
  return ss.str();
}

std::string programFragmentShaderToString(size_t p,Program const&prg,uint32_t id){
  if(!prg.fragmentShader)return "";
  std::stringstream ss;
  ss << padding(p) << "mem.programs["<<id<<"].fragmentShader = function;"<< std::endl;
  return ss.str();
}


std::string gpuMemActivatedIdsToString(size_t p,GPUMemory const&mem){
  std::stringstream ss;
  ss << padding(p) << "mem.activatedProgram     = " << mem.activatedProgram     << std::endl;
  ss << padding(p) << "mem.activatedVertexArray = " << mem.activatedVertexArray << std::endl;
  ss << padding(p) << "mem.activatedFramebuffer = " << mem.activatedFramebuffer << std::endl;
  return ss.str();
}

std::string vertexArrayToString(size_t p,VertexArray const&vao,uint32_t id){
  std::stringstream ss;
  ss << vertexArrayIndexingToString(p,vao,id);
  ss << vertexArrayAttribsToString (p,vao,id);
  return ss.str();
}

std::string vertexArrayIndexingToString(size_t p,VertexArray const&vao,uint32_t id){
  std::stringstream ss;
  if(vao.indexBufferID>=0){
    ss << padding(p) << "mem.vertexArrays["<<id<<"].indexBufferID = " <<     vao.indexBufferID  << ";" << std::endl;
    ss << padding(p) << "mem.vertexArrays["<<id<<"].indexOffset   = " <<     vao.indexOffset    << ";" << std::endl;
    ss << padding(p) << "mem.vertexArrays["<<id<<"].indexType     = " << str(vao.indexType    ) << ";" << std::endl;
  }
  return ss.str();
}

std::string vertexArrayAttribsToString(size_t p,VertexArray const&vao,uint32_t id){
  std::stringstream ss;
  for(uint32_t j=0;j<maxAttributes;++j){
    if(vao.vertexAttrib[j].type == AttributeType::EMPTY)continue;
    ss << padding(p) << "mem.vertexArrays["<<id<<"].vertexAttrib["<<j<<"].bufferID = " <<     vao.vertexAttrib[j].bufferID  << ";" << std::endl;
    ss << padding(p) << "mem.vertexArrays["<<id<<"].vertexAttrib["<<j<<"].offset   = " <<     vao.vertexAttrib[j].offset    << ";" << std::endl;
    ss << padding(p) << "mem.vertexArrays["<<id<<"].vertexAttrib["<<j<<"].stride   = " <<     vao.vertexAttrib[j].stride    << ";" << std::endl;
    ss << padding(p) << "mem.vertexArrays["<<id<<"].vertexAttrib["<<j<<"].type     = " << str(vao.vertexAttrib[j].type    ) << ";" << std::endl;
  }
  return ss.str();
}

std::string gpuMemVertexArraysToString(size_t p,GPUMemory const&mem){
  std::stringstream ss;
  for(uint32_t i=0;i<mem.maxVertexArrays;++i){
    auto const&vao = mem.vertexArrays[i];
    ss << vertexArrayToString(p,vao,i);
  }
  return ss.str();
}

std::string textureToString(size_t p,Texture const&tex,uint32_t i){
  std::stringstream ss;
  ss << padding(p) << "mem.textures["<<i<<"].width     = " << tex.width     << ";" << std::endl;
  ss << padding(p) << "mem.textures["<<i<<"].height    = " << tex.height    << ";" << std::endl;
  ss << padding(p) << "mem.textures["<<i<<"].img:" << std::endl;
  ss << imageToString(p+2,tex.img,tex.width,tex.height);
  return ss.str();
}

std::string gpuMemTexturesToString(size_t p,GPUMemory const&mem){
  std::stringstream ss;
  for(uint32_t i=0;i<mem.maxTextures;++i){
    auto const&tex = mem.textures[i];
    if(!tex.img.data)continue;
    ss << textureToString(p,tex,i);
  }
  return ss.str();
}

std::string gpuMemStructToString(size_t p,GPUMemory const&mem){
  std::stringstream ss;
  ss << gpuMemActivatedIdsToString(p,mem);
  ss << gpuMemFramebufferToString (p,mem);
  ss << gpuMemProgramsToString    (p,mem);
  ss << gpuMemBuffersToString     (p,mem);
  ss << gpuMemVertexArraysToString(p,mem);
  ss << gpuMemTexturesToString    (p,mem);
  return ss.str();
}

std::string gpuMemoryToStr(size_t p,GPUMemory const&mem){
  std::stringstream ss;
  ss << declareBuffers(p,mem);
  ss << padding(p) << "GPUMemory mem;" << std::endl;
  ss << gpuMemStructToString(p,mem);
  //for(size_t i=0;i<mem.maxBuffers;++i)
  //  ss << str(p,i,mem.buffers[i]);

  return ss.str();
}

}
