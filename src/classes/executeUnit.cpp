#include "procUnits.h"
#include "constants.h"

ExecuteUnit::ExecuteUnit(Pipeline *pl)
{   
    pipeline = pl;
    return;
};

void ExecuteUnit::execute(Instructions::Instruction *instrPtr)
{
    if (pipeline->getType() == Scalar)
    {
        std::cout << "Executing Instruction: " << instrPtr->instrString << std::endl;
        executeInScalarPipeline(instrPtr);
        return;
    }
}

void ExecuteUnit::executeInScalarPipeline(Instructions::Instruction *instrPtr)
{
    populateInstrSources(instrPtr);
    if (pipeline->stalled()) return;
    executeInstrType(instrPtr);
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
        case End:
            processor->scoreboard->inValidate($pc);
            pipeline->flush = 1;
            pipeline->stallPipeline();
            instrPtr->nextPipeStage();
            return;
    };
    populateResultForwarder(instrPtr);
    return;
};

void ExecuteUnit::populateInstrSources(Instructions::Instruction *instrPtr)
{
    Opcodes opcode = instrPtr->opcode;
    InstructionType instrType = instrPtr->type;
    if (instrType == JType) return;
    int isBranchInstr = (opcode == BEQ || opcode == BGTE || opcode == BNE || opcode == BL);
    std::pair<int, int> src1 = processor->resultForwarder->getValue(instrPtr->rs); // pair<int::valid, int::value>
    std::pair<int, int> src2 = processor->resultForwarder->getValue(instrPtr->rt); // pair<int::valid, int::value>
    int forwarded = 0;
    if (instrType == RType || isBranchInstr)
    {
        if (src1.first && src2.first) {
            std::cout<<GRN"Forwarding Results of Registers: "<<"$r"<<instrPtr->rs<<" "<<"$r"<<instrPtr->rt<<NC<<std::endl;
            instrPtr->src1 = src1.second;
            instrPtr->src2 = src2.second;
            return;
        }
        // Checks validity of source registers in scoreboard
        if (!processor->scoreboard->isValid(instrPtr->rs) || !processor->scoreboard->isValid(instrPtr->rt))
        {
            std::cout << REDB "Stalling pipeline" NC << std::endl;
            pipeline->stallPipeline();
            return;
        }
        instrPtr->src1 = processor->registers[instrPtr->rs];
        instrPtr->src2 = processor->registers[instrPtr->rt];
    }
    if (instrType == IType)
    {
        std::pair<int, int> src1 = processor->resultForwarder->getValue(instrPtr->rs); // pair<int::valid, int::value>
        if (src1.first) {
             std::cout<<GRN "Forwarding Results of Registers: "<<"$r"<<instrPtr->rs<<NC<< std::endl;
            instrPtr->src1 = src1.second;
            return;
        }
        // Checks validity of source register in scoreboard
        if (!processor->scoreboard->isValid(instrPtr->rs))
        {
            std::cout << REDB "Stalling pipeline" NC << std::endl;
            pipeline->stallPipeline();
            return;
        }
        instrPtr->src1 = processor->registers[instrPtr->rs];
        return;
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

void ExecuteUnit::executeITypeInstruction(Instructions::Instruction *instrPtr)
{
    Opcodes opcode = instrPtr->opcode;
    int immediate = instrPtr->immediateOrAddress;
    int isBranchInstr = (opcode == BEQ || opcode == BGTE || opcode == BNE || opcode == BL);
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
        break;
    }
    if (isBranchInstr && instrPtr->temp)
    {
        std::cout << REDB "Stalling pipeline in Branch Execute" NC << std::endl;
        pipeline->flush = 1;
        pipeline->stallPipeline();
        instrPtr->nextPipeStage();
        processor->scoreboard->inValidate($pc);
    }
    return;
}

void ExecuteUnit::executeJTypeInstruction(Instructions::Instruction *instrPtr)
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

void ExecuteUnit::populateResultForwarder(Instructions::Instruction *instrPtr)
{
    Opcodes opcode = instrPtr->opcode;
    InstructionType type = instrPtr->type;
    int isInstrInvalid = (opcode == LW || opcode == SW || opcode == BEQ || opcode == BGTE || opcode == BL || opcode == BNE || type == JType);
    if (isInstrInvalid) return;
    if (instrPtr->type == IType)
    {
        processor->resultForwarder->addValue(instrPtr->rt, instrPtr->temp);
        return;
    }
    processor->resultForwarder->addValue(instrPtr->rd, instrPtr->temp);
    return;
};