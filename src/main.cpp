#include "parser.h"
#include "constants.h"
#include "processor.h"
#include "prochelpers.h"
#include "pipeline.h"
#include "instruction.h"
#include "isa.h"
#include "util.h"
#include <iostream>
#include <string>
#include <map>
#include "termcolor.h"
#include <cassert>

int main(int argc, char* argv[]) {
    Processor *proc = Processor::fabricate();
    Scoreboard *sb = new Scoreboard();
    rs::ReservationStation* rs = new rs::ReservationStation(sb);
    Instructions::Instruction *ins1 = new Instructions::Instruction("abc");
    ins1->rd = $r4;
    ins1->rs = $r3;
    ins1->rt = $r2;
    Instructions::Instruction *ins2 = new Instructions::Instruction("abc");
    ins2->rd = $r4;
    ins2->rs = $r4;
    ins2->rt = $r1;
    Instructions::Instruction *ins3 = new Instructions::Instruction("abc");
    ins3->rd = $r5;
    ins3->rs = $r3;
    ins3->rt = $r4;
    Event<Instructions::Instruction*> event1 = Event<Instructions::Instruction*>();
    event1.set("ABC");
    event1.payload = ins1;
    Event<Instructions::Instruction*> event2 = Event<Instructions::Instruction*>();
    event2.set("ABC");
    event2.payload = ins2;
    Event<Instructions::Instruction*> event3 = Event<Instructions::Instruction*>();
    event3.set("ABC");
    event3.payload = ins3;
    rs->dispatch(event1);
    rs->dispatch(event2);
    rs->dispatch(event3);
    rs->print();
    // DecodeUnit *du = new ODecodeUnit(NULL);
    // du->run(NULL);
    
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