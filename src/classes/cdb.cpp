#include "cdb.h"
#include "processor.h"

// TODO: Add jump support here!

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
    std::cout << termcolor::bold << termcolor::on_bright_blue << "CDB Broadcast" << termcolor::reset << std::endl;
    rsv->populateTags(tag, value);
    lsq->populateTags(tag, value);
    robuff->populateEntry(tag, value);
    return;
}

// Write values to the register file. Invoked only by the reorder buffer.
void CommonDataBus::commit(Register destination, std::string tag, int value)
{
    if (destination == $noreg) return;
    if (destination == $pc) 
    {
        processor->PC = value;
        return;
    }
    if (sb->validate(destination, value, tag)) processor->registers[destination] = value;
    rsv->remove(tag);
    lsq->removeFromQueue(tag);
    return;
}