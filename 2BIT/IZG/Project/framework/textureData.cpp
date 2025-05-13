#include<framework/textureData.hpp>

#include<libs/stb_image/stb_image.h>

#include <iostream>


TextureData::TextureData(
    uint32_t w,
    uint32_t h,
    uint32_t c,
    Image::Format f){
  uint32_t formatSize = 0;
  switch(f){
    case Image::UINT8  :formatSize = sizeof(uint8_t);break;
    case Image::FLOAT32:formatSize = sizeof(float  );break;
  }
  memBacking.resize((size_t)w*h*c*formatSize,0);
  texture.width  = w;
  texture.height = h;
  texture.img.bytesPerPixel = c*formatSize               ;
  texture.img.channels      = c                          ;
  texture.img.data          = memBacking.data()          ;
  texture.img.format        = f                          ;
  texture.img.pitch         = w*texture.img.bytesPerPixel;
}

TextureData loadTexture(std::string const&fileName){
  TextureData res;

  int32_t w,h,channels;
  uint8_t* data = stbi_load(fileName.c_str(),&w,&h,&channels,0);
  if(!data){
    std::cerr << "Cannot open image file: "<<fileName<<std::endl;
    return res;
  }
  res.memBacking.resize(w*h*channels);

  for(int32_t y=0;y<h;++y)
    for(int32_t x=0;x<w;++x)
      for(int32_t c=0;c<channels;++c){
        res.memBacking[(y*w+x)*channels+c] = data[((h-y-1)*w+x)*channels+c];
      }

  res.texture.height = h;
  res.texture.width  = w;
  res.texture.img.bytesPerPixel = sizeof(uint8_t)*channels       ;
  res.texture.img.channels      = channels                       ;
  res.texture.img.data          = res.memBacking.data()          ;
  res.texture.img.format        = Image::UINT8                   ;
  res.texture.img.pitch         = w*res.texture.img.bytesPerPixel;

  stbi_image_free(data);
  return res;
}
