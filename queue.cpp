#include "queue.h"

void ProcessQueue::push(packet_t& packet) {
    // Szukanie miejsca do wstawienia pakietu
    std::lock_guard<std::mutex> lock(m_mutex); // blokowanie mutexa przy pomocy std::lock_guard

    auto it = m_packets->begin();
    while (it != m_packets->end() && (it->ts < packet.ts || (it->ts == packet.ts && it->src < packet.src))) {
        ++it;
    }
    // Wstawienie pakietu w odpowiednim miejscu
    m_packets->insert(it, packet);
}
bool ProcessQueue::isAvailable(packet_t& packet) {
    std::lock_guard<std::mutex> lock(m_mutex); // blokowanie mutexa przy pomocy std::lock_guard
    //Walidacja czy od tyłu wektora nie ma procesu innego koloru
    auto it = m_packets->begin();
    if(packet.processType == CLEANER_INT)
    {
        while(it != m_packets->end()){
            // std::cout<<packet.processType
            if(it->processType == CLEANER_INT) return false;
            ++it;
        }

    }
    if(this->hotelState == Int2ProcessType(colorEnemy(packet.processType))) return false; // jeżeli stan hotelu jest przeciwny do koloru procesu to odrzuć
    for (auto rit = m_packets->rbegin(); rit != m_packets->rend(); ++rit) {
        if(rit->processType == CLEANER_INT && rit->ts < packet.ts) break;
        if(rit->processType == colorEnemy(packet.processType)) return false;
     }
    return true;
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
        std::cout << "ts: " << packet.ts << ", src: " << packet.src << ", data: " << packet.data << ", kolor: "<<Int2ProcessType(static_cast<int>(packet.processType))<< std::endl;
    }
}
std::vector<packet_t> *ProcessQueue::getQueue(){
    std::lock_guard<std::mutex> lock(m_mutex); // blokowanie mutexa przy pomocy std::lock_guard
    return m_packets;
}
int ProcessQueue::getQueueSize(){
    std::lock_guard<std::mutex> lock(m_mutex); // blokowanie mutexa przy pomocy std::lock_guard
    return m_packets->size();

}
bool ProcessQueue::isCandidate(int rank, int n, std::string processType){
    // std::cout<<processType<<" "<<CLEANER<<(*m_packets)[0].processType<<std::endl;
    std::lock_guard<std::mutex> lock(m_mutex); // blokowanie mutexa przy pomocy std::lock_guard
    if( processType != CLEANER && (*m_packets)[0].processType == CLEANER_INT) { return false; } // sprzątacz jest na 1 - miejscu blokuje inne procesy
    else if(processType == CLEANER && (*m_packets)[0].processType == CLEANER_INT) {return true; }
    else if(processType == CLEANER) {return false; }
    // else{
    //     std::cout<<processType<<std::endl;
    // }
    auto it = m_packets->begin();
    int index = 0;
    while(it != m_packets->end() && index < n){  
        if(processType != CLEANER && it->processType == CLEANER_INT) return false; // sprzątacz jest przed kandydatem w kolejce
        else if(processType == CLEANER && it->src == rank && index ==0) return true; // sprzątacz jest pierwszy w kolejce
        else if(processType != CLEANER && it->src == rank) return true; // proces jest w pierwszych n-miejscach i nie ma przed nim sprzątacza   
        index++;
        ++it;
    }
    return false;
}

bool ProcessQueue::isOnFirstNthPlaces(int rank, int n){
    std::lock_guard<std::mutex> lock(m_mutex); // blokowanie mutexa przy pomocy std::lock_guard
    auto it = m_packets->begin();
    int index = 0;
    while(it != m_packets->end() && index < n){  
        if(it->src == rank) return true; // proces jest w pierwszych n-miejscach i nie ma przed nim sprzątacza   
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

 void ProcessQueue::setHotelState(std::string newHotelState){
    std::lock_guard<std::mutex> lock(m_mutex); // blokowanie mutexa przy pomocy std::lock_guard
    this->hotelState = newHotelState;
 }
std::vector<ProcessQueue> hotels(hotelNumber);
ProcessQueue guidesQueue;
