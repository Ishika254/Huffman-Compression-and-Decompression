#include <iostream>
#include <fstream>
#include "Encoder.h"

using namespace std;

class NumberWriter
{
public:
    static void write(ofstream& f, unsigned int n)
    {
        while (n >= 128)
        {
            f.put((char)((n & 127) | 128));
            n >>= 7;
        }

        f.put((char)n);
    }

    static int size(unsigned int n)
    {
        int s = 1;

        while (n >= 128)
        {
            n >>= 7;
            s++;
        }

        return s;
    }
};

class BitWriter
{
    unsigned char byte = 0;
    int bits = 0;

public:
    void write(ofstream& f, const string& code)
    {
        for (char c : code)
        {
            byte = (byte << 1) | (c == '1');

            if (++bits == 8)
            {
                f.put((char)byte);
                byte = 0;
                bits = 0;
            }
        }
    }

    int finish(ofstream& f)
    {
        if (!bits)
            return 0;

        int padding = 8 - bits;
        byte <<= padding;
        f.put((char)byte);

        return padding;
    }
};

class HeaderWriter
{
public:
    static int count(const int freq[])
    {
        int n = 0;

        for (int i = 0; i < 256; i++)
            if (freq[i] > 0)
                n++;

        return n;
    }

    static long long size(const int freq[])
    {
        long long s = 3;

        for (int i = 0; i < 256; i++)
            if (freq[i] > 0)
                s += 1 + NumberWriter::size(freq[i]);

        return s;
    }

    static void write(ofstream& f, const int freq[])
    {
        int n = count(freq);

        f.put('H');
        f.put('F');
        f.put((char)n);
        f.put((char)(n >> 8));

        for (int i = 0; i < 256; i++)
        {
            if (freq[i] > 0)
            {
                f.put((char)i);
                NumberWriter::write(f, freq[i]);
            }
        }

        f.put(0);
    }
};

class EncoderEngine
{
public:
    bool compress(string inputName, string outputName,
                  int freq[], map<unsigned char, string>& codes)
    {
        ifstream input(inputName, ios::binary);

        if (!input)
        {
            cout << "Input file could not be opened." << endl;
            return false;
        }

        // Check empty file using character frequencies
        if (HeaderWriter::count(freq) == 0)
        {
            cout << "The file is empty." << endl;
            input.close();
            return false;
        }

        input.seekg(0, ios::end);
        long long originalSize = input.tellg();
        input.seekg(0, ios::beg);

        if (originalSize == 0)
        {
            cout << "The file is empty." << endl;
            input.close();
            return false;
        }

        long long bits = 0;

        for (int i = 0; i < 256; i++)
        {
            if (freq[i] > 0)
            {
                if (codes.find((unsigned char)i) == codes.end())
                {
                    input.close();
                    return false;
                }

                bits += (long long)freq[i] *
                        codes[(unsigned char)i].length();
            }
        }

        long long dataSize = (bits + 7) / 8;
        long long headerSize = HeaderWriter::size(freq);

        if (headerSize + dataSize >= originalSize)
        {
            cout << "This file is too small to benefit from Huffman coding."
                 << endl;

            input.close();
            return false;
        }

        ofstream output(outputName, ios::binary);

        if (!output)
        {
            cout << "Output file could not be opened." << endl;
            input.close();
            return false;
        }

        HeaderWriter::write(output, freq);

        streampos paddingPos = output.tellp();
        output.put(0);

        BitWriter writer;
        char ch;

        while (input.get(ch))
            writer.write(output, codes[(unsigned char)ch]);

        int padding = writer.finish(output);

        output.seekp(paddingPos);
        output.put((char)padding);

        input.close();
        output.close();

        cout << "Huffman compression completed." << endl;

        return true;
    }
};

bool Encoder::compressFile(string inputFileName,
                           string compressedFileName,
                           int frequency[],
                           map<unsigned char, string>& codes)
{
    EncoderEngine engine;

    return engine.compress(inputFileName, compressedFileName,
                           frequency, codes);
}