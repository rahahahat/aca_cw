#include <iostream>
#include <map>
#include <string>
#include "isa.h"

#ifndef _SCOREBOARD_DEFINED_
#define _SCOREBOARD_DEFINED_

class Scoreboard
{
    private:
        std::map<Registers, int> board;
    public:
        void validate(Registers r);
        void inValidate(Registers r);
        int isValid(Registers r);
};

class ResultForwarder
{
    private:
        std::map<Registers, int> valueMap;
    public:
        void addValue(Registers r, int value);
        void removeValue(Registers r);
        std::pair<int, int> getValue(Registers r);
};

#endif