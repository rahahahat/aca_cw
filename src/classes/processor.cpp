#include "processor.h"
#include "termcolor.h"
#include <functional>
#include <iostream>

/*---------------------------------------------------*/
/*---------------------Processor---------------------*/
/*---------------------------------------------------*/
Processor::Processor() {

    Parser *pn = new Parser(this);
    this->parser = pn;
    this->clock = 0;

};

void Processor::loadProgram(std::string fn) {

    std::vector<std::string> program = parser->parseProgram(fn);
    for (auto it = std::begin(program); it != std::end(program); ++it) {
        loadInstructionIntoMemory(*it);
    }
    return;
}

Processor* Processor::fabricate() {

    // ScalarPipeline *pipeline = new ScalarPipeline();
    // Scoreboard *scoreboard = new Scoreboard();
    // ResultForwarder *resultForwarder = new ResultForwarder();
    Processor *processor = new Processor();
    processor->num_proc_units = {
        {EXECUTE, new std::pair<int, int>(1,1)},
        {MEMORYACCESS, new std::pair<int, int>(1,1)},
        {WRITEBACK, new std::pair<int, int>(1,1)},
    };
    // processor->proc_units = {
    //     {FETCH, new FetchUnit(NULL, 1)},
    //     {DECODE, new ODecodeUnit(NULL, 1)},
    //     {EXECUTE, new OExecuteUnit(NULL, 1)},
    //     {MEMORYACCESS, new MemoryUnit(NULL, 1)},
    //     {WRITEBACK, new WriteBackUnit(NULL, 1)}
    // };
    // pipeline->attachToProcessor(processor);
    // processor->attachPipeline(pipeline);
    // FetchUnit *fn = new FetchUnit(pipeline);
    // DecodeUnit *dn = new DecodeUnit(pipeline);
    // ExecuteUnit *en = new ExecuteUnit(pipeline);
    // MemRefUnit *mrf = new MemRefUnit(pipeline);
    // WriteBackUnit *wb = new WriteBackUnit(pipeline);
    // processor->attachProcHelper(resultForwarder);
    // processor->attachProcHelper(scoreboard);
    // processor->attachProcUnit(fn);
    // processor->attachProcUnit(dn);
    // processor->attachProcUnit(en);
    // processor->attachProcUnit(mrf);
    // processor->attachProcUnit(wb);
    // auto func = std::bind(&ScalarPipeline::stallPipelineOnEvent, pipeline, std::placeholders::_1);
    // EventWrapper::getEventWrapperInstance()->addEventListerner(PipelineEvents::StallPipelineEvent, func);

    return NULL;
}
 
void Processor::destroy(Processor *processor) {

    delete processor->resultForwarder;
    delete processor->scoreboard;
    delete processor->pipeline;
    delete processor;
    return;
}


void Processor::loadInstructionIntoMemory(std::string instruction) {

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

void Processor::attachPipeline(Pipeline *pipe) {

    pipeline = pipe;
    return;
};

void Processor::attachProcHelper(ResultForwarder *rf) {

    resultForwarder = rf;
    return;
}

void Processor::attachProcHelper(Scoreboard *sb)
{
    scoreboard = sb;
    return;
}

void Processor::fetch(Instructions::Instruction *instrPtr) {

    // proc_units[FETCH]->run(instrPtr);
    return;
}

void Processor::decode(Instructions::Instruction *instrPtr) {

    // proc_units[DECODE]->run(instrPtr);
    return;
}

void Processor::execute(Instructions::Instruction *instrPtr) {

    // proc_units[EXECUTE]->run(instrPtr);
    return;
}

void Processor::memref(Instructions::Instruction *instrPtr) {

    // proc_units[MEMORYACCESS]->run(instrPtr);
    return;
}

void Processor::writeback(Instructions::Instruction *instrPtr) {

    // proc_units[WRITEBACK]->run(instrPtr);
    return;
}

void Processor::runInstr(Instructions::Instruction *instrPtr) {

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
    if (!pipeline->stalled()) {
        instrPtr->nextPipeStage();
    }
}

void Processor::runProgram() { 

    int count = 0;
    // Starting execution by putting instruction in pipeline
    Instructions::Instruction instr = Instructions::Instruction();
    pipeline->addInstructionToPipeline(count);
    count += 1;
    while(!pipeline->isEmpty())
    {
        clock++;
        std::cout
        << termcolor::bold
        << "Clock cycle: "
        << termcolor::green
        << clock
        << termcolor::white
        << " starting -----------------------------------------\n"
        << termcolor::reset
        << std::endl;
        pipeline->pipeInstructionsToProcessor();
        pipeline->removeCompletedInstructions();
        // if (pipeline->flush)
        // {
        //     pipeline->flushPipelineOnBranchOrJump();
        // }
        if (PC < instrMemSize && pipeline->getInstrSize() < 5)
        {    
            pipeline->addInstructionToPipeline(count);
            count += 1;
        };
        resultForwarder->memDump();
        scoreboard->memDump();
        regDump();
        std::cout
        << termcolor::bold
        << "Clock cycle: "
        << termcolor::green
        << clock
        << termcolor::white
        << " has Ended -----------------------------------------"
        << termcolor::reset
        << std::endl;
        std::cout << std::endl;
    }
    std::cout << "Program has ended!" << std::endl;
    std::cout << "Clock: " << clock << std::endl;
    return;
}

/*---------------------------------------------------*/
/*---------------------Extras------------------------*/
/*---------------------------------------------------*/
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

void Processor::regDump() {
    std::cout 
    << termcolor::bold
    << "-----------RegDump-----------" 
    << std::endl;
    for (int i = 0; i < 32; i++)
    {
        std::cout
        << termcolor::bold
        << termcolor::green
        << "$r"
        << i
        << ":\t\t"
        << termcolor::white
        << registers[i]
        << termcolor::reset
        << std::endl;
    }
    std::cout << std::endl;
}