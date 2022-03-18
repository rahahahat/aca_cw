#include "procUnit.h"

class FetchUnit: public ProcUnit
{
    public:
        FetchUnit(Pipeline *pl);
        Instructions::Instruction* fetch(Instructions::Instruction *instrPtr);
};
