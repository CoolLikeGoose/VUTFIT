#ifndef ISA_ERROR_H
#define ISA_ERROR_H

#include <string>
#include <iostream>

enum error_code {
    ARGUMENT_ERROR
};

class Error {
    public:
        Error(error_code err_code);
};

#endif //ISA_ERROR_H