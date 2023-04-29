#include <vector>

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
    int top();


private:
    std::vector<packet_t> m_packets;
};

