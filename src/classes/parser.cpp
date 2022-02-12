#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

#include "parser.h"
#include "constants.h"
#include "pipeline.h"
#include "instruction.h"

using namespace Instructions;

Parser::Parser(Processor *proc)
{
    this->processor = proc;
}
std::vector<std::string> Parser::parseProgram(std::string filename) {
    std::string programLine;
    std::ifstream programFile;
    std::vector<std::string> program;
    programFile.open(filename);
    if (!programFile.is_open()) {
        std::cerr << "File not found at path: " << filename << std::endl;
        return program;
    }
    while (!programFile.eof()) {
        std::getline(programFile, programLine);
        program.push_back(programLine);
    }
    return program;
};

void Parser::readProgramsFromCL(int num, char* filenames[]) {
    int start = 0;
    for (int x = 0; x < num; x++) {
        if (start == 1) {
            programList.push_back(std::string(filenames[x]));
            totalPrograms++;
        }
        if (start == 0 && !PROGRAM_LIST.compare(std::string(filenames[x]))) {
            start = 1;
        }
    };
};