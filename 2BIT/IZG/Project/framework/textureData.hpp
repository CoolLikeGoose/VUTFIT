#pragma once

#include<vector>
#include<cstdint>
#include<string>
#include<student/fwd.hpp>

class TextureData{
  public:
    std::vector<uint8_t>memBacking;
    Texture texture;
    TextureData(){}
    TextureData(
        uint32_t w,
        uint32_t h,
        uint32_t c,
        Image::Format f = Image::UINT8);
    Texture getTexture(){
      return texture;
    }
};

TextureData loadTexture(std::string const&fileName);
