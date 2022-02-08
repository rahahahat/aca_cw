#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

#include "constants.h"
#include "instruction.h"
#include "processor.h"


#ifndef _PARSER_INCLUDED_
#define _PARSER_INCLUDED_

class Parser
{
    public:
        int totalPrograms;
        void loadProgramIntoCPU(std::string filename);
        void readProgramsFromCL(int num, char* filenames[]);
        void attachToProcessor(Processor *procPtr);
        Processor *processor;
    private:
        std::vector<std::string> programList;     
};

#endif