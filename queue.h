#pragma once
#include<iostream>
#include <vector>
#include "main.h"

class ProcessQueue {
public:
    void  push(packet_t& packet);
    void pop();
    void showQueue();
    std::vector<packet_t> getQueue();
    packet_t top();
    bool isCandidate(int rank, int n);
    


private:
    std::vector<packet_t> m_packets;
};


extern ProcessQueue sectionQueue;
