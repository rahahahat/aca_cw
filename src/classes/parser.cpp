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

void Parser::loadProgramIntoCPU(std::string filename) {
    std::string programLine;
    std::ifstream programFile;
    programFile.open(filename);
    if (!programFile.is_open()) {
        std::cerr << "File not found at path: " << filename << std::endl;
        return;
    }
    while (!programFile.eof()) {
        std::getline(programFile, programLine);
        processor->loadInstructionIntoMemory(programLine);
    }
    return;
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

void Parser::attachToProcessor(Processor *procPtr) {
    processor = procPtr;
    return;
}