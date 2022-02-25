#include "parser.h"
#include "constants.h"
#include "processor.h"
#include "pipeline.h"
#include "procUnits.h"
#include "instruction.h"
#include "isa.h"
#include "util.h"
#include <iostream>
#include <string>
#include <map>
#include "termcolor.h"
#include <cassert>

int main(int argc, char* argv[]) {
    std::cout << termcolor::on_green << "Starting" << termcolor::reset << std::endl;
    Processor *processor = Processor::fabricate();
    processor->loadProgram("./factorial.txt");
    std::cout << "ProgramSize: " << processor->instrMemSize <<std::endl;
    processor->runProgram();
    Processor::destroy(processor);
    return 0;
}