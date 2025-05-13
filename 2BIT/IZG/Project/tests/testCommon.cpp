#include <memory>
#include <framework/programContext.hpp>
#include <tests/testCommon.hpp>
#include <tests/gpuMemToStr.hpp>
#include <tests/commandBufferToStr.hpp>

#include <glm/glm.hpp>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstring>

void drawModel_vertexShader  (OutVertex  &,InVertex   const&,ShaderInterface const&);
void drawModel_fragmentShader(OutFragment&,InFragment const&,ShaderInterface const&);

namespace tests{



void fake_cmd_enqueueRec(GPUMemory&mem,CommandBuffer const&cb){
  for(uint32_t c=0;c<cb.nofCommands;++c){
    auto const&cmd = cb.commands[c];
    auto const&d   = cmd.data;
    switch(cmd.type){
      case CommandType::CLEAR:{
        auto&fbo=mem.framebuffers[mem.activatedFramebuffer];
        auto const&dd=d.clearCommand;
        for(uint32_t y=0;y<fbo.height;++y)
          for(uint32_t x=0;x<fbo.width;++x){
            if(dd.clearColor&&fbo.color.data)setColorF(fbo,x,y,dd.color);
            if(dd.clearDepth&&fbo.depth.data)setDepth (fbo,x,y,dd.depth);
          }
      }break;
      case CommandType::DRAW:{
        auto const&dd = d.drawCommand;
        auto const&prg = mem.programs[mem.activatedProgram];
        for(uint32_t i=0;i<(dd.nofVertices/3u)*3u;++i){
          InVertex iv;
          auto const&v=mem.vertexArrays[mem.activatedVertexArray];
          if(v.indexBufferID<0)
            iv.gl_VertexID = i;
          else{
            auto const&ebo = (uint8_t*)mem.buffers[v.indexBufferID].data;
            auto ptr = ebo + v.indexOffset;
            auto s = (uint32_t)v.indexType;
            memcpy(&iv.gl_VertexID,ptr+s*i,s);
          }
          for(uint32_t a=0;a<maxAttributes;++a){
            auto const&aa=v.vertexAttrib[a];
            if(aa.bufferID<0)continue;
            auto vbo=(uint8_t*)mem.buffers[aa.bufferID].data;
            vbo+=aa.offset+aa.stride*iv.gl_VertexID;
            memcpy(&iv.attributes[a],vbo,sizeof(float)*(uint32_t)aa.type);
          }

          OutVertex ov;
          ShaderInterface si;
          prg.vertexShader(ov,iv,{mem.uniforms,mem.textures,mem.gl_DrawID});
        }
        mem.gl_DrawID++;
      }break;
      case CommandType::SET_DRAW_ID:{
        mem.gl_DrawID = d.setDrawIdCommand.id;
      }break;
      case CommandType::BIND_FRAMEBUFFER:{
        mem.activatedFramebuffer = d.bindFramebufferCommand.id;
      }break;
      case CommandType::BIND_PROGRAM:{
        mem.activatedProgram = d.bindProgramCommand.id;
      }break;
      case CommandType::BIND_VERTEXARRAY:{
        mem.activatedVertexArray = d.bindVertexArrayCommand.id;
      }break;
      case CommandType::SUB_COMMAND:{
        fake_cmd_enqueueRec(mem,*d.subCommand.commandBuffer);
      }break;                              
      case CommandType::EMPTY:break;
    }
  }
}

void fake_cmd_enqueue(GPUMemory&mem,CommandBuffer const&cb){
  mem.gl_DrawID = 0;
  fake_cmd_enqueueRec(mem,cb);
}


glm::uvec4 getColorU(Image const&img,uint32_t x,uint32_t y){
  auto res   = glm::uvec4(0u);
  auto pixel = (uint8_t*)getPixel(img,x,y);

  for(uint32_t c=0;c<img.channels;++c){
    uint32_t v = 0;
    switch(img.format){
      case Image::FLOAT32:{
        float cc = *(float*)pixel;
        v = glm::clamp((uint32_t)(cc*255.f),0u,255u);
        pixel += sizeof(float);
      }break;
      case Image::UINT8  :{
        v = *(uint8_t*)pixel;
        pixel += sizeof(uint8_t);
      }
    }
    res[img.channelTypes[c]] = v;
  }

  return res;
}

glm:: vec4 getColorF(Image const&img,uint32_t x,uint32_t y){
  auto res   = glm::vec4(0.f);
  auto pixel = (uint8_t*)getPixel(img,x,y);

  for(uint32_t c=0;c<img.channels;++c){
    float v = 0.f;
    switch(img.format){
      case Image::FLOAT32:{
        float cc = *(float*)pixel;
        v = cc;
        pixel += sizeof(float);
      }break;
      case Image::UINT8  :{
        v = (*(uint8_t*)pixel)/255.f;
        pixel += sizeof(uint8_t);
      }
    }
    res[img.channelTypes[c]] = v;
  }

  return res;
}

glm::uvec4 getColorU(Texture const&tex,uint32_t x,uint32_t y){
  return getColorU(tex.img,x,y);
}

glm:: vec4 getColorF(Texture const&tex,uint32_t x,uint32_t y){
  return getColorF(tex.img,x,y);
}

glm::uvec4 getColorU(Framebuffer const&frame,uint32_t x,uint32_t y){
  return getColorU(frame.color,x,y);
}

glm:: vec4 getColorF(Framebuffer const&frame,uint32_t x,uint32_t y){
  return getColorF(frame.color,x,y);
}

float getDepth(Framebuffer const&frame,uint32_t x,uint32_t y){
  return getColorF(frame.depth,x,y).x;
}

void setColorU(Image&img,uint32_t x,uint32_t y,glm::uvec4 const&color){
  auto pixel = (uint8_t*)getPixel(img,x,y);
  for(uint32_t c=0;c<img.channels;++c){
    switch(img.format){
      case Image::FLOAT32:{
        auto cc = (float*)pixel;
        *cc = color[img.channelTypes[c]]/255.f;
        pixel += sizeof(float);
      }break;
      case Image::UINT8  :{
        auto cc = (uint8_t*)pixel;
        *cc = color[img.channelTypes[c]];
        pixel += sizeof(uint8_t);
      }
    }
  }
}

void setColorF(Image&img,uint32_t x,uint32_t y,glm::vec4 const&color){
  auto pixel = (uint8_t*)getPixel(img,x,y);
  for(uint32_t c=0;c<img.channels;++c){
    switch(img.format){
      case Image::FLOAT32:{
        auto cc = (float*)pixel;
        *cc = color[img.channelTypes[c]];
        pixel += sizeof(float);
      }break;
      case Image::UINT8  :{
        auto cc = (uint8_t*)pixel;
        *cc = glm::clamp(uint32_t(color[img.channelTypes[c]]*255u),0u,255u);
        pixel += sizeof(uint8_t);
      }
    }
  }
}

void setColorU(Texture &tex,uint32_t x,uint32_t y,glm::uvec4 const&color){
  setColorU(tex.img,x,y,color);
}

void setColorF(Texture &tex,uint32_t x,uint32_t y,glm:: vec4 const&color){
  setColorF(tex.img,x,y,color);
}

void setColorU(Framebuffer &frame,uint32_t x,uint32_t y,glm::uvec4 const&color){
  setColorU(frame.color,x,y,color);
}

void setColorF(Framebuffer &frame,uint32_t x,uint32_t y,glm:: vec4 const&color){
  setColorF(frame.color,x,y,color);
}

void setDepth (Texture     &tex,  uint32_t x,uint32_t y,float            depth){
  setColorF(tex,x,y,glm::vec4(depth));
}

void setDepth (Framebuffer &frame,uint32_t x,uint32_t y,float            depth){
  setColorF(frame.depth,x,y,glm::vec4(depth));
}

std::string testCounter(bool first){
  static int32_t counter = -1;
  if(first)counter=-1;
  ++counter;
  std::stringstream ss;
  ss << std::setfill('0') << std::setw(2) << counter;
  return ss.str();
}

std::string getImageDifference(uint32_t p,std::string const&prefix,Image const&ei,Image const&si,uint32_t h,uint32_t w){
  std::stringstream ss;
  if(ei.bytesPerPixel != si.bytesPerPixel    )ss << padding(p) << prefix << ".bytesPerPixel = " <<     si.bytesPerPixel  << ";" << std::endl;
  if(ei.channels      != si.channels         )ss << padding(p) << prefix << ".channels      = " <<     si.channels       << ";" << std::endl;
  if(ei.format        != si.format           )ss << padding(p) << prefix << ".format        = " << str(si.format       ) << ";" << std::endl;
  if(ei.pitch         != si.pitch            )ss << padding(p) << prefix << ".pitch         = " <<     si.pitch          << ";" << std::endl;
  if(ei.data == nullptr && si.data != nullptr)ss << padding(p) << prefix << ".data          = " <<     si.data           << ";" << std::endl;
  for(int j=0;j<4;++j)
    if(ei.channelTypes[j] != si.channelTypes[j])
      ss << padding(p) << prefix << ".channelTypes["<<j<<"] = " << str(ei.channelTypes[j]) << ";" << std::endl;
  if(si.data){
    auto const sptr = (uint8_t*)si.data;
    auto const eptr = (uint8_t*)ei.data;
    size_t nofDifferentPixels = 0;
    std::vector<glm::uvec2>diff;
    for(uint32_t y=0;y<h;++y)
      for(uint32_t x=0;x<w;++x){
        auto sss = sptr+y*si.pitch+x*si.bytesPerPixel;
        auto eee = eptr+y*ei.pitch+x*ei.bytesPerPixel;
        for(uint32_t k=0;k<si.bytesPerPixel;++k){
          if(sss[k] == eee[k])continue;
          diff.push_back(glm::uvec2(x,y));
          nofDifferentPixels++;
          break;
        }
      }
    if(nofDifferentPixels != 0){
      ss << padding(p) << "// " << nofDifferentPixels<<" pixels were changed" << std::endl;
      uint32_t const maxDiffs = 20;
      for(uint32_t i=0;i<maxDiffs&&i<diff.size();++i){
        uint32_t x = diff.at(i).x;
        uint32_t y = diff.at(i).y;
        ss << padding(p) << prefix << ".data[x="<<x<<",y="<<y<<"] = ";
        auto sss = sptr+y*si.pitch+x*si.bytesPerPixel;
        if(si.format == Image::UINT8){
          for(uint32_t k=0;k<si.bytesPerPixel;++k){
            ss << std::setw(2) << std::setfill('0') << std::hex << (int)sss[k];
          }
        }
        if(si.format == Image::FLOAT32){
          for(uint32_t c=0;c<si.channels;++c)
            ss << ((float*)sss)[c] << " ";
        }
        ss << ";" << std::endl;
      }
      ss << padding(p) << "..." << std::endl;
    }

  }
  return ss.str();
}

std::string getMemDifference(uint32_t p,GPUMemory const&emp,GPUMemory const&stu){
  std::stringstream ss;
  if(emp.activatedProgram     != stu.activatedProgram    )ss << padding(p) << "mem.activatedProgram     = " << stu.activatedProgram     << ";" << std::endl;
  if(emp.activatedFramebuffer != stu.activatedFramebuffer)ss << padding(p) << "mem.activatedFramebuffer = " << stu.activatedFramebuffer << ";" << std::endl;
  if(emp.activatedVertexArray != stu.activatedVertexArray)ss << padding(p) << "mem.activatedVertexArray = " << stu.activatedVertexArray << ";" << std::endl;

  for(uint32_t i=0;i<emp.maxBuffers;++i){
    auto const& e = emp.buffers[i];
    auto const& s = stu.buffers[i];
    if(e.size != s.size)
      ss << padding(p) << "mem.buffers["<<i<<"].size = " << s.size << ";" << std::endl;
    if(e.data == nullptr && s.data != nullptr)
      ss << padding(p) << "mem.buffers["<<i<<"].data = " << s.data << ";" << std::endl;
  }
  for(uint32_t i=0;i<emp.maxPrograms;++i){
    auto const& e = emp.programs[i];
    auto const& s = stu.programs[i];
    if(e.fragmentShader != s.fragmentShader)ss << padding(p) << "mem.programs["<<i<<"].fragmentShader = " << s.fragmentShader << ";" << std::endl;
    if(e.vertexShader   != s.vertexShader  )ss << padding(p) << "mem.programs["<<i<<"].vertexShader   = " << s.vertexShader   << ";" << std::endl;
    for(uint32_t a=0;a<maxAttributes;++a)
      if(e.vs2fs[a] != s.vs2fs[a])
        ss << padding(p) << "mem.programs["<<i<<"].vs2fs["<<a<<"] = " << str(s.vs2fs[a])  << ";" << std::endl;
  }
  for(uint32_t i=0;i<emp.maxUniforms;++i){
    auto const& e = emp.uniforms[i];
    auto const& s = stu.uniforms[i];
    if(e.m4 == s.m4)continue;
    ss << padding(p) << "mem.uniforms["<<i<<"].m4 = mat4" << str(s.m4) << ";" << std::endl;
  }
  for(uint32_t i=0;i<emp.maxTextures;++i){
    auto const&e = emp.textures[i];
    auto const&s = stu.textures[i];
    if(e.width  != s.width )ss << "mem.textures["<<i<<"].width  = " << s.width  << ";" << std::endl;
    if(e.height != s.height)ss << "mem.textures["<<i<<"].height = " << s.height << ";" << std::endl;
    auto const& ei = e.img;
    auto const& si = s.img;
    std::stringstream prefix;
    prefix << "mem.textures["<<i<<"].img";
    ss << getImageDifference(p,prefix.str(),ei,si,s.width,s.height);
  }
  for(uint32_t i=0;i<emp.maxVertexArrays;++i){
    auto const&e = emp.vertexArrays[i];
    auto const&s = stu.vertexArrays[i];
    if(e.indexBufferID != s.indexBufferID)ss << padding(p) << "mem.vertexArrays["<<i<<"].indexBufferID = " <<     s.indexBufferID  << ";" << std::endl;
    if(e.indexOffset   != s.indexOffset  )ss << padding(p) << "mem.vertexArrays["<<i<<"].indexOffset   = " <<     s.indexOffset    << ";" << std::endl;
    if(e.indexType     != s.indexType    )ss << padding(p) << "mem.vertexArrays["<<i<<"].indexType     = " << str(s.indexType    ) << ";" << std::endl;
    for(uint32_t a=0;a<maxAttributes;++a){
      auto const&ea = e.vertexAttrib[a];
      auto const&sa = s.vertexAttrib[a];
      if(ea.bufferID != sa.bufferID)ss << padding(p) << "mem.vertexArrays["<<i<<"].vertexAttrib["<<a<<"].bufferID = " <<     sa.bufferID  << ";" << std::endl;
      if(ea.offset   != sa.offset  )ss << padding(p) << "mem.vertexArrays["<<i<<"].vertexAttrib["<<a<<"].offset   = " <<     sa.offset    << ";" << std::endl;
      if(ea.stride   != sa.stride  )ss << padding(p) << "mem.vertexArrays["<<i<<"].vertexAttrib["<<a<<"].stride   = " <<     sa.stride    << ";" << std::endl;
      if(ea.type     != sa.type    )ss << padding(p) << "mem.vertexArrays["<<i<<"].vertexAttrib["<<a<<"].type     = " << str(sa.type    ) << ";" << std::endl;
    }
  }
  for(uint32_t i=0;i<emp.maxFramebuffers;++i){
    auto const&e = emp.framebuffers[i];
    auto const&s = stu.framebuffers[i];
    if(e.width     != s.width    )ss << padding(p) << "mem.framebuffers["<<i<<"].width     = " << s.width      << ";" << std::endl;
    if(e.height    != s.height   )ss << padding(p) << "mem.framebuffers["<<i<<"].height    = " << s.height     << ";" << std::endl;
    if(e.yReversed != s.yReversed)ss << padding(p) << "mem.framebuffers["<<i<<"].yReversed = " << s.yReversed  << ";" << std::endl;
    {
      auto const&ei = e.color;
      auto const&si = s.color;
      std::stringstream prefix;
      prefix << "mem.framebuffers["<<i<<"].color";
      ss << getImageDifference(p,prefix.str(),ei,si,s.width,s.height);
    }
    {
      auto const&ei = e.depth;
      auto const&si = s.depth;
      std::stringstream prefix;
      prefix << "mem.framebuffers["<<i<<"].depth";
      ss << getImageDifference(p,prefix.str(),ei,si,s.width,s.height);
    }
  }
  
  auto res = ss.str();
  if(res == ""){
    ss << padding(p) << "no changes" << std::endl;
    return ss.str();
  }
  return ss.str();
}


float const floatErr = 0.001f;

bool equalFloats(float const& a, float const& b,float err) {
  return glm::abs(a - b) <= err;
}

bool equalVec4(glm::vec4 const&a,glm::vec4 const&b,float err){
  return
    equalFloats(a.x,b.x,err) &&
    equalFloats(a.y,b.y,err) &&
    equalFloats(a.z,b.z,err) &&
    equalFloats(a.w,b.w,err) ;
}

bool equalVec3(glm::vec3 const&a,glm::vec3 const&b,float err){
  return
    equalFloats(a.x,b.x,err) &&
    equalFloats(a.y,b.y,err) &&
    equalFloats(a.z,b.z,err) ;
}

bool equalVec2(glm::vec2 const&a,glm::vec2 const&b,float err){
  return
    equalFloats(a.x,b.x,err) &&
    equalFloats(a.y,b.y,err);
}

bool equalCounts(size_t a,size_t b,size_t err){
  if(a<b)return (b-a)<err;
  return (a-b)<err;
}

bool greaterFloat(float a,float b,float err){
  return a>b-err;
}

bool lessFloat(float a,float b,float err){
  return a<b+err;
}

std::string str(bool v){
  if(v)return "true" ;
  else return "false";
}

template<>std::string str(uint8_t     const&a){
  std::stringstream ss;
  ss << (uint32_t)a;
  return ss.str();
}

template<>std::string str(glm::vec2 const&a){
  std::stringstream ss;
  ss << "(" << a.x << "," << a.y << ")";
  return ss.str();
}

template<>std::string str(glm::vec3 const&a){
  std::stringstream ss;
  ss << "(" << a.x << "," << a.y << "," << a.z << ")";
  return ss.str();
}

template<>std::string str(glm::vec4 const&a){
  std::stringstream ss;
  ss << "(" << a.x << "," << a.y << "," << a.z << "," << a.w << ")";
  return ss.str();
}

template<>std::string str(glm::uvec2 const&a){
  std::stringstream ss;
  ss << "(" << a.x << "," << a.y << ")";
  return ss.str();
}

template<>std::string str(glm::uvec3 const&a){
  std::stringstream ss;
  ss << "(" << a.x << "," << a.y << "," << a.z << ")";
  return ss.str();
}

template<>std::string str(glm::uvec4 const&a){
  std::stringstream ss;
  ss << "(" << a.x << "," << a.y << "," << a.z << "," << a.w << ")";
  return ss.str();
}

template<>std::string str(glm::ivec2 const&a){
  std::stringstream ss;
  ss << "(" << a.x << "," << a.y << ")";
  return ss.str();
}

template<>std::string str(glm::ivec3 const&a){
  std::stringstream ss;
  ss << "(" << a.x << "," << a.y << "," << a.z << ")";
  return ss.str();
}

template<>std::string str(glm::ivec4 const&a){
  std::stringstream ss;
  ss << "(" << a.x << "," << a.y << "," << a.z << "," << a.w << ")";
  return ss.str();
}

template<>std::string str(glm::mat4 const&m){
  std::stringstream ss;
  ss << "(";
  for(int i=0;i<16;++i){
    if(i>0)ss<<",";
    ss << m[i/4][i%4];
  }
  ss << ")";
  return ss.str();
}

template<>std::string str(IndexType const&i){
  if(i==IndexType::UINT8 )return "IndexType::UINT8" ;
  if(i==IndexType::UINT16)return "IndexType::UINT16";
  if(i==IndexType::UINT32)return "IndexType::UINT32";
  return "unknown";
}

template<>std::string str(AttributeType const&a){
  switch(a){
    case AttributeType::EMPTY:return "AttributeType::EMPTY";
    case AttributeType::FLOAT:return "AttributeType::FLOAT";
    case AttributeType::VEC2 :return "AttributeType::VEC2" ;
    case AttributeType::VEC3 :return "AttributeType::VEC3" ;
    case AttributeType::VEC4 :return "AttributeType::VEC4" ;
    case AttributeType::UINT :return "AttributeType::UINT" ;
    case AttributeType::UVEC2:return "AttributeType::UVEC2";
    case AttributeType::UVEC3:return "AttributeType::UVEC3";
    case AttributeType::UVEC4:return "AttributeType::UVEC4";
    default: return "unknown";
  }
}
template<> std::string str(CommandType const&a){
  switch(a){
    case CommandType::CLEAR           :return "clear"          ;
    case CommandType::DRAW            :return "draw"           ;
    case CommandType::SET_DRAW_ID     :return "setDrawId"      ;
    case CommandType::BIND_FRAMEBUFFER:return "bindFramebuffer";
    case CommandType::BIND_PROGRAM    :return "bindProgram"    ;
    case CommandType::BIND_VERTEXARRAY:return "bindVertexArray";
    case CommandType::SUB_COMMAND     :return "subCommand"     ;
    case CommandType::EMPTY           :return "empty"          ;
  }
  return "";
}

template<>std::string str(bool const&a){
  if(a)return "true";
  return "false";
}

void createDrawCMD(
    CommandBuffer&cb,
    uint32_t       n
    ){
  auto&a=cb.commands[cb.nofCommands];
  cb.nofCommands++;
  a.type = CommandType::DRAW;
  auto&cmd = a.data.drawCommand;

  cmd.nofVertices = n  ;
}


uint8_t floatColorToByte(float c){
  return (uint8_t)(c*255.f);
}


bool sameColor(glm::uvec4 const&a,glm::uvec4 const&b,uint32_t channels){
  for(uint32_t i=0;i<channels;++i)
    if(a[i] != b[i])return false;
  return true;
}

std::string colorToStr(glm::uvec4 const&a,uint32_t channels){
  std::stringstream ss;
  ss << "(";
  for(uint32_t i=0;i<channels;++i){
    if(i>0)ss << ",";
    ss << a[i];
  }
  ss << ")";
  return ss.str();
}

glm::uvec4 floatColorToBytes(glm::vec4 const&col){
  auto res = glm::uvec4(0);
  for(uint32_t i=0;i<4;++i)
    res[i] = floatColorToByte(col[i]);
  return res;
}

std::shared_ptr<MemCb>memCb;

MemCb::MemCb(){}

MemCb::MemCb(uint32_t w,uint32_t h){
  aframe = createFramebuffer(w,h);
}


void MemCb::printDetails(){
  std::cerr << R".(
  ==========================
  === DETAILNÍ INFORMACE ===
  ==========================
  ).";

