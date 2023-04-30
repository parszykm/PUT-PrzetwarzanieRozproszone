#include<iostream>
#include "queue.h"
using namespace std;

// ProcessQueue sectionQueue;
int main(){
    packet_t *xd = new packet_t{1,2,3};
   sectionQueue.push({1, 0, 123});
   sectionQueue.push({4, 3, 213});
   sectionQueue.push({4, 1, 213});
   sectionQueue.push({2, 2, 12});

    
    sectionQueue.showQueue();
    sectionQueue.pop();
    sectionQueue.showQueue();
    sectionQueue.push({2, 2, 12});
    sectionQueue.showQueue();
    sectionQueue.push(*xd);
    sectionQueue.showQueue();
    sectionQueue.removeBySrc(2);
    sectionQueue.showQueue();

    // printf("%d", sectionQueue.top());
 
}