// code has be written by sanxkar!!



#include "priorityqueue.h"

// Initialize the heap and its size
PriorityQueue::PriorityQueue()
    : heap{}, heapSize(0)
{
}

// Insert a node into the min-heap
bool PriorityQueue::insert(HuffmanNode* node)
{
    // Reject null nodes and a full heap
    if (node == nullptr || heapSize >= CAPACITY)
    {
        return false;
    }

    // Insert at the next available position
    int index = heapSize;
    heap[heapSize++] = node;

    // Restore the min-heap property
    heapifyUp(index);

    return true;
}
