#include "procUnit.h"
#include "memoryUnit.h"
#include "processor.h"
#include "pipeline.h"
#include "termcolor.h"
#include "cdb.h"
#include "util.h"

// #################################################################################################
// ScalarMemoryUnit
// #################################################################################################

void ScalarMemoryUnit::load(Instructions::Instruction *instrPtr)
{
    instrPtr->temp = processor->DataMemory[instrPtr->temp];

    std::cout << "hahahahahahpppppssss" << std::endl;
    processor->getSB()->validate(instrPtr->rt, instrPtr->temp, "~");
    instrPtr->stage = WRITEBACK;
};
void ScalarMemoryUnit::store(Instructions::Instruction *instrPtr)
{
    processor->DataMemory[instrPtr->temp] = processor->registers[instrPtr->rt];
    instrPtr->stage = DONE;
};

void ScalarMemoryUnit::post(Instructions::Instruction *instrPtr)
{
    if (instrPtr->stage != WRITEBACK) return;
    instrPtr->stage = DONE;
    if (isOpBranch(instrPtr->opcode))
    {
        processor->getPipeline()->resumePipeline(Branch);
    }
};

void ScalarMemoryUnit::writeback(Instructions::Instruction* instrPtr)
{
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
        std::cout << "Happens LOAD" << std::endl;
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
        }
        break;
    case HALT:
        // pipeline->flush = 0;
        return;
    case J:
        processor->PC = instrPtr->temp;
        // pipeline->flush = 0;
        // processor->scoreboard->validate($pc);
        break;
    default:
        break;
    }
};

void ScalarMemoryUnit::nextTick(Instructions::Instruction* instrPtr)
{
    if (busy) return;
    busy = true;
    if (instrPtr->opcode == LW && instrPtr->stage == MEMORYACCESS)
    {
        load(instrPtr);
        return;
    } 
    else if (instrPtr->opcode == SW && instrPtr->stage == WRITEBACK)
    {
        store(instrPtr);
    } 
    else 
    {
        std::cout << termcolor::bold << termcolor::cyan << "WritingBack Instruction: " << instrPtr->instrString << termcolor::reset << std::endl;
        writeback(instrPtr);
    }
    post(instrPtr);

}


// #################################################################################################
// OMemoryUnit
// #################################################################################################

OMemoryUnit::OMemoryUnit()
{
    busy = false;
    address = -1;
    destination = $noreg;
    store_val = 0;
    opcode = NOP;
    lsqTag = "~";
    cycle = 0;
    result = 0;
}

void OMemoryUnit::pre()
{
    if (busy) return;
    busy = seekInstruction();
    return;
}

bool OMemoryUnit::seekInstruction()
{
    LSQNode *node = processor->getLsq()->getValidInstruction();
    if (node == NULL) return false;
    node->busy = true;
    if (node->opcode == LW)
    {
        opcode = LW;
        address = node->address;
        destination = node->destination;
        lsqTag = node->getTag();
    }
    if (node->opcode == SW)
    {
        opcode = SW;
        address = node->address;
        destination = $noreg;
        // TODO: Check store format
        store_val = node->value_pair.second;
        lsqTag = node->getTag();
    }
    return true;
}

void OMemoryUnit::run()
{

    pre();
    if (opcode == LW && busy) load();
    if (opcode == SW && busy) store();
    post();
    return;
}

void OMemoryUnit::load()
{
    if (!busy) return;
    result = processor->DataMemory[address];
    cycle--;
    return;
};

void OMemoryUnit::store()
{
    if (!busy) return;
    return;
};

void OMemoryUnit::post()
{
    if (!busy) return;
    busy = false;
    if (opcode == LW)
    {
        processor->getCDB()->broadcast(destination, lsqTag, result);
        return;
    }
    if (opcode == SW)
    {
        processor->getCDB()->broadcast(address, lsqTag, store_val);
        return;
    }
    return;
}

void OMemoryUnit::nextTick()
{
    run();
}

void OMemoryUnit::cycleReset()
{
    cycle = 2;
}

void OMemoryUnit::flush()
{
    lsqTag = "";
    opcode = NOP;
    store_val = 0;
    address = 0;
    destination = $noreg;
    cycle = 0;
    result = 0;
}