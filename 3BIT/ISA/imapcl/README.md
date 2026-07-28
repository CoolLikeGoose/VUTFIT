# IMAP Client (imapcl) WIP

## Description
`imapcl` is a simple IMAP client that allows reading emails via the IMAP4rev1 protocol (RFC 3501). Upon execution, the program connects to the specified mail server, retrieves messages, and saves them individually in the specified directory. The program also outputs the number of downloaded messages to the standard output.

The program can be customized through various command-line parameters. It supports both secure and non-secure IMAP connections.

The main code is implemented in `imaps.cpp` for secure IMAPS connections and `imap.cpp` for non-secure IMAP connections. Configuration handling and argument parsing is done in `config.cpp`.

## Usage

### Command Line

imapcl server [-p port] [-T [-c certfile] [-C certaddr]] [-n] [-h] -a auth_file [-b MAILBOX] -o out_dir


### Parameters

- `server`: (Required) The server address (IP or domain name).
- `-p port`: (Optional) The port to connect to on the server.
- `-T`: (Optional) Enables SSL/TLS encryption (IMAPS). If this parameter is not used, a non-secure connection will be established.
- `-c certfile`: (Optional) Specifies a certificate file used for validating the server's SSL/TLS certificate.
- `-C certaddr`: (Optional) Specifies the directory to search for SSL/TLS certificates. Default is `/etc/ssl/certs`.
- `-n`: (Optional) Only fetch new messages (unseen messages).
- `-h`: (Optional) Only fetch message headers.
- `-a auth_file`: (Required) Specifies the authentication file containing the username and password.
- `-b MAILBOX`: (Optional) Specifies the mailbox to fetch messages from. Default is `INBOX`.
- `-o out_dir`: (Required) Specifies the directory to save the downloaded messages.

### Authentication File Format

The authentication file (`auth_file`) contains the username and password in the following format:
username = jmeno
password = heslo

Each line represents a key-value pair, and the file should be in a standard Unix text file format, ending with a newline character.

### Output

Upon execution, the application will display the number of downloaded messages. The message text should not exceed one line and should be adjusted based on the `-n` and `-h` parameters.

Each email is saved as an individual file in the specified output directory (`-o out_dir`) in the Internet Message Format (RFC 5322).

### Main Files

- `imaps.cpp`: Contains code for handling secure IMAPS connections.
- `imap.cpp`: Contains code for handling non-secure IMAP connections.
- `config.cpp`: Handles command-line argument parsing and configuration.

## Known Errors

- The secure IMAPS connection (`-T` parameter) does not currently work.
- Some messages may not be formatted correctly.
