#include <map>
#include <iostream>

#ifndef _BRANCH_INCLUDED_
#define _BRANCH_INCLUDED_

// #################################################################################################
// BranchTargetBuffer
// #################################################################################################

class BranchTargetBuffer
{
    protected:
        int max_size;
        std::map<std::string, int> buffer;
        void btbInsert(std::string instr, int pc, int target);
    public:
        int hit(std::string instr);
        BranchTargetBuffer();
};

// #################################################################################################
// BranchPredictor Base Class
// #################################################################################################

class BranchPredictor: public BranchTargetBuffer
{ 
    protected:
        int total_predictions;
        int correct_predictions;
    public:
        BranchPredictor();
        void printPredictions();
        virtual void insert(std::string instr, int pc, int target) 
        { 
            btbInsert(instr, pc, target);
        }
        virtual int predict(std::string instr) {
            return -1; 
        };
        virtual void update(std::string instr, int pc, bool correct) {
            return; 
        };
        virtual void incrementCount() { 
            total_predictions++;
        };
};

// #################################################################################################
// Speculative Execution
// #################################################################################################

class Speculate: public BranchPredictor
{
    private:
        bool take_branch;
    public:
        Speculate();
        int predict(std::string instr);
        void update(std::string instr, int pc, bool correct);
};

// #################################################################################################
// OneBit Branch Predictor
// #################################################################################################

class OneBit: public BranchPredictor
{
    private:
        std::map<std::string, bool> prediction_lookup;
    public:
        void insert(std::string instr, int pc, int target);
        int predict(std::string instr);
        void update(std::string instr, int pc, bool correct);
};

// #################################################################################################
// TwoBit Branch Predictor
// #################################################################################################

class TwoBit: public BranchPredictor
{
    enum BranchState: int {
        STRONGLY_TAKEN = 2,
        WEAKLY_TAKEN = 1,    
        STRONGLY_NOT_TAKEN = -1,  
        WEAKLY_NOT_TAKEN = -2
    };
    private:
        std::map<std::string, BranchState> prediction_lookup;
        BranchState FSM(BranchState current, bool result);
    public:
        void insert(std::string instr, int pc, int target);
        int predict(std::string instr);
        void update(std::string instr, int pc, bool correct);
};

#endif