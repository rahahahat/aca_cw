#include "procUnit.h"
#include "lsq.h"

#ifndef _MEMORYUNIT_DEFINED_
#define _MEMORYUNIT_DEFINED_

class MemoryUnit: public ProcUnit
{
    protected:
        // virtual void populateResultForwarder(Instructions::Instruction *instrPtr);
        virtual void load(Instructions::Instruction *instrPtr);
        virtual void store(Instructions::Instruction *instrPtr);
        virtual void pre(Instructions::Instruction *instrPtr) {};
        virtual void post(Instructions::Instruction *instrPtr) {};
        virtual void writeback(Instructions::Instruction *instrPtr);
    public:
        MemoryUnit() {};
        virtual void run(Instructions::Instruction *instrPtr) {
            std::cout << "This one executes" << std::endl;
        };
};

class OMemoryUnit: public MemoryUnit
{
    private:
        LSQueue *lsq;
        bool can_run;
    protected:
        virtual void pre(Instructions::Instruction *instrPtr);
        virtual void post(Instructions::Instruction *instrPtr);
    public:
        OMemoryUnit() 
        {
            can_run = false;
            lsq = NULL;
        };
        void run(Instructions::Instruction *instrPtr);
};

// class ScalarMemoryUnit: public MemoryUnit
// {
//     protected:
//         virtual void pre(Instructions::Instruction *instrPtr);
//         virtual void post(Instructions::Instruction *instrPtr);
//         virtual void populateResultForwarder(Instructions::Instruction *instrPtr);
//     public:
//         ScalarMemoryUnit(Pipeline *pl, int units): MemoryUnit(pl, units) {};
//         virtual void memref(Instructions::Instruction *instrPtr);
//         virtual void run(Instructions::Instruction * instr);
// };

#endif