#pragma once

#include<cstdint>
#include<student/fwd.hpp>
#include<student/prepareModel.hpp>
#include<tests/testCommon.hpp>
#include<tests/getExpectedPreparedModel.hpp>
#include<tests/createModel.hpp>
#include<string>
#include<memory>

namespace tests::model{

enum class Diff{
  NOF_COMMANDS,
  COMMAND_TYPE,
  CLEAR,
  DRAW_BACKFACE,
  DRAW_NOF_VERTICES,
  PROGRAMID,
  BIND_FBO_SIZE,
  BIND_FBO_YREVERSED,
  BIND_PRG_VERTEX_SHADER,
  BIND_PRG_FRAGMENT_SHADER,
  BIND_PRG_VS2FS,
  SET_DRAW_ID,
  BIND_VAO_INDEXING,
  BIND_VAO_ATTRIB,
  SUB_COMMAND,

  SHADERS,
  VS2FS,
  BUFFERS,
  TEXTURES,

  TEXTURE_ID,
  DOUBLE_SIDED,
  DIFFUSE_COLOR,
  MODEL_MATRIX,
  INV_MATRIX,

  SAME,
};

struct Check{
  PreparedModel expected;
  PreparedModel student ;
  Diff          status  ;
};

Check checkTranslation(Model const&model,Diff const&whatToTest);

std::string translatedModelErrors(
    size_t      p,
    Check const&check);

void checkTraverse(std::vector<MeshI>const&meshes,std::vector<NodeI>const&roots);

void checkModelMemory(Model const&model,Diff const&level = Diff::INV_MATRIX);

}
