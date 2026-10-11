#include "CodeGeneration.h"
#include <iostream>

void CodeGeneration::generateCodes(
    HuffmanNode* root,
    string code,
    map<unsigned char, string>& codes)
{
    if (root == nullptr)
    {
        return;
    }

    if (root->left == nullptr && root->right == nullptr)
    {
        if (code.empty())
        {
            code = "0";
        }

        codes[(unsigned char)root->data] = code;
        return;
    }

    generateCodes(root->left, code + "0", codes);
    generateCodes(root->right, code + "1", codes);
}

void CodeGeneration::printCodes(
    const map<unsigned char, string>& codes)
{
    cout << "\nHuffman Codes:\n";

    for (const auto& item : codes)
    {
        unsigned char ch = item.first;

        if (ch == ' ')
        {
            cout << "Space : " << item.second << endl;
        }
        else
        {
            cout << (char)ch << " : " << item.second << endl;
        }
    }
}
