#include "procUnit.h"
#include "executeUnit.h"
#include "termcolor.h"
#include "processor.h"
#include "pipeline.h"
#include "prochelpers.h"
#include "cdb.h"
#include "util.h"
#include "branch.h"

#include <cassert>

#include <map>

// TODO: Make Execute unit unbusy
// Incorporate branch resume logic

ExecuteUnit::ExecuteUnit()
{
    opcode = NOP;
    src1 = 0;
    src2 = 0;
    immediate = 0;
    result = 0;
    num_cycles = 0;
    instrStr = "";
    destination = $noreg;
    rsTag = "~";
    type = Nop;
    instr = NULL;

};

// #################################################################################################
// OExecuteUnit
// #################################################################################################

void OExecuteUnit::pre()
{
    if (busy) return;
    if (!busy)
    {
        busy = seekInstruction();
    }
    return;
}


void OExecuteUnit::run()
{
    pre();
    execute();
}

void OExecuteUnit::post()
{
    // TODO: Add Jump support
    if (!busy) return;
    instr->stage = COMMIT;
    Register broadcast_destination = destination; 
    int broadcast_val = result;
    if (isOpBranch(opcode))
    {
        broadcast_val = (result) ? immediate : instr->fetched_at_pc + 1;
        broadcast_destination = $pc;
    }
    processor->getCDB()->broadcast(broadcast_destination, rsTag, broadcast_val);
    busy = false;
    instr = NULL;
    return;
}

void OExecuteUnit::execute()
{
    bool print = config->debug->print;

    if (!busy) return;
    if (instr->stage != EXECUTE) return;
    IF_PRINT(
        std::cout << termcolor::bold << termcolor::bright_blue << "Executing Instruction: "
        << termcolor::white << instrStr << termcolor::reset << std::endl;
    )
    executeInstrType();
    return;
}

void OExecuteUnit::populateRSTags(Instructions::Instruction *instrPtr) {}

bool OExecuteUnit::seekInstruction()
{
    bool print = config->debug->print;

    rs::ReservationStationEntry* rse = processor->getRS()->getValidInstruction();
    if (rse == NULL) return false;
    rse->busy = true;
    instr = rse->getInstruction();
    rsTag = rse->getTag();
    opcode = rse->opcode;
    type = rse->instr_type;
    num_cycles = CycleMap.at(opcode);
    instrStr = rse->instrStr;
    instr->stage = EXECUTE;

    // TODO: Add support for halt
    if(opcode == HALT) return true;
    if (opcode == BEQ || opcode == BGTE || opcode == BNE || opcode == BL)
    {
        src1 = rse->value_pair.first;
        src2 = rse->value_pair.second;
        opcode = rse->opcode;
        destination = $noreg;
        immediate = rse->address;
    }
    else if (type == RType)
    {
        src1 = rse->value_pair.first;
        src2 = rse->value_pair.second;
        opcode = rse->opcode;
        destination = rse->destination;
    }
    else if (type == IType)
    {
        src1 = rse->value_pair.first;
        immediate = rse->value_pair.second;
        opcode = rse->opcode;
        destination = rse->destination;
    }
    else
    {
        immediate = rse->address;
    }
    IF_PRINT(
    std::cout << termcolor::bold << termcolor::bright_blue << "Seeking Instruction: "
    << termcolor::white << rse->instrStr << termcolor::reset << std::endl;
    );
    
    return true;
}

void OExecuteUnit::executeRTypeInstruction()
{
    switch (opcode)
    {
    case ADD:
    case ADDU:
        result = src1 + src2;
        break;
    case SUB: 
    case SUBU:
        result = src1 - src2;
        break;
    case MULT:
    case MULTU:
        result = src1 * src2;
        break;
    case DIV:
    case DIVU:
        result = src1 / src2;
        break;
    case AND:
        result = src1 & src2;
        break;
    case OR:
        result = src1 | src2;
        break;
    case NOR:
        result = ~(src1 | src2); 
        break;
    case XOR:
        result = src1 ^ src2;
        break;
    default:
        break;
    }
}

void OExecuteUnit::executeITypeInstruction()
{
    switch (opcode)
    {
    case ADDI:
    case ADDIU:
        result = src1 + immediate;
        break;
    case ANDI:
        result = src1 & immediate;
        break;
    case ORI:
        result = src1 | immediate;
        break;
    case LSR:
        result = src1 >> immediate;
        break;
    case BEQ:
        result = src1 == src2;
        break;
    case BNE:
        result = src1 != src2;
        break;
    case BGTE:
        result = src1 >= src2;
        break;
    case BL:
        result = src1 < src2;
        break;
    default:
        break;
    }
    return;
}

void OExecuteUnit::executeJTypeInstruction()
{
    switch (opcode)
    {
    case J:
        result = immediate;
        break;
    case JR:
        result = processor->PC - 1;
        break;
    default:
        break;
    }
};

void OExecuteUnit::executeInstrType()
{
    switch(type)
    {
        case RType:
            executeRTypeInstruction();
            break;
        case IType:
            executeITypeInstruction();
            break;
        case JType:
            executeJTypeInstruction();
            break;
        default:
            return;
    };
    return;
};

void OExecuteUnit::nextTick()
{
    run();
}

void OExecuteUnit::flush()
{
    opcode = NOP;
    busy = false;
    rsTag = "";
    destination = $noreg;
    src1 = 0;
    src2 = 0;
    immediate = 0;
    result = 0;
    num_cycles = 0;
    instrStr = "";
    type = Nop;
}