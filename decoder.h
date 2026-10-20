#ifndef Decoder_h
#define Decoder_h

#include <string>
using namespace std;

class Decoder
{
public:
    bool decompressFile(string compressedFileName,
                        string outputFileName);
};

#endif