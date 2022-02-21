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
    processor->registers[$r1] = 2;
    processor->registers[$r0] = 2;
    processor->registers[$r3] = 3;
    processor->runProgram();
    std::cout << processor->registers[$r2] << std::endl;
    std::cout << processor->registers[$r4] << std::endl;
    Processor::destroy(processor);
    return 0;
}