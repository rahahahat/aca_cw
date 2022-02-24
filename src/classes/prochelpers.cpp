#include "prochelpers.h"
#include "constants.h"
#include "termcolor.h"
Scoreboard::Scoreboard()
{
    board = {
        {$r0, 1},
        {$r1, 1},
        {$r2, 1},
        {$r3, 1},
        {$r4, 1},
        {$r5, 1},
        {$r6, 1},
        {$r7, 1},
        {$r8, 1},
        {$r9, 1},
        {$r10, 1},
        {$r11, 1},
        {$r12, 1},
        {$r13, 1},
        {$r14, 1},
        {$r15, 1},
        {$r16, 1},
        {$r17, 1},
        {$r18, 1},
        {$r19, 1},
        {$r20, 1},
        {$r21, 1},
        {$r22, 1},
        {$r23, 1},
        {$r24, 1},
        {$r25, 1},
        {$r26, 1},
        {$r27, 1},
        {$r28, 1},
        {$r29, 1},
        {$r30, 1},
        {$r31, 1},
        {$pc, 1}
    };
}

void Scoreboard::validate(Register r)
{
    auto itr = board.find(r);
    if (itr != board.end()) {
        itr->second = 1;
        return;
    }
    return;
};

void Scoreboard::inValidate(Register r) 
{
    auto itr = board.find(r);
    if (itr != board.end()) {
        itr->second = 0;
        return;
    }
    return;
}

int Scoreboard::isValid(Register r) 
{
    auto itr = board.find(r);
    if (itr != board.end()) {
        return itr->second;
    }
    return 0;
}

int Scoreboard::getSize()
{
    return board.size();
}

void Scoreboard::saveState()
{
    savedState = std::map<Register, int>(board);
    return;
}

void Scoreboard::restoreState()
{
    board = std::map<Register, int>(savedState);
    savedState = std::map<Register, int>();
    return;
}

void Scoreboard::equaliseSavedState()
{
    saveState();
    return;
}

void Scoreboard::memDump()
{
    std::cout 
    << termcolor::bold
    << "---------SB-MemDump---------" 
    << std::endl;
    for (auto it = board.begin(); it != board.end(); ++it)
    {
        std::cout
        << termcolor::bold
        << termcolor::green
        << "$r"
        << it->first
        << ":\t\t"
        << termcolor::white
        << it->second
        << termcolor::reset
        << std::endl;
    }
    std::cout << std::endl;
}

void ResultForwarder::addValue(Register r, int value) 
{
    auto itr = valueMap.find(r);
    if (itr != valueMap.end()) {
        itr->second = value;
        return;
    }
    valueMap.insert(std::pair<Register, int>(r, value));
    return;
}

void ResultForwarder::removeValue(Register r) 
{
    auto itr = valueMap.find(r);
    if (itr != valueMap.end()) {
        valueMap.erase(itr);
        return;
    }
    return;
}

std::pair<int, int> ResultForwarder::getValue(Register r)
{
    auto itr = valueMap.find(r);
    if (itr != valueMap.end()) {
        return std::pair<int, int>(1, itr->second);
    }
    return std::pair<int, int>(0, 0);
}

int ResultForwarder::getSize()
{
    return valueMap.size();
}

void ResultForwarder::saveState()
{
    savedState = std::map<Register, int>(valueMap);
    return;
}

void ResultForwarder::restoreState()
{
    valueMap = std::map<Register, int>(savedState);
    savedState = std::map<Register, int>();
    return;
}

void ResultForwarder::equaliseSavedState()
{
    saveState();
    return;
}

void ResultForwarder::memDump()
{
    std::cout 
    << termcolor::bold
    << "---------RF-MemDump---------" 
    << std::endl;
    for (auto it = valueMap.begin(); it != valueMap.end(); ++it)
    {
        std::cout
        << termcolor::bold
        << termcolor::green
        << "$r"
        << it->first
        << ":\t\t"
        << termcolor::white
        << it->second
        << termcolor::reset
        << std::endl;
    }
    std::cout << std::endl;
}