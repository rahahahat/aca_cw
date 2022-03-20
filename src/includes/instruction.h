#include <string>
#include <iostream>
#include "isa.h"
#include "pipestage.h"

#ifndef _INSTRUCTION_DEFINED_
#define _INSTRUCTION_DEFINED_

namespace Instructions {

    class Instruction {
        private:
            const int num_cycles;
            int curr_cycle;
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
            bool isReadyToExecute;
            InstructionType type;
            Opcodes opcode;
            std::string instrString;
            Instruction();
            Instruction(std::string instr);
            Instruction(std::string instr, int cycles);
            pipestage getCurrentPipeStage();
            void nextPipeStage();
            void incrementCycle();
            int getCurrCycle();
    };
}

#endif