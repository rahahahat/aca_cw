#include "processor.h"
/*-----------------------------------------------*/
/*-------------------Processor-------------------*/
/*-----------------------------------------------*/
Processor::Processor(Pipeline *pipe, pipelineType type) {
    FetchUnit fn = FetchUnit(this, type);
    this->pipeline = pipe;
    this->pipeType = type;
    this->fUnit = &fn;
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
};

void Processor::nonPipelinedExecution() {
    int clock = 0;
    while(PC != instrMemSize) {
        Instructions::Instruction *instr = this->fUnit->fetch();
        printClock(clock++);
        // decode
        // execute
        // mem
        //writeback
    };
};

/*-----------------------------------------------*/
/*-------------------FetchUnit-------------------*/
/*-----------------------------------------------*/
FetchUnit::FetchUnit(Processor *proc, pipelineType type) {
    processor = proc;
    pipeType = type;
    return;
};

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
};

/*-----------------------------------------------*/
/*-------------------DecodeUnit-------------------*/
/*-----------------------------------------------*/
void DecodeUnit::decodeRTypeInstruction(Instructions::Instruction *instrPtr, std::vector<std::string> splitInstr) 
{
    // Instruction format: opcode dest src1 src2
    instrPtr->rt = RegisterMap.at(splitInstr.back()); // src2
    splitInstr.pop_back();
    instrPtr->rs = RegisterMap.at(splitInstr.back()); // src1
    splitInstr.pop_back();
    instrPtr->rd = RegisterMap.at(splitInstr.back()); // dest
    splitInstr.pop_back();
};

void DecodeUnit::decodeJTypeInstruction(Instructions::Instruction *instrPtr, std::vector<std::string> splitInstr) 
{
    // Instruction format: opcode address
    instrPtr->immediateOrAddress = std::stoi(splitInstr.back(), 0, 16); // address
    splitInstr.pop_back();
};

void DecodeUnit::decode(Instructions::Instruction *instrPtr) {
    std::vector<std::string> splitInstr = splitString(instrPtr->instrString);
    std::pair<Opcodes, InstructionType> InsPair = InstructionPairMap.at(splitInstr.front());
    instrPtr->opcode = InsPair.first;
    instrPtr->type = InsPair.second;
    switch(InsPair.second) {
        case RType:
            decodeRTypeInstruction(instrPtr, splitInstr);
            return;
        case IType:
            return;
        case JType:
            decodeJTypeInstruction(instrPtr, splitInstr);
            return;
    };
};

/*-----------------------------------------------*/
/*-------------------Extras----------------------*/
/*-----------------------------------------------*/
void printInstructionMemory(Processor processor) {
    int size = processor.instrMemSize;
    for (int x = 0; x < size; x++) {
        std::cout << processor.instructionMemory[x] << std::endl;
    }
    return;
};

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
};

void printClock(int clock) {
    std::cout << "End of cycle: " << clock << std::endl;
    return;
};

