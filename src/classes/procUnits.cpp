#include "procUnits.h"

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
/* -------------------------------------------------FetchUnit----------------------------------------------- */
/* --------------------------------------------------------------------------------------------------------- */

FetchUnit::FetchUnit(Pipeline *pl)
{
    pipeline = pl;
    return;
};

Instructions::Instruction* FetchUnit::fetch(Instructions::Instruction *instrPtr)
{
    std::string instr = processor->instructionMemory[processor->PC];
    instrPtr->instrString = instr;
    processor->PC++;
    return instrPtr;
};

/* --------------------------------------------------------------------------------------------------------- */
/* -------------------------------------------------DecodeUnit---------------------------------------------- */
/* --------------------------------------------------------------------------------------------------------- */

DecodeUnit::DecodeUnit(Pipeline *pl) 
{
    pipeline = pl;
    return;
};

void DecodeUnit::decode(Instructions::Instruction *instrPtr)
{
    std::vector<std::string> splitInstr = splitString(instrPtr->instrString);
    std::pair<Opcodes, InstructionType> InsPair = InstructionPairMap.at(splitInstr.front());
    instrPtr->opcode = InsPair.first;
    instrPtr->type = InsPair.second;
    switch(InsPair.second)
    {
        case RType:
            decodeRTypeInstruction(instrPtr, splitInstr, InsPair);
            break;
        case IType:
            decodeITypeInstruction(instrPtr, splitInstr, InsPair);
            break;
        case JType:
            decodeJTypeInstruction(instrPtr, splitInstr, InsPair);
            break;
    };
    invalidateDestReg(instrPtr);
};

void DecodeUnit::decodeRTypeInstruction(Instructions::Instruction *instrPtr, std::vector<std::string> splitInstr, std::pair<Opcodes, InstructionType> insPair) 
{
    // Instruction format: opcode dest src1 src2
    instrPtr->rt = RegisterMap.at(splitInstr.back()); // src2
    splitInstr.pop_back();
    instrPtr->rs = RegisterMap.at(splitInstr.back()); // src1
    splitInstr.pop_back();
    instrPtr->rd = RegisterMap.at(splitInstr.back()); // dest
    splitInstr.pop_back();
};

void DecodeUnit::decodeITypeInstruction(Instructions::Instruction *instrPtr, std::vector<std::string> splitInstr, std::pair<Opcodes, InstructionType> insPair)
{
    Opcodes opcode = insPair.first;
    std::string label;
    switch(opcode)
    {
        case BEQ:
        case BNE:
            label = splitInstr.back();
            instrPtr->immediateOrAddress = processor->labelMap.at(label); //label
            splitInstr.pop_back();
            instrPtr->rt = RegisterMap.at(splitInstr.back()); // r2
            splitInstr.pop_back();
            instrPtr->rs = RegisterMap.at(splitInstr.back()); // r1
            splitInstr.pop_back();
            return;
        case BLEZ:
        case BGTZ:
            label = splitInstr.back();
            instrPtr->immediateOrAddress = processor->labelMap.at(label); //label
            splitInstr.pop_back();
            instrPtr->rs = RegisterMap.at(splitInstr.back()); // r1
            splitInstr.pop_back();
            return;
        default:
            // Instruction format: opcode r1 r2 immediate
            instrPtr->immediateOrAddress = std::stoi(splitInstr.back(), 0, 16); // immediate
            splitInstr.pop_back();
            instrPtr->rt = RegisterMap.at(splitInstr.back()); // r2
            splitInstr.pop_back();
            instrPtr->rs = RegisterMap.at(splitInstr.back()); // r1
            splitInstr.pop_back();
            return;
    };
}

void DecodeUnit::decodeJTypeInstruction(Instructions::Instruction *instrPtr, std::vector<std::string> splitInstr, std::pair<Opcodes, InstructionType> insPair) 
{
    // Instruction format: opcode address
    std::string label;
    label = splitInstr.back();
    instrPtr->immediateOrAddress = processor->labelMap.at(label);
    splitInstr.pop_back();
    return;
};

