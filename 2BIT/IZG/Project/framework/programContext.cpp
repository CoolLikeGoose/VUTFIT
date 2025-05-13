#include<framework/programContext.hpp>

#include <iomanip>

ProgramContext&ProgramContext::get(){
  if(!reg)reg = new ProgramContext();
  return *reg;
}

ProgramContext::ProgramContext(){}
ProgramContext::~ProgramContext(){
  delete reg;
}

ProgramContext*ProgramContext::reg = nullptr;


std::string methodCounter(){
  static int methodCounter=0;
  std::stringstream ss;
  ss << "izg" << std::setfill('0') << std::setw(2) << methodCounter;

  methodCounter++;
  return ss.str();
}

