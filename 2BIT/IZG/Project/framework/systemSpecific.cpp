#include<framework/systemSpecific.hpp>

#ifdef _MSC_VER
#include "windows.h"
#endif

#include<iostream>

void system_init(){
#ifdef _MSC_VER
  // windows specific 
  SetConsoleOutputCP(CP_UTF8);
  setvbuf(stdout, nullptr, _IOFBF, 1000);
#endif
}
