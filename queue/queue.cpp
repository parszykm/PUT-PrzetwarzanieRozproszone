#include "queue.h"


std::priority_queue<test_struct, std::vector<test_struct>, PrioComparator> queuePacket; 

bool PrioComparator::operator()(const test_struct& s1, const test_struct& s2){
        if (s1.ts != s2.ts) {
            return s1.ts > s2.ts;
        }
        return s1.src > s2.src;
    }

void showQueue(){
    // std::string result;
    std::cout << "Zawartosc kolejki: ";
    std::priority_queue<test_struct, std::vector<test_struct>, PrioComparator> tmpQueue = queuePacket;
    while (!tmpQueue.empty()) {
        std::cout <<std::endl<< "(" << tmpQueue.top().ts << ", " << tmpQueue.top().src << ", " << tmpQueue.top().data << ") ";
        // result += "(" + std::to_string(tmpQueue.top().ts) + ", " + std::to_string(tmpQueue.top().src) + ", " + std::to_string(tmpQueue.top().data) + ")\n";
        tmpQueue.pop();
        
    }
    std::cout << std::endl;
    // return result;
}