#ifndef ISA_CONFIG_H
#define ISA_CONFIG_H

#include "config.h"
#include "error.h"
#include <iostream>
#include <cstring>

Config::Config(int argc, char **argv) {
    this->parse_args(argc, argv);
}

void help() {
    std::cout << "Exporter SNMP Gauge metrik do OpenTelemetry (OTEL)" << std::endl;
    std::cout << std::endl;
    std::cout << "snmp2otel -t target [-C community] -o oids_file -e endpoint [-i interval] [-r retries] [-T timeout] [-p port] [-v]" << std::endl;
    std::cout << std::endl;
    std::cout << "Options descriptions:" << std::endl;
    std::cout << std::endl;
    std::cout << "Required:" << std::endl;
    std::cout << "  -t target         The IP address or DNS name of the SNMP agent." << std::endl;
    std::cout << "  -o oids_file      File with a list of OIDs to be queried." << std::endl;
    std::cout << "  -e endpoint       URL OTEL endpoint for export (OTLP/HTTP JSON)." << std::endl;
    std::cout << std::endl;
    std::cout << "Optional:" << std::endl;
    std::cout << "  -C community      SNMP v2c community string." << std::endl;
    std::cout << "  -i interval       Polling period in seconds." << std::endl;
    std::cout << "  -r retries        Number of retires in case of timeout." << std::endl;
    std::cout << "  -T timeout        SNMP querry timeout in ms." << std::endl;
    std::cout << "  -p port           UDP port of the SNMP agent." << std::endl;
    std::cout << "  -v                Verbose mode." << std::endl;
    std::cout << std::endl;
    exit(0);
}

void Config::parse_args(int argc, char **argv) {
    if (argc == 1 || strcmp(argv[1], "--help") == 0) {
        help();
    }

    if (argc < 7 || argc > 18) {
        Error Error(error_code::ARGUMENT_ERROR);
    }

    //server name always must be on first place
    // if (process_server_address(argv[1]) != 0) {
    //     Error Error(error_code::ARGUMENT_ERROR);
    // }

    for (int i = 1; i < argc; i++) {
        const std::string arg = argv[i];
        if (arg.length() != 2) {
            Error Error(error_code::ARGUMENT_ERROR);
        }

        switch (arg[1]) {
            case 't':
                if (argc == i+1) Error Error(error_code::ARGUMENT_ERROR);
                this->target = argv[++i];
                break;
            case 'C':
                if (argc == i+1) Error Error(error_code::ARGUMENT_ERROR);
                this->community = argv[++i];
                break;
            case 'o':
                if (argc == i+1) Error Error(error_code::ARGUMENT_ERROR);
                this->oids_file = argv[++i];
                break;
            case 'e':
                if (argc == i+1) Error Error(error_code::ARGUMENT_ERROR);
                this->endpoint = argv[++i];
                break;
            case 'i':
                if (argc == i+1) Error Error(error_code::ARGUMENT_ERROR);
                // this->interval = argv[++i];
                // TODO: ADD check for interval
                break;
            case 'r':
                if (argc == i+1) Error Error(error_code::ARGUMENT_ERROR);
                // this->retries = argv[++i];
                // TODO: ADD check for retries
                break;
            case 'T':
                if (argc == i+1) Error Error(error_code::ARGUMENT_ERROR);
                // this->timeout = argv[++i];
                // TODO: ADD check for timeout
                break;
            case 'p':
                if (argc == i+1) Error Error(error_code::ARGUMENT_ERROR);
                // this->port = argv[++i];
                // TODO: ADD check for port
                break;
            case 'v':
                this->verbose = true;
                break;
            case 'd':
                this->debug = true;
                break;
            default:
                Error Error(error_code::ARGUMENT_ERROR);
        }
    }

    //check if required fields are filled
    if (this->target.empty() || this->oids_file.empty() || this->endpoint.empty()) {
        Error Error(error_code::ARGUMENT_ERROR);
    }

    //TODO: Add check oids file processing
}

// int Config::process_auth_file() {
//     std::ifstream auth_f;
//     auth_f.open(this->auth_file);

//     if (!auth_f.is_open())
//         return -1;

//     std::string line;

//     while (std::getline(auth_f, line)) {
//         std::istringstream iss(line);
//         std::string key, equalSign, value;
//         if (iss >> key >> equalSign >> value) {
//             if (equalSign == "=") {
//                 if (key == "username") {
//                     this->username = value;
//                 } else if (key == "password") {
//                     this->password = value;
//                 }
//             }
//         }
//     }

//     return 0;
// }

// int Config::process_server_address(char *server_name) {
//     hostent *host = gethostbyname(server_name);
//     if (!host) {
//         return -1;
//     }
//     this->server = host->h_name;

//     return 0;
// }

//GETTERS
std::string Config::getTarget() const {
    return this->target;
}

std::string Config::getCommunity() const {
    return this->community;
}

std::string Config::getOidsFile() const {
    return this->oids_file;
}

std::string Config::getEndpoint() const {
    return this->endpoint;
}

int Config::getInterval() const {
    return this->interval;
}

int Config::getRetries() const {
    return this->retries;
}

int Config::getTimeout() const {
    return this->timeout;
}

int Config::getPort() const {
    return this->port;
}

bool Config::isVerbose() const {
    return this->verbose;
}

bool Config::isDebug() const
{
    return this->debug;
}

#endif // ISA_CONFIG_H