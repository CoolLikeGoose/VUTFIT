#include "logger.h"

#include <iostream>
#include <chrono>
// #include <sstream>
// #include <iomanip>

bool Logger::isVerbose = false;
bool Logger::isDebug = false;

void Logger::init(bool verbose, bool debug)
{
    Logger::isVerbose = verbose;
    Logger::isDebug = debug;
}

void Logger::debug(const std::string &msg)
{
    if (!isDebug) return;
    std::cout << timestamp() << "\t [DEBUG]\t" << msg << std::endl;
}

void Logger::log(const std::string &msg)
{
    if (!isVerbose) return;
    std::cout << timestamp() << "\t  [LOG]\t" << msg << std::endl;
}

void Logger::warning(const std::string &msg)
{
    if (!isVerbose) return;
    std::cerr << timestamp() << "\t[WARNING]\t" << msg << std::endl;
}

std::string Logger::timestamp()
{
    auto now = std::chrono::system_clock::now();
    auto t = std::chrono::system_clock::to_time_t(now);
    // std::stringstream ss;
    // ss << std::put_tim
    return std::string();
}
