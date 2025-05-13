#pragma once

#include<glm/glm.hpp>
#include<student/fwd.hpp>

namespace dtl{

void initOutVertices();
float area(glm::vec2 const&A,glm::vec2 const&B,glm::vec2 const&C);
glm::vec3 barycentrics(OutVertex const&a,OutVertex const&b,OutVertex const&c,glm::vec2 x,uint32_t w,uint32_t h);
glm::vec3 barycentricsPerspective(OutVertex const&a,OutVertex const&b,OutVertex const&c,glm::vec2 x,uint32_t w,uint32_t h);
glm::uvec2 computeNofRasterizedFragments(glm::vec2 const&a,glm::vec2 const&b,glm::vec2 const&c,glm::uvec2 const&res,bool backFaceCulling);

struct V2{
  V2(){}
  V2(glm::vec2 const&a):data(a){}
  glm::vec2 data = glm::vec2(0.f);
};

bool operator<(V2 const&a,V2 const&b);

struct UV2{
  UV2(){}
  UV2(glm::uvec2 const&a):data(a){}
  glm::uvec2 data = glm::uvec2(0);
};

bool operator<(UV2 const&a,UV2 const&b);

}
