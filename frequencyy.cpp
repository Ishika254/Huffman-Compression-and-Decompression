//updated frequency calculation
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

    for (int i = 0; i < 256; i++)
    {
        if (frequency[i] > 0)
        {
            if (i == 32)
                cout << "Space : " << frequency[i] << endl;
            else
                cout << (char)i << " : " << frequency[i] << endl;
        }
    }
}
