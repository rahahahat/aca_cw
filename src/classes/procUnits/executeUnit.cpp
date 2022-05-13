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
// ScalarExecuteUnit
// #################################################################################################

void ScalarExecuteUnit::executeInstrType(Instructions::Instruction *instrPtr)
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
        case End:
            return;
    };
    return;
};

void ScalarExecuteUnit::populateInstrSources(Instructions::Instruction *instrPtr)
{
    InstructionType instrType = instrPtr->type;
    if (instrType == JType) return;
    int isBranchInstr = isOpBranch(instrPtr->opcode);
    std::pair<int, int> sbe1 = processor->getSB()->isValid(instrPtr->rs);
    std::pair<int, int> sbe2 = processor->getSB()->isValid(instrPtr->rt);
    executable = true;
    if (instrType == RType || isBranchInstr)
    {
        if (instrPtr->rd == instrPtr->rs && instrPtr->rd == instrPtr->rt)
        {
            processor->getPipeline()->resumePipeline(Sources);

            instrPtr->src1 = sbe1.second;
            instrPtr->src2 = sbe2.second;
            return;
        }
        if (instrPtr->rd == instrPtr->rs) {
            processor->getPipeline()->resumePipeline(Sources);
            if (sbe2.first) {
                instrPtr->src1 = sbe1.second;
                instrPtr->src2 = sbe2.second;
                return;
            }
            executable = false;
        }
        if (instrPtr->rd == instrPtr->rt) {
            processor->getPipeline()->resumePipeline(Sources);
            if (sbe1.first) {
                instrPtr->src2 = sbe1.second;
                instrPtr->src1 = sbe2.second;
                return;
            }
            executable = false;
        }
        if (sbe1.first && sbe2.first)
        {
            processor->getPipeline()->resumePipeline(Sources);
            instrPtr->src1 = sbe1.second;
            instrPtr->src2 = sbe2.second;
            return;
        }
        executable = false;
        processor->getPipeline()->stallPipeline(Sources);
        return;
    }
    if (instrType == IType)
    {
        if (instrPtr->rs == instrPtr->rt)
        { 
            processor->getPipeline()->resumePipeline(Sources);
            instrPtr->src1 = sbe1.second;
            return;
        }
        if (sbe1.first)
        {
            processor->getPipeline()->resumePipeline(Sources);
            instrPtr->src1 = sbe1.second;
            return;
        }
        executable = false;
        processor->getPipeline()->stallPipeline(Sources);
        return;
    }
}

void ScalarExecuteUnit::executeRTypeInstruction(Instructions::Instruction *instrPtr)
{
    Register src1_rs = instrPtr->rs;
    Register src2_rt = instrPtr->rt; 
    switch (instrPtr->opcode)
    {
    case ADD:
    case ADDU:
        instrPtr->temp = instrPtr->src1 + instrPtr->src2;
        break;
    case SUB: 
    case SUBU:
        instrPtr->temp = instrPtr->src1 - instrPtr->src2;
        break;
    case MULT:
    case MULTU:
        instrPtr->temp = instrPtr->src1 * instrPtr->src2;
        break;
    case DIV:
    case DIVU:
        instrPtr->temp = instrPtr->src1 / instrPtr->src2;
        break;
    case AND:
        instrPtr->temp = instrPtr->src1 & instrPtr->src2;
        break;
    case OR:
        instrPtr->temp = instrPtr->src1 | instrPtr->src2;
        break;
    case NOR:
        instrPtr->temp = ~(instrPtr->src1 | instrPtr->src2); 
        break;
    case XOR:
        instrPtr->temp = instrPtr->src1 ^ instrPtr->src2;
        break;
    default:
        break;
    }
}

void ScalarExecuteUnit::executeITypeInstruction(Instructions::Instruction *instrPtr)
{
    Opcodes opcode = instrPtr->opcode;
    int immediate = instrPtr->immediateOrAddress;
    switch (instrPtr->opcode)
    {
    case ADDI:
    case ADDIU:
        instrPtr->temp = instrPtr->src1 + immediate;
        break;
    case LW:
        instrPtr->temp = instrPtr->src1 + immediate;
        break;
    case SW:
        instrPtr->temp = instrPtr->src1 + immediate;
        break;
    case ANDI:
        instrPtr->temp = instrPtr->src1 & immediate;
        break;
    case ORI:
        instrPtr->temp = instrPtr->src1 | immediate;
        break;
    case LSR:
        instrPtr->temp = instrPtr->src1 >> immediate;
        break;
    case BEQ:
        instrPtr->temp = instrPtr->src1 == instrPtr->src2;
        break;
    case BNE:
        instrPtr->temp = instrPtr->src1 != instrPtr->src2;
        break;
    case BGTE:
        instrPtr->temp = instrPtr->src1 >= instrPtr->src2;
        break;
    case BL:
        instrPtr->temp = instrPtr->src1 < instrPtr->src2;
        std::cout << "Take Branch: " << instrPtr->temp << std::endl;
        break;
    }
    return;
}

void ScalarExecuteUnit::executeJTypeInstruction(Instructions::Instruction *instrPtr)
{
    Register src = instrPtr->rs;
    int immediate = instrPtr->immediateOrAddress;
    switch (instrPtr->opcode)
    {
    case J:
        instrPtr->temp = immediate;
        break;
    case JR:
        instrPtr->temp = processor->PC - 1;
        break;
    default:
        break;
    }
};

void ScalarExecuteUnit::pre(Instructions::Instruction *instrPtr) 
{
    populateInstrSources(instrPtr);
};
void ScalarExecuteUnit::post(Instructions::Instruction *instrPtr)
{
    if (!executable) return;
    executable = false;
    instrPtr->stage = WRITEBACK;

    if (instrPtr->opcode == SW) return;
    if (instrPtr->opcode == LW)
    {
        instrPtr->stage = MEMORYACCESS;
        return;
    }
    if (isOpBranch(instrPtr->opcode)) return;
    if (instrPtr->type == JType) return;

    if (instrPtr->type == RType) processor->getSB()->validate(instrPtr->rd, instrPtr->temp, "~");
    if (instrPtr->type == IType) processor->getSB()->validate(instrPtr->rt, instrPtr->temp, "~");
    
    return;
};

void ScalarExecuteUnit::execute(Instructions::Instruction *instrPtr)
{
    if (!executable) return;
    std::cout << termcolor::bold << termcolor:: blue << "Executing Instruction: " << instrPtr->instrString << termcolor::reset << std::endl;
    executeInstrType(instrPtr);
};

void ScalarExecuteUnit::nextTick(Instructions::Instruction *instrPtr)
{
    if (busy) return;
    busy = true;
    pre(instrPtr);
    execute(instrPtr);
    post(instrPtr);
}


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
        std::cout << "Result: " << result << std::endl;
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