#ifndef _CDB_DEFINED_
#define _CDB_DEFINED_

#include "lsq.h"
#include "robuff.h"

// class Processor;
class Scoreboard;
namespace rs {

    class ReservationStation;
}
// class LSQueue;

class CommonDataBus
{
    public:
        CommonDataBus();
        Processor *processor;
        rs::ReservationStation* rsv;
        Scoreboard* sb;
        LSQueue *lsq;
        ReorderBuffer* robuff;
        void broadcast(Register destination, std::string tag, int value);
        void commit(Register destination, std::string tag,int value);
};

#endif