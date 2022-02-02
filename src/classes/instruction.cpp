#include <iostream>
#include <string>

#include "instruction.h"
#include "pipestage.h"

using namespace std;
using namespace Instructions;

Instruction::Instruction(string instr) {
    instrString = instr;
    stage = FETCH;
};

void Instruction::advancePipeStage() {
    switch (stage) {
        case FETCH:
            stage = DECODE;
        case DECODE:
            stage = EXECUTE;
        case EXECUTE:
            stage = MEMORYACCESS;
        case MEMORYACCESS:
            stage = WRITEBACK;
        case WRITEBACK:
            stage = DONE;
        default:
            cerr << "Instruction: " << instrString <<" has been completed" << endl;
    };
};
