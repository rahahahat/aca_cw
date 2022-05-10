#include <functional>
#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>

#include "processor.h"
#include "termcolor.h"
#include "lsq.h"
#include "config.h"
#include "cdb.h"
#include "robuff.h"
#include "util.h"
#include "branch.h"

// #################################################################################################
// Processor
// #################################################################################################

Processor::Processor() {

    Parser *pn = new Parser(this);
    this->parser = pn;
    this->clock = 0;
    this->btb = new BranchTargetBuffer();
};

void Processor::loadProgram(std::string fn) {

    std::vector<std::string> program = parser->parseProgram(fn);
    for (auto it = std::begin(program); it != std::end(program); ++it) {
        loadInstructionIntoMemory(*it);
    }
    printInstructionMemory(this);
    printLabelMap();
    return;
};

void Processor::attachLSQ(LSQueue *queue)
{
    lsq = queue;
    return;
};

void Processor::attachCDB(CommonDataBus *bus)
{
    cdb = bus;
    return;
};

void Processor::attachReorderBuffer(ReorderBuffer *rb)
{
    robuff = rb;
    return;
};

Processor* Processor::fabricate() {
    
    Scoreboard *scoreboard = new Scoreboard(false);
    rs::ReservationStation* rs = new rs::ReservationStation(scoreboard, false);
    Pipeline *pipeline = new OoOPipeline();
    LSQueue *queue = new LSQueue();
    Parser *parser = new Parser(this);
    ReorderBuffer *robuff = new ReorderBuffer(64);
    BranchTargetBuffer* btb = new BranchTargetBuffer();
    BranchPredictor* brp = new BranchPredictor(btb);

    // TODO: Validate attach order for ROB
    attachProcHelper(scoreboard);
    attachProcHelper(rs);
    attachParser(parser);
    attachPipeline(pipeline);
    attachLSQ(queue);
    attachReorderBuffer(robuff);
    attachBTB(btb);
    attachBranchPredictor(brp);

    CommonDataBus *bus = new CommonDataBus();
    attachCDB(bus);

    return this;
};

void Processor::destroy(Processor *processor) {

    // delete processor->resultForwarder;
    delete processor->scoreboard;
    delete processor->pipeline;
    delete processor;
    return;
};

Pipeline* Processor::getPipeline()
{
    return pipeline;
};

Scoreboard* Processor::getSB()
{
    return scoreboard;
};

CommonDataBus* Processor::getCDB()
{
    return cdb;
};

rs::ReservationStation* Processor::getRS()
{
    return reservation_station;
};

LSQueue* Processor::getLsq()
{
    return lsq;
};

ReorderBuffer* Processor::getRB()
{
    return robuff;
};

BranchTargetBuffer* Processor::getBTB()
{
    return btb;
};

BranchPredictor* Processor::getPredictor()
{
    return predictor;
};

void Processor::loadDataMemory()
{
    nlohmann::json jdata;
    std::ifstream i("data.json");
    i >> jdata;
    i.close();
    std::vector<int> data = jdata.get<std::vector<int>>();
    for (int i = 0; i < data.size(); i++)
    {
        DataMemory[i] = data.at(i);
    }
};

void Processor::dumpDataMemory()
{
    conf* config = getConfig();
    int max_iter = config->print->all ? 1024 : config->print->num_bytes;
    std::ofstream output;
    output.open("sim.out");
    for (int i = 0; i < max_iter; i++)
    {
        output << i << ": " << DataMemory[i] << "\t\t\t\t\t\t\t";
        if ((i+1) % 5 == 0 && i != 0)
        {
            output << std::endl;
        }
    }
};

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

void Processor::attachParser(Parser *psr)
{
    parser = psr;
    return;
};


void Processor::attachPipeline(Pipeline *pipe) {

    pipeline = pipe;
    return;
};

void Processor::attachProcHelper(Scoreboard *sb)
{
    scoreboard = sb;
    return;
};

void Processor::attachProcHelper(rs::ReservationStation* rs)
{
    reservation_station = rs;
    return;
};

void Processor::attachBranchPredictor(BranchPredictor* bpr)
{
    predictor = bpr;
    return;
}

void Processor::attachBTB(BranchTargetBuffer* bt)
{
    btb = bt;
    return;
}

void Processor::runProgram() { 

    conf* config = getConfig();
    this->clock = 0;
    int stopTime = config->stop_time;
    while(!progEnded)
    {
        this->clock++;
        printCycleStart(this->clock);

        stepMode();
        
        if (pipeline->stalledBy() == Flush) pipeline->resumePipeline(Flush);        
        pipeline->nextTick(this->clock);
        
        setProgramEnded();
        std::this_thread::sleep_for(std::chrono::milliseconds(stopTime));
    }
    regDump();
    printProgramEnd(this->clock);
    return;
};

void Processor::setProgramEnded()
{
    bool ended = true;
    ended &= robuff->getSize() == 0;
    ended &= reservation_station->areAllEntriesFree();
    ended &= pipeline->areAllProcUnitsFree();
    ended &= lsq->getNumEntries() == 0;
    ended &= pipeline->stalledBy() == Halt;
    progEnded = ended;
};

bool Processor::programEnded()
{
    return progEnded;
};



void Processor::regDump() {
    std::cout << std::endl;

    std::cout 
    << "\n"
    << termcolor::bright_green
    << "|Register\t|"
    << "Value\t\t\t|"
    << std::endl;
    for (int i = 0; i < 32; i++)
    {
        std::cout
        << termcolor::bright_blue
        << "|$r"
        << i
        << "\t\t|"
        << registers[i]
        << "\t\t\t|"
        << std::endl;
    }
    std::cout
    << "|$pc"
    << "\t\t|"
    << PC
    << "\t\t\t|"
    << "\n"
    << termcolor::reset
    << std::endl;

    std::cout << std::endl;
};

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
};

void Processor::stepMode()
{
    if (!getConfig()->debug->enabled) return;

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
            std::cout << "Print scoreboard (sb)" << std::endl;
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
        if (!ss.compare("sb"))
        {
            scoreboard->memDump();
        }
        if (!ss.compare("rb"))
        {
            robuff->print();
        }
        ss = "";
	}
};

// #################################################################################################
// Extras
// #################################################################################################

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