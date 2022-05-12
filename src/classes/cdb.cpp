#include "cdb.h"
#include "processor.h"
#include "util.h"
#include "pipeline.h"

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

void CommonDataBus::commitToMemory(int mem_addr, std::string tag, int value)
{
    broadcast($noreg, tag, value);
    processor->DataMemory[mem_addr] = value;
    lsq->removeFromQueue(tag);
}
// Write values to the register file. Invoked only by the reorder buffer.
void CommonDataBus::commit(Register destination, std::string tag, int value)
{

    // Second broadcast to make sure no tags remain after removal.
    broadcast(destination, tag, value);
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
    
    std::cout << "Destination: " << "$r" << destination << std::endl;
    std::cout << "Tag: " << tag << std::endl;
    std::cout << "Value: " << value << std::endl;

    if (sb->validate(destination, value, tag)) std::cout << "Validated" << std::endl;
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
    commit($pc, "~", pc_value);
}