//Criada por IA

#ifndef PING_H
#define PING_H

#include <cstdint>
#include <vector>
#include <string>
#include <memory>

// -------------------- ICMP Message --------------------
class IcmpMessage {
public:
    IcmpMessage(uint8_t type = 8, uint8_t code = 0, uint16_t id = 1, uint16_t seq = 1);

    std::vector<uint8_t> Encode();
    void Decode(const std::vector<uint8_t>& buffer);

    uint8_t type() const
    { return type_; }
    uint8_t code() const
    { return code_; }
    uint16_t id() const
    { return id_; }
    uint16_t seq() const
    { return seq_; }

private:
    uint16_t compute_checksum(const uint16_t* data, size_t length);

    uint8_t type_;
    uint8_t code_;
    uint16_t checksum_;
    uint16_t id_;
    uint16_t seq_;
    std::vector<uint8_t> data_;
};

class Socket {
public:
    Socket();
    bool Send(const std::vector<uint8_t>& buffer);
    std::vector<uint8_t> Recv();
};

class Client {
public:
    Client(std::shared_ptr<Socket> socket);
    bool Ping(const std::string& ip = "127.0.0.1", size_t pkt_counter = 1);

private:
    std::shared_ptr<Socket> socket_;
};

class Server {
public:
    bool Listen(const std::string& ip, size_t pkt_counter);

private:
    std::shared_ptr<Socket> socket_;
};

#endif // PING_H