  std::cerr << R".(
  Command buffer v tomto testu vypadá takto:
  )." << std::endl;
  print(commandBufferToStr(2,cb),Color::GRAY);

  std::cerr << R".(
  Jak se liší vaše změny nad pamětí od těch, co se měly provést:
  )." << std::endl;

  print(getMemDifference(2,expectedMem.mem,studentMem.mem),Color::GRAY);

  std::cerr << R".(
  Změny, které jste nad pamětí provedli:
  )." << std::endl;

  print(getMemDifference(2,mem,studentMem.mem),Color::GRAY);

  std::cerr << R".(
  Změny, které se nad pamětí měly provést:
  )." << std::endl;

  print(getMemDifference(2,mem,expectedMem.mem),Color::GRAY);

  std::cerr << R".(
  Jak vypadala paměť na začátku:
  )." << std::endl;

  print(gpuMemoryToStr(2,mem),Color::GRAY);
}


std::string commandBufferDumpToStr(
    size_t                 p       ,
    DrawCommandsDump const&student ,
    DrawCommandsDump const&expected,
    DumpDiff         const&status  );

void MemCb::printStatus(){
  std::cerr << std::endl;
  std::cerr << padding(2) << "Takto to dopadlo:" << std::endl;
  std::cerr << std::endl;
  std::cerr << commandBufferDumpToStr(2,student,expected,status);
  std::cerr << std::endl;
}

