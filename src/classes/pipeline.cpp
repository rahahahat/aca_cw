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

void ScalarPipeline::addInstructionToPipeline(Instructions::Instruction *instr, int id)
{
    std::cout << "Putting new insruction in pipeline" << std::endl;
    if (instr == NULL) {
        Instructions::Instruction *new_inst = new Instructions::Instruction();
        new_inst->id = id;
        instructions.push_back(new_inst);
        return;
    }
    instructions.push_back(instr);
    return;
};

void ScalarPipeline::pipeInstructionsToProcessor() {
    resume();
    int count = 0;
    std::cout << std::endl;
    std::cout << "Instructions in pipeline: " << instructions.size() << std::endl;
    for (int i = 0; i < instructions.size(); i++) {
        std::cout << "Iterations: " << count << std::endl;
        count += 1;
        Instructions::Instruction *instr = instructions.at(i);
        processor->runInstr(instr);
        if (stalled()) break;
    }
    std::cout << "------------------------ All instructions for current cycle have been run ----------------------------" << std::endl;
    std::cout << std::endl;
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