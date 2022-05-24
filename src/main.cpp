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
#include "config.h"

int main(int argc, char* argv[]) {

    Processor *processor = Processor::getProcessorInstance()->fabricate();
    // std::cout << "DATA MEMORY SIZE: " << processor->dataMemSize << std::endl;
    std::string program = getConfig()->program;
    processor->loadProgram(program);
    processor->runProgram();
    processor->dumpDataMemory();
}