#include "ping.h"
#include <cstring>
#include <iostream>

// -------------------- IcmpMessage --------------------
IcmpMessage::IcmpMessage(uint8_t type, uint8_t code, uint16_t id, uint16_t seq)
    : type_(type), code_(code), checksum_(0), id_(id), seq_(seq) {}

uint16_t IcmpMessage::compute_checksum(const uint16_t* data, size_t length) {
    uint32_t sum = 0;
    while (length > 1) {
        sum += *data++;
        length -= 2;
    }
    if (length == 1) {
        sum += *(const uint8_t*)data;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return (uint16_t)(~sum);
}

std::vector<uint8_t> IcmpMessage::Encode() {
    std::vector<uint8_t> buffer(8 + data_.size(), 0);
    buffer[0] = type_;
    buffer[1] = code_;
    *reinterpret_cast<uint16_t*>(&buffer[4]) = id_;
    *reinterpret_cast<uint16_t*>(&buffer[6]) = seq_;

    checksum_ = compute_checksum((uint16_t*)buffer.data(), buffer.size());
    *reinterpret_cast<uint16_t*>(&buffer[2]) = checksum_;

    return buffer;
}

void IcmpMessage::Decode(const std::vector<uint8_t>& buffer) {
    if (buffer.size() < 8) return;
    type_ = buffer[0];
    code_ = buffer[1];
    checksum_ = *reinterpret_cast<const uint16_t*>(&buffer[2]);
    id_ = *reinterpret_cast<const uint16_t*>(&buffer[4]);
    seq_ = *reinterpret_cast<const uint16_t*>(&buffer[6]);
}

// -------------------- Client --------------------
Client::Client(std::shared_ptr<Socket> socket) : socket_(std::move(socket)) {}

bool Client::Ping(const std::string& ip, size_t pkt_counter) {
    (void)ip; // não usamos aqui no mock
    for (size_t i = 0; i < pkt_counter; i++) {
        IcmpMessage msg;
        auto buffer_send = msg.Encode();
        socket_->Send(buffer_send);

        auto buffer_recv = socket_->Recv();
        msg.Decode(buffer_recv);
    }
    return true;
}

// -------------------- Server --------------------
bool Server::Listen(const std::string& ip, size_t pkt_counter) {
    (void)ip;
    (void)pkt_counter;
    std::cout << "Server listening..." << std::endl;
    return true;
}
