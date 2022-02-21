#include "catch.h"
#include "processor.h"
#include "pipeline.h"
#include "instruction.h"

TEST_CASE( "Add works", "" ) {
    Processor *processor = Processor::fabricate();
    Instructions::Instruction instr = Instructions::Instruction();
    processor->instrMemSize = 1;
    processor->instructionMemory[0] = "add r2 r1 r0";
    processor->runInstr(&instr);
    processor->runInstr(&instr);
    processor->registers[instr.rs] = 4;
    processor->registers[instr.rt] = 6;
    processor->runInstr(&instr);
    processor->runInstr(&instr);
    processor->runInstr(&instr);
    REQUIRE(processor->registers[instr.rd] == 10);
    Processor::destroy(processor);
}