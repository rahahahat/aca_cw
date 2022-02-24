#include "catch.h"
#include "processor.h"
#include "parser.h"

using Catch::Matchers::Equals;

TEST_CASE("Decode unit correctly decode branch instruction #1", "[Decode_Branch]")
{
    Processor *proc = Processor::fabricate();
    Instructions::Instruction instr = Instructions::Instruction();
    proc->loadProgram("./testprogs/br_dc_1.txt");
    instr.instrString = proc->instructionMemory[2];
    instr.stage = DECODE;
    REQUIRE(proc->labelMap.size() == 3);
    REQUIRE(proc->instrMemSize == 6);
    REQUIRE(proc->labelMap.at("loop_a") == 3);
    REQUIRE(proc->labelMap.at("loop_b") == 4);
    REQUIRE(proc->labelMap.at("loop_c") == 5);
    REQUIRE_THAT(instr.instrString, Equals("beq r1 r2 loop_a"));
    proc->runInstr(&instr);
    REQUIRE(instr.rs == $r1);
    REQUIRE(instr.rt == $r2);
    REQUIRE(instr.immediateOrAddress == 3);
    Processor::destroy(proc);
}

TEST_CASE("Decode unit correctly decode branch instruction #2", "[Decode_Branch]")
{
    Processor *proc = Processor::fabricate();
    Instructions::Instruction instr_a = Instructions::Instruction();
    Instructions::Instruction instr_b = Instructions::Instruction();
    Instructions::Instruction instr_c = Instructions::Instruction();
    proc->loadProgram("./testprogs/br_dc_2.txt");
    instr_a.instrString = proc->instructionMemory[2];
    instr_b.instrString = proc->instructionMemory[3];
    instr_c.instrString = proc->instructionMemory[4];
    instr_a.stage = DECODE;
    instr_b.stage = DECODE;
    instr_c.stage = DECODE;
    REQUIRE(proc->labelMap.size() == 3);
    REQUIRE(proc->instrMemSize == 8);
    REQUIRE(proc->labelMap.at("loop_a") == 5);
    REQUIRE(proc->labelMap.at("loop_b") == 6);
    REQUIRE(proc->labelMap.at("loop_c") == 7);
    REQUIRE_THAT(instr_a.instrString, Equals("beq r1 r2 loop_a"));
    REQUIRE_THAT(instr_b.instrString, Equals("beq r3 r4 loop_b"));
    REQUIRE_THAT(instr_c.instrString, Equals("beq r5 r6 loop_c"));
    proc->runInstr(&instr_a);
    proc->runInstr(&instr_b);
    proc->runInstr(&instr_c);
    REQUIRE(instr_a.rs == $r1);
    REQUIRE(instr_a.rt == $r2);
    REQUIRE(instr_a.immediateOrAddress == 5);
    REQUIRE(instr_b.rs == $r3);
    REQUIRE(instr_b.rt == $r4);
    REQUIRE(instr_b.immediateOrAddress == 6);
    REQUIRE(instr_c.rs == $r5);
    REQUIRE(instr_c.rt == $r6);
    REQUIRE(instr_c.immediateOrAddress == 7);
    Processor::destroy(proc);
}