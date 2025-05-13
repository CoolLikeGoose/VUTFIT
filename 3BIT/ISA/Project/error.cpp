//
// Created by thekr on 11/11/2024.
//

#include "error.h"

Error::Error(error_code err_code) {
    std::string error_msg;

    switch (err_code) {
        case error_code::ARGUMENT_ERROR:
            error_msg = "Program started with wrong arguments. Try help (--help) for more information";
            break;
        case error_code::AUTH_DATA_ERROR:
            error_msg = "Auth file must contain login and password in the following format:\nLOGIN\nPASSWORD";
            break;
        case error_code::SERVER_CONNECTION_ERROR:
            error_msg = "Failed while connecting to the server";
            break;
        case error_code::MAILBOX_ERROR:
            error_msg = "Mailbox doesn't exist";
            break;
    }

    std::cerr << error_msg << std::endl;
    exit((int)err_code);
}