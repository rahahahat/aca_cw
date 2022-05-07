#include "procUnit.h"
#include "util.h"

#ifndef _DECODEUNIT_DEFINED_
#define _DECODEUNIT_DEFINED_

class FetchUnit;

class DecodeUnit: public ProcUnit
{
    private:
        InstructionType getInstrType(Instructions::Instruction *instrPtr);
    protected:
        void decodeRTypeInstruction(Instructions::Instruction *instrPtr, std::vector<std::string> splitInstr, std::pair<Opcodes, InstructionType> insPair);
        void decodeITypeInstruction(Instructions::Instruction *instrPtr, std::vector<std::string> splitInstr, std::pair<Opcodes, InstructionType> insPair);
        void decodeJTypeInstruction(Instructions::Instruction *instrPtr, std::vector<std::string> splitInstr, std::pair<Opcodes, InstructionType> insPair);
        virtual void pre(Instructions::Instruction *instrPtr) {};
        virtual void pre();
        virtual void invalidateDestReg(Instructions::Instruction *instrPtr) {};
        virtual void decode(Instructions::Instruction *instrPtr);
        virtual void decode();
        virtual void post(Instructions::Instruction *instrPtr) {};
    public:
        DecodeUnit();
        void attachToProcessor(Processor *proc);
        virtual void run(Instructions::Instruction *instr);
        virtual void run() {};
};

class ODecodeUnit: public DecodeUnit
{ 
    private:
        FetchUnit *fn;
        Instructions::Instruction *instr;
        void decode();
        void pre() {};
    public:
        ODecodeUnit();
        void flush(std::string);
        void nextTick();
        void fetchTick();
        void decodeTick();
        void run();
        void post();
};

#endif

// class ScalarDecodeUnit: public DecodeUnit {

//     protected:
//         virtual void pre(Instructions::Instruction *instr);
//         virtual void post(Instructions::Instruction *instr);
//         virtual void invalidateDestReg(Instructions::Instruction *instrPtr);
//     public:
//         ScalarDecodeUnit(Pipeline *pl, int units): DecodeUnit(pl, units) {}
//         virtual void decode(Instructions::Instruction *instrPtr);
//         virtual void run(Instructions::Instruction * instr);
// };
