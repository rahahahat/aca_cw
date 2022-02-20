#include "processor.h"
/*---------------------------------------------------*/
/*---------------------Processor---------------------*/
/*---------------------------------------------------*/
Processor::Processor(Pipeline *pipe, Scoreboard *sb, ResultForwarder *rf)
{
    Parser pn = Parser(this);
    this->pipeline = pipe;
    this->parser = &pn;
    this->scoreboard = sb;
    this->resultForwarder = rf;
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

void Processor::attachProcUnit(FetchUnit *pu) 
{
    fUnit = pu;
    fUnit->attachToProcessor(this);
    return;
}

void Processor::attachProcUnit(DecodeUnit *pu)
{
    dUnit = pu;
    dUnit->attachToProcessor(this);
    return;
}

void Processor::attachProcUnit(ExecuteUnit *pu)
{
    eUnit = pu;
    eUnit->attachToProcessor(this);
    return;
}
void Processor::attachProcUnit(MemRefUnit *pu) {
    mrUnit = pu;
    mrUnit->attachToProcessor(this);
    return;
}

void Processor::attachProcUnit(WriteBackUnit *pu)
{
    wbUnit = pu;
    wbUnit->attachToProcessor(this);
    return;
};

void Processor::attachPipeline(Pipeline *pipe)
{
    pipeline = pipe;
    return;
};

void Processor::fetch(Instructions::Instruction *instrPtr)
{
    std::cout << "Starting Fetch" << std::endl;
    fUnit->fetch(instrPtr);
    return;
}

void Processor::decode(Instructions::Instruction *instrPtr)
{
    std::cout << "Starting Decode" << std::endl;
    dUnit->decode(instrPtr);
    return;
}

void Processor::execute(Instructions::Instruction *instrPtr)
{
    std::cout << "Starting Execute" << std::endl;
    eUnit->execute(instrPtr);
    return;
}

void Processor::memref(Instructions::Instruction *instrPtr)
{
    std::cout << "Starting MemoryAccess" << std::endl;
    mrUnit->memref(instrPtr);
    return;
}

void Processor::writeback(Instructions::Instruction *instrPtr)
{
    std::cout << "Starting WriteBack" << std::endl;
    wbUnit->writeback(instrPtr);
    return;
}

void Processor::runInstr(Instructions::Instruction *instrPtr)
{
    switch (instrPtr->stage)
    {
    case FETCH:
        fetch(instrPtr);
        break;
    case DECODE:
        decode(instrPtr);
        break;
    case EXECUTE:
        execute(instrPtr);
        break;
    case MEMORYACCESS:
        memref(instrPtr);
        break;
    case WRITEBACK:
        writeback(instrPtr);
        break;
    default:
        return;
    }
    instrPtr->nextPipeStage();
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

