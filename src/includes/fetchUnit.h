#ifndef _FETCHUNIT_DEFINED_
#define _FTECHUNIT_DEFINED_

class ProcUnit;

class FetchUnit: public ProcUnit 
{
    public:
        FetchUnit(Pipeline *pl);
        Instructions::Instruction* fetch(Instructions::Instruction *instrPtr);
};

#endif