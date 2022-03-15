#include "parser.h"
#include "constants.h"
#include "processor.h"
#include "pipeline.h"
#include "procUnits.h"
#include "instruction.h"
#include "isa.h"
#include "util.h"
#include <iostream>
#include <string>
#include <map>
#include "termcolor.h"
#include <cassert>


// Event<T> *new_event = dynamic_cast<T*>(event);

void printBold(EventBase *base)
{
    Event<int> *new_event = dynamic_cast<Event<int>*>(base);
    std::cout << "Hello1" << new_event->payload << std::endl;
}

int main(int argc, char* argv[]) {
    std::cout << termcolor::on_green << "Starting" << termcolor::reset << std::endl;
    Processor *processor = Processor::fabricate();
    processor->loadProgram("./factorial.txt");
    std::cout << "ProgramSize: " << processor->instrMemSize <<std::endl;
    // processor->DataMemory[0] = 1;
    // processor->DataMemory[1] = 1;
    processor->runProgram();
    Processor::destroy(processor);

    // EventWrapper* eventWrapper = EventWrapper::getEventWrapperInstance();
    // EventDispatcher dispatch1 = EventDispatcher();
    // function_ptr func_ptr = &printBold;
    // eventType type = "printbold";
    // eventWrapper->addEventListerner(type, func_ptr);
    // Event<int> *event = new Event<int>();
    // event->setType(type);
    // event->payload = 1;
    // // eventWrapper.disptachEventToListerner(event);
    // dispatch1.dispatch(event);
    return 0;
}