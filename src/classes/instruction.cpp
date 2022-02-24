#include <iostream>
#include <string>

#include "instruction.h"
#include "pipeline.h"
#include "pipestage.h"

using namespace Instructions;

Instruction::Instruction() {
    rd = $noreg;
    rs = $noreg;
    rt = $noreg;
    instrString = "";
    stage = FETCH;
};

Instruction::Instruction(std::string instr) {
    rd = $noreg;
    rs = $noreg;
    rt = $noreg;
    instrString = instr;
    stage = FETCH;
};


void Instruction::nextPipeStage() {
    switch (stage) {
        case FETCH:
            stage = DECODE;
            break;
        case DECODE:
            stage = EXECUTE;
            break;
        case EXECUTE:
            stage = MEMORYACCESS;
            break;
        case MEMORYACCESS:
            stage = WRITEBACK;
            break;
        case WRITEBACK:
            stage = DONE;
    };
};

pipestage Instruction::getCurrentPipeStage() {
    return stage;
};