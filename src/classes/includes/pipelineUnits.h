#ifndef _PIPELINEUNITS_DEFINED_
#define _PIPELINEUNITS_DEFINED_

#include "processor.h"

class FetchUnit {
    private:
        pipelineType pipeType;
        Processor *processor;
        Pipeline *pipeline;
        void nonPipelinedFetch(Instructions::Instruction *instrPtr);
    public:
        FetchUnit(Processor *proc, Pipeline *pl ,pipelineType tp);
        Instructions::Instruction* fetch();
};

class DecodeUnit {
    private:
        pipelineType pipeType;
        Processor *processor;
        Pipeline *pipeline;
        void decodeRTypeInstruction(Instructions::Instruction *instrPtr, std::vector<std::string> splitInstr, std::pair<Opcodes, InstructionType> insPair);
        void decodeITypeInstruction(Instructions::Instruction *instrPtr, std::vector<std::string> splitInstr, std::pair<Opcodes, InstructionType> insPair);
        void decodeJTypeInstruction(Instructions::Instruction *instrPtr, std::vector<std::string> splitInstr, std::pair<Opcodes, InstructionType> insPair);
        InstructionType getInstrType(Instructions::Instruction *instrPtr);
    public:
        DecodeUnit(Processor *proc, Pipeline *pl ,pipelineType tp);
        void decode(Instructions::Instruction *instrPtr);
};

class ExecuteUnit {
    private:
        pipelineType pipeType;
        Processor *processor;
        Pipeline *pipeline;
        void executeRTypeInstruction(Instructions::Instruction *instrPtr);
        void executeITypeInstruction(Instructions::Instruction *instrPtr);
        void executeJTypeInstruction(Instructions::Instruction *instrPtr);
    public:
        ExecuteUnit(Processor *proc, Pipeline *pl ,pipelineType tp);
        void execute(Instructions::Instruction *instrPtr);
};

class MemRefUnit {
    private:
        pipelineType pipeType;
        Processor *processor;
        Pipeline *pipeline;
    public:
        MemRefUnit(Processor *proc, Pipeline *pl ,pipelineType tp);
        void ref(Instructions::Instruction *instrPtr);
};

class WriteBackUnit {
    private:
        pipelineType pipeType;
        Processor *processor;
        Pipeline *pipeline;
    public:
        WriteBackUnit(Processor *proc, Pipeline *pl ,pipelineType tp);
        void writeBack(Instructions::Instruction *instrPtr);
};

#endif