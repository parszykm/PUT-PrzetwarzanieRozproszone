#include "queue.h"

void PacketVector::push(packet_t& packet) {
    // Szukanie miejsca do wstawienia pakietu
    auto it = m_packets.begin();
    while (it != m_packets.end() && (it->ts < packet.ts || (it->ts == packet.ts && it->src < packet.src))) {
        ++it;
    }
    // Wstawienie pakietu w odpowiednim miejscu
    m_packets.insert(it, packet);
}

void PacketVector::pop() {
    // Usuwanie pierwszego pakietu
    if (!m_packets.empty()) {
        m_packets.erase(m_packets.begin());
    }
}

void PacketVector::showQueue(){
    std::cout << "Packet vector:" << std::endl;
    for (const auto& packet : m_packets) {
        std::cout << "ts: " << packet.ts << ", src: " << packet.src << ", data: " << packet.data << std::endl;
    }
}

packet_t PacketVector::top(){
    return m_packets[0];
}

PacketVector queuePacket;