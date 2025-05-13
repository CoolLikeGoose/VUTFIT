#include<tests/gpu/common.hpp>
#define __FILENAME__ "common"
#include<tests/testCommon.hpp>

using namespace tests;

namespace dtl{

void initOutVertices(){
  auto&outVertices = memCb->outVertices;
  outVertices.clear();
  outVertices.resize(3);
  outVertices[0].gl_Position=glm::vec4(-1.f,-1.f,-0.f,1.f);
  outVertices[1].gl_Position=glm::vec4(+1.f,-1.f,-0.f,1.f);
  outVertices[2].gl_Position=glm::vec4(-1.f,+1.f,-0.f,1.f);
}

float area(glm::vec2 const&A,glm::vec2 const&B,glm::vec2 const&C){
  auto a=glm::length(B-A);
  auto b=glm::length(C-B);
  auto c=glm::length(A-C);
  auto s = (a+b+c)/2.f;
  return glm::sqrt(s*glm::abs(s-a)*glm::abs(s-b)*glm::abs(s-c));
}

glm::vec3 barycentrics(OutVertex const&a,OutVertex const&b,OutVertex const&c,glm::vec2 x,uint32_t w,uint32_t h){
  glm::vec4 clip[3]={a.gl_Position,b.gl_Position,c.gl_Position};
  glm::vec3 ndc[3];
  for(int i=0;i<3;++i)ndc[i] = glm::vec3(clip[i])/clip[i].w;
  glm::vec3 wc[3];
  for(int i=0;i<3;++i)wc[i] = glm::vec3((glm::vec2(ndc[i])*.5f+.5f)*glm::vec2(w,h),ndc[i].z);
  float T = area(wc[0],wc[1],wc[2]);
  glm::vec3 l;
  for(int i=0;i<3;++i)l[i] = area(wc[(i+1)%3],wc[(i+2)%3],x)/T;
  return l;
}

glm::vec3 barycentricsPerspective(OutVertex const&a,OutVertex const&b,OutVertex const&c,glm::vec2 x,uint32_t w,uint32_t h){
  auto l = barycentrics(a,b,c,x,w,h);
  glm::vec4 clip[3]={a.gl_Position,b.gl_Position,c.gl_Position};
  for(int i=0;i<3;++i)l[i] /= clip[i].w;
  l /= glm::dot(l,glm::vec3(1.f));
  return l;
}

glm::uvec2 computeNofRasterizedFragments(glm::vec2 const&a,glm::vec2 const&b,glm::vec2 const&c,glm::uvec2 const&res,bool backFaceCulling){
  glm::vec2 P[3]={a,b,c};
  glm::vec2 E[3];for(int i=0;i<3;++i)E[i]=P[(i+1)%3]-P[i];
  glm::vec2 N[3];for(int i=0;i<3;++i)N[i]=glm::vec2(-E[i].y,E[i].x);
  glm::vec3 p[3];for(int i=0;i<3;++i)p[i]=glm::vec3(N[i],-dot(N[i],P[i]));

  if(!backFaceCulling)
    if(glm::dot(p[0],glm::vec3(P[2],1.f))<0.f)
      for(int i=0;i<3;++i)p[i]*=-1.f;

  glm::vec4 border = glm::ivec4(res,0,0);
  for(int i=0;i<3;++i){
    border.x=glm::min(border.x,P[i].x);
    border.y=glm::min(border.y,P[i].y);
    border.z=glm::max(border.z,P[i].x);
    border.w=glm::max(border.w,P[i].y);
  }
  border += glm::vec4(-1,-1,+1,+1);
  border = glm::clamp(border,glm::vec4(0,0,0,0),glm::vec4(res,res));
  auto bb = glm::uvec4(border);

  uint32_t counter=0;
  for(uint32_t y=bb.y;y<bb.w;++y)
    for(uint32_t x=bb.x;x<bb.z;++x){
      auto sample = glm::vec3(glm::vec2(x,y)+0.5f,1.f);
      bool inside = true;
      for(int i=0;i<3;++i)inside &= dot(p[i],sample)>=0.f;
      if(inside)counter++;
    }
  uint32_t allowedErr = (bb.z-bb.x) + 2*(bb.w-bb.y);
  return glm::uvec2(counter,allowedErr);
}

bool operator<(V2 const&a,V2 const&b){
  if(a.data.x < b.data.x)return true;
  if(a.data.x > b.data.x)return false;
  if(a.data.y < b.data.y)return true;
  if(a.data.y > b.data.y)return false;
  return false;
}


bool operator<(UV2 const&a,UV2 const&b){
  if(a.data.x < b.data.x)return true;
  if(a.data.x > b.data.x)return false;
  if(a.data.y < b.data.y)return true;
  if(a.data.y > b.data.y)return false;
  return false;
}

}
