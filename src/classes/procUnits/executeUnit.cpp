#include "procUnit.h"
#include "executeUnit.h"
#include "constants.h"
#include "termcolor.h"
#include "processor.h"
#include "pipeline.h"

ExecuteUnit::ExecuteUnit() {};

void ExecuteUnit::run(Instructions::Instruction *instr)
{

    std::cout << termcolor::bold << termcolor::blue
    << "Executing instruction: " << instr->instrString
    << std::endl;

    pre(instr);
    execute(instr);
    post(instr);
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
    switch (instrPtr->opcode)
    {
    case ADDI:
    case ADDIU:
        instrPtr->temp = instrPtr->src1 + immediate;
        break;
    case LW:
        instrPtr->immediateOrAddress = instrPtr->src1 + immediate;
        break;
    case SW:
        instrPtr->immediateOrAddress = instrPtr->src1 + immediate;
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
        break;
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
        default:
        // TODO: HALT Instruction logic needs to be added
            // processor->scoreboard->inValidate($pc);
            // pipeline->flush = 1;
            // pipeline->stallPipeline();
            // instrPtr->nextPipeStage();
            // return;
    };
    return;
};

/* ---------------------------------------------------------------------------------------- */
/* ----------------------------------    0ExecuteUnit   ----------------------------------- */
/* ---------------------------------------------------------------------------------------- */

void OExecuteUnit::pre(Instructions::Instruction *instrPtr)
{
    if (instrPtr->type == End)
    {
        instrPtr->isReadyToExecute = true;
        return;
    }
    if (instrPtr->isReadyToExecute) return;
    populateRS(instrPtr);
    return;
}

void OExecuteUnit::execute(Instructions::Instruction *instrPtr)
{
    if (!instrPtr->isReadyToExecute) return;
    instrPtr->decrementCycle();
    if (instrPtr->getCurrCycle() != 0) return;
    executeInstrType(instrPtr);
    return;
}

void OExecuteUnit::post(Instructions::Instruction *instrPtr)
{
    if (instrPtr->getCurrCycle() != 0) return;
    Opcodes opcode = instrPtr->opcode;
    instrPtr->stage = WRITEBACK;
    if (opcode == LW || opcode == SW)
    {
        instrPtr->stage = MEMORYACCESS;
    }
    Event<rsEventData> event = Event<rsEventData>();
    event.set(instrPtr->tag);
    event.payload.tag_name = instrPtr->tag;
    event.payload.value = instrPtr->temp;
    dispatch(event);
    if (instrPtr->type == RType) processor->getSB()->validate(instrPtr->rd, instrPtr->temp, instrPtr->tag);
    if (instrPtr->type == IType) processor->getSB()->validate(instrPtr->rt, instrPtr->temp, instrPtr->tag);
    return;
}

void OExecuteUnit::populateRS(Instructions::Instruction *instrPtr)
{ 
    processor->getRS()->populateInstruction(instrPtr);
    return;
}

void OExecuteUnit::validateRS(Instructions::Instruction *instrPtr)
{ 
    processor->getRS()->validate(instrPtr);
    return;
}

/* ---------------------------------------------------------------------------------------- */
/* ---------------------------------- ScalarExecuteUnit ----------------------------------- */
/* ---------------------------------------------------------------------------------------- */

// void ScalarExecuteUnit::populateInstrSources(Instructions::Instruction *instrPtr)
// {
//     Opcodes opcode = instrPtr->opcode;
//     InstructionType instrType = instrPtr->type;
//     if (instrType == JType) return;
//     int isBranchInstr = (opcode == BEQ || opcode == BGTE || opcode == BNE || opcode == BL);
//     std::pair<int, int> src1 = processor->resultForwarder->getValue(instrPtr->rs); // pair<int::valid, int::value>
//     std::pair<int, int> src2 = processor->resultForwarder->getValue(instrPtr->rt); // pair<int::valid, int::value>
//     int forwarded = 0;
//     if (instrType == RType || isBranchInstr)
//     {
//     std::cout << "rs = r" << instrPtr->rs << " rt = r" << instrPtr->rt << " rd = r" << instrPtr->rd << std::endl;
//         if (instrPtr->rd == instrPtr->rs) {
//             if (src2.first) {
//                 instrPtr->src1 = processor->registers[instrPtr->rs];
//                 instrPtr->src2 = src2.second;
//                 return;
//             }
//         }
//         if (instrPtr->rd == instrPtr->rt) {
//             if (src1.first) {
//                 instrPtr->src2 = processor->registers[instrPtr->rt];
//                 instrPtr->src1 = src1.second;
//                 return;
//             }
//         }
//         if (src1.first && src2.first) {
//             std::cout 
//             << termcolor::blue
//             << termcolor::bold
//             << "Result Forwaring registers: "
//             << "$r" << instrPtr->rs << " "
//             << "$r" << instrPtr->rt << " "
//             << termcolor::reset
//             << std::endl;
//             instrPtr->src1 = src1.second;
//             instrPtr->src2 = src2.second;
//             return;
//         }

