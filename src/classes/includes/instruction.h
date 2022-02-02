#include <string>
#include <iostream>

#include "pipestage.h"

#ifndef _INSTRUCTION_DEFINED_
#define _INSTRUCTION_DEFINED_

namespace Instructions {
    enum opcodes {
        ADD, ADD_I, ADD_UI, SUB, SUB_U, MUL, MULT, DIVIDE
    };
    class Instruction {
        private:
            string instrString;
        public:
            pipestage stage;
            opcodes opcode;
            Instruction(string instrString);
            void advancePipeStage();
    };
}

#endif