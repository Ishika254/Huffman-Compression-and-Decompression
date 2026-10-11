//this code is written by Sanxkar
#ifndef HUFFMANNODE_H
#define HUFFMANNODE_H

// here i is the class and the left right pointer for the node!

class HuffmanNode
{
public:
    char data;             
    int frequency;         

    HuffmanNode* left;     
    HuffmanNode* right;    

    HuffmanNode(char ch, int freq);
};

#endif 
