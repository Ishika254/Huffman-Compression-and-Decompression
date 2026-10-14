// this code is written by Sanxkar


//main work to make header file 
//read original file
//replace every character from huffman code
//pack 0and 1 in actual bytes
//write in compressed file

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "encoder.h"

using namespace std;

namespace
{
    bool writeNumber(ofstream& file, unsigned int number)
    {
        while (number >= 128)
        {
            file.put(static_cast<char>((number & 127) | 128));

            if (!file)
                return false;

            number >>= 7;
        }

        file.put(static_cast<char>(number));
        return static_cast<bool>(file);
    }

    int numberOfBytes(unsigned int number)
    {
        int bytes = 1;

        while (number >= 128)
        {
            ++bytes;
            number >>= 7;
        }

        return bytes;
    }
}

bool Encoder::validateCodes(
    const int frequency[],
    const map<unsigned char, string>& codes) const
{
    for (int i = 0; i < 256; ++i)
    {
        if (frequency[i] < 0)
            return false;

        if (frequency[i] == 0)
            continue;

        unsigned char ch = static_cast<unsigned char>(i);
        map<unsigned char, string>::const_iterator it = codes.find(ch);

        if (it == codes.end() || it->second.empty())
            return false;

        for (char bit : it->second)
        {
            if (bit != '0' && bit != '1')
                return false;
        }
    }

    return true;
}

int Encoder::countDifferentCharacters(const int frequency[]) const
{
    int different = 0;

    for (int i = 0; i < 256; ++i)
    {
        if (frequency[i] > 0)
            ++different;
    }

    return different;
}

long long Encoder::calculateHeaderSize(const int frequency[]) const
{
    long long headerSize = 5;

    for (int i = 0; i < 256; ++i)
    {
        if (frequency[i] > 0)
        {
            headerSize += 1 + numberOfBytes(
                static_cast<unsigned int>(frequency[i]));
        }
    }

    return headerSize;
}

bool Encoder::writeHeader(
    ofstream& output,
    const int frequency[],
    int differentCharacters,
    streampos& paddingPosition) const
{
    output.put('H');
    output.put('F');

    output.put(static_cast<char>(differentCharacters & 255));
    output.put(static_cast<char>((differentCharacters >> 8) & 255));

    if (!output)
        return false;

    for (int i = 0; i < 256; ++i)
    {
        if (frequency[i] > 0)
        {
            output.put(static_cast<char>(i));

            if (!output || !writeNumber(
                    output, static_cast<unsigned int>(frequency[i])))
            {
                return false;
            }
        }
    }

    paddingPosition = output.tellp();

    if (paddingPosition == streampos(-1))
        return false;

    output.put(0);
    return static_cast<bool>(output);
}

bool Encoder::writeEncodedBits(
    ofstream& output,
    const string& code,
    unsigned char& byte,
    int& bitCount) const
{
    for (char bit : code)
    {
        byte = static_cast<unsigned char>(byte << 1);

        if (bit == '1')
            byte = static_cast<unsigned char>(byte | 1);

        ++bitCount;

        if (bitCount == 8)
        {
            output.put(static_cast<char>(byte));

            if (!output)
                return false;

            byte = 0;
            bitCount = 0;
        }
    }

    return true;
}

bool Encoder::writePadding(
    ofstream& output,
    streampos paddingPosition,
    unsigned char byte,
    int bitCount) const
{
    int padding = 0;

    if (bitCount > 0)
    {
        padding = 8 - bitCount;
        byte = static_cast<unsigned char>(byte << padding);

        output.put(static_cast<char>(byte));

        if (!output)
            return false;
    }

    streampos endPosition = output.tellp();

    if (endPosition == streampos(-1))
        return false;

    output.seekp(paddingPosition);

    if (!output)
        return false;

    output.put(static_cast<char>(padding));

    if (!output)
        return false;

    output.seekp(endPosition);
    return static_cast<bool>(output);
}

bool Encoder::encodeData(
    ifstream& input,
    ofstream& output,
    const map<unsigned char, string>& codes,
    streampos paddingPosition) const
{
    input.clear();
    input.seekg(0, ios::beg);

    if (!input)
        return false;

    unsigned char byte = 0;
    int bitCount = 0;
    char ch;

    while (input.get(ch))
    {
        unsigned char character = static_cast<unsigned char>(ch);
        map<unsigned char, string>::const_iterator it =
            codes.find(character);

        if (it == codes.end() || it->second.empty())
            return false;

        if (!writeEncodedBits(output, it->second, byte, bitCount))
            return false;
    }

    if (input.bad())
        return false;

    return writePadding(output, paddingPosition, byte, bitCount);
}

bool Encoder::compressFile(
    string inputFileName,
    string compressedFileName,
    int frequency[],
    map<unsigned char, string>& codes)
{
    if (inputFileName == compressedFileName)
        return false;

    ifstream input(inputFileName, ios::binary);

    if (!input)
        return false;

    if (!validateCodes(frequency, codes))
    {
        input.close();
        return false;
    }

    input.seekg(0, ios::end);
    streampos inputEnd = input.tellg();

    if (inputEnd == streampos(-1))
    {
        input.close();
        return false;
    }

    long long originalSize = static_cast<long long>(inputEnd);

    input.clear();
    input.seekg(0, ios::beg);

    if (!input)
    {
        input.close();
        return false;
    }

    int differentCharacters = countDifferentCharacters(frequency);
    long long totalBits = 0;

    for (int i = 0; i < 256; ++i)
    {
        if (frequency[i] > 0)
        {
            unsigned char ch = static_cast<unsigned char>(i);

            totalBits += static_cast<long long>(frequency[i])
                       * static_cast<long long>(
                           codes.find(ch)->second.length());
        }
    }

    long long dataBytes = (totalBits + 7) / 8;
    long long headerSize = calculateHeaderSize(frequency);

    if (headerSize + dataBytes >= originalSize)
    {
        input.close();

        cout << "This file is too small to benefit from Huffman coding."
             << endl;

        return false;
    }

    ofstream output(compressedFileName, ios::binary);

    if (!output)
    {
        input.close();
        return false;
    }

    streampos paddingPosition;

    if (!writeHeader(
            output, frequency, differentCharacters, paddingPosition) ||
        !encodeData(input, output, codes, paddingPosition))
    {
        input.close();
        output.close();
        return false;
    }

    output.flush();
    bool success = static_cast<bool>(output);

    input.close();
    output.close();

    if (!success || !output)
        return false;

    cout << "Huffman compression completed." << endl;
    return true;
}
