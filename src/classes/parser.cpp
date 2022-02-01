#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "parser.h"
#include "constants.h"
#include "pipestage.h"
#include "instruction.h"

using namespace std;
using namespace Instructions;

void Parser::loadProgram(string filename) {
    string programLine;
    ifstream programFile;
    programFile.open(filename);
    if (!programFile.is_open()) {
        cerr << "File not found at path: " << filename << endl;
        return;
    }
    while (!programFile.eof()) {
        getline(programFile, programLine);
        cout << programLine << endl;
    }
    return;
};

void Parser::readProgramsFromCL(int num, char* filenames[]) {
    int start = 0;
    for (int x = 0; x < num; x++) {
        if (start == 1) {
            programList.push_back(string(filenames[x]));
            totalPrograms ++;
        }
        if (start == 0 && !PROGRAM_LIST.compare(string(filenames[x]))) {
            start = 1;
        }
    };
};

