#pragma once

#include<cstdint>
#include<vector>

#include<glm/glm.hpp>

#include<student/fwd.hpp>

namespace tests::model{

struct MeshI{
  MeshI(
      uint32_t        n = 0             ,
      glm::vec4 const&c = glm::vec4(1.f),
      int             t = -1            ,
      bool            d = false         ):
    nofIndices(n),diffuseColor(c),diffuseTexture(t),doubleSided(d){}
  uint32_t  nofIndices     = 0;
  glm::vec4 diffuseColor   = glm::vec4(1.f);
  int       diffuseTexture = -1;
  bool      doubleSided    = false;
};

struct NodeI{
  NodeI(
      int32_t                 m     = -1,
      std::vector<NodeI>const&c     = {},
      glm::mat4         const&model = glm::mat4(1.f));
  int32_t           mesh     = -1            ;
  std::vector<NodeI>children                 ;
  glm::mat4         model    = glm::mat4(1.f);
};

Model createModel(std::vector<MeshI>const&meshes,std::vector<NodeI>const&roots);

}
