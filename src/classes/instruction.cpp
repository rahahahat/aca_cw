#include <iostream>
#include <string>

#include "instruction.h"
#include "pipeline.h"
#include "pipestage.h"

using namespace Instructions;

Instruction::Instruction() {
    instrString = "";
    stage = INIT;
};

Instruction::Instruction(std::string instr) {
    instrString = instr;
    stage = FETCH;
};

void Instruction::nextPipeStage() {
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
            std::cerr << "Instruction has been completed" << std::endl;
    };
};

pipestage Instruction::getCurrentPipeStage() {
    return stage;
};