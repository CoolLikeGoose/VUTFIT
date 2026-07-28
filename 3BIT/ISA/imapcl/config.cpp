//
// Created by thekr on 11/11/2024.
//

#ifndef ISA_1_CONFIG_H
#define ISA_1_CONFIG_H

#include "config.h"

Config::Config(int argc, char **argv) {
    //basic configuration
    this->port = "143";
    this->encryption = false;
    this->certfile = "";
    this->certaddr = "";
    this->new_only = false;
    this->header_only = false;
    this->mailbox = "INBOX";
    this->parse_args(argc, argv);
}

void help() {
    std::cout << "IMAP client with TLS support" << std::endl;
    std::cout << std::endl;
    std::cout << "Usage: imapcl server [-p port] [-T [-c certfile] [-C certaddr]] [-n] [-h] -a auth_file [-b MAILBOX] -o out_dir" << std::endl;
    std::cout << std::endl;
    std::cout << "Options descriptions:" << std::endl;
    std::cout << std::endl;
    std::cout << "Required:" << std::endl;
    std::cout << "  server            The name or IP address of the target server." << std::endl;
    std::cout << "  -a auth_file      Specifies a file containing authentication credentials (LOGIN command)." << std::endl;
    std::cout << "  -o out_dir        Specifies the output directory where downloaded messages will be saved." << std::endl;
    std::cout << std::endl;
    std::cout << "Optional:" << std::endl;
    std::cout << "  -p port           Specifies the port number on the server." << std::endl;
    std::cout << "  -T                Enables encryption (imaps). If not specified, a non-encrypted variant of the protocol will be used." << std::endl;
    std::cout << "  -c certfile       Specifies a certificate file used to verify the SSL/TLS certificate presented by the server." << std::endl;
    std::cout << "  -C certaddr       Specifies a directory where certificates used to verify the SSL/TLS certificate presented by the server are located. Default is /etc/ssl/certs." << std::endl;
    std::cout << "  -n                Only work with (read) new messages." << std::endl;
    std::cout << "  -h                Download only message headers." << std::endl;
    std::cout << "  -b MAILBOX        Specifies the name of the mailbox to operate on. The default value is INBOX." << std::endl;
    std::cout << std::endl;
    exit(0);
}

int Config::process_auth_file() {
    std::ifstream auth_f;
    auth_f.open(this->auth_file);

    if (!auth_f.is_open())
        return -1;

    std::string line;

    while (std::getline(auth_f, line)) {
        std::istringstream iss(line);
        std::string key, equalSign, value;
        if (iss >> key >> equalSign >> value) {
            if (equalSign == "=") {
                if (key == "username") {
                    this->username = value;
                } else if (key == "password") {
                    this->password = value;
                }
            }
        }
    }

    return 0;
}

int Config::process_server_address(char *server_name) {
    hostent *host = gethostbyname(server_name);
    if (!host) {
        return -1;
    }
    this->server = host->h_name;

    return 0;
}

void Config::parse_args(int argc, char **argv) {
    if (argc == 1 || strcmp(argv[1], "--help") == 0) {
        help();
    }

    if (argc < 6 || argc > 17) {
        Error Error(error_code::ARGUMENT_ERROR);
    }

    //server name always must be on first place
    if (process_server_address(argv[1]) != 0) {
        Error Error(error_code::ARGUMENT_ERROR);
    }

    for (int i = 2; i < argc; i++) {
        const std::string arg = argv[i];
        if (arg.length() != 2) {
            Error Error(error_code::ARGUMENT_ERROR);
        }

        switch (arg[1]) {
            case 'p':
                if (argc == i+1) Error Error(error_code::ARGUMENT_ERROR);
                this->port = argv[++i];
                break;
            case 'T':
                this->encryption = true;
                if (this->port.compare("143") == 0) this->port = "993";
                break;
            case 'c':
                if (argc == i+1 || !this->encryption) Error Error(error_code::ARGUMENT_ERROR);
                this->certfile = argv[++i];
                break;
            case 'C':
                if (argc == i+1 || !this->encryption) Error Error(error_code::ARGUMENT_ERROR);
                this->certaddr = argv[++i];
                break;
            case 'n':
                this->new_only = true;
                break;
            case 'h':
                this->header_only = true;
                break;
            case 'a':
                if (argc == i+1) Error Error(error_code::ARGUMENT_ERROR);
                this->auth_file = argv[++i];
                break;
            case 'b':
                if (argc == i+1) Error Error(error_code::ARGUMENT_ERROR);
                this->mailbox = argv[++i];
                break;
            case 'o':
                if (argc == i+1) Error Error(error_code::ARGUMENT_ERROR);
                this->out_dir = argv[++i];
                break;
            default:
                Error Error(error_code::ARGUMENT_ERROR);
        }
    }

    //check if required fields are filled
    if (this->auth_file.empty() || this->out_dir.empty()) {
        Error Error(error_code::ARGUMENT_ERROR);
    }

    //check auth file
    if (process_auth_file() != 0) {
        Error Error(error_code::AUTH_DATA_ERROR);
    }

    //if -T argument is selected then the user must provide -C or -c
    if (this->encryption && (this->certaddr.empty() && this->certfile.empty())) {
        Error Error(error_code::ARGUMENT_ERROR);
    }
}

//GETTERS
std::string Config::getServer() {
    return this->server;
}

std::string Config::getPort() {
    return this->port;
}

bool Config::isEncrypted() {
    return this->encryption;
}

std::string Config::getCertfile() {
    return this->certfile;
}

std::string Config::getCertaddr() {
    return this->certaddr;
}

bool Config::isNewOnly() {
    return this->new_only;
}

bool Config::isHeadersOnly() {
    return this->header_only;
}

std::string Config::getUsername() {
    return this->username;
}

std::string Config::getPassword() {
    return this->password;
}

std::string Config::getMailbox() {
    return this->mailbox;
}

std::string Config::getOutdir() {
    return this->out_dir;
}


#endif //ISA_1_CONFIG_H