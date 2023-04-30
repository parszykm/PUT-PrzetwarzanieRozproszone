#pragma once
#include<iostream>
#include <vector>
#include <mutex>
#include<algorithm>
// #include "main.h"

typedef struct {
    int ts;
    int src;
    int data;
} packet_t;

class ProcessQueue {
public:
    void  push(const packet_t& packet);
    void pop();
    void showQueue();
    std::vector<packet_t>* getQueue();
    packet_t top();
    bool isCandidate(int rank, int n);
    void removeBySrc(int src);
    
    


private:
    std::vector<packet_t> *m_packets = new std::vector<packet_t>;
    std::mutex m_mutex;
};


extern ProcessQueue sectionQueue;
