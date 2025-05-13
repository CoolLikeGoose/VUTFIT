#pragma once

#include<student/fwd.hpp>

namespace tests{

enum class MemDiffStatus{
  ACTIVATED_FRAMEBUFFER = 0,
  ACTIVATED_PROGRAM        ,
  ACTIVATED_VERTEXARRAY    ,
  DRAWID                   ,
  BUFFER_SIZE              ,
  BUFFER_HAS_DATA          ,
  BUFFER_DATA              ,
  UNIFORM                  ,
  PROGRAM_VS               ,
  PROGRAM_FS               ,
  PROGRAM_VS2FS            ,
  VAO_HAS_INDEXING         ,
  VAO_INDEX_OFFSET         ,
  VAO_INDEX_TYPE           ,
  VAO_ATT_BUFFER           ,
  VAO_ATT_OFFSET           ,
  VAO_ATT_STRIDE           ,
  VAO_ATT_TYPE             ,
  TEX_WIDTH                ,
  TEX_HEIGHT               ,
  TEX_BYTESPERPIX          ,
  TEX_CHANNELS             ,
  TEX_FORMAT               ,
  TEX_PITCH                ,
  TEX_CHANNELTYPES         ,
  TEX_HAS_DATA             ,
  TEX_DATA                 ,
  FBO_WIDTH                ,
  FBO_HEIGHT               ,
  FBO_YREVERSED            ,
  FBO_COLOR_BYTESPERPIX    ,
  FBO_COLOR_CHANNELS       ,
  FBO_COLOR_FORMAT         ,
  FBO_COLOR_PITCH          ,
  FBO_COLOR_CHANNELTYPES   ,
  FBO_COLOR_HAS_DATA       ,
  FBO_COLOR_DATA           ,
  FBO_DEPTH_BYTESPERPIX    ,
  FBO_DEPTH_CHANNELS       ,
  FBO_DEPTH_FORMAT         ,
  FBO_DEPTH_PITCH          ,
  FBO_DEPTH_CHANNELTYPES   ,
  FBO_DEPTH_HAS_DATA       ,
  FBO_DEPTH_DATA           ,
  SAME                     ,
};

struct MemDiff{
  MemDiffStatus status = MemDiffStatus::SAME;
  uint32_t      id     = 0                  ;
  MemDiff(){};
  MemDiff(MemDiffStatus s):status(s){};
  MemDiff(MemDiffStatus s,uint32_t i):status(s),id(i){}
};

MemDiff isMemDifferent(GPUMemory const&a,GPUMemory const&b);

}
