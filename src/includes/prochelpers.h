#include <iostream>
#include <map>
#include <string>
#include "isa.h"

#ifndef _SCOREBOARD_DEFINED_
#define _SCOREBOARD_DEFINED_

class Scoreboard
{
    private:
        std::map<Register, int> board;
    public:
        Scoreboard();
        void validate(Register r);
        void inValidate(Register r);
        int isValid(Register r);
        int getSize();
};

class ResultForwarder
{
    private:
        std::map<Register, int> valueMap;
    public:
        void addValue(Register r, int value);
        void removeValue(Register r);
        std::pair<int, int> getValue(Register r);
        int getSize();
};

#endif