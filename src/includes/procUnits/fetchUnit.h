#ifndef _FETCHUNIT_DEFINED_
#define _FTECHUNIT_DEFINED_

class ProcUnit;

class FetchUnit: public ProcUnit 
{
    public:
        FetchUnit();
        Instructions::Instruction* fetch(Instructions::Instruction *instrPtr);
        Instructions::Instruction* scalarFetch(Instructions::Instruction *instrPtr);
        void run(Instructions::Instruction* instrPtr);
};

#endif