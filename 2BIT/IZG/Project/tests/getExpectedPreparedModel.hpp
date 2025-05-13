#pragma once

#include <tests/testCommon.hpp>

namespace tests::model{

enum DrawCallUniform{
  MODEL        = 0,
  INV_MODEL       ,
  DIFF_COLOR      ,
  TEXTURE_ID      ,
  DOUBLE_SIDED    ,
};

uint32_t const drawCallUniformOffset = 10;
uint32_t const drawCallNofUniforms   = (uint32_t)DOUBLE_SIDED+1;

using PreparedModel = std::shared_ptr<MemCb>;

uint32_t drawCallUniforms(uint32_t drawId);
uint32_t uniformIdToDrawCallUniform(uint32_t uId);
PreparedModel getExpectedPreparedModel(Model const&model);

}
