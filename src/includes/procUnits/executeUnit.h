#include "procUnit.h"

#ifndef _EXECUTEUNIT_DEFINED_
#define _EXECUTEUNIT_DEFINED_


class ExecuteUnit: public ProcUnit
{
    protected:
        rs::ReservationStation* rs;
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

        virtual void executeRTypeInstruction(Instructions::Instruction *instrPtr);
        virtual void executeITypeInstruction(Instructions::Instruction *instrPtr);
        virtual void executeJTypeInstruction(Instructions::Instruction *instrPtr);
        virtual void executeInstrType(Instructions::Instruction *instrPtr);
        virtual void execute(Instructions::Instruction *instrPtr) {};
        virtual void execute() {};
        virtual void pre() {};
        virtual void post() {};
    public:
        ExecuteUnit();
        virtual void nextTick() {};
        void attachToProcessor(Processor *proc);
        virtual void run() {};
};

class OExecuteUnit: public ExecuteUnit
{

    protected:
        bool isInstrBranch();
        void executeInstrType();
        void executeITypeInstruction();
        void executeJTypeInstruction();
        void executeRTypeInstruction();
        void execute();
        void pre();
        void post();
        void populateRSTags(Instructions::Instruction *instrPtr);
        bool seekInstruction();
    public:
        void run();
        void flush(std::string tag);
        OExecuteUnit(): ExecuteUnit() {};
        void nextTick();
};

#endif

// class ScalarExecuteUnit: public ProcUnit
// {
//     protected:
//         virtual void executeRTypeInstruction(Instructions::Instruction *instrPtr);
//         virtual void executeITypeInstruction(Instructions::Instruction *instrPtr);
//         virtual void executeJTypeInstruction(Instructions::Instruction *instrPtr);
//         virtual void executeInstrType(Instructions::Instruction *instrPtr);
//         virtual void execute(Instructions::Instruction *instrPtr) {};
//         virtual void pre(Instructions::Instruction *instr);
//         virtual void post(Instructions::Instruction *instr);
//     private:
//         void executeInScalarPipeline(Instructions::Instruction *instrPtr);
//         void populateInstrSources(Instructions::Instruction *instrPtr);
//         void populateResultForwarder(Instructions::Instruction *instrPtr);
//     public:
//         ScalarExecuteUnit(Pipeline *pl, int units);
//         virtual void run(Instructions::Instruction * instr);
// };