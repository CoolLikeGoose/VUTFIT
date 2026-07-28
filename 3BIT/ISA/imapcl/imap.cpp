//
// Created by thekr on 11/11/2024.
//

#include "imap.h"

Imap::Imap(Config *config) {
    this->server = config->getServer();
    this->port = config->getPort();
    this->command_id = 1;
    this->msg_filter = config->isNewOnly() ? "UNSEEN" : "ALL";
    this->out_dir = config->getOutdir();
    this->headers = config->isHeadersOnly();
}

int Imap::connect_server() {
    int sfd, s;
    struct addrinfo hints{};
    struct addrinfo *result, *rp;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    s = getaddrinfo(this->server.c_str(), this->port.c_str(), &hints, &result);
    if (s != 0) {
        return error_code::SERVER_CONNECTION_ERROR;
    }

    for (rp = result; rp != nullptr; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
            continue;

        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;

        close(sfd);
    }

    freeaddrinfo(result);

    if (rp == nullptr) {
        return error_code::SERVER_CONNECTION_ERROR;
    }

    this->sockfd = sfd;

    std::string response = receive_response_1024();
    return response.find("OK") == std::string::npos;
}

void Imap::disconnect_server() {
    close(this->sockfd);
}

int Imap::authenticate(const std::string& username, const std::string& password) {
    send_command("LOGIN " + username + " " + password);
    std::string response = receive_response_1024();
//    std::cout << response << std::endl;
    return response.find("OK") == std::string::npos;
}

int Imap::logout() {
    send_command("LOGOUT");
    std::string response = receive_response_1024();
    return response.find("OK") == std::string::npos;
}

void Imap::send_command(const std::string &command) {
    std::string cmd = this->get_command_iterator() + command + "\r\n";
//    std::cout << ">>>SERVER CMD:\t" + cmd << std::endl;
    ::send(this->sockfd, cmd.c_str(), cmd.size(), 0);
}

std::string Imap::receive_response_1024() {
    char buffer[1024];
    int bytes = ::recv(this->sockfd, buffer, sizeof buffer - 1, 0);
    if (bytes > 0) {
        buffer[bytes] = '\0';
//        std::cout << "<<<SERVER RESPONSE:\t" + std::string(buffer) << std::endl;
        return std::string(buffer);
    }
    return "";
}

std::string Imap::receive_response() {
    std::string msg, res;

    while(true) {
        res = this->receive_response_1024();
        size_t end = res.find("OK");

        msg.append(res);
        if(end != std::string::npos) {
            break;
        }
    }
    return msg;
}


std::string Imap::get_command_iterator() {
    return "a"+std::to_string(this->command_id++) + " ";
}

int Imap::select_mailbox(const std::string &mailbox) {
    send_command("SELECT " + mailbox);
    std::string response = receive_response_1024();
    return response.find("OK") == std::string::npos;
}

std::vector<std::string> Imap::parseIDs(const std::string& response) {
    std::vector<std::string> ids;
    std::istringstream stream(response);
    std::string word;

    while (stream >> word) {
        if (word == "*") {
            stream >> word;
            if (word == "SEARCH") {
                std::string id;
                while (stream >> id) {
                    if (id.compare("a3") == 0) break;
                    ids.push_back(id);
                }
            }
        }
    }

    return ids;
}

std::string Imap::getMessage(const std::string &uid) {
    std::string msg;

    this->send_command("UID FETCH " + uid + " BODY[HEADER]");

    return this->receive_response();
}

int Imap::saveMessage(const std::string &msg, const std::string &id) {
    std::string file_path = "./" + this->out_dir + "/msg_" + id;
    std::ofstream msg_file(file_path);

    if (!msg_file.is_open()) {
        //TODO: return error later instead of printing
        std::cerr << "Error: Unable to open file for writing: " << file_path << std::endl;
        return -1;
    }

    msg_file << msg;
    msg_file.close();
    return 0;
}

int Imap::get_messages() {
    this->send_command("UID SEARCH " + this->msg_filter);
    std::string uids = this->receive_response();

    std::vector<std::string> ids = parseIDs(uids);

    std::string msg;
    for (std::string id : ids) {
        msg = this->getMessage(id);
        this->saveMessage(this->formatMsg(msg, id), id);
    }


    return ids.size();
}

std::string parseHeader(const std::string& message) {
    std::string result;
    std::regex header_regex(R"((^|\r\n)(Date: .*|From: .*|To: .*|Subject: .*|Message-ID: .*))", std::regex::icase);
    std::smatch match;

    auto start = message.cbegin();
    auto end = message.cend();

    while (std::regex_search(start, end, match, header_regex)) {
        result += match[2].str() + "\n";
        start = match.suffix().first;
    }

    return result;
}

std::string parseBody(std::string message) {
    size_t fetch_end = message.find("OK FETCH completed");
    if (fetch_end != std::string::npos) {
        message = message.substr(0, fetch_end);
    }

    size_t first_line_end = message.find("\r\n");
    if (first_line_end != std::string::npos) {
        message.erase(0, first_line_end + 2);
    }

    if (!message.empty() && message.back() == ')') {
        message.pop_back();
    }

    return message;
}

std::string Imap::formatMsg(const std::string &msg, const std::string &id) {
    std::string headers = parseHeader(msg);
    if (this->headers) {
        return headers;
    }

    this->send_command("UID FETCH " + id + " BODY[TEXT]");
    std::string body = this->receive_response();
    body = parseBody(body);

    return headers + body;
}
