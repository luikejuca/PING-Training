#include "ping.h"
#include <iostream>
#include <memory>

// Uma implementação simples de Socket "real" para demonstração
// (apenas prints, não faz rede de verdade).
class DummySocket : public Socket {
public:
    bool Send(const std::vector<uint8_t>& buffer) override {
        std::cout << "[DummySocket] Enviando pacote de " << buffer.size() << " bytes\n";
        return true;
    }

    std::vector<uint8_t> Recv() override {
        std::cout << "[DummySocket] Recebendo pacote fake...\n";
        // devolve um pacote ICMP "falso"
        IcmpMessage msg;
        return msg.Encode();
    }
};

int main() {
    // Cria um socket "fake"
    auto socket = std::make_shared<DummySocket>();

    // Cria um cliente de ping
    Client client(socket);

    std::cout << "Executando ping para 127.0.0.1...\n";
    bool ok = client.Ping("127.0.0.1", 1);

    if (ok) {
        std::cout << "Ping OK!\n";
    } else {
        std::cout << "Ping falhou.\n";
    }

    return 0;
}
