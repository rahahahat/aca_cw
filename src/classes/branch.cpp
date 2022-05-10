#include "branch.h"
#include "config.h"

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

void BranchTargetBuffer::insert(std::string instr, int pc, int target)
{
    std::string insert_str = instr + "-" + std::to_string(pc);
    if (hit(insert_str) == -1)
    {
        buffer.insert(std::pair<std::string, int>(insert_str, target));
    }
    return;
};

BranchPredictor::BranchPredictor(BranchTargetBuffer* bt)
{
    btb = bt;
    return;
};

Speculate::Speculate(BranchTargetBuffer* bt) : BranchPredictor(bt)
{
    conf* config = getConfig();
    take_branch = config->speculate->take_branch;
    return;
};

int Speculate::predict(std::string instr)
{
    if (!take_branch) return -1;
    return btb->hit(instr);
};
