#include "error.h"
#include "imap.h"
#include "imaps.h"

int main(int argc, char *argv[]) {
    Config config(argc, argv);

    //choose connection type
    Imap *connection;
    if (config.isEncrypted()) {
        connection = new Imaps(&config);
    } else {
        connection = new Imap(&config);
    }
    //connect to server
    int connection_result = connection->connect_server();
    if (connection_result != 0) {
        delete connection;
        Error Error((error_code)(connection_result));
    }

    //login
    int login_result = connection->authenticate(config.getUsername(), config.getPassword());
    if (login_result != 0) {
        connection->disconnect_server();
        delete connection;
        Error Error((error_code)(login_result));
    }

    //select mailbox
    int selecting_result = connection->select_mailbox(config.getMailbox());
    if (selecting_result != 0) {
        connection->logout();
        connection->disconnect_server();
        delete connection;
        Error Error((error_code)(login_result+1));
    }

    //main work
    int msg_cnt = connection->get_messages();
    std::cout << "Staženo " + std::to_string(msg_cnt) + " zpráv ze schránky " + config.getMailbox() + "." << std::endl;

    //clean-up
    connection->logout();
    connection->disconnect_server();
    delete connection;
    return 0;
}