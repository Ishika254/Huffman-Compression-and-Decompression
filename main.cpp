#include<iostream>
#include<fstream>
using namespace std;
int main()
{
    ifstream input("input.txt");
    ofstream output("output.txt");
    char ch;
    while(input.get(ch))
    {
        output.put(ch);
    }
    input.close();
    output.close();
    cout<<"file copied successfully";
    return 0;
}