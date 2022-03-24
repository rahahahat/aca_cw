#include <string>
#include <iostream>
#include "isa.h"
#include "pipestage.h"

#ifndef _INSTRUCTION_DEFINED_
#define _INSTRUCTION_DEFINED_

namespace Instructions {

    class Instruction {
        private:
            int num_cycles;
            int curr_cycle;
        public:
            void setNumCycle(int cycle);
            void decrementCycle();
            Register rs;
            pipestage stage;
            Register rt;
            Register rd;
            int id;
            int immediateOrAddress;
            int temp;
            int src1;
            int src2;
            std::string tag;
            bool isReadyToExecute;
            InstructionType type;
            Opcodes opcode;
            std::string instrString;
            Instruction();
            Instruction(std::string instr);
            pipestage getCurrentPipeStage();
            void nextPipeStage();
            int getCurrCycle();
    };
}

#endif