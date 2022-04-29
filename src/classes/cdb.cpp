#include "cdb.h"
#include "processor.h"

// TODO: Add jump support here!

CommonDataBus::CommonDataBus()
{
    processor = Processor::getProcessorInstance();
    rsv  = Processor::getProcessorInstance()->getRS();
    sb  = Processor::getProcessorInstance()->getSB();
    lsq = Processor::getProcessorInstance()->getLsq();
}

void CommonDataBus::broadcast(Register destination, std::string tag, int value)
{    
    rsv->populateTags(tag, value);
    lsq->populateTags(tag, value);
    
    if (destination == $noreg) return;
    if (destination == $pc) 
    {
        processor->PC = value;
        return;
    }
    if (sb->validate(destination, value, tag)) processor->registers[destination] = value;
    return;
}