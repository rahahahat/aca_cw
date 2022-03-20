#include <string>

#ifndef _CONSTANTS_DEFINED_
#define _CONSTANTS_DEFINED_

const std::string PROGRAM_LIST = "-programlist";

enum Cast {
    ScalarPipelineCast,
};

namespace ProcUnitEvents {
    const std::string POPULATE_RS = "put_instruction_into_reservation_station";
};

#endif