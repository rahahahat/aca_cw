#include "processor.h"
#include "termcolor.h"
#include <functional>
#include <iostream>
#include "fetchUnit.h"
#include "decodeUnit.h"
#include "executeUnit.h"
#include "memoryUnit.h"

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

void Processor::attachLSQ(LSQueue  *queue)
{
    lsq = queue;
    return;
}

Processor* Processor::fabricate() {

    std::ifstream i("config.json");
    i >> config;
    std::cout << config["program"] << std::endl;
    
    Processor *processor = Processor::getProcessorInstance();
    Pipeline *pipeline = new OoOPipeline();
    Scoreboard *scoreboard = new Scoreboard(false);
    rs::ReservationStation* rs = new rs::ReservationStation(scoreboard, false);
    LSQueue *queue = new LSQueue();
    Parser *parser = new Parser(processor);
    int num_fetch_units = config["proc_units"]["fetch"].get<int>();
    int num_decode_units = config["proc_units"]["decode"].get<int>();
    int num_exec_units = config["proc_units"]["execute"].get<int>();
    int num_mem_units = config["proc_units"]["memory"].get<int>();
    processor->num_proc_units = {
        {FETCHUNIT, new std::pair<int, int>(num_fetch_units,num_fetch_units)},
        {DECODEUNIT, new std::pair<int, int>(num_decode_units, num_decode_units)},
        {EXECUTEUNIT, new std::pair<int, int>(num_exec_units, num_exec_units)},
        {MEMORYUNIT, new std::pair<int, int>(num_mem_units, num_mem_units)},
    };

    processor->proc_units = {
        {FETCHUNIT, new FetchUnit()},
        {DECODEUNIT, new ODecodeUnit()},
        {EXECUTEUNIT, new OExecuteUnit()},
        {MEMORYUNIT, new OMemoryUnit()}
    };

    processor->attachParser(parser);
    processor->attachPipeline(pipeline);
    processor->attachProcHelper(rs);
    processor->attachProcHelper(scoreboard);
    processor->attachLSQ(queue);

    return processor;
}

void Processor::destroy(Processor *processor) {

    // delete processor->resultForwarder;
    delete processor->scoreboard;
    delete processor->pipeline;
    delete processor;
    return;
}

Pipeline* Processor::getPipeline()
{
    return pipeline;
}

ProcUnit* Processor::getProcUnit(ProcUnitTypes unit)
{
    auto itr = proc_units.find(unit);
    if (itr == proc_units.end()) return NULL;
    return itr->second;
}

Scoreboard* Processor::getSB()
{
    return scoreboard;
}

rs::ReservationStation* Processor::getRS()
{
    return reservation_station;
}

LSQueue* Processor::getLsq()
{
    return lsq;
}

void Processor::attachParser(Parser *psr)
{
    parser = psr;
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

// void Processor::attachProcHelper(ResultForwarder *rf) {

//     resultForwarder = rf;
//     return;
// }

void Processor::attachProcHelper(Scoreboard *sb)
{
    scoreboard = sb;
    return;
}

void Processor::attachProcHelper(rs::ReservationStation* rs)
{
    reservation_station = rs;
    return;
}

void Processor::fetch(Instructions::Instruction *instrPtr) {
    std::pair<int, int> *num_units = num_proc_units[FETCHUNIT];
    
    if (num_units->second > 0)
    {
        
        num_units->second -= 1;
        proc_units[FETCHUNIT]->run(instrPtr);
        return;
    }
    return;
}

void Processor::decode(Instructions::Instruction *instrPtr) {
    std::pair<int, int> *num_units = num_proc_units[DECODEUNIT];
    if (num_units->second > 0)
    {
        num_units->second -= 1;
        proc_units[DECODEUNIT]->run(instrPtr);
        return;
    }
}

void Processor::execute(Instructions::Instruction *instrPtr) {

    std::pair<int, int> *num_units = num_proc_units[EXECUTEUNIT];
    if (num_units->second > 0)
    {
        num_units->second -= 1;
        proc_units[EXECUTEUNIT]->run(instrPtr);
        return;
    }
}

void Processor::memref(Instructions::Instruction *instrPtr) {

    std::pair<int, int> *num_units = num_proc_units[MEMORYUNIT];
    if (num_units->second > 0)
    {
        num_units->second -= 1;
        proc_units[MEMORYUNIT]->run(instrPtr);
        return;
    }
}

void Processor::writeback(Instructions::Instruction *instrPtr) {

    std::pair<int, int> *num_units = num_proc_units[MEMORYUNIT];
    if (num_units->second > 0)
    {
        num_units->second -= 1;
        proc_units[MEMORYUNIT]->run(instrPtr);
        return;
    }
}

void Processor::resetProcResources()
{
    for (auto it = num_proc_units.begin(); it != num_proc_units.end(); it++)
    {
        it->second->second = it->second->first;
    }
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
}

void Processor::runProgram() { 

    int count = 0;
    // Starting execution by putting instruction in pipeline
    count += 1;
    bool isPipelineEmpty = false;
    while(!isPipelineEmpty)
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
        resetProcResources();
        // if (pipeline->flush)
        // {
        //     pipeline->flushPipelineOnBranchOrJump();
        // }
        if (PC < instrMemSize && !pipeline->stalled())
        {   
            fetch(pipeline->addInstructionToPipeline(count));
            count += 1;
        };
        // resultForwarder->memDump();
        // scoreboard->memDump();
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
        isPipelineEmpty = pipeline->isEmpty();
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

