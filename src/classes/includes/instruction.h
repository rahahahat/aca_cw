#include <string>
#include <iostream>

#include "pipestage.h"

#ifndef _INSTRUCTION_DEFINED_
#define _INSTRUCTION_DEFINED_

namespace Instructions {
    enum Opcode {
        ADD, ADD_I, ADD_UI, SUB, SUB_U, MUL, MULT, DIVIDE
    };
    class Instruction {
        public:
            pipestage stage;
            Opcode opcode;
    };
}

#endif