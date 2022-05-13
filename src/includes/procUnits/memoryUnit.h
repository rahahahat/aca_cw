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
        virtual void nextTick(Instructions::Instruction *instrPtr) {};
};

class OMemoryUnit: public MemoryUnit
{
    private:
        std::string lsqTag;
        Opcodes opcode;
        int store_val;
        int address;
        std::string instrStr;
        Register destination;
        int cycle;
        int result;
        void cycleReset();
    protected:
        void load();
        void store();
        void pre();
        bool seekInstruction();
    public:
        OMemoryUnit();
        void nextTick();
        void flush();
        void run();
        void post();
};

#endif