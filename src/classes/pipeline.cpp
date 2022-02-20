#include "pipeline.h"

void Pipeline::attachToProcessor(Processor *proc) {
    processor = proc;
};

pipelineType Pipeline::getType()
{
    return None;
}

ScalarPipeline::ScalarPipeline()
{
    processor = NULL;
};

pipelineType ScalarPipeline::getType()
{
    return Scalar;
}

void ScalarPipeline::addInstructionToPipeline(Instructions::Instruction *instr) {
    if (instr == NULL) {
        return;
    }
    instructions.push_back(instr);
    return;
};

void ScalarPipeline::pipeInstructionsToProcessor() {
    for (auto instr = begin(instructions); instr != end(instructions); ++instr) {
        processor->runInstr(*instr);
    }
    // Fetch at each clock cycle.
    // if (instructions.size() < 3)
    // {
    //     Instructions::Instruction* lastestFetch = processor->fetch();
    //     addInstructionToPipeline(lastestFetch);
    // }
}

