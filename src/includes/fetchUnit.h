#include "procUnit.h"

class FetchUnit: public ProcUnit
{
    public:
        FetchUnit(Pipeline *pl, int units);
        Instructions::Instruction* fetch(Instructions::Instruction *instrPtr);
};
