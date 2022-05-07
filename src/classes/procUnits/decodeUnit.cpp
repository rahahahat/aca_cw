#include "procUnit.h"
#include "decodeUnit.h"
#include "fetchUnit.h"
#include "termcolor.h"
#include "processor.h"
#include "pipestage.h"
#include "lsq.h"
#include "config.h"
#include "robuff.h"
#include "util.h"

DecodeUnit::DecodeUnit()
{
    return;
};

void DecodeUnit::run(Instructions::Instruction *instrPtr)
{
    pre(instrPtr);
    decode(instrPtr);
    post(instrPtr);
    return;
};

void DecodeUnit::decode(Instructions::Instruction *instrPtr)
{
    // TODO: Add halt support to end program
    std::vector<std::string> splitInstr = splitString(instrPtr->instrString);
    std::pair<Opcodes, InstructionType> InsPair;
    if (!instrPtr->instrString.compare("")) {
        InsPair = InstructionPairMap.at("halt");
    } else {
        InsPair = InstructionPairMap.at(splitInstr.front());
    }
    instrPtr->opcode = InsPair.first;
    instrPtr->type = InsPair.second;
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
            // Instruction format: opcode rs rt immediate
            instrPtr->immediateOrAddress = std::stoi(splitInstr.back(), 0, 16); // immediate
            splitInstr.pop_back();
            instrPtr->rt = RegisterMap.at(splitInstr.back()); // rt
            splitInstr.pop_back();
            instrPtr->rs = RegisterMap.at(splitInstr.back()); // rs
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

ODecodeUnit::ODecodeUnit()
{
    fn = new FetchUnit();
    return;
};

void ODecodeUnit::post()
{
    if (!busy) return;
    busy = false;
    instr = NULL;
    switch(instr->type)
    {
        case JType:
            // TODO: How should jumps work? 
            // instrP
        case End:
            // TODO: Think about the stall guard here!
            // instrPtr->stage = DONE;
            if (!processor->getPipeline()->stalled()) processor->getPipeline()->stallPipeline(Halt);
            processor->getSB()->invalidatePC();
            return;
        default:
            instr->stage = ISSUE;
            if (instr->opcode == LW || instr->opcode == SW) 
            {
                LSQNode* node = processor->getLsq()->addToQueue(instr);
                processor->getRB()->addEntry(node->getTag(), instr);
                return;
            }
            if (isInstrBranch(instr))
            {
                processor->getPipeline()->stallPipeline(Branch); 
            }
            std::string tag = processor->getRS()->reserve(instr);
            processor->getRB()->addEntry(tag, instr);
            return;
    }
};

void ODecodeUnit::fetchTick()
{
    if (busy) return;
    if (processor->getPipeline()->stalled()) return; 
    
    Instructions::Instruction *instrPtr = new Instructions::Instruction();
    fn->run(instrPtr);
    instr = instrPtr;
    busy = true;
    instr->stage = DECODE;
    return;
}

void ODecodeUnit::decodeTick()
{
    if (!busy) return;
    if (processor->getPipeline()->stalled()) return;
    run();
};

void ODecodeUnit::flush(std::string tag)
{
    
}

void ODecodeUnit::run()
{
    pre();
    decode();
}