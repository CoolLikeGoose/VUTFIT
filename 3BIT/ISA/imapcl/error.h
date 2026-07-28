//
// Created by thekr on 11/11/2024.
//

#ifndef ISA_1_ERROR_H
#define ISA_1_ERROR_H

#include <string>
#include <iostream>

enum error_code {
    ARGUMENT_ERROR,
    AUTH_DATA_ERROR,
    SERVER_CONNECTION_ERROR,
    MAILBOX_ERROR
};

class Error {
    public:
        Error(error_code err_code);
};

#endif //ISA_1_ERROR_H
