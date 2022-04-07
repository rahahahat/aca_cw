#include "procUnit.h"
#include "decodeUnit.h"
#include "constants.h"
#include "termcolor.h"
#include "processor.h"

DecodeUnit::DecodeUnit()
{
    return;
};

void DecodeUnit::run(Instructions::Instruction *instrPtr)
{
    std::cout << termcolor::bold << termcolor::blue
    << "Decoding instruction: " << instrPtr->instrString
    << std::endl;

    pre(instrPtr);
    decode(instrPtr);
    post(instrPtr);
    return;
};

void DecodeUnit::decode(Instructions::Instruction *instrPtr)
{
    std::vector<std::string> splitInstr = splitString(instrPtr->instrString);
    std::pair<Opcodes, InstructionType> InsPair;
    if (!instrPtr->instrString.compare("")) {
        InsPair = InstructionPairMap.at("halt");
    } else {
        InsPair = InstructionPairMap.at(splitInstr.front());
    }
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
        case End:
            return;
    };
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
    if (instrPtr->opcode == MULT) instrPtr->setNumCycle(4);
    instrPtr->setNumCycle(2);

};

void DecodeUnit::decodeITypeInstruction(Instructions::Instruction *instrPtr, std::vector<std::string> splitInstr, std::pair<Opcodes, InstructionType> insPair)
{
    Opcodes opcode = insPair.first;
    std::string label;
    switch(opcode)
    {
        case BEQ:
        case BNE:
        case BL:
        case BGTE:
            label = splitInstr.back();
            instrPtr->immediateOrAddress = processor->labelMap.at(label); //label
            splitInstr.pop_back();
            instrPtr->rt = RegisterMap.at(splitInstr.back()); // r2
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
            instrPtr->setNumCycle(2);
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



// void ScalarDecodeUnit::invalidateDestReg(Instructions::Instruction *instrPtr)
// {   
//     Opcodes opcode = instrPtr->opcode;
//     if (opcode == BEQ || opcode == BGTE || opcode == BNE || opcode == BL) return;
//     if (instrPtr->type == RType) {
//         processor->resultForwarder->removeValue(instrPtr->rd);
//         processor->scoreboard->inValidate(instrPtr->rd);
//         return;
//     }
//     if (instrPtr->type == IType) {
//         processor->resultForwarder->removeValue(instrPtr->rt);
//         processor->scoreboard->inValidate(instrPtr->rt);
//         return;
//     }
//     return;
// }

// void ScalarDecodeUnit::pre(Instructions::Instruction *instrPtr)
// {
//     std::cout 
//     << termcolor::green
//     << termcolor::bold
//     << "Decoding Instruction: "
//     << termcolor::reset 
//     << instrPtr->instrString 
//     << std::endl;
//     return;
// };

// void ScalarDecodeUnit::post(Instructions::Instruction *instrPtr)
// {
//     invalidateDestReg(instrPtr);
//     return;
// }

ODecodeUnit::ODecodeUnit() {};

void ODecodeUnit::post(Instructions::Instruction *instrPtr)
{
    switch(instrPtr->type)
    {
        case JType:
            instrPtr->stage = WRITEBACK;
            return;
        case End:
            // TODO: Think about the stall guard here!
            instrPtr->stage = EXECUTE;
            if (!processor->getPipeline()->stalled()) processor->getPipeline()->stallPipeline(Halt);
            return;
        default:
            instrPtr->stage = ISSUE;
            processor->getRS()->reserve(instrPtr);
            if (isInstrBranch(instrPtr) && !processor->getPipeline()->stalled()) processor->getPipeline()->stallPipeline(Branch);
            if (instrPtr->opcode == LW || instrPtr->opcode == SW) processor->getLsq()->addToQueue(instrPtr);
            return;
    }
}