#ifndef _PROCUNITS_DEFINED_
#define _PROCUNITS_DEFINED_

#include "instruction.h"
#include "events.h"
#include "prochelpers.h"

class Processor;

namespace ProcUnitEvents
{
    const std::string POPULATE_RS = "put_instruction_into_reservation_station";
};

class ProcUnit: public EventDispatcher
{
    protected:
        Processor *processor;
        virtual void populateResultForwarder(Instructions::Instruction *instrPtr) {};
        virtual void validateDestReg(Instructions::Instruction *instrPtr) {};
        virtual void invalidateDestReg(Instructions::Instruction *instrPtr) {};
        virtual void pre(Instructions::Instruction *instrPtr) {};
        virtual void post(Instructions::Instruction *instrPtr) {};
    public:
        virtual void run(Instructions::Instruction *instr) {};
        bool isInstrBranch(Instructions::Instruction *instrPtr);
        ProcUnit();
};

#endif