void MemCb::printHelp(std::string const&str){
  std::cerr << padding(2) << "Nápověda:" << std::endl;
  print(str,Color::BLUE);
}

bool operator==(Attribute const&a,Attribute const&b){
  return a.v4 == b.v4;
}

bool operator==(InVertex const&a,InVertex const&b){
  for(size_t i=0;i<maxAttributes;++i)
    if(!(a.attributes[i] == b.attributes[i]))return false;
  return a.gl_VertexID == b.gl_VertexID;
}

DumpDiff filterErrorLevel(DumpDiff status,DumpDiff maxLevel){
  if(status <= maxLevel)return status;
  return DumpDiff::SAME;
}

DumpDiff isDrawDumpSame(
    DrawCommandsDump const&expected,
    DrawCommandsDump const&student ,
    DumpDiff         const&level   
    ){
  if(expected.size() != student.size())return filterErrorLevel(DumpDiff::NOF_COMMANDS_DIFFER,level);

  for(uint32_t i=0;i<expected.size();++i){
    auto ee = expected.at(i);
    auto ss = student .at(i);

    auto const&emem = ee->memCopy.mem;
    auto const&smem = ss->memCopy.mem;

    if(emem.activatedFramebuffer != smem.activatedFramebuffer)return filterErrorLevel(DumpDiff::DIFFERENT_BIND_FRAMEBUFFER,level);
    if(emem.activatedProgram     != smem.activatedProgram    )return filterErrorLevel(DumpDiff::DIFFERENT_BIND_PROGRAM    ,level);
    if(emem.activatedVertexArray != smem.activatedVertexArray)return filterErrorLevel(DumpDiff::DIFFERENT_BIND_VERTEXARRAY,level);

    if(emem.gl_DrawID != smem.gl_DrawID)return filterErrorLevel(DumpDiff::DIFFERENT_DRAWID,level);

    auto const& eIns = ee->inVertices;
    auto const& sIns = ss->inVertices;

    if(eIns.size() != sIns.size())return filterErrorLevel(DumpDiff::DIFFERENT_NOF_VERTICES,level);

    for(size_t v=0;v<eIns.size();++v){
      auto const&eIn = eIns.at(v);
      auto const&sIn = sIns.at(v);
      if(eIn.gl_VertexID != sIn.gl_VertexID)return filterErrorLevel(DumpDiff::DIFFERENT_VERTEXID,level);
      for(size_t a=0;a<maxAttributes;++a)
        if(eIn.attributes[a].v4 != sIn.attributes[a].v4)return filterErrorLevel(DumpDiff::DIFFERENT_ATTRIBUTE,level);
    }
  }

  return DumpDiff::SAME;
}

