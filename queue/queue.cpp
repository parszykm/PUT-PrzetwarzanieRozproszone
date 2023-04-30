#include "queue.h"

// ProcessQueue::ProcessQueue(){
//     // std::vector<string> *data = new std::vector<string>();
// }
void ProcessQueue::push(const packet_t& packet) {
    // Szukanie miejsca do wstawienia pakietu
    std::lock_guard<std::mutex> lock(m_mutex); // blokowanie mutexa przy pomocy std::lock_guard

    auto it = m_packets->begin();
    while (it != m_packets->end() && (it->ts < packet.ts || (it->ts == packet.ts && it->src < packet.src))) {
        ++it;
    }
    // Wstawienie pakietu w odpowiednim miejscu
    m_packets->insert(it, packet);
}

void ProcessQueue::pop() {
    // Usuwanie pierwszego pakietu
    std::lock_guard<std::mutex> lock(m_mutex); // blokowanie mutexa przy pomocy std::lock_guard

    if (!m_packets->empty()) {
        m_packets->erase(m_packets->begin());
    }
}

void ProcessQueue::showQueue(){
    std::cout << "Packet vector:" << std::endl;
    std::lock_guard<std::mutex> lock(m_mutex); // blokowanie mutexa przy pomocy std::lock_guard
    for (const auto& packet : *m_packets) {
        std::cout << "ts: " << packet.ts << ", src: " << packet.src << ", data: " << packet.data << std::endl;
        // queueState += "ts: " + std::to_string(packet.ts) + ", src: " + std::to_string(packet.src) + ", data: " + std::to_string(packet.data) + ", type" + packet.processType + "\n";

    }
}
std::vector<packet_t> *ProcessQueue::getQueue(){
    std::lock_guard<std::mutex> lock(m_mutex); // blokowanie mutexa przy pomocy std::lock_guard
    return m_packets;
}

bool ProcessQueue::isCandidate(int rank, int n){
    std::lock_guard<std::mutex> lock(m_mutex); // blokowanie mutexa przy pomocy std::lock_guard

    auto it = m_packets->begin();
    int index = 0;
    while(it != m_packets->end() && index < n){  
        // printf("rank: %d, index: %d, element: %d, bool %d\n", rank, index, it->src, it->src == rank);
        // showQueue();
        if(it->src == rank) return true;
        index++;
        ++it;
    }
    return false;
}

packet_t ProcessQueue::top(){
    std::lock_guard<std::mutex> lock(m_mutex); // blokowanie mutexa przy pomocy std::lock_guard

    return (*m_packets)[0];
}

void ProcessQueue::removeBySrc(int src) {
    std::lock_guard<std::mutex> lock(m_mutex); // blokowanie mutexa przy pomocy std::lock_guard

    m_packets->erase(std::remove_if(m_packets->begin(), m_packets->end(),
        [src](const packet_t& packet) { return packet.src == src; }), m_packets->end());
}

ProcessQueue sectionQueue;