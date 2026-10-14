//code for calculating frequency of each character
#include <iostream>
#include <fstream>
#include "frequency.h"
using namespace std;

void calculateFrequency(const string& fileName, int frequency[])
{
    ifstream file(fileName);

    if (!file)
    {
        cout << "File could not be opened.";
        return;
    }

    char ch;

    while (file.get(ch))
    {
        frequency[(unsigned char)ch]++;
    }

    file.close();
