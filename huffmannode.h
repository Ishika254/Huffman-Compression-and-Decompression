//this code is written by Sanxkar
#ifndef HuffmanNode_h
#define HuffmanNode_h
// i have made classes here which define the left and right node!!
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
