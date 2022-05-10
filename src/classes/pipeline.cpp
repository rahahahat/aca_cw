#include <bits/stdc++.h>
#include <functional>
#include <cassert>
#include <stdexcept>
#include <unistd.h>

#include "pipeline.h"
#include "termcolor.h"
#include "config.h"
#include "processor.h"
#include "fetchUnit.h"
#include "decodeUnit.h"
#include "executeUnit.h"
#include "memoryUnit.h"
#include "procUnit.h"
#include "robuff.h"

Pipeline::Pipeline()
{
    instructions = new PipelineLL();
    processor = Processor::getProcessorInstance();
    stall = 0;
    stalled_by = NoSrc;
}

int Pipeline::completedInstr(Instructions::Instruction *instrPtr)
{
    // if (instrPtr->stage == DONE) return 1;
    return 0;
}

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

void Pipeline::resumePipelineByForce()
{
    stall = false;
    stalled_by = NoSrc;
    return;
}
int Pipeline::stalled()
{
    return (stall == 1 && stalled_by != NoSrc);
}

Instructions::Instruction* Pipeline::addInstructionToPipeline(int id)
{
    Instructions::Instruction *new_inst = instructions->addInstructionForFetch();
    // new_inst->id = id;
    return new_inst;
};

void Pipeline::stallPipeline(StallSource by)
{
    if (by == stalled_by) return;
    if (stalled_by == NoSrc) 
    {
        std::cout << termcolor::bold << termcolor::on_red
        << "Pipeline has been stalled by: "
        << by 
        << termcolor::reset << std::endl;

        stalled_by = by;
        stall = true;
        return;
    }
    std::stringstream ss;
    ss
    << "Pipeline has already been stalled by: "
    << std::to_string(stalled_by) << ". "
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
OoOPipeline::OoOPipeline()
{
    conf* config = getConfig();

    int num_decode_units = config->units->decode;
    int num_exec_units = config->units->execute;
    int num_mem_units = config->units->memory;
    num_proc_units = {
        {DECODEUNIT, new std::pair<int, int>(num_decode_units, num_decode_units)},
        {EXECUTEUNIT, new std::pair<int, int>(num_exec_units, num_exec_units)},
        {MEMORYUNIT, new std::pair<int, int>(num_mem_units, num_mem_units)},
    };
    for (int d = 0; d < num_decode_units; d++)
    {;
        dn.push_back(new ODecodeUnit());
    }
    for (int e = 0; e < num_exec_units; e++)
    {
        en.push_back(new OExecuteUnit());
    }
    for (int m = 0; m < num_mem_units; m++)
    {
        mn.push_back(new OMemoryUnit());
    }
    stalled_by = NoSrc;
    stall = false;
}

void OoOPipeline::post()
{
    for (auto &unit: dn)
    {
        unit->post();
    }
    for (auto &unit: en)
    {
        unit->post();
    }
    for (auto &unit: mn)
    {
        unit->post();
    }
};

void OoOPipeline::issueTick()
{
    for (auto &unit: dn)
    {
        unit->nextTick();
    }
};


void OoOPipeline::execTick()
{
    for (auto &unit: en)
    {
        unit->nextTick();
    }
};

void OoOPipeline::memTick()
{
    for (auto &unit: mn)
    {
        unit->nextTick();
    }
};

void OoOPipeline::lsqTick()
{
    processor->getLsq()->nextTick();
}

void OoOPipeline::robuffTick()
{
    processor->getRB()->nextTick();
}

void OoOPipeline::nextTick(int cycle)
{
    // getConf();
    lsqTick();
    robuffTick();
    printSB();
    if (stalled_by == Flush) return;


    execTick();
    memTick();
    issueTick();
    
    post();

};


void OoOPipeline::pipeInstructionsToProcessor()
{
    PipelineLLNode *curr = instructions->head;
    while(curr != NULL)
    {
        Instructions::Instruction *instr = curr->payload;
        curr = curr->next;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
};

void OoOPipeline::printSB()
{
    conf* config = getConfig();
    if (!config->debug->printSb) return;
    std::string ss;
    std::cout << "Print SB (y/n): ";
    std::cin >> ss;
    if (!ss.compare("y")) processor->getSB()->memDump();
    return;
};

bool OoOPipeline::areAllProcUnitsFree()
{
    // std::vector<ProcUnit*> iv = proc_units[DECODEUNIT];
    bool busy = true;
    for (auto &unit: en)
    {
        busy &= unit->busy;
    }
    for (auto &unit: mn)
    {
        busy &= unit->busy;
    }
    return !busy;
}

void OoOPipeline::flush()
{
    resumePipelineByForce();
    stallPipeline(Flush);
    for (auto &unit: dn)
    {
        unit->flush();
    }
    for (auto &unit: en)
    {
        unit->flush();
    }
    for (auto &unit: mn)
    {
        unit->flush();
    }
}