#include "processor.h"
/*---------------------------------------------------*/
/*---------------------Processor---------------------*/
/*---------------------------------------------------*/
Processor::Processor(Pipeline *pipe, pipelineType type)
{
    FetchUnit fn = FetchUnit(this, pipe, type);
    DecodeUnit dn = DecodeUnit(this, pipe, type);
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
    const char ch = instruction.back();
    if (ch == ':') {
        instruction.pop_back();
        this->labelMap.insert(std::pair<std::string, int>(instruction, instrMemSize));
        return;
    }
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

/*-----------------------------------------------------*/
/*----------------------MemRefUnit---------------------*/
/*-----------------------------------------------------*/
MemRefUnit::MemRefUnit(Processor *proc, Pipeline *pl, pipelineType tp) 
{
    processor = proc;
    pipeType = tp;
    pipeline = pl;
    return;
};

void MemRefUnit::ref(Instructions::Instruction *instrPtr) 
{
    switch (instrPtr->opcode)
    {
    case LW:
        instrPtr->temp = processor->DataMemory[instrPtr->temp];
        break;
    case SW:
        processor->DataMemory[instrPtr->temp] = processor->registers[instrPtr->rt];
        break;
    }
};

/*-----------------------------------------------------*/
/*----------------------WriteBackUnit------------------*/
/*-----------------------------------------------------*/
WriteBackUnit::WriteBackUnit(Processor *proc, Pipeline *pl ,pipelineType tp)
{
    processor = proc;
    pipeType = tp;
    pipeline = pl;
    return;
}

void WriteBackUnit::writeBack(Instructions::Instruction *instrPtr)
{
    switch (instrPtr->opcode)
    {
    case ADD:
    case ADDU:
        processor->registers[instrPtr->rd] = instrPtr->temp;
        break;
    case ADDI:
    case ADDIU:
        processor->registers[instrPtr->rt] = instrPtr->temp;
        break;
    case SUB:
    case SUBU:
        processor->registers[instrPtr->rd] = instrPtr->temp;
        break;
    case MULT:
    case MULTU:
        processor->registers[instrPtr->rd] = instrPtr->temp;
        break;
    case DIV:
    case DIVU:
        processor->registers[instrPtr->rd] = instrPtr->temp;
        break;
    case AND:
        processor->registers[instrPtr->rd] = instrPtr->temp;
        break;
    case ANDI:
        processor->registers[instrPtr->rt] = instrPtr->temp;
        break;
    case OR:
        processor->registers[instrPtr->rd] = instrPtr->temp;
        break;
    case ORI:
        processor->registers[instrPtr->rt] = instrPtr->temp;
        break;
    case NOR:
        processor->registers[instrPtr->rd] = instrPtr->temp;
        break;
    case XOR:
        processor->registers[instrPtr->rd] = instrPtr->temp;
        break;
    case LW:
        processor->registers[instrPtr->rt] = instrPtr->temp;
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

