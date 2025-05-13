#include <tests/takeScreenShot.hpp>
#include <tests/renderMethodFrame.hpp>
#include <tests/testCommon.hpp>
#include <framework/application.hpp>

#include <libs/stb_image/stb_image_write.h>

#include <SDL.h>
#include <string>

void takeScreenShot(std::string const&groundTruthFile){

  auto aframe = tests::createFramebuffer(500,500,true);
  auto&frame = aframe.frame;
  renderMethodFrame(aframe.frame);

  stbi_write_png(groundTruthFile.c_str(),frame.width,frame.height,frame.color.channels,frame.color.data,0);

  std::cerr << "storing screenshot to: \"" << groundTruthFile << "\"" << std::endl;
}
