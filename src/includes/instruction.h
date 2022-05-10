#include <string>
#include <iostream>

#include "isa.h"


#ifndef _INSTRUCTION_DEFINED_
#define _INSTRUCTION_DEFINED_

#include "pipestage.h"


namespace Instructions {

    class Instruction {
        public:
            int immediateOrAddress;
            int pc_no_pred;
            int pred;
            int fetched_at_pc;

            pipestage stage;
            Register rs;
            Register rt;
            Register rd;
            Opcodes opcode;
            InstructionType type;

            std::string tag;
            std::string instrString;

            Instruction();
            Instruction(std::string instr);
            Register getDestination();
    };
};

#endif