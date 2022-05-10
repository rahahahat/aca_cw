#include <map>
#include <iostream>

#ifndef _BRANCH_INCLUDED_
#define _BRANCH_INCLUDED_

class BranchTargetBuffer
{
    private:
        int max_size;
        std::map<std::string, int> buffer;
    public:
        BranchTargetBuffer();
        int hit(std::string instr);
        void insert(std::string instr, int pc, int target);
};

class BranchPredictor
{
    protected:
        BranchTargetBuffer* btb;
    public:
        BranchPredictor(BranchTargetBuffer* bt);
        virtual int predict(std::string instr) {};
};

class Speculate: BranchPredictor
{
    private:
        bool take_branch;
    public:
        Speculate(BranchTargetBuffer* bt);
        int predict(std::string instr);
};

#endif