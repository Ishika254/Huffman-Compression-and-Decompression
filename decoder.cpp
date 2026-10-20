#include <iostream>
#include <fstream>
#include "Decoder.h"
#include "HuffmanNode.h"
#include "PriorityQueue.h"
#include "HuffmanTree.h"

using namespace std;

class NumberReader
{
public:
    static bool read(ifstream& f, unsigned int& n)
    {
        n = 0;
        int shift = 0;
        char ch;

        while (f.get(ch))
        {
            unsigned char b = (unsigned char)ch;
            n |= (unsigned int)(b & 127) << shift;

            if (!(b & 128))
                return true;

            if ((shift += 7) >= 32)
                return false;
        }

        return false;
    }
};

class HeaderReader
{
public:
    int freq[256] = {};
    long long total = 0;
    int padding = 0;

    bool read(ifstream& f)
    {
        char a, b;

        if (!f.get(a) || !f.get(b) || a != 'H' || b != 'F')
            return false;

        if (!f.get(a) || !f.get(b))
            return false;

        int different = (unsigned char)a |
                        ((unsigned char)b << 8);

        if (different > 256)
            return false;

        for (int i = 0; i < different; i++)
        {
            char ch;
            unsigned int n;

            if (!f.get(ch) || !NumberReader::read(f, n))
                return false;

            if (!n || total > 2147483647LL - n)
                return false;

            freq[(unsigned char)ch] = (int)n;
            total += n;
        }

        if (!f.get(a))
            return false;

        padding = (unsigned char)a;

        return padding <= 7;
    }
};

class TreeBuilder
{
public:
    static HuffmanTree build(const int freq[])
    {
        PriorityQueue pq;

        for (int i = 0; i < 256; i++)
            if (freq[i] > 0)
                pq.insert(new HuffmanNode((char)i, freq[i]));

        HuffmanTree tree;
        tree.buildTree(pq);

        return tree;
    }
};

class BitReader
{
public:
    static long long decode(ifstream& input, ofstream& output,
                            HuffmanTree& tree, long long total)
    {
        HuffmanNode* current = tree.root;
        long long decoded = 0;
        char ch;

        while (decoded < total && input.get(ch))
        {
            unsigned char byte = (unsigned char)ch;

            for (int bit = 7; bit >= 0; bit--)
            {
                current = ((byte >> bit) & 1)
                              ? current->right
                              : current->left;

                if (!current)
                    return decoded;

                if (current->left == nullptr &&
                    current->right == nullptr)
                {
                    output.put(current->data);

                    if (++decoded == total)
                        return decoded;

                    current = tree.root;
                }
            }
        }

        return decoded;
    }
};

class DecoderEngine
{
public:
    bool decompress(string compressedName, string outputName)
    {
        ifstream input(compressedName, ios::binary);

        if (!input)
        {
            cout << "Cannot open compressed file." << endl;
            return false;
        }

        HeaderReader header;

        if (!header.read(input))
        {
            cout << "Invalid compressed file." << endl;
            return false;
        }

        HuffmanTree tree = TreeBuilder::build(header.freq);

        ofstream output(outputName, ios::binary);

        if (!output)
            return false;

        if (header.total == 0)
        {
            close(input, output);
            cout << "Decompression completed." << endl;
            return true;
        }

        if (!tree.root)
            return false;

        if (tree.root->left == nullptr &&
            tree.root->right == nullptr)
        {
            for (long long i = 0; i < header.total; i++)
                output.put(tree.root->data);

            close(input, output);
            cout << "Decompression completed." << endl;
            return true;
        }

        long long decoded =
            BitReader::decode(input, output, tree, header.total);

        close(input, output);

        if (decoded != header.total)
        {
            cout << "Decompression failed." << endl;
            return false;
        }

        cout << "Decompression completed successfully." << endl;
        cout << "Output file: " << outputName << endl;

        return true;
    }

private:
    void close(ifstream& input, ofstream& output)
    {
        input.close();
        output.close();
    }
};

bool Decoder::decompressFile(string compressedFileName,
                             string outputFileName)
{
    DecoderEngine engine;

    return engine.decompress(compressedFileName,
                             outputFileName);
}