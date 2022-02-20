#include "catch.h"
#include "processor.h"
#include "pipeline.h"
#include "instruction.h"

TEST_CASE( "Add works", "" ) {
    ScalarPipeline pipe = ScalarPipeline();
    ResultForwarder rf = ResultForwarder();
    Scoreboard sb = Scoreboard();
    Processor processor = Processor(&pipe, &sb, &rf);
    FetchUnit fn = FetchUnit(&pipe);
    DecodeUnit dn = DecodeUnit(&pipe);
    ExecuteUnit en = ExecuteUnit(&pipe);
    MemRefUnit mref = MemRefUnit(&pipe);
    WriteBackUnit wbu = WriteBackUnit(&pipe);
    processor.attachProcUnit(&fn);
    processor.attachProcUnit(&dn);
    processor.attachProcUnit(&en);
    processor.attachProcUnit(&mref);
    processor.attachProcUnit(&wbu);
    Instructions::Instruction instr = Instructions::Instruction();
    instr.instrString = "add r2 r1 r0";
    instr.nextPipeStage();
    processor.runInstr(&instr);
    processor.registers[instr.rs] = 1;
    processor.registers[instr.rt] = 1;
    processor.runInstr(&instr);
    processor.runInstr(&instr);
    processor.runInstr(&instr);
    REQUIRE(processor.registers[instr.rd] == 2);
}