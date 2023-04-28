#include<iostream>
#include "queue.h"
using namespace std;

PacketVector queuePacket;
int main(){
    packet_t *xd = new packet_t{1,2,3};
   queuePacket.push({1, 0, 123});
   queuePacket.push({4, 3, 213});
   queuePacket.push({4, 1, 213});
   queuePacket.push({2, 2, 12});

    
    queuePacket.showQueue();

    queuePacket.pop();
    queuePacket.showQueue();
    queuePacket.push({2, 2, 12});
    queuePacket.showQueue();
    queuePacket.push(*xd);
    queuePacket.showQueue();

    printf("%d", queuePacket.top());

    
}