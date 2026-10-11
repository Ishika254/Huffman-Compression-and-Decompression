#include "HuffmanTree.h"

HuffmanTree::HuffmanTree()
{
    root = nullptr;
}

void HuffmanTree::buildTree(PriorityQueue& pq)
{
    while (pq.size() > 1)
    {
        HuffmanNode* left = pq.getMin();
        HuffmanNode* right = pq.getMin();

        HuffmanNode* newNode = new HuffmanNode(
            '\0',
            left->frequency + right->frequency
        );

        newNode->left = left;
        newNode->right = right;

        pq.insert(newNode);
    }

    if (pq.size() == 1)
    {
        root = pq.getMin();
    }
}