//std::string clearCommandToStr(size_t p,ClearDump const*cc){
//  std::stringstream ss;
//  ss << padding(p);
//  if(cc->clearColor){
//    if(cc->clearDepth){
//      ss << "clearColorAndDepth(uvec4"<<str(cc->color)<<","<<cc->depth<<")";
//    }else{
//      ss << "clearColor(uvec4"<<str(cc->color)<<")";
//    }
//  }else{
//    ss << "clearDepth("<<cc->depth<<")";
//  }
//  ss << std::endl;
//  return ss.str();
//}

int32_t findProgramID(GPUMemory const&mem,void*ptr){
  for(uint32_t i=0;i<mem.maxPrograms;++i)
    if((void*)mem.programs[i].vertexShader == ptr)return i;
  return -1;
}

std::string drawCommandToStr(size_t p,GPUMemory const&mem,std::shared_ptr<DrawDump>cc){
  std::stringstream ss;
  ss << padding(p) << "draw(nofVertices=";
  ss << cc->inVertices.size();
  ss << "backfaceCulling="<<",";
  ss << ")";
  ss << std::endl;
  return ss.str();
}

std::string setDrawIdCommandToStr(size_t p,DrawDump const*cc){
  std::stringstream ss;
  ss << padding(p) << "setDrawId(drawId=";
  ss << cc->memCopy.mem.gl_DrawID;
  ss << ")";
  ss << std::endl;
  return ss.str();
}

std::string bindFramebufferCommandToStr(size_t p,DrawDump const*cc){
  std::stringstream ss;
  ss << padding(p) << "bindFramebuffer(framebufferId=";
  ss << cc->memCopy.mem.activatedFramebuffer;
  ss << ")";
  ss << std::endl;
  return ss.str();
}

//std::string bindProgramCommandToStr(size_t p,BindProgramDump const*cc){
//  std::stringstream ss;
//  ss << padding(p) << "bindProgram(programId=";
//  ss << cc->programId;
//  ss << ")";
//  ss << std::endl;
//  return ss.str();
//}
//
//std::string bindVertexArrayCommandToStr(size_t p,BindVertexArrayDump const*cc){
//  std::stringstream ss;
//  ss << padding(p) << "bindVertexArray(vaoId=";
//  ss << cc->vaoId;
//  ss << ")";
//  ss << std::endl;
//  return ss.str();
//}
//
//std::string copyDataCommandToStr(size_t p,CopyDataDump const*cc){
//  std::stringstream ss;
//  ss << padding(p) << "copyData(dst=";
//  ss << cc->dst;
//  ss << ",src=";
//  ss << cc->src;
//  ss << ",size=";
//  ss << cc->size;
//  ss << ")";
//  ss << std::endl;
//  return ss.str();
//}

std::string commandDumpToStr(size_t p,GPUMemory const&mem){
  std::stringstream ss;
  ss << padding(p) << "počet příkazů: " << memCb->commands.size() << std::endl;
  for(auto const&c:memCb->commands){
    ss << drawCommandToStr(p,mem,c);
  }
  return ss.str();
}

