#include "processor.h"

ExecuteUnit::ExecuteUnit(Processor *proc, Pipeline *pl, pipelineType tp)
{   
    processor = proc;
    pipeType = tp;
    pipeline = pl;
    return;
};

void ExecuteUnit::execute(Instructions::Instruction *instrPtr)
{
    switch(instrPtr->type)
    {
        case RType:
            executeRTypeInstruction(instrPtr);
            break;
        case IType:
            executeITypeInstruction(instrPtr);
            break;
        case JType:
            executeJTypeInstruction(instrPtr);
            break;
    };
    
};

void ExecuteUnit::executeJTypeInstruction(Instructions::Instruction *instrPtr) {};
void ExecuteUnit::executeITypeInstruction(Instructions::Instruction *instrPtr)
{
    Registers r1 = instrPtr->rs;
    Registers r2 = instrPtr->rt;
    int immediate = instrPtr->immediateOrAddress;
    int memAddr;
    switch (instrPtr->opcode)
    {
    case ADDI:
    case ADDIU:
        instrPtr->temp = processor->registers[r2] + immediate;
        break;
    case LW:
        instrPtr->temp = processor->registers[r1] + immediate;
        break;
    case SW:
        instrPtr->temp = processor->registers[r1] + immediate;
        break;
    case ANDI:
        instrPtr->temp = processor->registers[instrPtr->rs] & immediate;
        break;
    case ORI:
        instrPtr->temp = processor->registers[instrPtr->rs] | immediate;
        break;
    }
}
void ExecuteUnit::executeRTypeInstruction(Instructions::Instruction *instrPtr)
{
    Registers src1_rs = instrPtr->rs;
    Registers src2_rt = instrPtr->rt;
    switch (instrPtr->opcode)
    {
    case ADD:
    case ADDU:
        instrPtr->temp = processor->registers[src1_rs] + processor->registers[src2_rt];
        break;
    case SUB:
    case SUBU:
        instrPtr->temp = processor->registers[src1_rs] - processor->registers[src2_rt];
        break;
    case MULT:
    case MULTU:
        instrPtr->temp = processor->registers[src1_rs] * processor->registers[src2_rt];
        break;
    case DIV:
    case DIVU:
        instrPtr->temp = processor->registers[src1_rs] / processor->registers[src2_rt];
        break;
    case AND:
        instrPtr->temp = processor->registers[src1_rs] & processor->registers[src2_rt];
        break;
    case OR:
        instrPtr->temp = processor->registers[src1_rs] | processor->registers[src2_rt];
        break;
    case NOR:
        instrPtr->temp = ~(processor->registers[src1_rs] | processor->registers[src2_rt]);
        break;
    case XOR:
        instrPtr->temp = processor->registers[src1_rs] ^ processor->registers[src2_rt];
        break;
    default:
        break;
    }
}