#include <iostream>
#include <string>
#include <map>
#include <fstream>

#include "HuffmanNode.h"
#include "PriorityQueue.h"
#include "HuffmanTree.h"
#include "CodeGeneration.h"
#include "Encoder.h"

using namespace std;

void calculateFrequency(const string& fileName, int frequency[]);

int main()
{
    int choice;

    do
    {
        cout << "\n-------------------------------------------\n";
        cout << " HUFFMAN FILE COMPRESSION AND DECOMPRESSION\n";
        cout << "-------------------------------------------\n";
        cout << "1. Compress File\n";
        cout << "2. Decompress File\n";
        cout << "3. Compression Ratio\n";
        cout << "4. Exit\n";


        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            string fileName;

            cout << "\nEnter input file name: ";
            cin >> fileName;

            int frequency[256] = {0};

            calculateFrequency(fileName, frequency);

            PriorityQueue pq;

            for (int i = 0; i < 256; i++)
            {
                if (frequency[i] > 0)
                {
                    HuffmanNode* node =
                        new HuffmanNode((char)i, frequency[i]);

                    pq.insert(node);
                }
            }

            cout << "\nNumber of different characters: "
                 << pq.size() << endl;

            HuffmanTree tree;

            tree.buildTree(pq);

            cout << "Huffman Tree created successfully." << endl;

            CodeGeneration codeGenerator;

            map<unsigned char, string> codes;

            codeGenerator.generateCodes(tree.root, "", codes);

            codeGenerator.printCodes(codes);
            string compressedFileName = fileName;

// Find the last dot in the file name
int dot = compressedFileName.find_last_of('.');

if (dot != -1)
{
    compressedFileName = compressedFileName.substr(0, dot);
}

compressedFileName = compressedFileName + ".huff";

// Compress the file
Encoder encoder;

bool success = encoder.compressFile(
    fileName,
    compressedFileName,
    frequency,
    codes
);

if (success)
{
    ifstream originalFile(fileName, ios::binary | ios::ate);
    ifstream compressedFile(compressedFileName, ios::binary | ios::ate);

    long long originalSize = originalFile.tellg();
    long long compressedSize = compressedFile.tellg();

    originalFile.close();
    compressedFile.close();

    cout << "\nFile Compression Summary\n";
    cout << "------------------------\n";
    cout << "Original file: " << fileName << endl;
    cout << "Compressed file: " << compressedFileName << endl;

    cout << "Original size: "
         << originalSize << " bytes" << endl;

    cout << "Compressed size: "
         << compressedSize << " bytes" << endl;

    if (originalSize > 0)
    {
        double ratio = (double)compressedSize / originalSize * 100;

        cout << "Compressed size: "
             << ratio << "% of original size" << endl;

        double saved = originalSize - compressedSize;

        cout << "Space saved: " << saved << " bytes" << endl;
    }
}

            break;
        }

        case 2:
            cout << "\nDecompress File selected.\n";
            break;

        case 3:
            cout << "\nCompression Ratio selected.\n";
            break;

        case 4:
            cout << "\nExiting program...\n";
            break;

        default:
            cout << "\nInvalid choice. Please try again.\n";
        }

    } while (choice != 4);

    return 0;
}