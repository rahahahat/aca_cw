#include "branch.h"
#include "config.h"
#include "termcolor.h"

// #################################################################################################
// BranchTargetBuffer
// ################################################################################################

BranchTargetBuffer::BranchTargetBuffer()
{
    max_size = 32;
    return;
};

int BranchTargetBuffer::hit(std::string instr)
{
    if (buffer.find(instr) != buffer.end())
    {
        return buffer.at(instr);
    }
    return -1;
};

void BranchTargetBuffer::btbInsert(std::string instr, int pc, int target)
{
    std::string insert_str = instr + "-" + std::to_string(pc);
    if (hit(insert_str) == -1)
    { 
        buffer.insert(std::pair<std::string, int>(insert_str, target));
    }
    return;
};

// #################################################################################################
// BranchPredictor Base Class
// #################################################################################################

BranchPredictor::BranchPredictor() : BranchTargetBuffer()
{
    total_predictions = 0;
    correct_predictions = 0;
    return;
};

void BranchPredictor::printPredictions()
{
    float accuracy = (float)correct_predictions/(float)total_predictions;
    std::cout
    << "\n"
    << termcolor::on_bright_blue
    << "Total Predictions:"
    << termcolor::reset << " " <<  total_predictions << "\n" 
    << termcolor::on_bright_green
    << "Correct Predictions:" 
    << termcolor::reset << " " << correct_predictions << "\n"
    << termcolor::on_bright_cyan << "Accuracy:"
    << termcolor::reset << " " << accuracy << std::setprecision(5) << std::endl;
}

// #################################################################################################
// Speculative Execution
// #################################################################################################

Speculate::Speculate() : BranchPredictor()
{
    conf* config = getConfig();
    take_branch = config->speculate->take_branch;
    return;
};

int Speculate::predict(std::string instr)
{
    if (!take_branch) return -1;
    return hit(instr);
};

void Speculate::update(std::string instr, int pc, bool correct)
{
    total_predictions++;
    if (correct)
    {
        correct_predictions++;
    }
    return;
}

// #################################################################################################
// OneBit Branch Predictor
// #################################################################################################

void OneBit::insert(std::string instr, int pc, int target)
{
    
    std::string insert_str = instr + "-" + std::to_string(pc);
    if (hit(insert_str) == -1)
    {
        btbInsert(instr, pc, target);
        prediction_lookup.insert(std::pair<std::string, bool>(insert_str, false));
    }
}

void OneBit::update(std::string instr, int pc, bool correct)
{
    // total_predictions++;
    std::string insert_str = instr + "-" + std::to_string(pc);

    if (!correct)
    {
        auto pair = prediction_lookup.find(insert_str);
        pair->second = !pair->second;
        return;
    }
    correct_predictions++;
    return;
}

int OneBit::predict(std::string instr)
{
    int target = hit(instr);
    if (target != -1)
    {
        if (prediction_lookup.at(instr))
        {
            return target;
        }
    }
    return -1;
}

// #################################################################################################
// TwoBit Branch Predictor
// #################################################################################################
void TwoBit::insert(std::string instr, int pc, int target)
{

    std::string insert_str = instr + "-" + std::to_string(pc);
    if (hit(insert_str) == -1)
    {
        btbInsert(instr, pc, target);
        prediction_lookup.insert(std::pair<std::string, BranchState>(insert_str, WEAKLY_NOT_TAKEN));
    }
};

int TwoBit::predict(std::string instr)
{
    int target = hit(instr);
    if (target != -1)
    {
        BranchState state = prediction_lookup.at(instr);
        if (state == STRONGLY_TAKEN || state == WEAKLY_TAKEN)
        {
            return target;
        }
    }
    return -1;
}

void TwoBit::update(std::string instr, int pc, bool correct)
{
    // total_predictions++;
    std::string insert_str = instr + "-" + std::to_string(pc);
    auto pair= prediction_lookup.find(insert_str);
    BranchState newState = FSM(pair->second, correct);
    pair->second = newState;
    if (correct)
    {
        correct_predictions++;
        return;
    }
    return;
};

TwoBit::BranchState TwoBit::FSM(BranchState current, bool result)
{
    switch(current) {
        case STRONGLY_TAKEN:
            if (result)
            {
                return STRONGLY_TAKEN;
            }
            return WEAKLY_TAKEN;
        case WEAKLY_TAKEN:
            if (result)
            {
                return STRONGLY_TAKEN;
            }
            return STRONGLY_NOT_TAKEN;
        case WEAKLY_NOT_TAKEN:
            if (result)
            {
                return STRONGLY_NOT_TAKEN;
            }
            return STRONGLY_TAKEN;
        case STRONGLY_NOT_TAKEN:
            if (result)
            {
                return STRONGLY_NOT_TAKEN;
            }
            return WEAKLY_NOT_TAKEN;
    }
    return WEAKLY_NOT_TAKEN;
}

