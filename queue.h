#pragma once
#include<iostream>
#include<algorithm>
#include <vector>
#include <mutex>
#include "main.h"

class ProcessQueue {
public:
    void push(packet_t& packet);
    bool isAvailable(packet_t& packet);
    void pop();
    void showQueue();
    std::vector<packet_t>* getQueue();
    packet_t top();
    bool isCandidate(int rank, int n, std::string processType);
    void removeBySrc(int src);
    bool isOnFirstNthPlaces(int rank, int n);
    int getQueueSize();
    


private:
    std::vector<packet_t> *m_packets = new std::vector<packet_t>;
    std::mutex m_mutex;
};


extern std::vector<ProcessQueue> sectionQueues;
extern ProcessQueue guidesQueue;
extern ProcessQueue sectionQueue;
