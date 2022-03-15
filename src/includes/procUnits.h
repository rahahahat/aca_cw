#ifndef _PROCUNITS_DEFINED_
#define _PROCUNITS_DEFINED_
#include "processor.h"
#include "pipeline.h"
#include "events.h"


class Processor;
class Pipeline;

class ProcUnit: public EventDispatcher
{
    protected:
        Processor *processor;
        Pipeline *pipeline;
        virtual void populateResultForwarder(Instructions::Instruction *instrPtr) {};
        virtual void validateDestReg(Instructions::Instruction *instrPtr) {};
        virtual void invalidateDestReg(Instructions::Instruction *instrPtr) {};
    public:
        void attachToProcessor(Processor *proc);
        ProcUnit();
};

class FetchUnit: public ProcUnit
{
    public:
        FetchUnit(Pipeline *pl);
        Instructions::Instruction* fetch(Instructions::Instruction *instrPtr);
};

class DecodeUnit: public ProcUnit
{
    private:
        void decodeRTypeInstruction(Instructions::Instruction *instrPtr, std::vector<std::string> splitInstr, std::pair<Opcodes, InstructionType> insPair);
        void decodeITypeInstruction(Instructions::Instruction *instrPtr, std::vector<std::string> splitInstr, std::pair<Opcodes, InstructionType> insPair);
        void decodeJTypeInstruction(Instructions::Instruction *instrPtr, std::vector<std::string> splitInstr, std::pair<Opcodes, InstructionType> insPair);
        InstructionType getInstrType(Instructions::Instruction *instrPtr);
    protected:
        virtual void invalidateDestReg(Instructions::Instruction *instrPtr);
    public:
        DecodeUnit(Pipeline *pl);
        void decode(Instructions::Instruction *instrPtr);
};

class ExecuteUnit: public ProcUnit
{
    private:
        void executeRTypeInstruction(Instructions::Instruction *instrPtr);
        void executeITypeInstruction(Instructions::Instruction *instrPtr);
        void executeJTypeInstruction(Instructions::Instruction *instrPtr);
        void executeInstrType(Instructions::Instruction *instrPtr);
        void executeInScalarPipeline(Instructions::Instruction *instrPtr);
        void populateInstrSources(Instructions::Instruction *instrPtr);
    protected:
        virtual void populateResultForwarder(Instructions::Instruction *instrPtr);
    public:
        ExecuteUnit(Pipeline *pl);
        void execute(Instructions::Instruction *instrPtr);
};

class MemRefUnit: public ProcUnit
{
    protected:
        virtual void populateResultForwarder(Instructions::Instruction *instrPtr);
    public:
        MemRefUnit(Pipeline *pl);
        void memref(Instructions::Instruction *instrPtr);
};

class WriteBackUnit: public ProcUnit
{
    protected:
        virtual void validateDestReg(Instructions::Instruction *instrPtr);
    public:
        WriteBackUnit(Pipeline *pl);
        void writeback(Instructions::Instruction *instrPtr);
};

#endif