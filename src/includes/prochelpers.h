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
        std::map<Register, int> savedState;
    public:
        Scoreboard();
        void saveState();
        void restoreState();
        void equaliseSavedState();
        void validate(Register r);
        void inValidate(Register r);
        int isValid(Register r);
        int getSize();
        void memDump();
};

class ResultForwarder
{
    private:
        std::map<Register, int> valueMap;
        std::map<Register, int> savedState;
    public:
        void saveState();
        void restoreState();
        void equaliseSavedState();
        void addValue(Register r, int value);
        void removeValue(Register r);
        std::pair<int, int> getValue(Register r);
        int getSize();
        void memDump();
};

#endif