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

// #################################################################################################
// Pipeline
// #################################################################################################

Pipeline::Pipeline()
{
    processor = Processor::getProcessorInstance();
    stall = false;
    stalled_by = NoSrc;
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

void Pipeline::stallPipeline(StallSource by)
{
    if (by == stalled_by) return;
    if (stalled_by == NoSrc) 
    {
        auto itr = stallMap.find(by);
        if (itr == stallMap.end())
        {
            stallMap.insert(std::pair<StallSource, int>(by, 1));
        }
        itr->second = itr->second + 1;
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

void Pipeline::printStalls()
{
    int total_stalls = 0;
    std::map<StallSource, std::string> wordmap = {
        {Halt, "HALT"},
        {Branch, "Branch"},
        {Capacity, "Capacity"},
        {Flush, "Flush"},
    };
    for (auto itr = stallMap.begin(); itr != stallMap.end(); itr++)
    {
        total_stalls += itr->second;
        std::cout <<
        termcolor::bold <<
        termcolor:: red <<
        "Stall Source: " <<
        wordmap.at(itr->first)
        << termcolor::blue
        << " - Stalls: "
        << termcolor::bold
        << itr->second
        << std::endl;
    }
    std::cout << termcolor::bright_cyan << "Total Stall: " << total_stalls << std::endl;
    return;
}

// #################################################################################################
// ScalarPipeline
// #################################################################################################
ScalarPipeline::ScalarPipeline() {
    instructions = new PipelineLL();
    fn = new FetchUnit();
    dn = new ScalarDecodeUnit();
    en = new ScalarExecuteUnit();
    mn = new ScalarMemoryUnit();
    stallNode == NULL;
    return;
};

void ScalarPipeline::resumePipeline(StallSource from)
{
    if (stalled_by == NoSrc) return;
    if (from == stalled_by)
    {
        stallNode = NULL;
        stall = false;
        stalled_by = NoSrc;
        return;
    }
    return;
}

void ScalarPipeline::removeCompletedInstructions() {
    instructions->flushCompletedInstructions();
    return;
}
void ScalarPipeline::addInstructionToPipeline(int id) {
    if (!processor->getSB()->isPCValid()) return;
    std::cout
    << termcolor::bold
    << termcolor::yellow
    << "Placing new instruction in pipeline"
    << termcolor::reset
    << std::endl;
    Instructions::Instruction *new_inst = instructions->addInstructionForFetch();
    return;
};

void ScalarPipeline::addInstructionToPipeline(Instructions::Instruction *instrPtr) 
{
    instructions->add(instrPtr);
    return;
};

void ScalarPipeline::nextTick(int cycle) {

    PipelineLLNode *curr = instructions->head;
    dn->reset();
    en->reset();
    mn->reset();
    while(curr != NULL)
    {
        Instructions::Instruction *instr = curr->payload;
        
        switch(instr->stage)
        {
            case DECODE:
                dn->nextTick(instr);
                break;
            case EXECUTE:
                en->nextTick(instr);
                break;
            default:
                mn->nextTick(instr);
                break;
        }
        if (stalled() && curr == stallNode)
        {
            if (stallNode == NULL)
            {
                stallNode = curr;
            }
            removeCompletedInstructions();
            return;
        }
        curr = curr->next;
    };
    removeCompletedInstructions();
    Instructions::Instruction* instr = new Instructions::Instruction();
    if (instructions->size < 5)
    {
        if (fn->scalarFetch(instr) != NULL) addInstructionToPipeline(instr);
    }
    std::cout << termcolor::red  << "Instructions in pipeline: " << instructions->size << std::endl;
};

void ScalarPipeline::flushPipelineOnBranchOrJump()
{
    std::cout << termcolor::bold << termcolor::red << "FLUSHING ON BRANCH" << termcolor::reset << std::endl;
    instructions->flushAfterNode(flushNode);
    return;
}

int ScalarPipeline::pipelineSize()
{
    return instructions->size;
}

// #################################################################################################
// OoOPipeline
// #################################################################################################

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
    instrQ = new LinkedList<Instructions::Instruction>();
}

void OoOPipeline::post()
{
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

LinkedList<Instructions::Instruction>* OoOPipeline::getInstrQ()
{
    return instrQ;
}

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
};

void OoOPipeline::robuffTick()
{
    processor->getRB()->nextTick();
};

void OoOPipeline::fetchTick()
{
    for (auto &unit: dn)
    {
        unit->fetchTick();
    }
};

void OoOPipeline::decodeTick()
{
    for (auto &unit: dn)
    {
        unit->decodeTick();
    }
};

void OoOPipeline::nextTick(int cycle)
{
    if (stalled_by == Capacity)
    {
        if (instrQ->size < getConfig()->capacity->instrQ)
        {
            resumePipeline(Capacity);
        }
    }
    lsqTick();
    robuffTick();
    if (stalled_by == Flush) return;

    execTick();
    memTick();

    post();

    decodeTick();
    fetchTick();
};


void OoOPipeline::pipeInstructionsToProcessor() {};

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
    instrQ->flushAfterNode(instrQ->head);
    instrQ->removeAndDestroy(instrQ->head);
    assert(instrQ->size == 0);

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