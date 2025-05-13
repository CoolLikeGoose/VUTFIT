#include <catch2/catch_test_macros.hpp>

#include <libs/stb_image/stb_image_write.h>

#include <iostream>
#include <string.h>

#include <algorithm>
#include <numeric>

#include <framework/textureData.hpp>
#include <student/gpu.hpp>

#include <tests/conformanceTests.hpp>
#define __FILENAME__ "finalImageTest"
#include <tests/testCommon.hpp>
#include <tests/renderMethodFrame.hpp>

std::string extern groundTruthFile;
float       extern mseThreshold;


using namespace tests;

SCENARIO(TEST_NAME){
  std::cerr << testCounter() << " - image to image comparison" << std::endl;

  auto ref = loadTexture(groundTruthFile);

  if (ref.memBacking.size() == 0) {
    std::cerr << "ERROR: reference image: \"" << groundTruthFile
              << "\" is corrupt!" << std::endl;
    REQUIRE(false);
  }

  auto framebuffer = createFramebuffer(ref.texture.width,ref.texture.height);
  renderMethodFrame(framebuffer.frame);

  float meanSquareError = 0;
  for (uint32_t y = 0; y < ref.texture.height; ++y)
    for (uint32_t x = 0; x < ref.texture.width; ++x){
      auto ucol = getColorU(framebuffer.frame,x,y);
      auto gcol = getColorU(ref.texture,x,y);
      for(uint32_t c=0;c<3;++c){
        float diff = glm::abs((float)ucol[c] - (float)gcol[c]);
        diff *= diff;
        meanSquareError += diff;
      }
    }

  meanSquareError /= (float)(ref.texture.width * ref.texture.height * 3);
  float tol = mseThreshold;

  if(!breakTest()&&meanSquareError < tol)return;

  std::cerr << "  Finální obrázek se moc liší od reference!" << std::endl;
  std::cerr << "  MSE je: "<<meanSquareError<< std::endl;
  std::cerr << "  Akceptovatelná chyba je: "<<tol<<std::endl;
  REQUIRE(false);
}
