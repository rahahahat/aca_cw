#include "cdb.h"
#include "processor.h"
#include "util.h"
#include "pipeline.h"
#include "termcolor.h"

// #################################################################################################
// CommonDataBus
// #################################################################################################

CommonDataBus::CommonDataBus()
{
    processor = Processor::getProcessorInstance();
    rsv  = Processor::getProcessorInstance()->getRS();
    sb  = Processor::getProcessorInstance()->getSB();
    lsq = Processor::getProcessorInstance()->getLsq();
    robuff = Processor::getProcessorInstance()->getRB();
}
// Broadcast results to LSQ, RS and ROBUFF. Invoked by ExecuteUnit and MemoryUnit.
// Tags and corresponding values are passed on to each component. Components are
// responsible to handling updates individually.
void CommonDataBus::broadcast(Register destination, std::string tag, int value)
{    
    rsv->populateTags(tag, value);
    lsq->populateTags(tag, value);
    robuff->populateEntry(tag, value);
    return;
}

void CommonDataBus::broadcast(int mem_addr, std::string tag, int value)
{    
    rsv->populateTags(tag, value);
    lsq->populateTags(tag, value);
    robuff->populateEntry(tag, value, mem_addr);
    return;
}

void CommonDataBus::commitToMemory(int mem_addr, std::string tag, int value, std::string instrStr)
{
    broadcast($noreg, tag, value);
    bool print = getConfig()->debug->print;
    IF_PRINT(
        std::cout << termcolor::bright_green
        << termcolor::bold 
        << "Writing store to memory: " 
        << termcolor::white << instrStr << termcolor::reset << std::endl;
    );
    processor->DataMemory[mem_addr] = value;
    lsq->removeFromQueue(tag);
}
// Write values to the register file. Invoked only by the reorder buffer.
void CommonDataBus::commit(Register destination, std::string tag, int value, std::string instrStr)
{
    bool print = getConfig()->debug->print;
    // Second broadcast to make sure no tags remain after removal.
    broadcast(destination, tag, value);

    processor->total_commits++;
    
    IF_PRINT(
        std::cout << termcolor::bright_green
        << termcolor::bold 
        << "Comitting instruction: " 
        << termcolor::white << instrStr << termcolor::reset << std::endl;
    );

    if (destination == $noreg) 
    {
        rsv->remove(tag);
        lsq->removeFromQueue(tag);
        return;
    }

    if (destination == $pc) 
    {
        rsv->remove(tag);
        processor->PC = value;
        return;
    }

    sb->validate(destination, value, tag);
    processor->registers[destination] = value;
    
    rsv->remove(tag);
    lsq->removeFromQueue(tag);
    return;
}

void CommonDataBus::flushAll(int pc_value)
{
    rsv->flush();
    lsq->flush();
    sb->flush();
    processor->getPipeline()->flush();
    commit($pc, "~", pc_value, "Commiting PC on Flush");
}