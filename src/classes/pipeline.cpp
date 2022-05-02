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
    std::ifstream i("config.json");
    i >> config;
    i.close();
    auto procUnits = config["procUnits"];
    std::cout << procUnits << std::endl;
    int num_decode_units = procUnits["decode"].get<int>();
    int num_exec_units = procUnits["execute"].get<int>();
    int num_mem_units = procUnits["memory"].get<int>();
    num_proc_units = {
        {DECODEUNIT, new std::pair<int, int>(num_decode_units, num_decode_units)},
        {EXECUTEUNIT, new std::pair<int, int>(num_exec_units, num_exec_units)},
        {MEMORYUNIT, new std::pair<int, int>(num_mem_units, num_mem_units)},
    };
    std::vector<ProcUnit*> dv;
    std::vector<ProcUnit*> ev;
    std::vector<ProcUnit*> mv;
    for (int d = 0; d < num_decode_units; d++)
    {
        ProcUnit* unit = new ODecodeUnit();
        dv.push_back(unit);
    }
    for (int e = 0; e < num_exec_units; e++)
    {
        ProcUnit* unit = new OExecuteUnit();
        ev.push_back(unit);
    }
    for (int m = 0; m < num_mem_units; m++)
    {
        ProcUnit* unit = new OMemoryUnit();
        mv.push_back(unit);
    }
    proc_units[DECODEUNIT] = dv;
    proc_units[EXECUTEUNIT] = ev;
    proc_units[MEMORYUNIT] = mv;
}

void OoOPipeline::issueTick()
{
    std::vector<ProcUnit*> iv = proc_units[DECODEUNIT];
    for (auto &unit: iv)
    {
        unit->nextTick();
    }
};

void OoOPipeline::execTick()
{
    std::vector<ProcUnit*> ev = proc_units[EXECUTEUNIT];
    for (auto &unit: ev)
    {
        unit->nextTick();
    }
};

void OoOPipeline::memTick()
{
    std::vector<ProcUnit*> ev = proc_units[MEMORYUNIT];
    for (auto &unit: ev)
    {
        unit->nextTick();
    }
}

void OoOPipeline::nextTick(int cycle)
{
    issueTick();
    if (cycle == 1) 
    {
        std::cout << termcolor::on_bright_red << cycle << termcolor::reset << std::endl;
        return;
    }
    execTick();
    processor->getLsq()->nextTick();
    memTick();
};


void OoOPipeline::pipeInstructionsToProcessor()
{
    PipelineLLNode *curr = instructions->head;
    while(curr != NULL)
    {
        Instructions::Instruction *instr = curr->payload;
        // processor->runInstr(instr);
        curr = curr->next;
    }
    // TODO: logic to differentiate between pipeline stall and branch stall
    // if (stalled())
    // {
    //     if (processor->getRS()->size() < 64)
    //     {
    //         resumePipeline(RS);
    //         return;
    //     }
    //     return;
    // }
    // if (processor->getRS()->hasEmptyEntries() == NULL) stallPipeline(RS);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
};

void OoOPipeline::stepMode()
{
    nlohmann::json conf;
    std::ifstream i("config.json");
    i >> conf;
    i.close();
    auto mode = conf["stepMode"];
    // int till = mode["till"].get<int>();
    // if (clock < )
    if (!(mode["enabled"].get<bool>() && mode["instruction"].get<bool>())) return;
    std::string ss;
	while(1)
	{
        std::cout << "\nStep through instruction (Enter -h for help): ";
        std::cin >> ss;
        
        if (!ss.compare("n")) break;
        if (!ss.compare("-h"))
        {
            std::cout << "Execute next instruction (n)" << std::endl;
            std::cout << "Print registers (rg)" << std::endl;
            std::cout << "Print reservation station (rs)" << std::endl;
        }
		if (!ss.compare("rg"))
		{
            processor->regDump();
		}
        if (!ss.compare("rs"))
        {
            processor->getRS()->print();
        }
        ss = "";
	}
}

bool OoOPipeline::areAllProcUnitsFree()
{
    // std::vector<ProcUnit*> iv = proc_units[DECODEUNIT];
    std::vector<ProcUnit*> ev = proc_units[EXECUTEUNIT];
    std::vector<ProcUnit*> mv = proc_units[MEMORYUNIT];
    bool busy = true;
    for (auto &unit: ev)
    {
        busy &= unit->busy;
    }
    for (auto &unit: mv)
    {
        busy &= unit->busy;
    }
    return !busy;
}