std::string drawCommandToStr(size_t p,DrawDump const*cc){
  std::stringstream ss;
  ss << padding(p) << "nofVertices: " << cc->inVertices.size() << std::endl;
  auto const& verts = cc->inVertices;
  for(size_t i=0;i<verts.size();++i){
    auto const& v = verts.at(i);
    ss << padding(p+2) << "vertex" << i << ":" << std::endl;
    ss << padding(p+4) << "gl_VertexID: " << v.gl_VertexID << std::endl;
    for(size_t a=0;a<maxAttributes;++a){
      auto const&vao = cc->memCopy.mem.vertexArrays[cc->memCopy.mem.activatedVertexArray];
      auto const& type = vao.vertexAttrib[a].type;
      auto const& att  = v.attributes[a];
      if(type == AttributeType::EMPTY)continue;
      ss << padding(p+6) << "attribute: ";
      if(type == AttributeType::FLOAT)ss << str(att.v1) << std::endl;
      if(type == AttributeType::VEC2 )ss << str(att.v2) << std::endl;
      if(type == AttributeType::VEC3 )ss << str(att.v3) << std::endl;
      if(type == AttributeType::VEC4 )ss << str(att.v4) << std::endl;
      if(type == AttributeType::UINT )ss << str(att.u1) << std::endl;
      if(type == AttributeType::UVEC2)ss << str(att.u2) << std::endl;
      if(type == AttributeType::UVEC3)ss << str(att.u3) << std::endl;
      if(type == AttributeType::UVEC4)ss << str(att.u4) << std::endl;
    }
  }
  return ss.str();
}

std::string nofCommandDifferStr(
    size_t p,
    DrawCommandsDump const&expected,
    DrawCommandsDump const&student ){
  std::stringstream ss;
  ss << padding(p) << "Liší se počet zpracovaných příkazů." << std::endl;
  ss << padding(p) << "Očekávaný počet: " << expected.size() << std::endl;
  ss << padding(p) << "Váš počet      : " << student .size() << std::endl;
  return ss.str();
}

//std::string listCommandTypes(
//    size_t p,
//    CmdsDump const&dump
//    ){
//  std::stringstream ss;
//  for(auto const&c:dump){
//    ss << padding(p);
//    switch(c->type){
//      case CommandType::CLEAR           :ss << "clear"          ;break;
//      case CommandType::DRAW            :ss << "draw"           ;break;
//      case CommandType::SET_DRAW_ID     :ss << "setDrawId"      ;break;
//      case CommandType::BIND_FRAMEBUFFER:ss << "bindFramebuffer";break;
//      case CommandType::BIND_PROGRAM    :ss << "bindProgram"    ;break;
//      case CommandType::BIND_VERTEXARRAY:ss << "bindVertexArray";break;
//      case CommandType::COPY_DATA       :ss << "copyData"       ;break;
//      case CommandType::EMPTY           :ss << "empty"          ;break;
//
//    }
//    ss << std::endl;
//  }
//  return ss.str();
//}

//std::string differentCommandsStr(
//    size_t p,
//    CmdsDump const&expected,
//    CmdsDump const&student ){
//  std::stringstream ss;
//  ss << padding(p) << "Liší se typ zpracovaných příkazů." << std::endl;
//  ss << padding(p) << "Očekáváné příkazy:" << std::endl;
//  ss << listCommandTypes(p+2,expected);
//  ss << padding(p) << "Vaše příkazy:" << std::endl;
//  ss << listCommandTypes(p+2,student );
//  return ss.str();
//}
//
//
//std::string listClearStr(
//    size_t p,
//    CmdsDump const&dump){
//  std::stringstream ss;
//
//  for(auto const&c:dump){
//    if(c->type != CommandType::CLEAR)continue;
//    ss << clearCommandToStr(p,c->asClear());
//  }
//  return ss.str();
//}
//
//std::string differentClearStr(
//    size_t p,
//    CmdsDump const&expected,
//    CmdsDump const&student ){
//  std::stringstream ss;
//  ss << padding(p) << "Liší se čistící příkazy." << std::endl;
//  ss << padding(p) << "Očekáváné čistící příkazy:" << std::endl;
//  ss << listClearStr(p+2,expected);
//  ss << padding(p) << "Vaše příkazy:" << std::endl;
//  ss << listClearStr(p+2,student );
//  return ss.str();
//}

std::string listSetDrawIdStr(
    size_t p,
    DrawCommandsDump const&dump){
  std::stringstream ss;

  for(auto const&c:dump){
    ss << setDrawIdCommandToStr(p,&*c);
  }
  return ss.str();
}

std::string differentSetDrawIdStr(
    size_t p,
    DrawCommandsDump const&expected,
    DrawCommandsDump const&student ){
  std::stringstream ss;
  ss << padding(p) << "Liší se příkazy pro nastavení drawId." << std::endl;
  ss << padding(p) << "Očekáváné příkazy pro nastavení drawId:" << std::endl;
  ss << listSetDrawIdStr(p+2,expected);
  ss << padding(p) << "Vaše příkazy pro nastavení drawId:" << std::endl;
  ss << listSetDrawIdStr(p+2,student );
  return ss.str();
}

std::string listBindFramebufferStr(
    size_t p,
    DrawCommandsDump const&dump){
  std::stringstream ss;

  for(auto const&c:dump){
    ss << bindFramebufferCommandToStr(p,&*c);
  }
  return ss.str();
}

std::string differentBindFramebufferStr(
    size_t p,
    DrawCommandsDump const&expected,
    DrawCommandsDump const&student ){
  std::stringstream ss;
  ss << padding(p) << "Liší se příkazy pro navázání framebufferu." << std::endl;
  ss << padding(p) << "Očekáváné příkazy pro navázání framebufferu:" << std::endl;
  ss << listBindFramebufferStr(p+2,expected);
  ss << padding(p) << "Vaše příkazy pro navázání framebufferu:" << std::endl;
  ss << listBindFramebufferStr(p+2,student );
  return ss.str();
}

std::string listBindPrograms(
    size_t p,
    DrawCommandsDump const&dump){
  std::stringstream ss;

  for(auto const&c:dump){
    ss << padding(p) << c->memCopy.mem.activatedProgram << std::endl;
  }
  return ss.str();
}

std::string differentBindProgramStr(
    size_t p,
    DrawCommandsDump const&expected,
    DrawCommandsDump const&student ){
  std::stringstream ss;
  ss << padding(p) << "Liší se program příkazu bindProgram." << std::endl;
  ss << padding(p) << "Očekáváné bindProgram příkazy:" << std::endl;
  ss << listBindPrograms(p+2,expected);
  ss << padding(p) << "Vaše bindProgram příkazy:" << std::endl;
  ss << listBindPrograms(p+2,student );
  return ss.str();
}

std::string listBindVertexArrays(
    size_t p,
    DrawCommandsDump const&dump){
  std::stringstream ss;

  for(auto const&c:dump){
    ss << padding(p) << c->memCopy.mem.activatedVertexArray << std::endl;
  }
  return ss.str();
}

std::string differentBindVertexArrayStr(
    size_t p,
    DrawCommandsDump const&expected,
    DrawCommandsDump const&student ){
  std::stringstream ss;
  ss << padding(p) << "Liší se id příkazu bindVertexArray." << std::endl;
  ss << padding(p) << "Očekáváné id bindVertexArray příkazů:" << std::endl;
  ss << listBindVertexArrays(p+2,expected);
  ss << padding(p) << "Vaše bindVertexArray příkazy:" << std::endl;
  ss << listBindVertexArrays(p+2,student );
  return ss.str();
}

