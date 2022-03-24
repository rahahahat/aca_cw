#include "prochelpers.h"
#include "constants.h"
#include "termcolor.h"
#include <sstream>

ScoreboardEntry::ScoreboardEntry(Register r)
{
    reg = r;
    valid = 1;
    value = 0;
    tag = "~";
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

void Scoreboard::inValidate(Register r, std::string new_tag) 
{
    auto itr = board.find(r);
    if (itr != board.end()) {
        itr->second->updateValidity(0);
        itr->second->updateTag(new_tag);
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

ScoreboardEntry* Scoreboard::getEntry(Register r)
{
    return board[r];
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
    src_one = std::make_tuple("~", 0, 0);
    src_two = std::make_tuple("~", 0, 0);
    return;
}

rs::ReservationStation::ReservationStation(Scoreboard* sb)
{
    scoreboard = sb;
    size = 64;
    for (int x = 0; x < size; x++)
    {
        int tagIndex = x+1;
        std::stringstream ss;
        tagIndex < 10 ? ss << "tag0"<< tagIndex : ss << "tag"<< tagIndex;
        std::string tag = ss.str();
        entries.insert(std::pair<std::string, ReservationStationEntry*>(tag, new ReservationStationEntry(tag)));
    }
    auto func = std::bind(&rs::ReservationStation::reserveEntryOnEvent, this, std::placeholders::_1);
    EventWrapper::getEventWrapperInstance()->addEventListerner("ABC", func);
};

rs::ReservationStationEntry* rs::ReservationStation::getEntry(std::string tag_name)
{
    if (tag_name.at(0) > 112 || tag_name.at(0) < 97) return NULL;
    return entries.at(tag_name);
}

rs::ReservationStationEntry* rs::ReservationStation::hasEmptyEntries()
{
    for (auto const& pair : entries)
    {
        if (!pair.second->isReserved)
        {
            return pair.second;
        }
    }
    return NULL;
}

void rs::ReservationStation::reserve(Instructions::Instruction *instrPtr)
{
    rs::ReservationStationEntry* entry = hasEmptyEntries();
    if (!hasEmptyEntries()) return;
    instrPtr->stage = EXECUTE;
    instrPtr->tag = entry->tag;
    switch (instrPtr->type)
    {
    case RType:
        reserveRType(entry, instrPtr);
        return;
    case IType:
        reserveIType(entry, instrPtr);
        return;
    default:
        return;
    }
}

ScoreboardEntry* rs::ReservationStation::getScoreboardEntry(Register r)
{
    return scoreboard->getEntry(r);
}



void rs::ReservationStation::reserveRType(ReservationStationEntry *entry, Instructions::Instruction *instrPtr)
{
    entry->isReserved = true;
    entry->instr_type = instrPtr->type;
    ScoreboardEntry* sb_entry_one = getScoreboardEntry(instrPtr->rs);
    ScoreboardEntry* sb_entry_two = getScoreboardEntry(instrPtr->rt);
    
    std::get<0>(entry->src_one) = sb_entry_one->getTag();
    std::get<1>(entry->src_one) = sb_entry_one->isValid();
    std::get<2>(entry->src_one) = sb_entry_one->getValue();

    std::get<0>(entry->src_two) = sb_entry_two->getTag();
    std::get<1>(entry->src_two) = sb_entry_two->isValid();
    std::get<2>(entry->src_two) = sb_entry_two->getValue();

    scoreboard->inValidate(instrPtr->rd, entry->tag);
    return;
}

void rs::ReservationStation::reserveIType(ReservationStationEntry *entry, Instructions::Instruction *instrPtr)
{
    entry->isReserved = true;
    entry->instr_type = instrPtr->type;
    ScoreboardEntry* sb_entry_one = getScoreboardEntry(instrPtr->rt);

    std::get<0>(entry->src_one) = sb_entry_one->getTag();
    std::get<1>(entry->src_one) = sb_entry_one->isValid();
    std::get<2>(entry->src_one) = sb_entry_one->getValue();

    scoreboard->inValidate(instrPtr->rs, entry->tag);
    return;
}

void rs::ReservationStation::reserveJType(ReservationStationEntry *entry, Instructions::Instruction *instrPtr)
{
    entry->isReserved = true;
    entry->instr_type = instrPtr->type;

    std::get<0>(entry->src_one) = "~";
    std::get<1>(entry->src_one) = 1;
    std::get<2>(entry->src_one) = 0;

    std::get<0>(entry->src_two) = "~";
    std::get<1>(entry->src_two) = 1;
    std::get<2>(entry->src_two) = 0;

    return;
}


void rs::ReservationStation::reserveEntryOnEvent(const EventBase& base)
{
    const Event<Instructions::Instruction*>& event = static_cast<const Event<Instructions::Instruction*>&>(base);
    reserve(event.payload);
    return;
}

void rs::ReservationStation::populateInstruction(Instructions::Instruction *instrPtr)
{
    std::string rs_tag = instrPtr->tag;
    rs::ReservationStationEntry* rs_entry = getEntry(rs_tag);
    if (!rs_entry->isReserved) return;
    int isValid = std::get<1>(rs_entry->src_one) && std::get<1>(rs_entry->src_two);
    if (!isValid) return;
    switch(instrPtr->type)
    {
        case RType:
            instrPtr->src2 = std::get<2>(rs_entry->src_two);
            instrPtr->src1 = std::get<2>(rs_entry->src_one);
            break;
        case IType:
        // TODO: Verify ITYPE instruction source in decode and execute
            instrPtr->src1 = std::get<2>(rs_entry->src_one);
            break;
    }
    instrPtr->isReadyToExecute = true;
    return;
}

void rs::ReservationStation::print()
{
    for (auto const& entry : entries)
    {
        std::cout
        << termcolor::red << termcolor:: bold
        << "Tag: "
        << entry.first
        << termcolor::reset
        << termcolor:: green << termcolor:: bold
        << "     | "
        << "tag: "
        << std::get<0>(entry.second->src_one)
        << " | "
        << "valid: "
        << std::get<1>(entry.second->src_one)
        << " | "
        << "value: "
        << std::get<2>(entry.second->src_one)
        << " |"
        << termcolor::reset
        << termcolor::blue << termcolor:: bold
        << "     | "
        << "tag: "
        << std::get<0>(entry.second->src_two)
        << " | "
        << "valid: "
        << std::get<1>(entry.second->src_two)
        << " | "
        << "value: "
        << std::get<2>(entry.second->src_two)
        << " |"
        << termcolor::reset
        << std::endl;
    }
}