#ifndef CodeGeneration_h
#define CodeGeneration_h

#include "HuffmanTree.h"
#include <map>
#include <string>

using namespace std;

class CodeGeneration
{
public:
    void generateCodes(HuffmanNode* root, string code,
                       map<unsigned char, string>& codes);

    void printCodes(const map<unsigned char, string>& codes);
};

#endif