//std::string listCopyData(
//    size_t p,
//    CmdsDump const&dump){
//  std::stringstream ss;
//
//  for(auto const&c:dump){
//    if(c->type != CommandType::COPY_DATA)continue;
//    auto const&cd = c->asCopyData();
//    ss << padding(p) << cd->dst  << std::endl;
//    ss << padding(p) << cd->src  << std::endl;
//    ss << padding(p) << cd->size << std::endl;
//  }
//  return ss.str();
//}
//
//std::string differentCopyDataStr(
//    size_t p,
//    CmdsDump const&expected,
//    CmdsDump const&student ){
//  std::stringstream ss;
//  ss << padding(p) << "Liší se příkaz pro kopírování paměti." << std::endl;
//  ss << padding(p) << "Očekáváné kopírovací příkazy:" << std::endl;
//  ss << listCopyData(p+2,expected);
//  ss << padding(p) << "Vaše kopírovací příkazy:" << std::endl;
//  ss << listCopyData(p+2,student );
//  return ss.str();
//}

std::string listNofVertices(
    size_t p,
    DrawCommandsDump const&dump){
  std::stringstream ss;

  for(auto const&c:dump){
    ss << padding(p) << c->inVertices.size() << std::endl;
  }
  return ss.str();
}

std::string differentNofVerticesStr(
    size_t p,
    DrawCommandsDump const&expected,
    DrawCommandsDump const&student ){
  std::stringstream ss;
  ss << padding(p) << "Liší se počet spuštění vertex shaderu." << std::endl;
  ss << padding(p) << "Očekáváný počet spuštění:" << std::endl;
  ss << listNofVertices(p+2,expected);
  ss << padding(p) << "Váš počet spuštění:" << std::endl;
  ss << listNofVertices(p+2,student );
  return ss.str();
}

std::string listDrawID(
    size_t p,
    DrawCommandsDump const&dump){
  std::stringstream ss;

  uint32_t counter = 0;
  for(auto const&c:dump)
    ss << padding(p) << "drawCommand" << counter++ << " has gl_DrawID=" << c->memCopy.mem.gl_DrawID << std::endl;
  return ss.str();
}

std::string differentDrawIDStr(
    size_t p,
    DrawCommandsDump const&expected,
    DrawCommandsDump const&student ){
  std::stringstream ss;
  ss << padding(p) << "Liší se gl_DrawID." << std::endl;
  ss << padding(p) << "Očekávané hodnoty gl_DrawID:" << std::endl;
  ss << listDrawID(p+2,expected);
  ss << padding(p) << "Vaše hodnoty gl_DrawID:" << std::endl;
  ss << listDrawID(p+2,student );
  return ss.str();
}

std::string listVertexID(
    size_t p,
    DrawCommandsDump const&dump){
  std::stringstream ss;

  uint32_t counter = 0;
  for(auto const&c:dump){
    ss << padding(p) << "drawCommand" << counter++ << ": ";
    for(auto const&v:c->inVertices)
      ss << v.gl_VertexID << ", ";
    ss << std::endl;
  }
  return ss.str();
}


std::string differentVertexIDStr(
    size_t p,
    DrawCommandsDump const&expected,
    DrawCommandsDump const&student ){
  std::stringstream ss;
  ss << padding(p) << "Liší se gl_VertexID." << std::endl;
  ss << padding(p) << "Očekávané hodnoty gl_VertexID:" << std::endl;
  ss << listVertexID(p+2,expected);
  ss << padding(p) << "Vaše hodnoty gl_VertexID:" << std::endl;
  ss << listVertexID(p+2,student );
  return ss.str();
}

std::string listAttributes(
    size_t p,
    DrawCommandsDump const&cmds ){
  std::stringstream ss;

  uint32_t counter = 0;
  for(size_t i=0;i<cmds.size();++i){
    auto const&s = cmds .at(i);
    ss << padding(p) << "draw" << counter++ << std::endl;
    for(uint32_t a=0;a<maxAttributes;++a){
      auto const&smem = s->memCopy.mem;
      auto const&svao = smem.vertexArrays[smem.activatedVertexArray];
      auto const&type = svao.vertexAttrib[a].type;
      if(type == AttributeType::EMPTY)continue;
      ss << padding(p+2) << "attrib" << a << ": ";
      for(auto const&v:s->inVertices){
        auto const&att = v.attributes[a];
        if(type == AttributeType::FLOAT)ss << str(att.v1) << ", ";
        if(type == AttributeType::VEC2 )ss << str(att.v2) << ", ";
        if(type == AttributeType::VEC3 )ss << str(att.v3) << ", ";
        if(type == AttributeType::VEC4 )ss << str(att.v4) << ", ";
        if(type == AttributeType::UINT )ss << str(att.u1) << ", ";
        if(type == AttributeType::UVEC2)ss << str(att.u2) << ", ";
        if(type == AttributeType::UVEC3)ss << str(att.u3) << ", ";
        if(type == AttributeType::UVEC4)ss << str(att.u4) << ", ";
      }
      ss << std::endl;
    }
    ss << std::endl;
  }
  return ss.str();
}

std::string differentAttributeStr(
    size_t p,
    DrawCommandsDump const&expected,
    DrawCommandsDump const&student ){
  std::stringstream ss;
  ss << padding(p) << "Liší se attributy vrcholů." << std::endl;
  ss << padding(p) << "Očekávané attributy:" << std::endl;
  ss << listAttributes(p+2,expected);
  ss << padding(p) << "Vaše hodnoty attributů:" << std::endl;
  ss << listAttributes(p+2,student );
  return ss.str();
}

void printDrawCommandsDump(DrawCommandsDump const&cmds){
  for(auto const&c:cmds){
    std::cerr << gpuMemoryToStr(0,c->memCopy.mem);
    std::cerr << "gl_DrawID: " << c->si.gl_DrawID << std::endl;
    for(auto const&iv:c->inVertices){
      std::cerr << "  gl_VertexID: " << iv.gl_VertexID << std::endl;
      for(uint32_t i=0;i<maxAttributes;++i)
      std::cerr << "    " << str(iv.attributes[i].v4) << std::endl;
    }
  }
}

std::string commandBufferDumpToStr(
    size_t p,
    DrawCommandsDump const&student ,
    DrawCommandsDump const&expected,
    DumpDiff         const&status  ){
  switch(status){
    case DumpDiff::NOF_COMMANDS_DIFFER       :return nofCommandDifferStr        (p,expected,student);
    //case DumpDiff::DIFFERENT_COMMAND_TYPES   :return differentCommandsStr       (p,expected,student);
    //case DumpDiff::DIFFERENT_CLEAR           :return differentClearStr          (p,expected,student);
    case DumpDiff::DIFFERENT_SET_DRAW_ID     :return differentSetDrawIdStr      (p,expected,student);
    case DumpDiff::DIFFERENT_BIND_FRAMEBUFFER:return differentBindFramebufferStr(p,expected,student);
    case DumpDiff::DIFFERENT_BIND_PROGRAM    :return differentBindProgramStr    (p,expected,student);
    case DumpDiff::DIFFERENT_BIND_VERTEXARRAY:return differentBindVertexArrayStr(p,expected,student);
    //case DumpDiff::DIFFERENT_COPY_DATA       :return differentCopyDataStr       (p,expected,student);
    case DumpDiff::DIFFERENT_NOF_VERTICES    :return differentNofVerticesStr    (p,expected,student);
    case DumpDiff::DIFFERENT_DRAWID          :return differentDrawIDStr         (p,expected,student);
    case DumpDiff::DIFFERENT_VERTEXID        :return differentVertexIDStr       (p,expected,student);
    case DumpDiff::DIFFERENT_ATTRIBUTE       :return differentAttributeStr      (p,expected,student);
    case DumpDiff::DIFFERENT_SHADERINTERFACE :return "shader interface problem";//TODO co s tim
    case DumpDiff::SAME                      :return padding(p) + "Nemáte problém.";
    default:return "???";
  }
  return "";
}