void DecodeUnit::invalidateDestReg(Instructions::Instruction *instrPtr)
{
    if (instrPtr->type == RType) {
        processor->resultForwarder->removeValue(instrPtr->rd);
        processor->scoreboard->inValidate(instrPtr->rd);
        return;
    }
    if (instrPtr->type == IType) {
        processor->resultForwarder->removeValue(instrPtr->rt);
        processor->scoreboard->inValidate(instrPtr->rt);
        return;
    }
    return;
}

/* --------------------------------------------------------------------------------------------------------- */
/* -------------------------------------------------ExecuteUnit--------------------------------------------- */
/* --------------------------------------------------------------------------------------------------------- */

ExecuteUnit::ExecuteUnit(Pipeline *pl)
{   
    pipeline = pl;
    return;
};

void ExecuteUnit::executeInstrType(Instructions::Instruction *instrPtr)
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

void ExecuteUnit::executeInScalarPipeline(Instructions::Instruction *instrPtr)
{
    InstructionType instrType = instrPtr->type;
    if (instrType == RType)
    {
        // Performs results forwarding
        // pair<int::valid, int::value>
        std::pair<int, int> src1 = processor->resultForwarder->getValue(instrPtr->rs);
        std::pair<int, int> src2 = processor->resultForwarder->getValue(instrPtr->rt);
        int forwarded = 0;
        if (src1.first && src2.first) {
            std::cout << "User RF" << std::endl;
            instrPtr->src1 = src1.second;
            instrPtr->src2 = src2.second;
            forwarded = 1;
        }
        if (!forwarded) {
            // Checks validity of source registers in scoreboard
            if (!processor->scoreboard->isValid(instrPtr->rs) || !processor->scoreboard->isValid(instrPtr->rt))
            {
                std::cout << "stalling pipeline" << std::endl;
                pipeline->stallPipeline();
                return;
            }
            instrPtr->src1 = processor->registers[instrPtr->rs];
            instrPtr->src2 = processor->registers[instrPtr->rt];
        }
    }
    if (instrType == IType)
    {
        // Performs results forwarding
        // pair<int::valid, int::value>
        std::pair<int, int> src1 = processor->resultForwarder->getValue(instrPtr->rs);
        if (src1.first) {
            instrPtr->src1 = src1.second;
        }
        if (!src1.first) {
            // Checks validity of source registers in scoreboard
            if (!processor->scoreboard->isValid(instrPtr->rs))
            {
                pipeline->stallPipeline();
                return;
            }
            instrPtr->src1 = processor->registers[instrPtr->rs];
        }
    }
    executeInstrType(instrPtr);
};

void ExecuteUnit::execute(Instructions::Instruction *instrPtr)
{
    if (pipeline->getType() == Scalar)
    {
        executeInScalarPipeline(instrPtr);
        return;
    }
}

void ExecuteUnit::executeJTypeInstruction(Instructions::Instruction *instrPtr) {};
void ExecuteUnit::executeITypeInstruction(Instructions::Instruction *instrPtr)
{
    Register src = instrPtr->rs;
    int immediate = instrPtr->immediateOrAddress;
    int memAddr;
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
    }
}
void ExecuteUnit::executeRTypeInstruction(Instructions::Instruction *instrPtr)
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

void ExecuteUnit::populateResultForwarder(Instructions::Instruction *instrPtr)
{
    Opcodes opcode = instrPtr->opcode;
    if (opcode == LW || opcode == SW || opcode == JR) return;
    processor->resultForwarder->addValue(instrPtr->rd, instrPtr->temp);
    return;
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
    switch (instrPtr->opcode)
    {
    case LW:
        instrPtr->temp = processor->DataMemory[instrPtr->temp];
        break;
    case SW:
        processor->DataMemory[instrPtr->temp] = processor->registers[instrPtr->rt];
        break;
    }
};

void MemRefUnit::populateResultForwarder(Instructions::Instruction *instrPtr)
{
    Opcodes opcode = instrPtr->opcode;
    if (opcode == LW) {
        processor->resultForwarder->addValue(instrPtr->rd, instrPtr->temp);
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
    default:
        break;
    }
    validateDestReg(instrPtr);
}

void WriteBackUnit::validateDestReg(Instructions::Instruction *instrPtr)
{
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
