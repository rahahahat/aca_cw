#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

#include "constants.h"
#include "instruction.h"

#ifndef _PARSER_INCLUDED_
#define _PARSER_INCLUDED_
#include "processor.h"
class Processor;

class Parser
{
    private:
        std::vector<std::string> programList; 
    public:
        int totalPrograms;
        Parser(Processor *processor);
        std::vector<std::string> parseProgram(std::string filename);
        void readProgramsFromCL(int num, char* filenames[]);
        Processor *processor;
};

#endif