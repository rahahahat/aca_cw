#include "procUnits.h"
#include "constants.h"
#include "termcolor.h"

ProcUnit::ProcUnit()
{
    processor = NULL;
    pipeline = NULL;
    return;
}

void ProcUnit::attachToProcessor(Processor *proc)
{
    processor = proc;
}

/* --------------------------------------------------------------------------------------------------------- */
/* -------------------------------------------------FetchUnit---------------------------------------------- */
/* --------------------------------------------------------------------------------------------------------- */

FetchUnit::FetchUnit(Pipeline *pl)
{
    pipeline = pl;
    return;
};

Instructions::Instruction* FetchUnit::fetch(Instructions::Instruction *instrPtr)
{
    std::string instr = processor->instructionMemory[processor->PC];
    std::cout 
    << termcolor::green
    << termcolor::bold
    << "Fetching Instruction: "
    << termcolor::reset 
    << instr
    << std::endl;
    instrPtr->instrString = instr;
    processor->PC++;
    return instrPtr;
};

/* --------------------------------------------------------------------------------------------------------- */
/* -------------------------------------------------MemRefUnit---------------------------------------------- */
/* --------------------------------------------------------------------------------------------------------- */

MemRefUnit::MemRefUnit(Pipeline *pl) 
{
    pipeline = pl;
    return;
};

void MemRefUnit::memref(Instructions::Instruction *instrPtr) 
{
    std::cout 
    << termcolor::green
    << termcolor::bold
    << "Memory Accessing Instruction: "
    << termcolor::reset 
    << instrPtr->instrString 
    << std::endl;
    switch (instrPtr->opcode)
    {
    case LW:
        instrPtr->temp = processor->DataMemory[instrPtr->temp];
        break;
    case SW:
        processor->DataMemory[instrPtr->temp] = processor->registers[instrPtr->rt];
        break;
    case HALT:
        return;
    }
    // populateResultForwarder(instrPtr);
};

void MemRefUnit::populateResultForwarder(Instructions::Instruction *instrPtr)
{
    Opcodes opcode = instrPtr->opcode;
    if (opcode == LW) {
        processor->resultForwarder->addValue(instrPtr->rt, instrPtr->temp);
    }
    return;
}

/* --------------------------------------------------------------------------------------------------------- */
/* -------------------------------------------------WritebackUnit------------------------------------------- */
/* --------------------------------------------------------------------------------------------------------- */

WriteBackUnit::WriteBackUnit(Pipeline *pl)
{
    pipeline = pl;
    return;
}

void WriteBackUnit::writeback(Instructions::Instruction *instrPtr)
{
    std::cout 
    << termcolor::green
    << termcolor::bold
    << "Writing Back Instruction: "
    << termcolor::reset 
    << instrPtr->instrString 
    << std::endl;
    switch (instrPtr->opcode)
    {
    case ADD:
    case ADDU:
        processor->registers[instrPtr->rd] = instrPtr->temp;
        break;
    case ADDI:
    case ADDIU:
        processor->registers[instrPtr->rt] = instrPtr->temp;
        break;
    case SUB:
    case SUBU:
        processor->registers[instrPtr->rd] = instrPtr->temp;
        break;
    case MULT:
    case MULTU:
        processor->registers[instrPtr->rd] = instrPtr->temp;
        break;
    case DIV:
    case DIVU:
        processor->registers[instrPtr->rd] = instrPtr->temp;
        break;
    case AND:
        processor->registers[instrPtr->rd] = instrPtr->temp;
        break;
    case ANDI:
        processor->registers[instrPtr->rt] = instrPtr->temp;
        break;
    case OR:
        processor->registers[instrPtr->rd] = instrPtr->temp;
        break;
    case ORI:
        processor->registers[instrPtr->rt] = instrPtr->temp;
        break;
    case NOR:
        processor->registers[instrPtr->rd] = instrPtr->temp;
        break;
    case XOR:
        processor->registers[instrPtr->rd] = instrPtr->temp;
        break;
    case LW:
        processor->registers[instrPtr->rt] = instrPtr->temp;
        break;
    case LSR:
        processor->registers[instrPtr->rt] = instrPtr->temp;
        break;
    case BEQ:
    case BNE:
    case BL:
    case BGTE:
        if (instrPtr->temp) // if we are branching
        {
            processor->PC = instrPtr->immediateOrAddress;
            pipeline->flush = 0;
            processor->scoreboard->validate($pc);
        }
        break;
    case HALT:
        pipeline->flush = 0;
        return;
    case J:
        processor->PC = instrPtr->temp;
        pipeline->flush = 0;
        processor->scoreboard->validate($pc);
        break;
    default:
        break;
    }
    validateDestReg(instrPtr);
}

void WriteBackUnit::validateDestReg(Instructions::Instruction *instrPtr)
{
    Opcodes opcode = instrPtr->opcode;
    if (opcode == BEQ || opcode == BNE || opcode == BL || opcode == BGTE || opcode == HALT) return;
    if (instrPtr->type == IType)
    {
        processor->scoreboard->validate(instrPtr->rt);
        return;
    } 
    if (instrPtr->type == RType)
    {
        processor->scoreboard->validate(instrPtr->rd);
        return;
    }
    return;
}