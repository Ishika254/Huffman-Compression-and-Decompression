// code is written by sanxkkar!!

#include "huffmannode.h"

//here i am initializing a Huffman Node
HuffmanNode::HuffmanNode(char ch, int freq)
    : data(ch),
      frequency(freq),
      left(nullptr),
      right(nullptr)
{
}


