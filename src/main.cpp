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
    processor.loadProgram("./abc.txt");
    Instructions::Instruction instr = Instructions::Instruction();
    processor.runInstr(&instr);
    processor.runInstr(&instr);
    processor.runInstr(&instr);
    processor.runInstr(&instr);
    processor.runInstr(&instr);
    std::cout << "Result: " << processor.registers[instr.rd] << std::endl;
    return 0;
}