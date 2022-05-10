#include "procUnit.h"
#include "executeUnit.h"
#include "constants.h"
#include "termcolor.h"
#include "processor.h"
#include "pipeline.h"
#include "cdb.h"
#include "util.h"

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

void ExecuteUnit::executeRTypeInstruction(Instructions::Instruction *instrPtr) {}

void ExecuteUnit::executeITypeInstruction(Instructions::Instruction *instrPtr) {}

void ExecuteUnit::executeJTypeInstruction(Instructions::Instruction *instrPtr) {};

void ExecuteUnit::executeInstrType(Instructions::Instruction *instrPtr) {};

/* ---------------------------------------------------------------------------------------- */
/* ----------------------------------    0ExecuteUnit   ----------------------------------- */
/* ---------------------------------------------------------------------------------------- */

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
        broadcast_val = (result) ? immediate : instr->pc_no_pred;
        broadcast_destination = $pc;
    }
    processor->getCDB()->broadcast(broadcast_destination, rsTag, broadcast_val);
    busy = false;
    instr = NULL;
    return;
}

void OExecuteUnit::execute()
{
    if (!busy) return;
    if (instr->stage != EXECUTE) return;
    std::cout << termcolor::bold << termcolor::bright_red << "Executing Instruction: "
    << instrStr << termcolor::reset << std::endl;

    executeInstrType();
    return;
}

void OExecuteUnit::populateRSTags(Instructions::Instruction *instrPtr) {}

bool OExecuteUnit::seekInstruction()
{
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
    std::cout << termcolor::bold << termcolor::red << "Seeking Instruction: "
    << termcolor::white << rse->instrStr << termcolor::reset << std::endl;
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