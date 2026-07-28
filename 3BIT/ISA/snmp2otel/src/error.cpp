#include "error.h"

Error::Error(error_code err_code) {
    std::string error_msg;

    switch (err_code) {
        case error_code::ARGUMENT_ERROR:
            error_msg = "Program started with wrong arguments. Try help (--help) for more information";
            break;
    }

    std::cerr << error_msg << std::endl;
    exit((int)err_code);
}