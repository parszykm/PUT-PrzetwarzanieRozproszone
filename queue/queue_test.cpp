#include<iostream>
#include "queue.h"
using namespace std;
int main(){
    test_struct *xd = new test_struct{1,2,3};
   queuePacket.push({1, 0, 123});
   queuePacket.push({4, 3, 213});
   queuePacket.push({4, 1, 213});
   queuePacket.push({2, 2, 12});

    std::cout << "Priority queue size is " <<queuePacket.size() << std::endl;

    
    showQueue();

    queuePacket.pop();
    showQueue();
    queuePacket.push({2, 2, 12});
    showQueue();
    queuePacket.push(*xd);
    showQueue();

    
}