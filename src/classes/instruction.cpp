#include <iostream>
#include <string>

#include "instruction.h"
#include "pipeline.h"
#include "pipestage.h"

using namespace Instructions;

Instruction::Instruction(): num_cycles(0)
{
    rd = $noreg;
    rs = $noreg;
    rt = $noreg;
    instrString = "";
    stage = FETCH;
    isReadyToExecute = false;
};

Instruction::Instruction(std::string instr): num_cycles(0)
{
    rd = $noreg;
    rs = $noreg;
    rt = $noreg;
    instrString = instr;
    stage = FETCH;
    isReadyToExecute = false;
};

Instruction::Instruction(std::string instr, int cycles): num_cycles(cycles)
{
    rd = $noreg;
    rs = $noreg;
    rt = $noreg;
    instrString = instr;
    stage = FETCH;
    isReadyToExecute = false;
};

int Instruction::getCurrCycle()
{
    return curr_cycle;
}

void Instruction::incrementCycle() 
{
    if (curr_cycle < num_cycles)
    {
        curr_cycle++;
    }
    return;
}

void Instruction::nextPipeStage()
{
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

pipestage Instruction::getCurrentPipeStage()
{
    return stage;
};