// #include "catch.h"
// #include "processor.h"
// #include "parser.h"

// using Catch::Matchers::Equals;

// TEST_CASE("Parser correctly populates label map #1", "[LabelMap]")
// {
//     Processor proc = Processor();
//     proc.loadProgram("./testprogs/test_1.txt");
//     REQUIRE(proc.labelMap.size() == 1);
//     REQUIRE(proc.instrMemSize == 3);
//     REQUIRE(proc.labelMap.at("loop") == 2);
//     REQUIRE_THAT(proc.instructionMemory[0], Equals("lw r0 r1 0x0"));
//     REQUIRE_THAT(proc.instructionMemory[proc.labelMap.at("loop")], Equals("lw r0 r3 0x0"));
// }

// TEST_CASE("Parser correctly populates label map #2", "[LabelMap]")
// {
//     Processor proc = Processor();
//     proc.loadProgram("./testprogs/test_2.txt");
//     REQUIRE(proc.labelMap.size() == 3);
//     REQUIRE(proc.instrMemSize == 5);
//     REQUIRE(proc.labelMap.at("loop_a") == 2);
//     REQUIRE(proc.labelMap.at("loop_b") == 3);
//     REQUIRE(proc.labelMap.at("loop_c") == 4);
//     REQUIRE_THAT(proc.instructionMemory[proc.labelMap.at("loop_a")], Equals("lw r0 r3 0x0"));
//     REQUIRE_THAT(proc.instructionMemory[proc.labelMap.at("loop_b")], Equals("lw r0 r4 0x0"));
//     REQUIRE_THAT(proc.instructionMemory[proc.labelMap.at("loop_c")], Equals("lw r0 r5 0x0"));
// }