#include <string>
#include "cstring"
#include "fstream"
#include "error.h"
#include "netdb.h"
#include "sstream"

class Config {
    public:
        Config(int argc, char **argv);
        std::string getServer();
        std::string getPort();
        bool isEncrypted();
        std::string getCertfile();
        std::string getCertaddr();
        bool isNewOnly();
        bool isHeadersOnly();
        std::string getUsername();
        std::string getPassword();
        std::string getMailbox();
        std::string getOutdir();
    
    private:
        std::string server;
        std::string port;
        bool encryption;
        std::string certfile;
        std::string certaddr;
        bool new_only;
        bool header_only;
        std::string auth_file;
        std::string username;
        std::string password;
        std::string mailbox;
        std::string out_dir;
        int process_auth_file();
        int process_server_address(char *server_name);
        void parse_args(int argc, char **argv);
};