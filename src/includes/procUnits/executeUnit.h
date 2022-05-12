#include "procUnit.h"

#ifndef _EXECUTEUNIT_DEFINED_
#define _EXECUTEUNIT_DEFINED_


class ExecuteUnit: public ProcUnit
{
    protected:
        Instructions::Instruction* instr;
        Opcodes opcode;
        InstructionType type;
        std::string rsTag;
        Register destination;
        std::string instrStr;
        int src1;
        int src2;
        int immediate;
        int result;
        int num_cycles;

        virtual void executeRTypeInstruction(Instructions::Instruction *instrPtr) {};
        virtual void executeITypeInstruction(Instructions::Instruction *instrPtr) {};
        virtual void executeJTypeInstruction(Instructions::Instruction *instrPtr) {};
        virtual void executeInstrType(Instructions::Instruction *instrPtr) {};
        virtual void execute(Instructions::Instruction *instrPtr) {};
        virtual void pre(Instructions::Instruction *instrPtr) {};
        virtual void post(Instructions::Instruction *instrPtr) {};

        virtual void execute() {};
        virtual void pre() {};
        virtual void post() {};
    public:
        ExecuteUnit();
        virtual void nextTick() {};
        virtual void nextTick(Instructions::Instruction *instrPtr) {};
        void attachToProcessor(Processor *proc);
        virtual void run() {};
};

class ScalarExecuteUnit: public ExecuteUnit
{

    private:
        bool executable;
        bool busy;
        void executeRTypeInstruction(Instructions::Instruction *instrPtr);
        void executeITypeInstruction(Instructions::Instruction *instrPtr);
        void executeJTypeInstruction(Instructions::Instruction *instrPtr);
        void executeInstrType(Instructions::Instruction *instrPtr);
        void populateInstrSources(Instructions::Instruction *instrPtr);
        void pre(Instructions::Instruction *instrPtr);
        void post(Instructions::Instruction *instrPtr);
        void execute(Instructions::Instruction *instrPtr);
    public:
        ScalarExecuteUnit() { executable = false; };
        void reset() { busy = false; };
        void nextTick(Instructions::Instruction *instrPtr);
};

class OExecuteUnit: public ExecuteUnit
{

    protected:
        void executeInstrType();
        void executeITypeInstruction();
        void executeJTypeInstruction();
        void executeRTypeInstruction();
        void execute();
        void pre();
        void populateRSTags(Instructions::Instruction *instrPtr);
        bool seekInstruction();
    public:
        void post();
        void run();
        void flush();
        OExecuteUnit(): ExecuteUnit() {};
        void nextTick();
};

#endif