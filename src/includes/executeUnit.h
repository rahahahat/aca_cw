#include "procUnit.h"

class ExecuteUnit: public ProcUnit
{
    protected:
        virtual void executeRTypeInstruction(Instructions::Instruction *instrPtr);
        virtual void executeITypeInstruction(Instructions::Instruction *instrPtr);
        virtual void executeJTypeInstruction(Instructions::Instruction *instrPtr);
        virtual void executeInstrType(Instructions::Instruction *instrPtr) {};
        virtual void execute(Instructions::Instruction *instrPtr) {};
        virtual void pre(Instructions::Instruction *instr) {};
        virtual void post(Instructions::Instruction *instr) {};
    public:
        ExecuteUnit(Pipeline *pl, int units);
        virtual void run(Instructions::Instruction * instr);
};

class ScalarExecuteUnit: public ProcUnit
{
    protected:
        virtual void executeRTypeInstruction(Instructions::Instruction *instrPtr);
        virtual void executeITypeInstruction(Instructions::Instruction *instrPtr);
        virtual void executeJTypeInstruction(Instructions::Instruction *instrPtr);
        virtual void executeInstrType(Instructions::Instruction *instrPtr);
        virtual void execute(Instructions::Instruction *instrPtr) {};
        virtual void pre(Instructions::Instruction *instr);
        virtual void post(Instructions::Instruction *instr);
    private:
        void executeInScalarPipeline(Instructions::Instruction *instrPtr);
        void populateInstrSources(Instructions::Instruction *instrPtr);
        void populateResultForwarder(Instructions::Instruction *instrPtr);
    public:
        ScalarExecuteUnit(Pipeline *pl, int units);
        virtual void run(Instructions::Instruction * instr);
};