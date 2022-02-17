#include <string>
#include <iostream>
#include "isa.h"
#include "pipestage.h"

#ifndef _INSTRUCTION_DEFINED_
#define _INSTRUCTION_DEFINED_

namespace Instructions {

    class Instruction {
        private:
            pipestage stage;
        public:
            Registers rs;
            Registers rt;
            Registers rd;
            int immediateOrAddress;
            int temp;
            InstructionType type;
            Opcodes opcode;
            std::string instrString;
            Instruction();
            Instruction(std::string instr);
            void nextPipeStage();
            pipestage getCurrentPipeStage();
    };
} 

#endif