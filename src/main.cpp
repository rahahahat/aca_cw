#include "parser.h"
#include "processor.h"
#include "prochelpers.h"
#include "pipeline.h"
#include "instruction.h"
#include "isa.h"
#include "util.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "termcolor.h"
#include <cassert>
#include "backward.hpp"
#include "config.h"

int main(int argc, char* argv[]) {

    Processor *processor = Processor::getProcessorInstance()->fabricate();
    // processor->DataMemory[0] = 3;
    // processor->DataMemory[1] = 1;
    // processor->DataMemory[2] = 1;
    // processor->DataMemory[3] = 1;
    // processor->DataMemory[4] = 4;
    // processor->DataMemory[5] = 4;
    // processor->DataMemory[6] = 4;
    std::cout << "DATA MEMORY SIZE: " << processor->dataMemSize << std::endl;
    std::string program = getConfig()->program;
    processor->loadProgram(program);
    processor->runProgram();
    processor->dumpDataMemory();
}