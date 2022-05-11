#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

#include "parser.h"
#include "processor.h"

// #################################################################################################
// Parser
// #################################################################################################

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