#include "catch.h"
#include "pipeline.h"
#include "instruction.h"

using Catch::Matchers::Equals;

TEST_CASE("Removing instructions in pipeline", "[Pipeline]")
{
    SECTION("Pipline removes completed instruction")
    {
        ScalarPipeline pipe = ScalarPipeline();
        Instructions::Instruction instr_a = Instructions::Instruction();
        instr_a.instrString = "A";
        instr_a.stage = DONE;
        Instructions::Instruction instr_b = Instructions::Instruction();
        instr_b.instrString = "B";
        pipe.addInstructionToPipeline(&instr_a);
        pipe.addInstructionToPipeline(&instr_b);
        REQUIRE(pipe.getInstrSize() == 2);
        pipe.removeCompletedInstructions();
        REQUIRE(pipe.getInstrSize() == 1);
    }
    SECTION("Pipeline preserves order of instructions after removing completed instructions")
    {
        ScalarPipeline pipe = ScalarPipeline();
        Instructions::Instruction instr_1 = Instructions::Instruction();
        Instructions::Instruction instr_2 = Instructions::Instruction();
        Instructions::Instruction instr_3 = Instructions::Instruction();
        Instructions::Instruction instr_4 = Instructions::Instruction();
        Instructions::Instruction instr_5 = Instructions::Instruction();
        instr_1.instrString = "1";
        instr_2.instrString = "2";
        instr_3.instrString = "3";
        instr_4.instrString = "4";
        instr_5.instrString = "5";
        pipe.addInstructionToPipeline(&instr_1);
        pipe.addInstructionToPipeline(&instr_2);
        pipe.addInstructionToPipeline(&instr_3);
        pipe.addInstructionToPipeline(&instr_4);
        pipe.addInstructionToPipeline(&instr_5);
        instr_1.stage = DONE;
        instr_3.stage = DONE;
        pipe.removeCompletedInstructions();
        auto vec = pipe.getInstructions();
        Instructions::Instruction *instr = vec.front();
        std::string instrStr = instr->instrString;
        vec.erase(vec.begin());
        REQUIRE_THAT(instrStr, Equals("2"));
        instr = vec.front();
        instrStr = instr->instrString;
        vec.erase(vec.begin());
        REQUIRE_THAT(instrStr, Equals("4"));
        instr = vec.front();
        instrStr = instr->instrString;
        vec.erase(vec.begin());
        REQUIRE_THAT(instrStr, Equals("5"));
        REQUIRE(vec.size() == 0);
    }

}