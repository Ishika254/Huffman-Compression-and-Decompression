// code written by sanxkar


#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include "huffmannode.h"
class PriorityQueue
{
private:
    static constexpr int CAPACITY = 256;
    HuffmanNode* heap[CAPACITY];
    int heapSize;
    void heapifyUp(int index);
    void heapifyDown(int index);
public:
    PriorityQueue();
    bool insert(HuffmanNode* node);
    HuffmanNode* getMin();
    int size() const noexcept;
    bool empty() const noexcept;
};

#endif 
