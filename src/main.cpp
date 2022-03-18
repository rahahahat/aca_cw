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

int main(int argc, char* argv[]) {
    DecodeUnit *du = new ODecodeUnit(NULL);
    du->run(NULL);
    
    // processor->DataMemory[0] = 1;
    // processor->DataMemory[1] = 1;

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