#ifndef HuffmanTree_h
#define HuffmanTree_h

#include "PriorityQueue.h"

class HuffmanTree
{
public:
    HuffmanNode* root;

    HuffmanTree();

    void buildTree(PriorityQueue& pq);
};

#endif
