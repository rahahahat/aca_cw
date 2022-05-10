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
    stall = 0;
    stalled_by = NoSrc;
}

pipelineType Pipeline::getType()
{
    return None; 
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

void Pipeline::pipeInstructionsToProcessor() {};

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