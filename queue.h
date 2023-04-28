#pragma once
#include<iostream>
#include <vector>
#include "main.h"

class PacketVector {
public:
    void  push(packet_t& packet);
    void pop();
    void showQueue();
    packet_t top();


private:
    std::vector<packet_t> m_packets;
};

extern PacketVector queuePacket;

