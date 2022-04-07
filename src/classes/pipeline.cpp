#include "pipeline.h"
#include <bits/stdc++.h>
#include <unistd.h>
#include "termcolor.h"
#include <thread>
#include <chrono>
#include <functional>
#include <cassert>
#include <stdexcept>

Pipeline::Pipeline()
{
    instructions = new PipelineLL();
    processor = Processor::getProcessorInstance();
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
    processor = proc;
    return;
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

void Pipeline::resumePipeline(StallSource from)
{
    if (stalled_by == NoSrc) return;
    if (from == stalled_by)
    {
        stall = false;
        stalled_by = NoSrc;
        return;
    }
    return;
}

int Pipeline::stalled()
{
    return (stall == 1 && stalled_by != NoSrc);
}

Instructions::Instruction* Pipeline::addInstructionToPipeline(int id)
{
    std::cout
    << termcolor::bold
    << termcolor::yellow
    << "Placing new instruction in pipeline"
    << termcolor::reset
    << std::endl;
    Instructions::Instruction *new_inst = instructions->addInstructionForFetch();
    new_inst->id = id;
    return new_inst;
};

void Pipeline::stallPipeline(StallSource by)
{
    if (by == stalled_by) return;
    if (by == NoSrc) 
    {
        stalled_by = by;
        stall = true;
        return;
    }
    std::stringstream ss;
    ss
    << "Pipeline has already been stalled by: "
    << std::to_string(stalled_by) << "."
    << "All operations causing a stall should"
    << "not execute further until previous"
    << "stall is resolved."
    << "Extra stall caused by: "
    << by;

    throw std::runtime_error(ss.str());
}

void Pipeline::pipeInstructionsToProcessor()
{
    // std::cout << termcolor::on_blue << termcolor::bold
    // << "Number of Instructions in Pipeline: " << instructions->size 
    // << termcolor::reset << std::endl << std::endl;

    // PipelineLLNode *curr = instructions->head;

    // while(curr != NULL)
    // {
    //     Instructions::Instruction *instr = curr->payload;
    //     processor->runInstr(instr);
    //     curr = curr->next;
    // }
    // 
    // if (stalled())
    // {
    //     if (processor->getRS()->hasEmptyEntries() != NULL)
    //     {
    //         resumePipeline();
    //         return;
    //     }
    // }
    // if (processor->getRS()->hasEmptyEntries() == NULL) stallPipeline();
    // std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

/* ---------------------------------------------------------------------------------------- */
/* ------------------------------------- OoOPipeline -------------------------------------- */
/* ---------------------------------------------------------------------------------------- */

// void OoOPipeline::stallPipelineOnEvent(const EventBase& base)
// {
//     stall = true;
//     static_cast<const Event<int>&>(base);
//     return;
// }

// void OoOPipeline::flushPipelineOnEvent(const EventBase& base)
// {
//     flush = true;
//     const Event<PipelineLLNode*>& new_event = static_cast<const Event<PipelineLLNode*>&>(base);
//     this->instructions->flushAfterNode(new_event.payload);
//     return;
// }



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

void OoOPipeline::pipeInstructionsToProcessor()
{
    std::cout << termcolor::on_blue << termcolor::bold
    << "Number of Instructions in Pipeline: " << instructions->size 
    << termcolor::reset << std::endl << std::endl;

    PipelineLLNode *curr = instructions->head;
    while(curr != NULL)
    {
        Instructions::Instruction *instr = curr->payload;
        processor->runInstr(instr);
        curr = curr->next;
    }
    // TODO: logic to differentiate between pipeline stall and branch stall
    if (stalled())
    {
        if (processor->getRS()->hasEmptyEntries() != NULL)
        {
            resumePipeline(RS);
            return;
        }
        return;
    }
    if (processor->getRS()->hasEmptyEntries() == NULL) stallPipeline(RS);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}