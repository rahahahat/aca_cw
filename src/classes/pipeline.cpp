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

ScalarPipeline::ScalarPipeline()
{
    instructions = new PipelineLL();
    processor = NULL;
    stall = 0;
};

pipelineType ScalarPipeline::getType()
{
    return Scalar;
}

void ScalarPipeline::addInstructionToPipeline(int id)
{
    std::cout << "Putting new insruction in pipeline" << std::endl;
    Instructions::Instruction *new_inst = instructions->addInstructionForFetch();
    new_inst->id = id;
    return;
};

void ScalarPipeline::addInstructionToPipeline(Instructions::Instruction *instrPtr)
{
    std::cout << "Putting new insruction in pipeline" << std::endl;
    instructions->add(instrPtr);
    return;
};

void ScalarPipeline::pipeInstructionsToProcessor() {
    resume();
    int count = 0;
    std::cout << std::endl;
    std::cout << "Instructions in pipeline: " << instructions->size << std::endl;
    PipelineLLNode *curr = instructions->head;
    while(curr != NULL)
    {
        Instructions::Instruction *instr = curr->payload;
        processor->runInstr(instr);
        if (stalled()) {
            curr = curr->next;
            break;
        }
        curr = curr->next;
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