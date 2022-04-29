#include "catch.h"
#include "processor.h"
#include "lsq.h"
#include "instruction.h"

using Catch::Matchers::Equals;

TEST_CASE("Load is correctly added to LSQueue")
{
    Processor *proc = Processor::getProcessorInstance();
    Scoreboard *sb = new Scoreboard(false);
    proc->attachProcHelper(sb);
    LSQueue *lsq = new LSQueue();
    Instructions::Instruction *instr1 = new Instructions::Instruction();

    instr1->instrString = "LW r1 r2 0x1";
    instr1->opcode = LW;
    instr1->rt = $r2;
    instr1->rs = $r1;
    instr1->immediateOrAddress = 1;

    LSQNode* node = lsq->addToQueue(instr1);
    REQUIRE(lsq->getNumEntries() == 1);
    REQUIRE(node->opcode == LW);

};

TEST_CASE("Store is correctly added to LSQueue")
{
    Processor *proc = Processor::getProcessorInstance();
    Scoreboard *sb = new Scoreboard(false);
    proc->attachProcHelper(sb);
    LSQueue *lsq = new LSQueue();
    Instructions::Instruction *instr1 = new Instructions::Instruction();

    instr1->instrString = "SW r1 r2 0x1";
    instr1->opcode = SW;
    instr1->rt = $r2;
    instr1->rs = $r1;
    instr1->immediateOrAddress = 1;

    LSQNode* node = lsq->addToQueue(instr1);
    REQUIRE(lsq->getNumEntries() == 1);
    REQUIRE(node->opcode == SW);

};

TEST_CASE("Address is correctly calculate in LSQueue", "[1]")
{
    Processor *proc = Processor::getProcessorInstance();
    Scoreboard *sb = new Scoreboard(false);
    proc->attachProcHelper(sb);
    LSQueue *lsq = new LSQueue();
    Instructions::Instruction *instr1 = new Instructions::Instruction();

    instr1->instrString = "LW r1 r2 0x2";
    instr1->opcode = LW;
    instr1->rt = $r2;
    instr1->rs = $r1;
    instr1->immediateOrAddress = 2;

    LSQNode* node = lsq->addToQueue(instr1);
    REQUIRE(lsq->getNumEntries() == 1);
    REQUIRE(node->getValids().first == 1);
    REQUIRE(node->address == -1);
    REQUIRE(node->opcode == LW);

    lsq->calculateAddrs();

    REQUIRE(node->address == 2);

};

TEST_CASE("Addresses is correctly calculate in LSQueue")
{
    Processor *proc = Processor::getProcessorInstance();
    Scoreboard *sb = new Scoreboard(false);
    proc->attachProcHelper(sb);
    LSQueue *lsq = new LSQueue();
    Instructions::Instruction *instr1 = new Instructions::Instruction();

    instr1->instrString = "LW r1 r2 0x2";
    instr1->opcode = LW;
    instr1->rt = $r2;
    instr1->rs = $r1;
    instr1->immediateOrAddress = 2;

    Instructions::Instruction *instr2 = new Instructions::Instruction();

    instr2->instrString = "LW r2 r3 0x4";
    instr2->opcode = LW;
    instr2->rt = $r3;
    instr2->rs = $r2;
    instr2->immediateOrAddress = 4;

    LSQNode* node1 = lsq->addToQueue(instr1);
    LSQNode* node2 = lsq->addToQueue(instr2);
    REQUIRE(lsq->getNumEntries() == 2);
    REQUIRE(node1->getValids().first == 1);
    REQUIRE(node2->getValids().first == 0);

    lsq->calculateAddrs();

    REQUIRE(node1->address == 2);
    REQUIRE(node2->address == -1);

};

TEST_CASE("Instruction is correctly removed")
{
    Processor *proc = Processor::getProcessorInstance();
    Scoreboard *sb = new Scoreboard(false);
    proc->attachProcHelper(sb);
    LSQueue *lsq = new LSQueue();
    Instructions::Instruction *instr1 = new Instructions::Instruction();

    instr1->instrString = "LW r1 r2 0x2";
    instr1->opcode = LW;
    instr1->rt = $r2;
    instr1->rs = $r1;
    instr1->immediateOrAddress = 2;

    Instructions::Instruction *instr2 = new Instructions::Instruction();

    instr2->instrString = "LW r2 r3 0x4";
    instr2->opcode = LW;
    instr2->rt = $r3;
    instr2->rs = $r2;
    instr2->immediateOrAddress = 4;

    LSQNode* node1 = lsq->addToQueue(instr1);
    LSQNode* node2 = lsq->addToQueue(instr2);
    REQUIRE(lsq->getNumEntries() == 2);

    lsq->removeFromQueue(node1->getTag());
    REQUIRE(lsq->getNumEntries() == 1);
    REQUIRE_THAT(lsq->getHead()->getTag(), Equals(node2->getTag()));
}

