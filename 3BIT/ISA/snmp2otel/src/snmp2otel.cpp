#include <iostream>
#include <vector>
#include <cstring>
#include <arpa/inet.h>     // для sockaddr_in, htons и т.д.
#include <unistd.h>        // для close()

// простая функция печати буфера в hex
void print_hex(const std::vector<uint8_t>& data) {
    for (uint8_t byte : data)
        printf("%02X ", byte);
    printf("\n");
}

// кодирует длину для ASN.1 (BER)
void encode_length(std::vector<uint8_t>& buf, size_t length) {
    if (length < 128) {
        buf.push_back(static_cast<uint8_t>(length));
    } else {
        std::vector<uint8_t> tmp;
        while (length > 0) {
            tmp.push_back(static_cast<uint8_t>(length & 0xFF));
            length >>= 8;
        }
        buf.push_back(0x80 | tmp.size());
        for (auto it = tmp.rbegin(); it != tmp.rend(); ++it)
            buf.push_back(*it);
    }
}

// кодирует ASN.1 OID (например 1.3.6.1.2.1.1.3.0)
void encode_oid(std::vector<uint8_t>& buf, const std::string& oid_str) {
    std::vector<uint8_t> oid;
    int a, b;
    const char* s = oid_str.c_str();
    sscanf(s, "%d.%d", &a, &b);
    oid.push_back(40 * a + b);

    const char* p = strchr(s, '.');
    for (int i = 0; p && (p = strchr(p + 1, '.'));)
        ;

    int last_pos = 0;
    std::vector<int> parts;
    std::string tmp;
    for (char c : oid_str) {
        if (c == '.') {
            if (!tmp.empty()) {
                parts.push_back(std::stoi(tmp));
                tmp.clear();
            }
        } else tmp += c;
    }
    if (!tmp.empty()) parts.push_back(std::stoi(tmp));

    for (size_t i = 2; i < parts.size(); ++i) {
        int v = parts[i];
        std::vector<uint8_t> subid;
        do {
            subid.insert(subid.begin(), (v & 0x7F) | (subid.empty() ? 0x00 : 0x80));
            v >>= 7;
        } while (v > 0);
        for (auto b : subid)
            oid.push_back(b);
    }

    buf.push_back(0x06); // OID tag
    encode_length(buf, oid.size());
    buf.insert(buf.end(), oid.begin(), oid.end());
}

int main() {
    const char* community = "public";
    const std::string oid = "1.3.6.1.2.1.1.3.0"; // sysUpTime
    const int port = 16100;

    // SNMPv2c GetRequest (PDU type = 0xA0)
    std::vector<uint8_t> pdu;

    // request-id
    pdu.push_back(0x02); pdu.push_back(0x01); pdu.push_back(0x01);  // INTEGER 1
    // error-status
    pdu.push_back(0x02); pdu.push_back(0x01); pdu.push_back(0x00);
    // error-index
    pdu.push_back(0x02); pdu.push_back(0x01); pdu.push_back(0x00);

    // VarBindList
    std::vector<uint8_t> varbind;
    std::vector<uint8_t> oid_encoded;
    encode_oid(oid_encoded, oid);

    // value = NULL
    varbind.insert(varbind.end(), oid_encoded.begin(), oid_encoded.end());
    varbind.push_back(0x05); varbind.push_back(0x00); // NULL

    // wrap VarBind in sequence
    std::vector<uint8_t> vb_seq = {0x30};
    encode_length(vb_seq, varbind.size());
    vb_seq.insert(vb_seq.end(), varbind.begin(), varbind.end());

    // wrap VarBindList
    std::vector<uint8_t> vbl = {0x30};
    encode_length(vbl, vb_seq.size());
    vbl.insert(vbl.end(), vb_seq.begin(), vb_seq.end());

    // append to PDU
    pdu.insert(pdu.end(), vbl.begin(), vbl.end());

    // wrap full GetRequest PDU
    std::vector<uint8_t> pdu_full = {0xA0};
    encode_length(pdu_full, pdu.size());
    pdu_full.insert(pdu_full.end(), pdu.begin(), pdu.end());

    // SNMP message: version(1), community("public"), PDU
    std::vector<uint8_t> message;
    // version = INTEGER 1 (v2c)
    message.push_back(0x02); message.push_back(0x01); message.push_back(0x01);
    // community
    message.push_back(0x04); message.push_back(strlen(community));
    message.insert(message.end(), community, community + strlen(community));
    // append PDU
    message.insert(message.end(), pdu_full.begin(), pdu_full.end());

    // outer sequence
    std::vector<uint8_t> final = {0x30};
    encode_length(final, message.size());
    final.insert(final.end(), message.begin(), message.end());

    // print packet for debug
    std::cout << "Sending SNMP GET request:\n";
    print_hex(final);

    // create UDP socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // send
    sendto(sock, final.data(), final.size(), 0, (sockaddr*)&addr, sizeof(addr));

    // receive
    uint8_t buffer[1024];
    socklen_t addrlen = sizeof(addr);
    ssize_t n = recvfrom(sock, buffer, sizeof(buffer), 0, (sockaddr*)&addr, &addrlen);
    if (n > 0) {
        std::cout << "\nReceived response (" << n << " bytes):\n";
        for (ssize_t i = 0; i < n; ++i) printf("%02X ", buffer[i]);
        printf("\n");
    } else {
        perror("recvfrom");
    }

    close(sock);
    return 0;
}
