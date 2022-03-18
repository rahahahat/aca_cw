#include "procUnit.h"

class DecodeUnit: public ProcUnit
{
    private:
        InstructionType getInstrType(Instructions::Instruction *instrPtr);
    protected:
        void decodeRTypeInstruction(Instructions::Instruction *instrPtr, std::vector<std::string> splitInstr, std::pair<Opcodes, InstructionType> insPair);
        void decodeITypeInstruction(Instructions::Instruction *instrPtr, std::vector<std::string> splitInstr, std::pair<Opcodes, InstructionType> insPair);
        void decodeJTypeInstruction(Instructions::Instruction *instrPtr, std::vector<std::string> splitInstr, std::pair<Opcodes, InstructionType> insPair);
        virtual void post(Instructions::Instruction *instrPtr) {};
        virtual void pre(Instructions::Instruction *instrPtr) {};
        virtual void invalidateDestReg(Instructions::Instruction *instrPtr) {};
        void decode(Instructions::Instruction *instrPtr);
    public:
        DecodeUnit(Pipeline *pl);
        void attachToProcessor(Processor *proc);
        virtual void run(Instructions::Instruction *instr);
};

class ODecodeUnit: public DecodeUnit
{
    private:
        void hello()
        {
            std::cout << "derived hello" << std::endl;
        }
    protected:
        void decode(Instructions::Instruction *instr);
    public:
        ODecodeUnit(Pipeline *pl);
        void run(Instructions::Instruction *instr) {
            hello();
        };
};



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
