#include <string>

class Config {
    public:
        Config(int argc, char **argv);

        std::string getTarget() const;
        std::string getCommunity() const;
        std::string getOidsFile() const;
        std::string getEndpoint() const;
        int getInterval() const;
        int getRetries() const;
        int getTimeout() const;
        int getPort() const;
        bool isVerbose() const;
        bool isDebug() const;
    
    private:
        void parse_args(int argc, char **argv);

        std::string target;
        std::string community = "public";
        std::string oids_file;
        std::string endpoint;
        std::string out_dir;
        int interval = 10;
        int retries = 2;
        int timeout = 1000;
        int port = 161;
        bool verbose = false;
        bool debug = false;
};