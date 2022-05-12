#ifndef _PROCUNITS_DEFINED_
#define _PROCUNITS_DEFINED_

#include "instruction.h"
#include "events.h"
#include "prochelpers.h"

class Processor;

class ProcUnit
{
    protected:
        Processor *processor;
        conf* config;
        virtual void populateResultForwarder(Instructions::Instruction *instrPtr) {};
        virtual void validateDestReg(Instructions::Instruction *instrPtr) {};
        virtual void invalidateDestReg(Instructions::Instruction *instrPtr) {};
        virtual void pre(Instructions::Instruction *instrPtr) {};
        virtual void pre() {};
    public:
        bool busy;
        virtual void run(Instructions::Instruction *instr) {};
        virtual void run() {};
        virtual void post(Instructions::Instruction *instrPtr) {};
        virtual void post() {};
        virtual void nextTick() {};
        virtual void nextTick(Instructions::Instruction* instrPtr) {};
        virtual void flush() {};
        ProcUnit();
};

#endif