bool prgSame(Program const&a,Program const&b){
  if(a.fragmentShader != b.fragmentShader)return false;
  if(a.vertexShader   != b.vertexShader  )return false;
  for(uint32_t i=0;i<maxAttributes;++i)
    if(a.vs2fs[i] != b.vs2fs[i])return false;
  return true;
}

bool vaoSame(VertexArray const&a,VertexArray const&b){
  if(a.indexBufferID != b.indexBufferID)return false;
  if(a.indexOffset   != b.indexOffset  )return false;
  if(a.indexType     != b.indexType    )return false;
  for(uint32_t i=0;i<maxAttributes;++i){
    auto const&aa = a.vertexAttrib[i];
    auto const&bb = b.vertexAttrib[i];
    if(aa.bufferID != bb.bufferID)return false;
    if(aa.offset   != bb.offset  )return false;
    if(aa.stride   != bb.stride  )return false;
    if(aa.type     != bb.type    )return false;
  }
  return true;
}

bool empty(void const*ptr){
  return ptr == nullptr;
}

bool imgSameShallow(Image const&a,Image const&b){
  if(a.bytesPerPixel != b.bytesPerPixel)return false;
  if(a.channels      != b.channels     )return false;
  if(a.format        != b.format       )return false;
  if(a.pitch         != b.pitch        )return false;
  if(empty(a.data)   != empty(b.data)  )return false;
  for(int i=0;i<4;++i)
    if(a.channelTypes[i] != b.channelTypes[i])return false;
  return true;
}

bool imgSameDeep(Image const&a,Image const&b,uint32_t h){
  if(!imgSameShallow(a,b))return false;
  if(a.data)return memcmp(a.data,b.data,h*a.pitch)!=0;
  return true;
}

bool fboSameShallow(Framebuffer const&a,Framebuffer const&b){
  if(a.width     != b.width    )return false;
  if(a.height    != b.height   )return false;
  if(a.yReversed != b.yReversed)return false;
  if(!imgSameShallow(a.color,b.color))return false;
  if(!imgSameShallow(a.depth,b.depth))return false;
  return true;
}

bool fboSameDeep(Framebuffer const&a,Framebuffer const&b){
  if(!fboSameShallow(a,b))return false;
  if(!imgSameDeep(a.color,b.color,a.height))return false;
  if(!imgSameDeep(a.depth,b.depth,a.height))return false;
  return true;
}

bool texSameShallow(Texture const&a,Texture const&b){
  if(a.width     != b.width    )return false;
  if(a.height    != b.height   )return false;
  if(!imgSameShallow(a.img,b.img))return false;
  return true;
}

bool texSameDeep(Texture const&a,Texture const&b){
  if(!texSameShallow(a,b))return false;
  return imgSameDeep(a.img,b.img,a.height);
}

bool bufSameShallow(Buffer const&a,Buffer const&b){
  if(      a.size  !=       b.size )return false;
  if(empty(a.data) != empty(b.data))return false;
  return true;
}

bool bufSameDeep(Buffer const&a,Buffer const&b){
  if(!bufSameShallow(a,b))return false;
  if(a.data)return memcmp(a.data,b.data,a.size)==0;
  return true;
}

bool memSameShallow(GPUMemory const&a,GPUMemory const&b){
  if(a.activatedFramebuffer != b.activatedFramebuffer)return false;
  if(a.activatedProgram     != b.activatedProgram    )return false;
  if(a.activatedVertexArray != b.activatedVertexArray)return false;
  for(uint32_t i=0;i<a.maxVertexArrays;++i)
    if(!vaoSame(a.vertexArrays[i],b.vertexArrays[i]))return false;
  for(uint32_t i=0;i<a.maxPrograms;++i)
    if(!prgSame(a.programs[i],b.programs[i]))return false;
  for(uint32_t i=0;i<a.maxUniforms;++i)
    if(a.uniforms[i].m4 != b.uniforms[i].m4)return false;
  for(uint32_t i=0;i<a.maxFramebuffers;++i)
    if(!fboSameShallow(a.framebuffers[i],b.framebuffers[i]))return false;
  for(uint32_t i=0;i<a.maxTextures;++i)
    if(!texSameShallow(a.textures[i],b.textures[i]))return false;
  for(uint32_t i=0;i<a.maxBuffers;++i)
    if(!bufSameShallow(a.buffers[i],b.buffers[i]))return false;
  return true;
}

bool memSameDeep(GPUMemory const&a,GPUMemory const&b){
  if(!memSameShallow(a,b))return false;
  for(uint32_t i=0;i<a.maxFramebuffers;++i)
    if(!fboSameDeep(a.framebuffers[i],b.framebuffers[i]))return false;
  for(uint32_t i=0;i<a.maxTextures;++i)
    if(!texSameDeep(a.textures[i],b.textures[i]))return false;
  for(uint32_t i=0;i<a.maxBuffers;++i)
    if(!bufSameDeep(a.buffers[i],b.buffers[i]))return false;
  return true;
}

void insertDrawDump(InVertex const&i,ShaderInterface const&si){
  auto drawDump     = std::make_shared<DrawDump>();
  drawDump->memCopy = deepCopy(*memCb->inFlightMem);
  drawDump->si      = si;
  drawDump->inVertices.push_back(i);
  memCb->commands.push_back(drawDump);
}

void vertexShaderDumpGeneral(InVertex const&i,ShaderInterface const&si){
  if(memCb->commands.empty()){
    insertDrawDump(i,si);
    return;
  }
  auto&last = memCb->commands.back();

  if(
      memSameShallow(last->memCopy.mem,*memCb->inFlightMem) &&
      last->si.gl_DrawID == si.gl_DrawID
      ){
    last->inVertices.push_back(i);
  }else{
    insertDrawDump(i,si);
  }
}

void vertexShaderDump0(OutVertex&,InVertex const&i,ShaderInterface const&si){
  vertexShaderDumpGeneral(i,si);
}
void vertexShaderDump1(OutVertex&,InVertex const&i,ShaderInterface const&si){
  vertexShaderDumpGeneral(i,si);
}
void vertexShaderDump2(OutVertex&,InVertex const&i,ShaderInterface const&si){
  vertexShaderDumpGeneral(i,si);
}
void vertexShaderDump3(OutVertex&,InVertex const&i,ShaderInterface const&si){
  vertexShaderDumpGeneral(i,si);
}

void vertexShaderInject(OutVertex&outVertex,InVertex const&inVertex,ShaderInterface const&){
  if(inVertex.gl_VertexID<memCb->outVertices.size()){
    outVertex = memCb->outVertices.at(inVertex.gl_VertexID);
  }
}

//Uniform unif[GPUMemory::maxUniforms];
//Texture texs[GPUMemory::maxTextures];

void fragmentShaderDump(OutFragment&,InFragment const&inF,ShaderInterface const&){
  memCb->inFragments.push_back(inF);
}

void vertexShaderEmpty(OutVertex&,InVertex const&,ShaderInterface const&){
}
void fragmentShaderEmpty(OutFragment&,InFragment const&,ShaderInterface const&){
}


void clearFrame(Framebuffer&frame,glm::uvec3 const&color,float d){
  for(uint32_t y=0;y<frame.height;++y)
    for(uint32_t x=0;x<frame.width;++x){
      auto pix = glm::uvec2(x,y);
      setColorU(frame,pix.x,pix.y,glm::uvec4(color,0u));
      setDepth (frame,pix.x,pix.y,d);
    }
}

