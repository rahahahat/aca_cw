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

TEST_CASE("removing an instruction from Pipeline LL")
{
    SECTION("Pipeline LL has no elements")
    {
        PipelineLL plnLL = PipelineLL();
        REQUIRE(plnLL.remove(0) == NULL);
    }
    SECTION("Index to be removed is greater than size")
    {
        PipelineLL plnLL = PipelineLL();
        Instructions::Instruction instr_a = Instructions::Instruction();
        instr_a.id = 0;
        plnLL.add(&instr_a);
        REQUIRE(plnLL.remove(1) == NULL);
        REQUIRE(plnLL.size == 1);
    }
    SECTION("Removing the head of Pipeline LL")
    {
        PipelineLL plnLL = PipelineLL();
        Instructions::Instruction instr_a = Instructions::Instruction();
        Instructions::Instruction instr_b = Instructions::Instruction();
        instr_a.id = 0;
        instr_b.id = 1;
        plnLL.add(&instr_a);
        plnLL.add(&instr_b);
        REQUIRE(plnLL.size == 2);
        plnLL.remove(0);
        REQUIRE(plnLL.size == 1);
        REQUIRE(plnLL.head == plnLL.tail);
        REQUIRE(plnLL.head->payload == &instr_b);
        REQUIRE(plnLL.tail->payload == &instr_b);
    }
    SECTION("Removing the tail of Pipeline LL", "#1")
    {
        PipelineLL plnLL = PipelineLL();
        Instructions::Instruction instr_a = Instructions::Instruction();
        Instructions::Instruction instr_b = Instructions::Instruction();
        instr_a.id = 0;
        instr_b.id = 1;
        plnLL.add(&instr_a);
        plnLL.add(&instr_b);
        REQUIRE(plnLL.size == 2);
        plnLL.remove(1);
        REQUIRE(plnLL.size == 1);
        REQUIRE(plnLL.head == plnLL.tail);
        REQUIRE(plnLL.head->payload == &instr_a);
        REQUIRE(plnLL.tail->payload == &instr_a);
    }
    SECTION("Removing the tail of Pipeline LL", "#2")
    {
        PipelineLL plnLL = PipelineLL();
        Instructions::Instruction instr_a = Instructions::Instruction();
        Instructions::Instruction instr_b = Instructions::Instruction();
        Instructions::Instruction instr_c = Instructions::Instruction();
        instr_a.id = 0;
        instr_b.id = 1;
        instr_c.id = 2;
        plnLL.add(&instr_a);
        plnLL.add(&instr_b);
        plnLL.add(&instr_c);
        REQUIRE(plnLL.size == 3);
        plnLL.remove(2);
        REQUIRE(plnLL.size == 2);
        REQUIRE(plnLL.head->payload == &instr_a);
        REQUIRE(plnLL.tail->payload == &instr_b);
    }
    SECTION("Removing an element in between head and tail")
    {
        PipelineLL plnLL = PipelineLL();
        Instructions::Instruction instr_a = Instructions::Instruction();
        Instructions::Instruction instr_b = Instructions::Instruction();
        Instructions::Instruction instr_c = Instructions::Instruction();
        instr_a.id = 0;
        instr_b.id = 1;
        instr_c.id = 2;
        plnLL.add(&instr_a);
        plnLL.add(&instr_b);
        plnLL.add(&instr_c);
        REQUIRE(plnLL.size == 3);
        plnLL.remove(1);
        REQUIRE(plnLL.size == 2);
        REQUIRE(plnLL.head->payload == &instr_a);
        REQUIRE(plnLL.tail->payload == &instr_c);
    }
}

TEST_CASE("PipelineLL flushes all completed instructions")
{
    PipelineLL plnLL = PipelineLL();
    Instructions::Instruction instr_a = Instructions::Instruction();
    Instructions::Instruction instr_b = Instructions::Instruction();
    Instructions::Instruction instr_c = Instructions::Instruction();
    instr_a.id = 0;
    instr_a.stage = DONE;
    instr_b.id = 1;
    instr_c.id = 2;
    plnLL.add(&instr_a);
    plnLL.add(&instr_b);
    plnLL.add(&instr_c);
    plnLL.flushCompletedInstructions();
    REQUIRE(plnLL.size == 2);
    REQUIRE(plnLL.head->payload == &instr_b);
    REQUIRE(plnLL.tail->payload == &instr_c);
}