#include "pipeline.h"
#include "processor.h"

void Pipeline::addInstructionToPipeline(Instructions::Instruction instr) {};
std::vector<Instructions::Instruction>Pipeline::getInstructionsInPipeline() {
    return instructions;
}

// void ScalarPipeline::addInstructionToPipeline(Instructions::Instruction instr) {
//     if (instructions.size() == 0) {
//         instructions.push_back(instr);
//         return;
//     }
//     Instructions::Instruction instruction = instructions.back();
//     pipestage stage = instruction.getCurrentPipeStage();
//     if (stage != DONE || stage != INIT) {
//         std::cerr << "Current instruction in simple pipeline is not done, can't add more." << std::endl;
//         return;
//     }
//     instructions.pop_back();
//     instructions.push_back(instr);
//     return;
// };

