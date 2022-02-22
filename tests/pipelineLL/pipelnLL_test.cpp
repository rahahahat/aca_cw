#include "catch.h"
#include "pipeline.h"
#include "instruction.h"

TEST_CASE("Creating an empty Pipeline LL")
{
    PipelineLL plnLL = PipelineLL();
    REQUIRE(plnLL.size == 0);
    REQUIRE(plnLL.head == NULL);
    REQUIRE(plnLL.tail == NULL);
};

TEST_CASE("Adding instructions to Pipeline LL")
{
    SECTION("Pipeline LL is empty")
    {
        PipelineLL plnLL = PipelineLL();
        Instructions::Instruction instr = Instructions::Instruction();
        instr.id = 0;
        plnLL.add(&instr);
        REQUIRE(plnLL.size == 1);
        REQUIRE(plnLL.head != NULL);
        REQUIRE(plnLL.tail != NULL);
        REQUIRE(plnLL.tail == plnLL.head);
    }
    SECTION("Pipeline LL is not empty")
    {
        PipelineLL plnLL = PipelineLL();
        Instructions::Instruction instr_a = Instructions::Instruction();
        Instructions::Instruction instr_b = Instructions::Instruction();
        instr_a.id = 0;
        instr_b.id = 1;
        plnLL.add(&instr_a);
        plnLL.add(&instr_b);
        REQUIRE(plnLL.size == 2);
        REQUIRE(plnLL.head != NULL);
        REQUIRE(plnLL.tail != NULL);
        REQUIRE(plnLL.head != plnLL.tail);
        REQUIRE(plnLL.head->payload == &instr_a);
        REQUIRE(plnLL.tail->payload == &instr_b);
    }
};

TEST_CASE("Poping an instruction from Pipeline LL")
{
    SECTION("Pipeline LL only has one element")
    {
        PipelineLL plnLL = PipelineLL();
        Instructions::Instruction instr = Instructions::Instruction();
        instr.id = 0;
        plnLL.add(&instr);
        REQUIRE(plnLL.size == 1);
        plnLL.pop();
        REQUIRE(plnLL.size == 0);
        REQUIRE(plnLL.head == NULL);
        REQUIRE(plnLL.tail == NULL);
    }
    SECTION("Pipeline LL has more than elements")
    {
        PipelineLL plnLL = PipelineLL();
        Instructions::Instruction instr_a = Instructions::Instruction();
        Instructions::Instruction instr_b = Instructions::Instruction();
        instr_a.id = 0;
        instr_b.id = 1;
        plnLL.add(&instr_a);
        plnLL.add(&instr_b);
        REQUIRE(plnLL.size == 2);
        REQUIRE((plnLL.head != NULL && plnLL.head->payload == &instr_a));
        REQUIRE((plnLL.tail != NULL && plnLL.tail->payload == &instr_b));
        REQUIRE(plnLL.head != plnLL.tail);
        REQUIRE(plnLL.pop() == &instr_a);
        REQUIRE(plnLL.size == 1);
        REQUIRE((plnLL.head != NULL && plnLL.head->payload == &instr_b));
        REQUIRE((plnLL.tail != NULL && plnLL.head->payload == &instr_b));
        REQUIRE(plnLL.head == plnLL.tail);
    }
    SECTION("Pipeline LL has no elements")
    {
        PipelineLL plnLL = PipelineLL();
        REQUIRE(plnLL.pop() == NULL);
        REQUIRE(plnLL.size == 0);
        REQUIRE(plnLL.head == NULL);
        REQUIRE(plnLL.tail == NULL);
    }
};

