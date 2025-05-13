#include<framework/colorPrinting.hpp>

#include <iostream>
#include <string>
#include <sstream>


#if defined(_MSC_VER) || defined(__MINGW32__)
#include <windows.h>
#include <wincon.h>

void print(std::string const&t,Color c){
  CONSOLE_SCREEN_BUFFER_INFO info;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
  auto ret = info.wAttributes;
  auto col = ret;
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  switch(c){
    case Color::GRAY    :col = 8  ;break;
    case Color::RED     :col = 4  ;break;
    case Color::BLUE    :col = 9  ;break;
    case Color::STANDARD:col = ret;break;
  }

  SetConsoleTextAttribute(hConsole, col);
  std::cerr << t;
  SetConsoleTextAttribute(hConsole, ret);
}
#elif defined(__linux__)
void print(std::string const&t,Color c){

  int col = 0;
  switch(c){
    case Color::GRAY     :col = 90;break;
    case Color::RED      :col = 31;break;
    case Color::STANDARD :col = 0 ;break;
    case Color::BLUE     :col = 34;break;
  }

  std::cerr << "\033[1;"<<col<<"m";
  std::cerr << t;
  std::cerr << "\033[0m";
}
#else
void print(std::string const&t,Color c){
  std::cerr << t;
}
#endif