TEST_CASE("Valid Instruction is correctly returned #1")
{
    Processor *proc = Processor::getProcessorInstance();
    Scoreboard *sb = new Scoreboard(false);
    proc->attachProcHelper(sb);
    LSQueue *lsq = new LSQueue();
    Instructions::Instruction *instr1 = new Instructions::Instruction();

    instr1->instrString = "LW r1 r2 0x2";
    instr1->opcode = LW;
    instr1->rt = $r2;
    instr1->rs = $r1;
    instr1->immediateOrAddress = 2;

    Instructions::Instruction *instr2 = new Instructions::Instruction();

    instr2->instrString = "LW r2 r3 0x4";
    instr2->opcode = LW;
    instr2->rt = $r3;
    instr2->rs = $r2;
    instr2->immediateOrAddress = 4;

    LSQNode* node1 = lsq->addToQueue(instr1);
    LSQNode* node2 = lsq->addToQueue(instr2);
    REQUIRE(lsq->getNumEntries() == 2);
    REQUIRE(node1->getValids().first == 1);
    REQUIRE(node2->getValids().first == 0);

    lsq->calculateAddrs();

    REQUIRE(node1->address == 2);
    REQUIRE(node2->address == -1);

    LSQNode* vNode = lsq->getValidInstruction();
    REQUIRE(vNode != NULL);
    REQUIRE_THAT(vNode->getTag(), Equals(node1->getTag()));
};

TEST_CASE("Valid Instruction is correctly returned #2")
{
    Processor *proc = Processor::getProcessorInstance();
    Scoreboard *sb = new Scoreboard(false);
    proc->attachProcHelper(sb);
    LSQueue *lsq = new LSQueue();
    Instructions::Instruction *instr1 = new Instructions::Instruction();

    instr1->instrString = "LW r1 r2 0x2";
    instr1->opcode = LW;
    instr1->rt = $r2;
    instr1->rs = $r1;
    instr1->immediateOrAddress = 2;

    Instructions::Instruction *instr2 = new Instructions::Instruction();

    instr2->instrString = "LW r2 r3 0x4";
    instr2->opcode = LW;
    instr2->rt = $r3;
    instr2->rs = $r2;
    instr2->immediateOrAddress = 4;

    Instructions::Instruction *instr3 = new Instructions::Instruction();

    instr3->instrString = "LW r5 r6 0x4";
    instr3->opcode = LW;
    instr3->rt = $r5;
    instr3->rs = $r6;
    instr3->immediateOrAddress = 4;

    LSQNode* node1 = lsq->addToQueue(instr1);
    LSQNode* node2 = lsq->addToQueue(instr2);
    LSQNode* node3 = lsq->addToQueue(instr3);
    REQUIRE(lsq->getNumEntries() == 3);
    REQUIRE(node1->getValids().first == 1);
    REQUIRE(node2->getValids().first == 0);
    REQUIRE(node3->getValids().first == 1);

    lsq->calculateAddrs();

    REQUIRE(node1->address == 2);
    REQUIRE(node2->address == -1);
    REQUIRE(node3->address == 4);

    LSQNode* vNode = lsq->getValidInstruction();

    REQUIRE(vNode != NULL);
    REQUIRE_THAT(vNode->getTag(), Equals(node1->getTag()));

    lsq->removeFromQueue(vNode->getTag());
    REQUIRE_THAT(lsq->getHead()->getTag(), Equals(node2->getTag()));
    REQUIRE(lsq->getNumEntries() == 2);
    vNode = lsq->getValidInstruction();
    REQUIRE_THAT(vNode->getTag(), Equals(node3->getTag()));

};


TEST_CASE("Valid Instruction is correctly returned #3")
{
    Processor *proc = Processor::getProcessorInstance();
    Scoreboard *sb = new Scoreboard(false);
    proc->attachProcHelper(sb);
    LSQueue *lsq = new LSQueue();
    Instructions::Instruction *instr1 = new Instructions::Instruction();

    instr1->instrString = "LW r1 r2 0x2";
    instr1->opcode = LW;
    instr1->rt = $r2;
    instr1->rs = $r1;
    instr1->immediateOrAddress = 2;

    Instructions::Instruction *instr2 = new Instructions::Instruction();

    instr2->instrString = "SW r2 r3 0x4";
    instr2->opcode = SW;
    instr2->rt = $r3;
    instr2->rs = $r2;
    instr2->immediateOrAddress = 4;

    Instructions::Instruction *instr3 = new Instructions::Instruction();

    instr3->instrString = "LW r5 r6 0x4";
    instr3->opcode = LW;
    instr3->rt = $r5;
    instr3->rs = $r6;
    instr3->immediateOrAddress = 4;

    LSQNode* node1 = lsq->addToQueue(instr1);
    LSQNode* node2 = lsq->addToQueue(instr2);
    LSQNode* node3 = lsq->addToQueue(instr3);
    REQUIRE(lsq->getNumEntries() == 3);
    REQUIRE(node1->getValids().first == 1);
    REQUIRE(node2->getValids().first == 0);
    REQUIRE(node3->getValids().first == 1);

    lsq->calculateAddrs();

    REQUIRE(node1->address == 2);
    REQUIRE(node2->address == -1);
    REQUIRE(node3->address == 4);

    LSQNode* vNode = lsq->getValidInstruction();

    REQUIRE(vNode != NULL);
    REQUIRE_THAT(vNode->getTag(), Equals(node1->getTag()));

    lsq->removeFromQueue(vNode->getTag());

    REQUIRE_THAT(lsq->getHead()->getTag(), Equals(node2->getTag()));
    REQUIRE(lsq->getNumEntries() == 2);
    vNode = lsq->getValidInstruction();
    REQUIRE(vNode == NULL);
};

