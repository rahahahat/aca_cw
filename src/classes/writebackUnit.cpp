#include "procUnit.h"
#include "writebackUnit.h"

WriteBackUnit::WriteBackUnit(Pipeline *pl)
{
    pipeline = pl;
    return;
}

void WriteBackUnit::run(Instructions::Instruction *instrPtr)
{
    pre(instrPtr);
    writeback(instrPtr);
    post(instrPtr);
    return;
}


// void ScalarWriteBackUnit::pre(Instructions::Instruction *instrPtr)
// {
//     std::cout 
//     << termcolor::green
//     << termcolor::bold
//     << "Writing Back Instruction: "
//     << termcolor::reset 
//     << instrPtr->instrString 
//     << std::endl;
//     return;
// }

// void ScalarWriteBackUnit::post(Instructions::Instruction *instrPtr)
// {
//     validateDestReg(instrPtr);
//     return;
// }

// void ScalarWriteBackUnit::writeback(Instructions::Instruction *instrPtr)
// {
//     switch (instrPtr->opcode)
//     {
//     case ADD:
//     case ADDU:
//         processor->registers[instrPtr->rd] = instrPtr->temp;
//         break;
//     case ADDI:
//     case ADDIU:
//         processor->registers[instrPtr->rt] = instrPtr->temp;
//         break;
//     case SUB:
//     case SUBU:
//         processor->registers[instrPtr->rd] = instrPtr->temp;
//         break;
//     case MULT:
//     case MULTU:
//         processor->registers[instrPtr->rd] = instrPtr->temp;
//         break;
//     case DIV:
//     case DIVU:
//         processor->registers[instrPtr->rd] = instrPtr->temp;
//         break;
//     case AND:
//         processor->registers[instrPtr->rd] = instrPtr->temp;
//         break;
//     case ANDI:
//         processor->registers[instrPtr->rt] = instrPtr->temp;
//         break;
//     case OR:
//         processor->registers[instrPtr->rd] = instrPtr->temp;
//         break;
//     case ORI:
//         processor->registers[instrPtr->rt] = instrPtr->temp;
//         break;
//     case NOR:
//         processor->registers[instrPtr->rd] = instrPtr->temp;
//         break;
//     case XOR:
//         processor->registers[instrPtr->rd] = instrPtr->temp;
//         break;
//     case LW:
//         processor->registers[instrPtr->rt] = instrPtr->temp;
//         break;
//     case LSR:
//         processor->registers[instrPtr->rt] = instrPtr->temp;
//         break;
//     case BEQ:
//     case BNE:
//     case BL:
//     case BGTE:
//         if (instrPtr->temp) // if we are branching
//         {
//             processor->PC = instrPtr->immediateOrAddress;
//             pipeline->flush = 0;
//             processor->scoreboard->validate($pc);
//         }
//         break;
//     case HALT:
//         pipeline->flush = 0;
//         return;
//     case J:
//         processor->PC = instrPtr->temp;
//         pipeline->flush = 0;
//         processor->scoreboard->validate($pc);
//         break;
//     default:
//         break;
//     }
// }

// void ScalarWriteBackUnit::validateDestReg(Instructions::Instruction *instrPtr)
// {
//     Opcodes opcode = instrPtr->opcode;
//     if (opcode == BEQ || opcode == BNE || opcode == BL || opcode == BGTE || opcode == HALT) return;
//     if (instrPtr->type == IType)
//     {
//         processor->scoreboard->validate(instrPtr->rt);
//         return;
//     } 
//     if (instrPtr->type == RType)
//     {
//         processor->scoreboard->validate(instrPtr->rd);
//         return;
//     }
//     return;
// }