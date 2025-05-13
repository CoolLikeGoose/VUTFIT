#include "catch2/catch_config.hpp"
#include "catch2/catch_test_case_info.hpp"
#include "catch2/interfaces/catch_interfaces_config.hpp"
#include "catch2/internal/catch_context.hpp"
#include "catch2/internal/catch_stdstreams.hpp"
#include <catch2/catch_session.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>

#include <tests/conformanceTests.hpp>

//#define CATCH_CONFIG_RUNNER
//#include <tests/catch.hpp>

#include <catch2/catch_test_macros.hpp>

std::string groundTruthFile;
std::string modelFile      ;
float       mseThreshold   ;

void runConformanceTests(std::string const&groundTruth,std::string const&model,float mse,int test,bool upTo) {
  groundTruthFile = groundTruth;
  modelFile       = model      ;
  mseThreshold    = mse        ;
  //int         argc   = 1;
  //char const* argv[1] = {"test"};


#if 1
  Catch::Config cfg;
  auto const&tests = Catch::getAllTestCasesSorted(cfg);
  auto nofTests = tests.size();
  std::vector<std::string>testNames;
  for(size_t i=0;i<nofTests;++i)
    testNames.push_back(tests[i].getTestCaseInfo().name);

  std::vector<char const*>argv;
  std::vector<std::string>argvs;
  argvs.push_back("test");

  auto insertTest = [&](size_t i){
    std::stringstream ss;
    ss <<  testNames.at(i) << ",";
    argvs.push_back(ss.str().c_str());
  };

  if(test>=0&&(size_t)test<nofTests){
    if(upTo){
      for(size_t i=0;i<=(size_t)test;++i)
        insertTest(i);
    }else{
      insertTest(test);
    }
  }else{
    for(size_t i=0;i<nofTests;++i)
      insertTest(i);
  }

  //Catch::Session session;


  for(auto const&s:argvs)argv.push_back(s.c_str());
  int result = Catch::Session().run((int)argv.size(), argv.data());

  size_t maxPoints = 20;
  std::cout << std::fixed << std::setprecision(1) << maxPoints * (float)(nofTests-result)/(float)nofTests << std::endl;

  //if(test>=0 && test < (int)nofTests){
  //  if(upTo){
  //    for(int i=0;i<=test;++i){
  //      //try{
  //        tests.at(i).invoke();
  //      //}catch(std::exception&){}
  //    }
  //  }else{


  //    

  //    //argv.push_back("--list-test-names-only");
  //    //argv.push_back("\"Scenario: 00\"");
  //    //
  //    //std::stringstream ss;
  //    //ss << "\"Scenario: 00\"";
  //    //argv.push_back(ss.str().c_str());
  //    for(auto const&a:argv)std::cerr << "#" << a << "#" << std::endl;

  //    //tests.at(test).invoke();
  //  }
  //  return;
  //}


  //int  result = Catch::Session().run(argc, argv);
 
#endif
}
