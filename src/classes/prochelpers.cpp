#include "prochelpers.h"


void Scoreboard::validate(Register r)
{
    auto itr = board.find(r);
    if (itr != board.end()) {
        itr->second = 1;
        return;
    }
    board.insert(std::pair<Register, int>(r, 1));
    return;
};

void Scoreboard::inValidate(Register r) 
{
    auto itr = board.find(r);
    if (itr != board.end()) {
        itr->second = 0;
        return;
    }
    board.insert(std::pair<Register, int>(r, 0));
    return;
}

int Scoreboard::isValid(Register r) 
{
    auto itr = board.find(r);
    if (itr != board.end()) {
        return itr->second;
    }
    return -1;
}

void ResultForwarder::addValue(Register r, int value) 
{
    auto itr = valueMap.find(r);
    if (itr != valueMap.end()) {
        itr->second = r;
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

