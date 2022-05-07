#include "procUnit.h"
#include "lsq.h"

#ifndef _MEMORYUNIT_DEFINED_
#define _MEMORYUNIT_DEFINED_

class MemoryUnit: public ProcUnit
{
    protected:
        // virtual void populateResultForwarder(Instructions::Instruction *instrPtr);
        virtual void load(Instructions::Instruction *instrPtr){};
        virtual void store(Instructions::Instruction *instrPtr){};
        virtual void pre(Instructions::Instruction *instrPtr) {};
        virtual void post(Instructions::Instruction *instrPtr) {};
        virtual void load(){};
        virtual void store(){};
        virtual void pre(){};
        virtual void post(){};
    public:
        MemoryUnit() {};
        virtual void run(Instructions::Instruction *instrPtr) {};
        virtual void run() {};
};

class OMemoryUnit: public MemoryUnit
{
    private:
        LSQueue *lsq;
        std::string lsqTag;
        Opcodes opcode;
        int store_val;
        int address;
        Register destination;
        int cycle;
        int result;
        void cycleReset();
    protected:
        virtual void load();
        virtual void store();
        virtual void pre();
        virtual void post();
        bool seekInstruction();
    public:
        OMemoryUnit();
        void nextTick();
        void flush(std::string tag);
        void run();
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