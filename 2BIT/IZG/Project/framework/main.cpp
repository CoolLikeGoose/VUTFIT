#include <assert.h>

#include<framework/window.hpp>
#include<framework/application.hpp>
#include<framework/arguments.hpp>
#include<framework/systemSpecific.hpp>
#include<tests/conformanceTests.hpp>
#include<tests/performanceTest.hpp>
#include<tests/takeScreenShot.hpp>

int main(int argc,char*argv[]){
  system_init();

  try{
    auto&args = ProgramContext::get().args = Arguments(argc,argv);
    if(args.stop)
      return 0;

    if(args.runConformanceTests){
      runConformanceTests(args.groundTruthFile,args.modelFile,args.mseThreshold,args.selectedTest,args.upToTest);
      return 0;
    }

    if(args.runPerformanceTests){
      runPerformanceTest(args.perfTests);
      return 0;
    }

    if(args.takeScreenShot){
      takeScreenShot(args.groundTruthFile);
      return 0;
    }

    auto app = Application(args.windowSize[0],args.windowSize[1]);
    app.setMethod(args.method);
    app.start();

  }catch(std::exception&e){
    std::cerr << e.what() << std::endl;
  }

  return EXIT_SUCCESS;
}

