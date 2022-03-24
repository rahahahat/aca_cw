#include "procUnit.h"

#ifndef _EXECUTEUNIT_DEFINED_
#define _EXECUTEUNIT_DEFINED_


class ExecuteUnit: public ProcUnit
{
    protected:
        virtual void executeRTypeInstruction(Instructions::Instruction *instrPtr);
        virtual void executeITypeInstruction(Instructions::Instruction *instrPtr);
        virtual void executeJTypeInstruction(Instructions::Instruction *instrPtr);
        virtual void executeInstrType(Instructions::Instruction *instrPtr);
        virtual void execute(Instructions::Instruction *instrPtr) {};
        virtual void pre(Instructions::Instruction *instr) {};
        virtual void post(Instructions::Instruction *instr) {};
    public:
        ExecuteUnit(Pipeline *pl);
        virtual void run(Instructions::Instruction * instr);
        void attachToProcessor(Processor *proc);
};

class OExecuteUnit: public ExecuteUnit
{
    private:
        rs::ReservationStation *reservation_station;
    protected:
        virtual void execute(Instructions::Instruction *instrPtr);
        virtual void pre(Instructions::Instruction *instr);
        virtual void post(Instructions::Instruction *instr);
    public:
        OExecuteUnit(Pipeline *pl): ExecuteUnit(pl) {};
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