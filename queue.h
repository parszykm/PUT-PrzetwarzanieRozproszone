#include <iostream>
#include <queue>
#include <string>
#include<vector>
#include "main.h"
// #pragma once
// struct test_st{
//     int ts;       /* timestamp (zegar lamporta */
//     int src;  
//     int data; 
// };
class PrioComparator {
public:
    bool operator()(packet_t const& a, packet_t const& b);
};
extern std::priority_queue<packet_t, std::vector<packet_t>, PrioComparator> queuePacket;

void showQueue();