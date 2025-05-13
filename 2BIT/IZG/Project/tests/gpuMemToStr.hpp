#pragma once

#include<string>
#include<student/fwd.hpp>

namespace tests{
std::string imageToString(size_t p,Image const&img,uint32_t w = 0,uint32_t h = 0);
std::string gpuMemoryToStr(size_t p,GPUMemory const&mem);

std::string programToString              (size_t p,Program     const&prg,uint32_t id);
std::string programVertexShaderToString  (size_t p,Program     const&prg,uint32_t id);
std::string programFragmentShaderToString(size_t p,Program     const&prg,uint32_t id);
std::string programVS2FSToString         (size_t p,Program     const&prg,uint32_t id);
std::string vertexArrayToString          (size_t p,VertexArray const&vao,uint32_t id);
std::string vertexArrayIndexingToString  (size_t p,VertexArray const&vao,uint32_t id);
std::string vertexArrayAttribsToString   (size_t p,VertexArray const&vao,uint32_t id);
}
