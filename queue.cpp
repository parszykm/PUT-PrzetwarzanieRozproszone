#include "queue.h"


std::priority_queue<packet_t, std::vector<packet_t>, PrioComparator> queuePacket; 

bool PrioComparator::operator()(const packet_t& s1, const packet_t& s2){
        if (s1.ts != s2.ts) {
            return s1.ts > s2.ts;
        }
        return s1.src > s2.src;
    }

void showQueue(){
    // std::string result;
    std::cout << "Zawartosc kolejki: ";
    std::priority_queue<packet_t, std::vector<packet_t>, PrioComparator> tmpQueue = queuePacket;
    while (!tmpQueue.empty()) {
        std::cout <<std::endl<< "(" << tmpQueue.top().ts << ", " << tmpQueue.top().src << ", " << tmpQueue.top().data << ") ";
        // result += "(" + std::to_string(tmpQueue.top().ts) + ", " + std::to_string(tmpQueue.top().src) + ", " + std::to_string(tmpQueue.top().data) + ")\n";
        tmpQueue.pop();
        
    }
    std::cout << std::endl;
    // return result;
}