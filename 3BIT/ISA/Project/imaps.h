//
// Created by thekr on 11/11/2024.
//

#ifndef ISA_1_IMAPS_H
#define ISA_1_IMAPS_H

#include "imap.h"
#include "openssl/ssl.h"
#include "openssl/err.h"
#include "openssl/bio.h"

class Imaps : public Imap {
    public:
        explicit Imaps(Config *config);
        int connect_server() override;
        void disconnect_server() override;
        int authenticate(const std::string& username, const std::string& password) override;
    private:
        SSL *ssl;
        SSL_CTX *ctx;
        BIO *bio;
        std::string cert_dir;
        std::string cert_file;
        std::string host;
        void send_command(const std::string& command);
        std::string receive_response_1024();
};


#endif //ISA_1_IMAPS_H
