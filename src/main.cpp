#include "parser.h"
#include "constants.h"
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

    std::ifstream i("config.json");
    i >> config;
    i.close();
    Processor *processor = Processor::getProcessorInstance()->fabricate();
    processor->DataMemory[1] = 2;
    processor->loadProgram(config["program"].get<std::string>());
    processor->runProgram();
}