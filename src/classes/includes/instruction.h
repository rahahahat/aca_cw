#include <string>
#include <iostream>
#include "isa.h"
#include "pipestage.h"

#ifndef _INSTRUCTION_DEFINED_
#define _INSTRUCTION_DEFINED_

namespace Instructions {
    enum instructionType {
        R_Type, I_Type, J_Type
    };
    class Instruction {
        private:
            pipestage stage;
        public:
            instructionType type;
            std::string instrString;
            Instruction();
            Instruction(std::string instr);
            void nextPipeStage();
            pipestage getCurrentPipeStage();
    };
}

#endif