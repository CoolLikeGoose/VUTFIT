//
// Created by thekr on 11/11/2024.
//

#include "imaps.h"

Imaps::Imaps(Config *config) : Imap(config) {
    OpenSSL_add_ssl_algorithms();
    SSL_load_error_strings();

    this->cert_dir = config->getCertaddr();
    this->cert_file = config->getCertfile();
    this->host = config->getServer() + ":" + config->getPort();
}

int Imaps::connect_server() {
    this->ctx = SSL_CTX_new(SSLv23_client_method());
    if (!this->ctx) {
        return error_code::SERVER_CONNECTION_ERROR;
    }

    if (!this->cert_dir.empty()) {
        if (!SSL_CTX_load_verify_dir(this->ctx, this->cert_dir.c_str())) {
            return error_code::SERVER_CONNECTION_ERROR;
        }
    } else if (!this->cert_file.empty()) {
        if (!SSL_CTX_load_verify_file(this->ctx, this->cert_file.c_str())) {
            return error_code::SERVER_CONNECTION_ERROR;
        }
    } else {
        return error_code::SERVER_CONNECTION_ERROR;
    }

    this->bio = BIO_new_ssl_connect(this->ctx);
    BIO_get_ssl(this->bio, this->ssl);
    SSL_set_mode(this->ssl, SSL_MODE_AUTO_RETRY);
    BIO_set_conn_hostname(this->bio, this->host.c_str());

    if (BIO_do_connect(this->bio) <= 0)
        return error_code::SERVER_CONNECTION_ERROR;

    std::string response = receive_response_1024();
    return response.find("OK") == std::string::npos;
}

void Imaps::disconnect_server() {
    SSL_shutdown(this->ssl);
    SSL_free(this->ssl);
    SSL_CTX_free(this->ctx);
    BIO_free_all(this->bio);
    EVP_cleanup();
}

void Imaps::send_command(const std::string &command) {
    std::string cmd = this->get_command_iterator() + command + "\r\n";
    std::cout << ">>>SERVER CMD:\t" + cmd << std::endl;
    BIO_write(this->bio, cmd.c_str(), cmd.size());
}

std::string Imaps::receive_response_1024() {
    char buffer[1024];

    int res = BIO_read(this->bio, buffer, sizeof buffer);
    if (res == 0) {
        std::cout << "<<<SERVER RESPONSE:\t" + std::string(buffer) << std::endl;
        return std::string(buffer);
    }
    return "";
}

int Imaps::authenticate(const std::string &username, const std::string &password) {
    return 0;
}

