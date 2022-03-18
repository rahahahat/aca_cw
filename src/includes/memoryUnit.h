#include "procUnit.h"

class MemoryUnit: public ProcUnit
{
    protected:
        // virtual void populateResultForwarder(Instructions::Instruction *instrPtr);
        virtual void load(Instructions::Instruction *instrPtr);
        virtual void store(Instructions::Instruction *instrPtr);
        virtual void pre(Instructions::Instruction *instrPtr) {};
        virtual void post(Instructions::Instruction *instrPtr) {};
    public:
        MemoryUnit(Pipeline *pl);
        void attachToProcessor(Processor *proc);
        // virtual void memref(Instructions::Instruction *instrPtr);
        virtual void run(Instructions::Instruction * instr) {};
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