//         int isSrc1Valid = instrPtr->rs == instrPtr->rd || processor->scoreboard->isValid(instrPtr->rs);
//         int isSrc2Valid = instrPtr->rt == instrPtr->rd || processor->scoreboard->isValid(instrPtr->rt);
//         std::cout << termcolor::bold << termcolor::blue << "rs " << (instrPtr->rs == instrPtr->rd) << " " << processor->scoreboard->isValid(instrPtr->rs) << termcolor::reset << std::endl;
//         std::cout << termcolor::bold << termcolor::blue << "rt " << (instrPtr->rt == instrPtr->rd) << " " << processor->scoreboard->isValid(instrPtr->rt) << termcolor::reset << std::endl;
//         // Checks validity of source registers in scoreboard
//         if (!isSrc1Valid || !isSrc2Valid)
//         {
//             std::cout 
//             << termcolor::on_red
//             << termcolor::bold
//             << "Stalling Pipeline"
//             << termcolor::reset
//             << std::endl;
//             pipeline->stallPipeline(); 
//             Event<int> event = Event<int>();
//             event.set(PipelineEvents::StallPipelineEvent);
//             event.payload = 200;
//             this->dispatch(event);
//             return;
//         }
//         instrPtr->src1 = processor->registers[instrPtr->rs];
//         instrPtr->src2 = processor->registers[instrPtr->rt];
//     }
//     if (instrType == IType)
//     {
//         std::pair<int, int> src1 = processor->resultForwarder->getValue(instrPtr->rs); // pair<int::valid, int::value>
//         if (src1.first) {
//             std::cout 
//             << termcolor::blue
//             << termcolor::bold
//             << "Result Forwaring register: "
//             << "$r" << instrPtr->rs << " "
//             << termcolor::reset
//             << std::endl;
//             instrPtr->src1 = src1.second;
//             return;
//         }
//         int isSrcValid = instrPtr->rs == instrPtr->rt || processor->scoreboard->isValid(instrPtr->rs);
//         std::cout << termcolor::bold << termcolor::blue << "rs " << (instrPtr->rt == instrPtr->rs) << " " << processor->scoreboard->isValid(instrPtr->rs) << termcolor::reset << std::endl;
//         // Checks validity of source register in scoreboard
//         if (!isSrcValid)
//         {
//             std::cout 
//             << termcolor::on_red
//             << termcolor::bold
//             << "Stalling Pipeline"
//             << termcolor::reset
//             << std::endl;
//             pipeline->stallPipeline();
//             return;
//         }
//         instrPtr->src1 = processor->registers[instrPtr->rs];
//         return;
//     }
// }

// void ScalarExecuteUnit::populateResultForwarder(Instructions::Instruction *instrPtr)
// {
//     Opcodes opcode = instrPtr->opcode;
//     InstructionType type = instrPtr->type;
//     int isInstrInvalid = (opcode == LW || opcode == SW || opcode == BEQ || opcode == BGTE || opcode == BL || opcode == BNE || type == JType);
//     if (isInstrInvalid) return;
//     if (instrPtr->type == IType)
//     {
//         processor->resultForwarder->addValue(instrPtr->rt, instrPtr->temp);
//         return;
//     }
//     if (instrPtr->type == RType) {
//         processor->resultForwarder->addValue(instrPtr->rd, instrPtr->temp);
//         return;
//     }
//     return;
// };

// void ScalarExecuteUnit::post(Instructions::Instruction *instrPtr)
// {
//     populateResultForwarder(instrPtr);
//     Opcodes opcode = instrPtr->opcode;
//     int isBranchInstr = (opcode == BEQ || opcode == BGTE || opcode == BNE || opcode == BL);
//     if (isBranchInstr && instrPtr->temp)
//     {
//         std::cout 
//         << termcolor::bold
//         << termcolor::red
//         << "Executing a branch instruction (pseudo stall)"
//         << termcolor::reset
//         << std::endl;
//         pipeline->flush = 1;
//         pipeline->stallPipeline();
//         instrPtr->nextPipeStage();
//         processor->scoreboard->inValidate($pc);
//         return;
//     }
//     if  (opcode == J) {
//         std::cout 
//         << termcolor::bold
//         << termcolor::red
//         << "Executing a jump instruction (pseudo stall)"
//         << termcolor::reset
//         << std::endl;
//         pipeline->flush = 1;
//         pipeline->stallPipeline();
//         instrPtr->nextPipeStage();
//         processor->scoreboard->inValidate($pc);
//     }
// }

// void ScalarExecuteUnit::execute(Instructions::Instruction *instrPtr)
// {
//     if (pipeline->getType() == Scalar)
//     {
//         std::cout 
//         << termcolor::green
//         << termcolor::bold
//         << "Executing Instruction: "
//         << termcolor::reset 
//         << instrPtr->instrString 
//         << std::endl;
//         executeInScalarPipeline(instrPtr);
//         return;
//     }
// }

// void ScalarExecuteUnit::executeInScalarPipeline(Instructions::Instruction *instrPtr)
// {
//     populateInstrSources(instrPtr);
//     if (pipeline->stalled()) return;
//     executeInstrType(instrPtr);
// };

// void ScalarExecuteUnit::executeInstrType(Instructions::Instruction *instrPtr)
// {
//     switch(instrPtr->type)
//     {
//         case RType:
//             executeRTypeInstruction(instrPtr);
//             break;
//         case IType:
//             executeITypeInstruction(instrPtr);
//             break;
//         case JType:
//             executeJTypeInstruction(instrPtr);
//             break;
//         case End:
//             processor->scoreboard->inValidate($pc);
//             pipeline->flush = 1;
//             pipeline->stallPipeline();
//             instrPtr->nextPipeStage();
//             return;
//     };
//     return;
// };
