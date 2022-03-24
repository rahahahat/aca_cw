#include "pipeline.h"
#include <bits/stdc++.h>
#include <unistd.h>
#include "termcolor.h"
#include <thread>
#include <chrono>
#include <functional>

Pipeline::Pipeline()
{
    instructions = new PipelineLL();
    processor = NULL;
    stall = 0;
    flush = 0;
}

int Pipeline::completedInstr(Instructions::Instruction *instrPtr)
{
    if (instrPtr->stage == DONE) return 1;
    return 0;
}

void Pipeline::attachToProcessor(Processor *proc)
{ 
    processor = proc; return;
};

pipelineType Pipeline::getType()
{
    return None; 
}

int Pipeline::isEmpty()
{
    return instructions->size == 0;
}

void Pipeline::removeCompletedInstructions()
{
    instructions->flushCompletedInstructions();
}

int Pipeline::getInstrSize()
{
    return instructions->size;
}

void Pipeline::resume()
{
    stall = 0;
    return;
}

int Pipeline::stalled()
{
    return (stall == 1);
}

void Pipeline::addInstructionToPipeline(int id)
{
    std::cout
    << termcolor::bold
    << termcolor::yellow
    << "Placing new instruction in pipeline"
    << termcolor::reset
    << std::endl;
    Instructions::Instruction *new_inst = instructions->addInstructionForFetch();
    new_inst->id = id;
    return;
};

void Pipeline::pipeInstructionsToProcessor()
{
    std::cout 
    << termcolor::on_blue
    << termcolor::bold
    << "Number of Instructions in Pipeline: "
    << instructions->size 
    << termcolor::reset
    << "\n"
    << std::endl;
    resume();
    PipelineLLNode *curr = instructions->head;
    while(curr != NULL)
    {
        Instructions::Instruction *instr = curr->payload;
        processor->runInstr(instr);
        if (flush) flushNode = curr;
        if (stalled()) break;
        curr = curr->next;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

/* ---------------------------------------------------------------------------------------- */
/* ------------------------------------- OoOPipeline -------------------------------------- */
/* ---------------------------------------------------------------------------------------- */

void OoOPipeline::stallPipelineOnEvent(const EventBase& base)
{
    stall = true;
    static_cast<const Event<int>&>(base);
    return;
}

void OoOPipeline::flushPipelineOnEvent(const EventBase& base)
{
    flush = true;
    const Event<PipelineLLNode*>& new_event = static_cast<const Event<PipelineLLNode*>&>(base);
    this->instructions->flushAfterNode(new_event.payload);
    return;
}



/* ---------------------------------------------------------------------------------------- */
/* ----------------------------------- ScalarPipeline ------------------------------------- */
/* ---------------------------------------------------------------------------------------- */

/*
ScalarPipeline::ScalarPipeline() {
    instructions = new PipelineLL();
    processor = NULL;
    stall = 0;
};

pipelineType ScalarPipeline::getType() {
    return Scalar;
}


void ScalarPipeline::addInstructionToPipeline(Instructions::Instruction *instrPtr) {
    instructions->add(instrPtr);
    return;
};

void ScalarPipeline::pipeInstructionsToProcessor() {
    std::cout 
    << termcolor::on_blue
    << termcolor::bold
    << "Number of Instructions in Pipeline: "
    << instructions->size 
    << termcolor::reset
    << "\n"
    << std::endl;
    resume();
    PipelineLLNode *curr = instructions->head;
    while(curr != NULL)
    {
        Instructions::Instruction *instr = curr->payload;
        processor->runInstr(instr);
        if (flush) flushNode = curr;
        if (stalled()) break;
        curr = curr->next;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void ScalarPipeline::stallPipeline() {
    this->stall = 1;
    return;
}

void ScalarPipeline::flushPipeline() {
    this->flush = 1;
    return;
}

void ScalarPipeline::stallPipelineOnEvent(const EventBase &base) {
    this->flushPipelineOnEvent(base);
    return;
}

void ScalarPipeline::flushPipelineOnEvent(const EventBase &base) {
    const Event<int>& new_event = static_cast<const Event<int>&>(base);
}

void ScalarPipeline::resume() {
    if (stall == 1)
    {
        stall = 0;
        return;
    }
    return;
}

int ScalarPipeline::stalled() {
    return (stall == 1);
}

void ScalarPipeline::flushPipelineOnBranchOrJump()
{
    if (flushNode != NULL || flush != 1)
    {
        instructions->flushAfterNode(flushNode);
        flush = 0;
        return;
    }
    return;
}
*/