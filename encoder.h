//  written by sanxkar
#ifndef ENCODER_H
#define ENCODER_H

#include <fstream>
#include <map>
#include <string>

class Encoder
{
private:
    bool validateCodes(
        const int frequency[],
        const std::map<unsigned char, std::string>& codes) const;

    int countDifferentCharacters(const int frequency[]) const;

    long long calculateHeaderSize(const int frequency[]) const;

    bool writeHeader(
        std::ofstream& output,
        const int frequency[],
        int differentCharacters,
        std::streampos& paddingPosition) const;

    bool encodeData(
        std::ifstream& input,
        std::ofstream& output,
        const std::map<unsigned char, std::string>& codes,
        std::streampos paddingPosition) const;

    bool writeEncodedBits(
        std::ofstream& output,
        const std::string& code,
        unsigned char& byte,
        int& bitCount) const;

    bool writePadding(
        std::ofstream& output,
        std::streampos paddingPosition,
        unsigned char byte,
        int bitCount) const;

public:

//important function manager 
    bool compressFile(
        std::string inputFileName,
        std::string compressedFileName,
        int frequency[],
        std::map<unsigned char, std::string>& codes);
};

#endif

