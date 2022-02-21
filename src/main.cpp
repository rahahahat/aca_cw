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

int main(int argc, char* argv[]) {
    Processor *processor = Processor::fabricate();
    processor->loadProgram("./abc.txt");
    std::cout << "ProgramSize: " << processor->instrMemSize <<std::endl;
    processor->DataMemory[0] = 1;
    processor->DataMemory[1] = 1;
    processor->runProgram();
    std::cout << processor->registers[$r1] << std::endl;
    std::cout << processor->registers[$r2] << std::endl;
    std::cout << processor->registers[$r3] << std::endl;
    std::cout << processor->registers[$r4] << std::endl;
    Processor::destroy(processor);
    return 0;
}