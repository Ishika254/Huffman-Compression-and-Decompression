#ifndef ENCODER_H
#define ENCODER_H

#include <string>
#include <map>

using namespace std;

class Encoder
{
public:
    bool compressFile(string inputFileName,
                      string compressedFileName,
                      int frequency[],
                      map<unsigned char, string>& codes);
};

#endif