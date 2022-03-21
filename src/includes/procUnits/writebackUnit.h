#include "procUnit.h"

#ifndef _WBUNIT_DEFINED_
#define _WBUNIT_DEFINED_

class WriteBackUnit: public ProcUnit
{
    protected:
        virtual void validateDestReg(Instructions::Instruction *instrPtr) {};
        virtual void pre(Instructions::Instruction *instr) {};
        virtual void post(Instructions::Instruction *instr) {};
    public:
        WriteBackUnit(Pipeline *pl);
        virtual void writeback(Instructions::Instruction *instrPtr) {};

        virtual void run(Instructions::Instruction * instr);
};

// class ScalarWriteBackUnit: public WriteBackUnit
// {
//     protected:
//         virtual void validateDestReg(Instructions::Instruction *instrPtr);
//         virtual void pre(Instructions::Instruction *instr) {};
//         virtual void post(Instructions::Instruction *instr) {};
//     public:
//         ScalarWriteBackUnit(Pipeline *pl, int units): WriteBackUnit(pl, units) {};
//         virtual void writeback(Instructions::Instruction *instrPtr);
//         virtual void run(Instructions::Instruction *instr);
// };
#endif