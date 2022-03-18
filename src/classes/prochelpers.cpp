#include "prochelpers.h"
#include "constants.h"
#include "termcolor.h"

ScoreboardEntry::ScoreboardEntry(Register r)
{
    reg = r;
    valid = 1;
    value = 0;
    tag = "";
};

Scoreboard::Scoreboard()
{
    board = {
        {$r0, new ScoreboardEntry($r0)},
        {$r1, new ScoreboardEntry($r1)},
        {$r2, new ScoreboardEntry($r2)},
        {$r3, new ScoreboardEntry($r3)},
        {$r4, new ScoreboardEntry($r4)},
        {$r5, new ScoreboardEntry($r5)},
        {$r6, new ScoreboardEntry($r6)},
        {$r7, new ScoreboardEntry($r7)},
        {$r8, new ScoreboardEntry($r8)},
        {$r9, new ScoreboardEntry($r9)},
        {$r10, new ScoreboardEntry($r10)},
        {$r11, new ScoreboardEntry($r11)},
        {$r12, new ScoreboardEntry($r12)},
        {$r13, new ScoreboardEntry($r13)},
        {$r14, new ScoreboardEntry($r14)},
        {$r15, new ScoreboardEntry($r15)},
        {$r16, new ScoreboardEntry($r16)},
        {$r17, new ScoreboardEntry($r17)},
        {$r18, new ScoreboardEntry($r18)},
        {$r19, new ScoreboardEntry($r19)},
        {$r20, new ScoreboardEntry($r20)},
        {$r21, new ScoreboardEntry($r21)},
        {$r22, new ScoreboardEntry($r22)},
        {$r23, new ScoreboardEntry($r23)},
        {$r24, new ScoreboardEntry($r24)},
        {$r25, new ScoreboardEntry($r25)},
        {$r26, new ScoreboardEntry($r26)},
        {$r27, new ScoreboardEntry($r27)},
        {$r28, new ScoreboardEntry($r28)},
        {$r29, new ScoreboardEntry($r29)},
        {$r30, new ScoreboardEntry($r30)},
        {$r31, new ScoreboardEntry($r31)},
        {$pc, new ScoreboardEntry($pc)}
    };
};

void Scoreboard::validate(Register r, int value)
{
    auto itr = board.find(r);
    if (itr != board.end()) {
        itr->second->updateValidity(1);
        itr->second->updateValue(value);
        return;
    }
    return;
};

void Scoreboard::inValidate(Register r) 
{
    auto itr = board.find(r);
    if (itr != board.end()) {
        itr->second->updateValidity(0);
        return;
    }
    return;
}

std::pair<int, int> Scoreboard::isValid(Register r) 
{
    auto itr = board.find(r);
    if (itr != board.end()) {
        return std::pair<int, int>(itr->second->isValid(), itr->second->getValue());
    }
    return std::pair<int, int>(0,0);
}

int Scoreboard::getSize()
{
    return board.size();
}

void Scoreboard::saveState()
{
    savedState = std::map<Register, ScoreboardEntry*>(board);
    return;
}

void Scoreboard::restoreState()
{
    board = std::map<Register, ScoreboardEntry*>(savedState);
    savedState = std::map<Register, ScoreboardEntry*>();
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

rs::ReservationStationEntry::ReservationStationEntry(std::string tag_name): tag(tag_name)
{
    isReserved = 0;
    src_one = std::tuple<std::string, int, int>("", 0, 0);
    src_two = std::tuple<std::string, int, int>("", 0, 0);
    return;
}

rs::ReservationStation::ReservationStation()
{
    entries = {
        {"tag01", new ReservationStationEntry("tag01")},
        {"tag02", new ReservationStationEntry("tag02")},
        {"tag03", new ReservationStationEntry("tag03")},
        {"tag04", new ReservationStationEntry("tag04")},
        {"tag05", new ReservationStationEntry("tag05")},
        {"tag06", new ReservationStationEntry("tag06")},
        {"tag07", new ReservationStationEntry("tag07")},
        {"tag08", new ReservationStationEntry("tag08")},
        {"tag09", new ReservationStationEntry("tag09")},
        {"tag10", new ReservationStationEntry("tag10")},
        {"tag11", new ReservationStationEntry("tag11")},
        {"tag12", new ReservationStationEntry("tag12")},
        {"tag13", new ReservationStationEntry("tag13")},
        {"tag14", new ReservationStationEntry("tag14")},
        {"tag15", new ReservationStationEntry("tag15")},
        {"tag16", new ReservationStationEntry("tag16")},
        {"tag17", new ReservationStationEntry("tag17")},
        {"tag18", new ReservationStationEntry("tag18")},
        {"tag19", new ReservationStationEntry("tag19")},
        {"tag20", new ReservationStationEntry("tag20")},
        {"tag21", new ReservationStationEntry("tag21")},
        {"tag22", new ReservationStationEntry("tag22")},
        {"tag23", new ReservationStationEntry("tag23")},
        {"tag24", new ReservationStationEntry("tag24")},
        {"tag25", new ReservationStationEntry("tag25")},
        {"tag26", new ReservationStationEntry("tag26")},
        {"tag27", new ReservationStationEntry("tag27")},
        {"tag28", new ReservationStationEntry("tag28")},
        {"tag29", new ReservationStationEntry("tag29")},
        {"tag30", new ReservationStationEntry("tag30")},
        {"tag31", new ReservationStationEntry("tag31")},
        {"tag32", new ReservationStationEntry("tag32")},
    };
};

rs::ReservationStationEntry* rs::ReservationStation::getEntry(std::string tag_name)
{
    if (tag_name.at(0) > 112 || tag_name.at(0) < 97) return NULL;
    return entries.at(tag_name);
}

bool rs::ReservationStation::hasEmptyEntries()
{
    for (auto const& pair : entries)
    {
        if (!pair.second->isReserved) return true;
    }
    return false;
}