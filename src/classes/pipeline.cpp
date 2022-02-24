#include "pipeline.h"
#include <bits/stdc++.h>
#include <unistd.h>

int Pipeline::completedInstr(Instructions::Instruction *instrPtr) { 
    if (instrPtr->stage == DONE) return 1;
    return 0;
}

void Pipeline::attachToProcessor(Processor *proc) { 
    processor = proc; return;
};

pipelineType Pipeline::getType() {
    return None; 
}

int Pipeline::isEmpty() {
    return instructions->size == 0;
}

void Pipeline::removeCompletedInstructions() {
    instructions->flushCompletedInstructions();
}

int Pipeline::getInstrSize() {
    return instructions->size;
}

ScalarPipeline::ScalarPipeline() {
    instructions = new PipelineLL();
    processor = NULL;
    stall = 0;
};

pipelineType ScalarPipeline::getType() {
    return Scalar;
}

void ScalarPipeline::addInstructionToPipeline(int id) {
    if (!processor->scoreboard->isValid($pc)) return;
    std::cout << "\n" << REDB "Putting new insruction in pipeline" NC << "\n" << std::endl;
    Instructions::Instruction *new_inst = instructions->addInstructionForFetch();
    new_inst->id = id;
    return;
};

void ScalarPipeline::addInstructionToPipeline(Instructions::Instruction *instrPtr) {
    std::cout << REDB "\nPutting new insruction in pipeline" NC << "\n" <<std::endl;
    instructions->add(instrPtr);
    return;
};

void ScalarPipeline::pipeInstructionsToProcessor() {
    resume();
    std::cout << GRN "Instructions in pipeline: " << instructions->size << "\n" << NC <<std::endl;
    PipelineLLNode *curr = instructions->head;
    while(curr != NULL)
    {
        Instructions::Instruction *instr = curr->payload;
        processor->runInstr(instr);
        if (flush) flushNode = curr;
        if (stalled()) break;
        curr = curr->next;
    }
    sleep(1);
}

void ScalarPipeline::stallPipeline() {
    if (stall == 0) {
        stall = 1;
        return;
    }
    return;
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