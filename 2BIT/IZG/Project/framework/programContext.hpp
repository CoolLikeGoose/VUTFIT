#pragma once

#include<functional>
#include<memory>
#include<sstream>

#include<framework/method.hpp>
#include<framework/arguments.hpp>

template<typename CLASS>
void registerMethod(std::string const&name,std::shared_ptr<MethodConstructionData>const&mcd = nullptr);

class MethodDatabase{
  private:
    using MethodFactory = std::function<std::shared_ptr<Method>(GPUMemory&,MethodConstructionData const*)>;
    std::vector<MethodFactory>     methodFactories                              ;
    std::vector<std::string>       methodName                                   ;
    std::vector<std::shared_ptr<MethodConstructionData>> methodConstructData    ;
    size_t                         selectedMethod    = 0                        ;
    std::shared_ptr<Method>        method                                       ;

    template<typename CLASS>
    friend
    void registerMethod(std::string const&name,std::shared_ptr<MethodConstructionData>const&mcd);
    friend class Application;
};

class ProgramContext{
  public:
    static ProgramContext&get();
    ProgramContext();
    ~ProgramContext();

    Arguments args;
    MethodDatabase methods;
  protected:
    ProgramContext(ProgramContext &other) = delete;
    void operator=(const ProgramContext &) = delete;
    static ProgramContext*reg;
};

std::string methodCounter();

template<typename CLASS>
void registerMethod(std::string const&name,std::shared_ptr<MethodConstructionData>const&mcd){
 
  auto finalName = methodCounter() + " - " + name;
  
  auto factory = [&](GPUMemory&me,MethodConstructionData const*m){
    return std::make_shared<CLASS>(me,m);
  };
  
  auto&mr = ProgramContext::get().methods;
  mr.methodFactories.push_back(factory);
  mr.methodName.push_back(finalName);
  mr.methodConstructData.push_back(mcd);
}




struct EntryPoint{
  template<typename T>
  EntryPoint(T t){
    t();
  }
};
