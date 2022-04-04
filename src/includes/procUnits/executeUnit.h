#include "procUnit.h"

#ifndef _EXECUTEUNIT_DEFINED_
#define _EXECUTEUNIT_DEFINED_


class ExecuteUnit: public ProcUnit
{
    private:
    protected:
        virtual void executeRTypeInstruction(Instructions::Instruction *instrPtr);
        virtual void executeITypeInstruction(Instructions::Instruction *instrPtr);
        virtual void executeJTypeInstruction(Instructions::Instruction *instrPtr);
        virtual void executeInstrType(Instructions::Instruction *instrPtr);
        virtual void execute(Instructions::Instruction *instrPtr) {};
        virtual void pre(Instructions::Instruction *instrPtr) {};
        virtual void post(Instructions::Instruction *instrPtr) {};
    public:
        ExecuteUnit();
        virtual void run(Instructions::Instruction * instrPtr);
        void attachToProcessor(Processor *proc);
};

class OExecuteUnit: public ExecuteUnit
{
    protected:
        virtual void execute(Instructions::Instruction *instrPtr);
        virtual void pre(Instructions::Instruction *instrPtr);
        virtual void post(Instructions::Instruction *instrPtr);
        virtual void populateRS(Instructions::Instruction *instrPtr);
        virtual void validateRS(Instructions::Instruction *instrPtr);
    public:
        OExecuteUnit(): ExecuteUnit() {};
        virtual void run(Instructions::Instruction * instr) {};
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