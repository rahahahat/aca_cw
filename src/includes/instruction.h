#include <string>
#include <iostream>

#include "isa.h"


#ifndef _INSTRUCTION_DEFINED_
#define _INSTRUCTION_DEFINED_

#include "pipestage.h"


namespace Instructions {

    class Instruction {
        public:
            Register rs;
            pipestage stage;
            Register rt;
            Register rd;
            int immediateOrAddress;
            int src1;
            int src2;
            std::string tag;
            bool isReadyToExecute;
            InstructionType type;
            Opcodes opcode;
            std::string instrString;
            Instruction();
            Instruction(std::string instr);
            Register getDestination();
    };
};

#endif