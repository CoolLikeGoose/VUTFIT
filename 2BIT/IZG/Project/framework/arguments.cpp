#include<framework/arguments.hpp>

#ifndef CMAKE_ROOT_DIR
/**
 * location of project
 */
#define CMAKE_ROOT_DIR ".."
#endif

Arguments::Arguments(int argc,char*argv[]){
  args = std::make_shared<argumentViewer::ArgumentViewer>(argc,argv);
  windowSize = args->geti32v("--window-size",{500,500},"size of the window");
  runPerformanceTests = args->isPresent("-p"          ,"runs performance tests");
  runConformanceTests = args->isPresent("-c"          ,"runs conformance tests");
  selectedTest        = args->geti32   ("--test"      ,-1,"run only this selected test");
  takeScreenShot      = args->isPresent("-s"          ,"takes screenshot of app");
  upToTest            = args->isPresent("--up-to-test","run all tests up to selected test by --test argument");
  method              = args->getu32   ("--method"    ,0,"selects a rendering method");
  groundTruthFile     = args->gets     ("-g"          ,std::string(CMAKE_ROOT_DIR)+"/resources/images/output.png"          ,"specify groundTruth image"    );
  modelFile           = args->gets     ("--model"     ,std::string(CMAKE_ROOT_DIR)+"/resources/models/fin.glb"             ,"model file in gltf/glb format");
  imageFile           = args->gets     ("--img"       ,std::string(CMAKE_ROOT_DIR)+"/resources/images/onceAliveNowForgottenAndDead.png","texture file for texturedQuadMethod"                 );
  perfTests           = args->getu32   ("-f"          ,10,"number of frames that are tests during performance tests");
  mseThreshold        = args->getf32   ("--mse"       ,40,"mse threshold for image to image test");
  testToBreak         = args->geti32   ("--breakTest" ,-1,"this will forcefully break test with this number");


  auto printHelp  = args->isPresent("-h"    ,"prints help");
  printHelp |= args->isPresent("--help","prints help");

  if(printHelp || !args->validate()){
    std::cerr << args->toStr() << std::endl;
    stop = true;
  }

}
