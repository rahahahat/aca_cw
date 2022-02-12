#include "processor.h"
/*---------------------------------------------------*/
/*---------------------Processor---------------------*/
/*---------------------------------------------------*/
Processor::Processor(Pipeline *pipe, pipelineType type)
{
    FetchUnit fn = FetchUnit(this, type);
    DecodeUnit dn = DecodeUnit(type);
    Parser pn = Parser(this);
    this->pipeline = pipe;
    this->pipeType = type;
    this->fUnit = &fn;
    this->dUnit = &dn;
    this->parser = &pn;
};

void Processor::loadProgram(std::string fn)
{
    std::vector<std::string> program = parser->parseProgram(fn);
    for (auto it = std::begin(program); it != std::end(program); ++it) {
        loadInstructionIntoMemory(*it);
    }
    return;
}

void Processor::loadInstructionIntoMemory(std::string instruction)
{
    instructionMemory[instrMemSize] = instruction;
    instrMemSize++;
    return;
};

void Processor::attachPipeline(Pipeline *pipe, pipelineType type)
{
    pipeline = pipe;
    pipeType = type;
    return;
};

void Processor::nonPipelinedExecution()
{
    int clock = 0;
    while(PC != instrMemSize) {
        Instructions::Instruction *instr = this->fUnit->fetch();
        printClock(clock++);
        // decode
        // execute
        // mem
        // writeback
    };
};

/*---------------------------------------------------*/
/*---------------------FetchUnit---------------------*/
/*---------------------------------------------------*/
FetchUnit::FetchUnit(Processor *proc, pipelineType type)
{
    processor = proc;
    pipeType = type;
    return;
};

Instructions::Instruction* FetchUnit::fetch()
{
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

void FetchUnit::nonPipelinedFetch(Instructions::Instruction *instrPtr)
{
    std::string instr = processor->instructionMemory[processor->PC];
    Instructions::Instruction instruction = Instructions::Instruction(instr);
    instrPtr = &instruction;
    processor->PC++;
    return;
};

/*----------------------------------------------------*/
/*---------------------DecodeUnit---------------------*/
/*----------------------------------------------------*/
DecodeUnit::DecodeUnit(pipelineType tp) 
{
    pipeType = tp;
    return;
};

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

void DecodeUnit::decodeITypeInstruction(Instructions::Instruction *instrPtr, std::vector<std::string> splitInstr)
{
    // Instruction format: opcode r1 r2 immediate
    instrPtr->immediateOrAddress = std::stoi(splitInstr.back(), 0, 16); // immediate
    splitInstr.pop_back();
    instrPtr->rt = RegisterMap.at(splitInstr.back()); // r2
    splitInstr.pop_back();
    instrPtr->rs = RegisterMap.at(splitInstr.back()); // r1
    splitInstr.pop_back();
    return;
};

void DecodeUnit::decodeJTypeInstruction(Instructions::Instruction *instrPtr, std::vector<std::string> splitInstr) 
{
    // Instruction format: opcode address
    instrPtr->immediateOrAddress = std::stoi(splitInstr.back(), 0, 16); // address
    splitInstr.pop_back();
    return;
};

void DecodeUnit::decode(Instructions::Instruction *instrPtr)
{
    std::vector<std::string> splitInstr = splitString(instrPtr->instrString);
    std::pair<Opcodes, InstructionType> InsPair = InstructionPairMap.at(splitInstr.front());
    instrPtr->opcode = InsPair.first;
    instrPtr->type = InsPair.second;
    switch(InsPair.second)
    {
        case RType:
            decodeRTypeInstruction(instrPtr, splitInstr);
            return;
        case IType:
            decodeITypeInstruction(instrPtr, splitInstr);
            return;
        case JType:
            decodeJTypeInstruction(instrPtr, splitInstr);
            return;
    };
};

/*---------------------------------------------------*/
/*---------------------ExecuteUnit---------------------*/
/*---------------------------------------------------*/
ExecuteUnit::ExecuteUnit(Processor *procPtr, pipelineType type)
{   
    processor = procPtr;
    pipeType = type;
    return;
};

void ExecuteUnit::execute(Instructions::Instruction *instrPtr)
{
    switch(instrPtr->type)
    {
        case RType:
            return;
        case IType:
            return;
        case JType:
            return;
    };
};

void ExecuteUnit::executeJTypeInstruction(Instructions::Instruction *instrPtr) {};
void ExecuteUnit::executeITypeInstruction(Instructions::Instruction *instrPtr)
{
    Registers r1 = instrPtr->rs;
    Registers r2 = instrPtr->rt;
    int immediate = instrPtr->immediateOrAddress;
    int memAddr;
    switch (instrPtr->opcode)
    {
    case ADDIU:
        processor->registers[r1] = processor->registers[r2] + immediate;
        break;
    case LW:
        memAddr = processor->registers[r1] + immediate;
        processor->registers[r2] = processor->DataMemory[memAddr];
        break;
    case SW:
        memAddr = processor->registers[r1] + immediate;
        processor->DataMemory[memAddr] = processor->registers[r2];
        break;
    }
}
void ExecuteUnit::executeRTypeInstruction(Instructions::Instruction *instrPtr)
{
    Registers dest_rd = instrPtr->rd;
    Registers src1_rs = instrPtr->rs;
    Registers src2_rt = instrPtr->rt;
    switch (instrPtr->opcode)
    {
    case ADDU:
        processor->registers[dest_rd] = processor->registers[src1_rs] + processor->registers[src2_rt];
        break;
    case SUBU:
        processor->registers[dest_rd] = processor->registers[src1_rs] - processor->registers[src2_rt];
        break;
    case MULTU:
        // Ask about hi/lo 
        processor->registers[dest_rd] = processor->registers[src1_rs] * processor->registers[src2_rt];
        break;
    case DIVU:
        // Ask about hi/lo
        processor->registers[dest_rd] = processor->registers[src1_rs] / processor->registers[src2_rt];
        break;
    case AND:
        processor->registers[dest_rd] = processor->registers[src1_rs] & processor->registers[src2_rt];
        break;
    case OR:
        processor->registers[dest_rd] = processor->registers[src1_rs] | processor->registers[src2_rt];
        break;
    case NOR:
        processor->registers[dest_rd] = ~(processor->registers[src1_rs] | processor->registers[src2_rt]);
        break;
    case XOR:
        processor->registers[dest_rd] = processor->registers[src1_rs] ^ processor->registers[src2_rt];
        break;
    default:
        break;
    }
}
/*---------------------------------------------------*/
/*---------------------Extras------------------------*/
/*---------------------------------------------------*/
void printInstructionMemory(Processor processor)
{
    int size = processor.instrMemSize;
    for (int x = 0; x < size; x++) {
        std::cout << processor.instructionMemory[x] << std::endl;
    }
    return;
};

void printInstructionMemoryAtIndex(Processor processor, int index)
{
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

void printClock(int clock)
{
    std::cout << "End of cycle: " << clock << std::endl;
    return;
};

