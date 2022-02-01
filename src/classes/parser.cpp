#include <iostream>
#include <fstream>
#include <string>
#include "parser.h"

using namespace std;

void Parser::readProgram(string filename) {
    string programLine;
    ifstream programFile;
    programFile.open(filename);
    if (!programFile.is_open() or !programFile.good()) {
        cerr << "File not found at path: " << filename << endl;
        return;
    }
    while (!programFile.eof()) {
        getline(programFile, programLine);
        cout << programLine << endl;
    }
    return;
}