#ifndef _PROCUNITS_DEFINED_
#define _PROCUNITS_DEFINED_

#include "processor.h"
#include "pipeline.h"
#include "events.h"


class Processor;
class Pipeline;

class ProcUnit: public EventDispatcher
{
    protected:
        Processor *processor;
        Pipeline *pipeline;
        virtual void populateResultForwarder(Instructions::Instruction *instrPtr) {};
        virtual void validateDestReg(Instructions::Instruction *instrPtr) {};
        virtual void invalidateDestReg(Instructions::Instruction *instrPtr) {};
        virtual void pre(Instructions::Instruction *instrPtr);
        virtual void post(Instructions::Instruction *instrPtr);
    public:
        const int total_units;
        int available_units;
        void attachToProcessor(Processor *proc);
        virtual void run(Instructions::Instruction * instr) {};
        ProcUnit(int units);
};

#endif