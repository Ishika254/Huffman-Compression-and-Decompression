// code has be written by sanxkar!!



#include "priorityqueue.h"

PriorityQueue::PriorityQueue()
{
    heapSize = 0;
}

void PriorityQueue::insert(HuffmanNode* node)
{
    //added a numm point check 
    if (node == nullptr)
    {
        return;
    }

    if (heapSize >= 256)
    {
        return;
    }
    //now heap isze will get updated more logically
    heap[heapSize] = node;
    heapSize++;

    heapifyUp(heapSize - 1);
}
//works to insert node
void PriorityQueue::heapifyUp(int index)
{
    while (index > 0)
    {
        int parent = (index - 1) / 2;

        if (heap[parent]->frequency <= heap[index]->frequency)
        {
            break;
        }

        HuffmanNode* temp = heap[parent];
        heap[parent] = heap[index];
        heap[index] = temp;

        index = parent;
    }
}
//work go give minimum code 
HuffmanNode* PriorityQueue::getMin()
{
    if (heapSize == 0)
    {
        return nullptr;
    }

    HuffmanNode* minimum = heap[0];

    heapSize--;

    if (heapSize > 0)
    {
        heap[0] = heap[heapSize];

        heapifyDown(0);
    }

    heap[heapSize] = nullptr;

    return minimum;
}

void PriorityQueue::heapifyDown(int index)
{
    while (true)
    {
        int smallest = index;

        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < heapSize &&
            heap[left]->frequency < heap[smallest]->frequency)
        {
            smallest = left;
        }

        if (right < heapSize &&
            heap[right]->frequency < heap[smallest]->frequency)
        {
            smallest = right;
        }

        if (smallest == index)
        {
            break;
        }

        HuffmanNode* temp = heap[index];
        heap[index] = heap[smallest];
        heap[smallest] = temp;

        index = smallest;
    }
}

int PriorityQueue::size()
{
    return heapSize;
}

bool PriorityQueue::empty()
{
    return heapSize == 0;
}
}
