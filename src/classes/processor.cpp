#include "processor.h"


Processor::Processor(Pipeline *pipe, pipelineType type) {
    this->pipeline = pipe;
    this->pipeType = type;
    this->fUnit = &FetchUnit(this, type);
};

void Processor::loadInstructionIntoMemory(std::string instruction) {
    instructionMemory[instrMemSize] = instruction;
    instrMemSize++;
    return;
};

void Processor::attachPipeline(Pipeline *pipe, pipelineType type) {
    pipeline = pipe;
    pipeType = type;
    return;
}

void Processor::fetch() {
    std::string instr = instructionMemory[PC];
    Instructions::Instruction instruction = Instructions::Instruction(instr);
    pipeline->addInstructionToPipeline(instruction);
    PC++;
    return;
}

void Processor::nonPipelinedExecution() {
    int clock = 0;
    while(PC != instrMemSize) {
        auto instr = this->fUnit->fetch();
        printClock(clock++);
        // decode
        // execute
        // mem
        //writeback
    }
}

void printClock(int clock) {
    std::cout << "End of cycle: " << clock << std::endl;
    return;
}

FetchUnit::FetchUnit(Processor *proc, pipelineType type) {
    processor = proc;
    pipeType = type;
    return;
}

Instructions::Instruction* FetchUnit::fetch() {
    switch(pipeType) {
        case Scalar:
            return NULL;
        default:
           Instructions::Instruction *ptr = NULL;
           nonPipelinedFetch(ptr);
           return ptr;
    }
    return NULL;
};

void FetchUnit::nonPipelinedFetch(Instructions::Instruction *instrPtr) {
    std::string instr = processor->instructionMemory[processor->PC];
    Instructions::Instruction instruction = Instructions::Instruction(instr);
    instrPtr = &instruction;
    processor->PC++;
    return;
} 

void printInstructionMemory(Processor processor) {
    int size = processor.instrMemSize;
    for (int x = 0; x < size; x++) {
        std::cout << processor.instructionMemory[x] << std::endl;
    }
    return;
}

void printInstructionMemoryAtIndex(Processor processor, int index) {
    if (index > 512) {
        std::cerr << "Error in printInstructionMemoryAtIndex: index " << index << " is greater than 512" << std::endl;
        return;
    }
    if (index > processor.instrMemSize - 1) {
        std::cerr << "Error in printInstructionMemoryAtIndex: index " << index << " is greater than number of instructions currently in memory." << std::endl;
        return;
    }
    std::cout << processor.instructionMemory[index] << std::endl;
    return;
}
