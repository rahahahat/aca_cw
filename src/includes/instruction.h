#include <string>
#include <iostream>
#include "isa.h"
#include "pipestage.h"

#ifndef _INSTRUCTION_DEFINED_
#define _INSTRUCTION_DEFINED_

namespace Instructions {

    class Instruction {
        public:
            Register rs;
            pipestage stage;
            Register rt;
            Register rd;
            int id;
            int immediateOrAddress;
            int temp;
            int src1;
            int src2;
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