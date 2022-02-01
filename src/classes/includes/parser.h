#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Parser
{
    public:
        int totalPrograms;
        void loadProgram(string filename);
        void readProgramsFromCL(int num, char* filenames[]);
    private:
        vector<string> programList;
};