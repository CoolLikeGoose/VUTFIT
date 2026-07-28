#ifndef ISA_LOGGER_H
#define ISA_LOGGER_H

#include <string>

class Logger {
public:
    /// @brief Initialize logger with verbosity and debug settings
    /// @param verbose show log and warning messages
    /// @param debug show debug messages
    static void init(bool verbose, bool debug);
    static void debug(const std::string& msg);
    static void log(const std::string& msg);
    static void warning(const std::string& msg);

private:
    static bool isVerbose;
    static bool isDebug;
    static std::string timestamp();
};

#endif // ISA_LOGGER_H