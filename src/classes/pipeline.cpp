#include "pipeline.h"
#include <bits/stdc++.h>
#include <unistd.h>

int Pipeline::completedInstr(Instructions::Instruction *instrPtr)
{
    if (instrPtr->stage == DONE)
    {
        return 1;
    }
    return 0;
}

void Pipeline::attachToProcessor(Processor *proc) {
    processor = proc;
};

pipelineType Pipeline::getType()
{
    return None;
}

int Pipeline::isEmpty()
{
    return instructions.size() == 0;
}

void Pipeline::removeCompletedInstructions()
{
    auto itr = std::remove_if(instructions.begin(), instructions.end(), Pipeline::completedInstr);
    instructions.erase(itr, instructions.end());
    return;
}

int Pipeline::getInstrSize()
{
    return instructions.size();
}

ScalarPipeline::ScalarPipeline()
{
    processor = NULL;
    stall = 0;
};

pipelineType ScalarPipeline::getType()
{
    return Scalar;
}

void ScalarPipeline::addInstructionToPipeline(Instructions::Instruction *instr) {
    if (instr == NULL) {
        return;
    }
    std::cout << "Putting new insruction in pipeline" << std::endl;
    instructions.push_back(instr);
    return;
};

void ScalarPipeline::pipeInstructionsToProcessor() {
    if (stalled()) resume();
    for (auto instr = begin(instructions); instr != end(instructions); ++instr) {
        Instructions::Instruction *ins = *instr;
        processor->runInstr(*instr);
        if (stalled()) break;
    }
    sleep(1);
}

void ScalarPipeline::stallPipeline()
{
    if (stall == 0)
    {
        stall = 1;
        return;
    }
    return;
}

void ScalarPipeline::resume()
{
    if (stall == 1)
    {
        stall = 0;
        return;
    }
    return;
}

int ScalarPipeline::stalled()
{
    return (stall == 1);
}