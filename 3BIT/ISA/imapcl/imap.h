//
// Created by thekr on 11/11/2024.
//

#ifndef ISA_1_IMAP_H
#define ISA_1_IMAP_H


#include <string>
#include "error.h"
#include "config.h"
#include "cstring"
#include "sys/socket.h"
#include "unistd.h"
#include "regex"
#include "vector"

class Imap {
    public:
        Imap(Config *config);
        virtual ~Imap() = default;
        virtual int connect_server();
        virtual void disconnect_server();
        virtual int authenticate(const std::string& username, const std::string& password);
        virtual int get_messages();
        virtual int logout();
        int select_mailbox(const std::string& mailbox);
    private:
        std::string server;
        std::string port;
        int sockfd;
        void send_command(const std::string& command);
        std::string receive_response_1024();
        std::string receive_response();
        std::vector<std::string> parseIDs(const std::string& response);
        std::string getMessage(const std::string& uid);
        int saveMessage(const std::string& msg, const std::string &id);
        std::string formatMsg(const std::string& msg, const std::string &id);
    protected:
        int command_id;
        bool headers;
        std::string out_dir;
        std::string msg_filter;
        std::string get_command_iterator();
};


#endif //ISA_1_IMAP_H