void clearFrame(Framebuffer&frame,glm:: vec3 const&c,float d){
  clearFrame(frame,glm::uvec3(glm::clamp(c*255.f,glm::vec3(0.f),glm::vec3(255.))),d);
}

glm::uvec3  alphaMix(glm::vec4 const&frameColor,glm::vec4 const&fragColor){
  auto frame = floatColorToBytes(frameColor);
  return glm::clamp(glm::mix(glm::vec3(frame)/255.f,glm::vec3(fragColor),fragColor[3]),0.f,1.f)*255.f;
}


std::string padding(size_t n){
  return std::string(n,' ');
}

void printChild(Node const&node,size_t child,size_t p);

void printNodeContent(Node const&node,size_t p){
  std::cerr << padding(p) << "  mesh = " << node.mesh << std::endl;
  //if(node.modelMatrix != glm::mat4(1))
  std::cerr << padding(p) << "  modelMatrix = " << str(node.modelMatrix) << std::endl;
  std::cerr << padding(p) << "  children["<<node.children.size()<<"] = {";
  if(!node.children.empty()){
    std::cerr << std::endl;
    for(size_t i=0;i<node.children.size();++i)
      printChild(node.children.at(i),i,p+2);
    std::cerr << padding(p) << "  ";
  }
  std::cerr << "}" << std::endl;
}

void printChild(Node const&node,size_t child,size_t p){
  std::cerr << padding(p+2) << "child" << child << "{" << std::endl;
  printNodeContent(node,p+2);
  std::cerr << padding(p+2) << "}" << std::endl;
}

void printVertexAttrib(std::string const&name,VertexAttrib const&a,uint32_t p){
  if(a.bufferID <0)return;
  std::cerr << padding(p) << name<<"{" << std::endl;
  std::cerr << padding(p) << "  bufferID   = " << a.bufferID  << std::endl;
  std::cerr << padding(p) << "  type       = " << str(a.type) << std::endl;
  std::cerr << padding(p) << "  stride     = " << a.stride    << std::endl;
  std::cerr << padding(p) << "  offset     = " << a.offset    << std::endl;
  std::cerr << padding(p) << "}" << std::endl;
}

void printModel(size_t p,Model const&model){
  std::cerr << padding(p)<<"Model vypadá takto:" << std::endl;
  std::cerr << padding(p)<<"model{" << std::endl;
  std::cerr << padding(p)<<"  meshes  [" << model.meshes  .size() << "]"<< std::endl;
  std::cerr << padding(p)<<"  roots   [" << model.roots   .size() << "]"<< std::endl;
  std::cerr << padding(p)<<"  textures[" << model.textures.size() << "]"<< std::endl;
  std::cerr << padding(p)<<"}" << std::endl;
  for(size_t i=0;i<model.meshes.size();++i){
    auto const&mesh = model.meshes.at(i);
    std::cerr << padding(p) << "mesh" << i << "{" << std::endl;
    std::cerr << padding(p) << "  nofIndices     = " << mesh.nofIndices        << std::endl;
    std::cerr << padding(p) << "  diffuseColor   = " << str(mesh.diffuseColor) << std::endl;
    std::cerr << padding(p) << "  doubleSided    = " << str(mesh.doubleSided)  << std::endl;
    std::cerr << padding(p) << "  diffuseTexture = " << mesh.diffuseTexture    << std::endl;
    std::cerr << padding(p) << "  indexBufferID  = " << mesh.indexBufferID     << std::endl;
    std::cerr << padding(p) << "  indexOffset    = " << mesh.indexOffset       << std::endl;
    std::cerr << padding(p) << "  indexType      = " << str(mesh.indexType)    << std::endl;
    printVertexAttrib("position",mesh.position,8);
    printVertexAttrib("normal"  ,mesh.normal  ,8);
    printVertexAttrib("texCoord",mesh.texCoord,8);
    std::cerr << padding(p) << "  }" << std::endl;
    std::cerr << padding(p) << "}" << std::endl;
  }
  for(size_t i=0;i<model.textures.size();++i){
    auto const&texture = model.textures.at(i);
    std::cerr << padding(p) << "texture" << i << "{" << std::endl;
    std::cerr << padding(p) << "  width    = " << texture.width       << std::endl;
    std::cerr << padding(p) << "  height   = " << texture.height      << std::endl;
    std::cerr << imageToString(p,texture.img) << std::endl;
    std::cerr << padding(p) << "}" << std::endl;
  }
  for(size_t i=0;i<model.roots.size();++i){
    auto const&node = model.roots.at(i);
    std::cerr << padding(p) << "root" << i << "{" << std::endl;
    printNodeContent(node,p);
    std::cerr << padding(p) << "}" << std::endl;
  }
}


bool operator==(VertexAttrib const&a,VertexAttrib const&b){
  if(a.bufferID != b.bufferID)return false;
  if(a.offset != b.offset)return false;
  if(a.stride != b.stride)return false;
  if(a.type != b.type)return false;
  return true;
}



bool breakTest(bool first){
  static int32_t counter = 0;
  if(first)counter = 0;
  bool res = false;

  res = counter == ProgramContext::get().args.testToBreak;

  counter++;

  return res;
}

AllocatedFramebuffer createFramebuffer(uint32_t width,uint32_t height,bool flipped){
  AllocatedFramebuffer res;

  res.colorBacking = std::vector<uint8_t>(width*height*3,0  );
  res.depthBacking = std::vector<float  >(width*height  ,0.f);

  auto&frame = res.frame;
  frame.width     = width  ;
  frame.height    = height ;
  frame.yReversed = flipped;

  auto&c = frame.color;
  c.channels      = 3                          ;
  c.bytesPerPixel = c.channels*sizeof(uint8_t) ;
  c.data          = res.colorBacking.data()    ;
  c.format        = Image::UINT8               ;
  c.pitch         = frame.width*c.bytesPerPixel;

  auto&d = frame.depth;
  d.channels      = 1                          ;
  d.bytesPerPixel = d.channels*sizeof(float)   ;
  d.data          = res.depthBacking.data()    ;
  d.format        = Image::FLOAT32             ;
  d.pitch         = frame.width*d.bytesPerPixel;
  return res;
}

DrawCommandsDump MemCb::getDrawCommandsDump(uint32_t n,GPUMemory&m,CommandBuffer const&c,void(*fce)(GPUMemory&,CommandBuffer const&)){
  commands.clear();
  inFlightMem=&m;
  for(uint32_t i=0;i<n;++i)
    fce(*inFlightMem,c);
  return commands;
}

void MemCb::test(uint32_t n){
  studentMem  = deepCopy(mem);
  expectedMem = deepCopy(mem);
  commands.clear();
  for(uint32_t i=0;i<n;++i)
    izg_enqueue(studentMem.mem,cb);
}


bool MemCb::test(uint32_t n,DumpDiff whatToTest,bool first){
  expectedMem = deepCopy(mem);
  studentMem  = deepCopy(mem);

  expected = getDrawCommandsDump(n,expectedMem.mem,cb,fake_cmd_enqueue);
  student  = getDrawCommandsDump(n,studentMem .mem,cb,izg_enqueue     );

  status = isDrawDumpSame(expected,student,whatToTest);
  if(breakTest(first))status = whatToTest;

  memStatus = isMemDifferent(expectedMem.mem,studentMem.mem);
  if(memStatus.status != MemDiffStatus::SAME){
    status = DumpDiff::DIFFERENT_MEMORY;
  }

  return status == DumpDiff::SAME;
}



}
