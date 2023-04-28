#include <iostream>
#include <queue>
#include <string>
#include<vector>
// #include "main.h"
// #pragma once
struct test_struct{
    int ts;       /* timestamp (zegar lamporta */
    int src;  
    int data; 
};
class PrioComparator {
public:
    bool operator()(test_struct const& a, test_struct const& b);
};
extern std::priority_queue<test_struct, std::vector<test_struct>, PrioComparator> queuePacket;

void showQueue();