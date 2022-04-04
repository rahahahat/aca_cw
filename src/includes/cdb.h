#include "prochelpers.h"
#include "lsq.h"
#include "instruction.h"

class CDB
{
    public:
        // ROB
        rs::ReservationStation* rst;
        LSQueue * load_store_queue;
        Scoreboard *scoreboard;
        void broadcast(Instructions::Instruction*);
};
