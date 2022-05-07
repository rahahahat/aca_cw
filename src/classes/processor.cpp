#include <functional>
#include <iostream>
#include <chrono>
#include <thread>

#include "processor.h"
#include "termcolor.h"
#include "lsq.h"
#include "config.h"
#include "cdb.h"
#include "robuff.h"
#include "util.h"

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
    printInstructionMemory(this);
    printLabelMap();
    return;
}

void Processor::attachLSQ(LSQueue *queue)
{
    lsq = queue;
    return;
}

void Processor::attachCDB(CommonDataBus *bus)
{
    cdb = bus;
    return;
}

void Processor::attachReorderBuffer(ReorderBuffer *rb)
{
    robuff = rb;
    return;
}

Processor* Processor::fabricate() {
    
    std::ifstream i("config.json");
    i >> config;
    i.close();

    
    Scoreboard *scoreboard = new Scoreboard(false);
    rs::ReservationStation* rs = new rs::ReservationStation(scoreboard, false);
    Pipeline *pipeline = new OoOPipeline();
    LSQueue *queue = new LSQueue();
    Parser *parser = new Parser(this);
    ReorderBuffer *robuff = new ReorderBuffer(64);

    // TODO: Validate attach order for ROB
    attachProcHelper(scoreboard);
    attachProcHelper(rs);
    attachParser(parser);
    attachPipeline(pipeline);
    attachLSQ(queue);
    attachReorderBuffer(robuff);

    CommonDataBus *bus = new CommonDataBus();
    attachCDB(bus);

    return this;
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

Scoreboard* Processor::getSB()
{
    return scoreboard;
}

CommonDataBus* Processor::getCDB()
{
    return cdb;
}

rs::ReservationStation* Processor::getRS()
{
    return reservation_station;
}

LSQueue* Processor::getLsq()
{
    return lsq;
}

ReorderBuffer* Processor::getRB()
{
    return robuff;
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

void Processor::runProgram() { 

    int clock = 0;
    while(!progEnded)
    {
        stepMode();
        clock++;
        printCycleStart(clock, pipeline->getInstrSize());        
        pipeline->nextTick(clock);
        if (config["debug"].get<bool>()) regDump();
        setProgramEnded();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    regDump();
    reservation_station->print();
    robuff->print();
    printProgramEnd(clock);
    return;
}

void Processor::setProgramEnded()
{
    bool ended = true;
    ended &= reservation_station->areAllEntriesFree();
    // std::cout << termcolor::bold << termcolor::red << "Ended RS: " << ended << std::endl;
    ended &= pipeline->areAllProcUnitsFree();
    // std::cout << termcolor::bold << termcolor::red << "Ended PU: " << ended << std::endl;
    ended &= lsq->getNumEntries() == 0;
    // std::cout << termcolor::bold << termcolor::red << "Ended LS: " << ended << std::endl;
    ended &= pipeline->stalledBy() == Halt;
    // std::cout << termcolor::bold << termcolor::red << "Ended HALT: " << ended << std::endl;
    progEnded = ended;
    // progEnded = false;
};

bool Processor::programEnded()
{
    return progEnded;
};

/*---------------------------------------------------*/
/*---------------------Extras------------------------*/
/*---------------------------------------------------*/

void printInstructionMemory(Processor *processor) {

    int size = processor->instrMemSize;
    for (int x = 0; x < size; x++) {
        std::cout << processor->instructionMemory[x] << ": " << termcolor::red << x << termcolor::reset << std::endl;
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
    std::cout
    << termcolor::bold
    << termcolor::green
    << "$pc"
    << ":\t\t"
    << termcolor::white
    << PC
    << termcolor::reset
    << std::endl;
    std::cout << std::endl;
}

void Processor::printLabelMap()
{
    for (auto it = labelMap.begin(); it != labelMap.end(); ++it)
    {
        std::cout
        << termcolor::bold
        << termcolor::green
        << "Label: "
        << it->first
        << " points to: "
        << termcolor::blue
        << it->second
        << termcolor::reset
        << std::endl;
    }
}
void Processor::stepMode()
{
    auto mode = config["stepMode"];
    if (!(mode["enabled"].get<bool>() && mode["cycle"].get<bool>())) return;

	std::string ss;
	while(1)
	{
        std::cout << "\nStep through cycle (Enter -h for help): ";
        std::cin >> ss;
        
        if (!ss.compare("n")) break;
        if (!ss.compare("-h"))
        {
            std::cout << "Execute next cycle (n)" << std::endl;
            std::cout << "Print registers (rg)" << std::endl;
            std::cout << "Print reservation station (rs)" << std::endl;
            std::cout << "Print load/store queue (ls)" << std::endl;
            std::cout << "Print reorder buffer (rb)" << std::endl;
        }
		if (!ss.compare("rg"))
		{
            regDump();
		}
        if (!ss.compare("rs"))
        {
            reservation_station->print();
        }
        if (!ss.compare("ls"))
        {
            lsq->print();
        }
        if (!ss.compare("rb"))
        {
            robuff->print();
        }
        ss = "";
	}
}