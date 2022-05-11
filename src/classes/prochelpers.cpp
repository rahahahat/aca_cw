#include "prochelpers.h"
#include "termcolor.h"
#include "processor.h"
#include "termcolor.h"
#include <sstream>
#include "util.h"

// #################################################################################################
// ProcHelper
// #################################################################################################

ProcHelper::ProcHelper(bool force_null)
{
    if (force_null) 
    {
        processor = NULL;
        return;
    }
    processor = Processor::getProcessorInstance();
    return;
};

// #################################################################################################
// Scoreboard
// #################################################################################################

ScoreboardEntry::ScoreboardEntry(Register r)
{
    reg = r;
    valid = 1;
    value = 0;
    tag = "~";
};

Scoreboard::Scoreboard(bool force_null): ProcHelper(force_null)
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
    processor = Processor::getProcessorInstance();
};

bool Scoreboard::validate(Register r, int value, std::string tag)
{
    auto itr = board.find(r);
    if (itr != board.end()) {
        if (tag.compare(itr->second->tag) != 0) return false;
        itr->second->valid = 1;
        itr->second->value = value;
        itr->second->tag = "~";
        return true;
    }
    return false;
};


void Scoreboard::validateAll()
{
    Register regs[] = {$r0, $r1, $r2, $r3, $r4, $r5, $r6, $r7, 
    $r8, $r9, $r10, $r11, $r12, $r13, $r14, $r15, 
    $r16, $r17, $r18, $r19, $r20, $r21, $r22, $r23, 
    $r24, $r25, $r26, $r27, $r28, $r29, $r30, $r31};
    for (int i = 0; i < 32; i++)
    {
        Register reg = regs[i];
        ScoreboardEntry *entry = board.at(reg);
        entry->valid = 1;
        entry->tag = "~";
        entry->value = processor->registers[reg];
    }
    ScoreboardEntry *entry = board.at($pc);
    entry->valid = 1;
    entry->tag = "~";
    entry->value = processor->PC;
};

void Scoreboard::flush()
{
    validateAll();
};

void Scoreboard::writeToARF() {};

void Scoreboard::invalidatePC()
{
    auto itr = board.find($pc);
    if (itr != board.end()) itr->second->valid = 0;
    return;
};

bool Scoreboard::isPCValid()
{
    return board.find($pc)->second->valid == 1;
};

void Scoreboard::inValidate(Register r, std::string new_tag) 
{
    auto itr = board.find(r);
    if (itr != board.end()) {
        itr->second->valid = 0;
        itr->second->tag = new_tag;
        return;
    }
    return;
};

std::pair<int, int> Scoreboard::isValid(Register r) 
{
    auto itr = board.find(r);
    if (itr != board.end()) {
        return std::pair<int, int>(itr->second->valid, itr->second->value);
    }
    return std::pair<int, int>(0,0);
};

int Scoreboard::getSize()
{
    return board.size();
};

ScoreboardEntry* Scoreboard::getEntry(Register r)
{
    auto itr = board.find(r);
    if (itr != board.end()) {
        return itr->second;
    }
};

void Scoreboard::memDump()
{
    std::cout
    << "\n"
    << termcolor::bright_green
    << "|Register"
    << "\t|"
    << "Tag"
    << "\t|"
    << "Valid"
    << "\t|"
    << "Value"
    << "\t\t|"
    << std::endl;

    Register regs[] = {$r0, $r1, $r2, $r3, $r4, $r5, $r6, $r7, 
    $r8, $r9, $r10, $r11, $r12, $r13, $r14, $r15, 
    $r16, $r17, $r18, $r19, $r20, $r21, $r22, $r23, 
    $r24, $r25, $r26, $r27, $r28, $r29, $r30, $r31, $pc};
    for (int i = 0; i < 32; i++)
    {
        Register reg = regs[i];
        ScoreboardEntry *entry = board.at(reg);
        std::cout
        << termcolor::bright_blue
        << "|$r"
        << reg
        << "\t\t|"
        << entry->tag
        << "\t|"
        << entry->valid
        << "\t|"
        << entry->value
        << "\t\t|"
        << std::endl;
    }
    ScoreboardEntry *entry = board.at($pc);

    std::cout 
    << "|$pc\t\t|"
    << entry->tag
    << "\t|"
    << entry->valid
    << "\t|"
    << entry->value
    << "\t\t|"
    << termcolor::reset
    << std::endl;

    std::cout << std::endl;
};

// #################################################################################################
// ReserveEntry
// #################################################################################################

ReserveEntry::ReserveEntry(std::string tag_name)
{
    valid_pair = std::pair<int, int>(0,0);
    value_pair = std::pair<int, int>(0,0);
    tag_pair = std::pair<std::string, std::string>("~", "~");
    busy = false;
    opcode = NOP;
    destination = $noreg;
    isReady = false;
    tag = tag_name;
    instr = NULL;
    instrStr = "";
    address = -1;
    return;
};

Instructions::Instruction* ReserveEntry::getInstruction()
{
     return instr;
};

void ReserveEntry::setInstruction(Instructions::Instruction *instrPtr)
{
    instr = instrPtr;
    return;
};

std::pair<int, int> ReserveEntry::updateValues(std::pair<int, int> pair)
{
    value_pair = pair;
    return value_pair;
};

std::pair<bool, bool> ReserveEntry::updateValids(std::pair<bool, bool> pair)
{
    valid_pair = pair;
    return valid_pair;
};
std::pair<std::string, std::string> ReserveEntry::updateTags(std::pair<std::string, std::string> pair) 
{
    tag_pair = pair;
    return tag_pair;
};

std::pair<int, int> ReserveEntry::getValues() 
{
    return value_pair;
};
std::pair<bool, bool> ReserveEntry::getValids() 
{
    return valid_pair;
};
std::pair<std::string, std::string> ReserveEntry::getTags()
{
    return tag_pair;
};
std::string ReserveEntry::getTag()
{
    return tag;
};

// #################################################################################################
// ReservationStationEntry
// #################################################################################################

rs::ReservationStationEntry::ReservationStationEntry(std::string tag_name): ReserveEntry(tag_name)
{
    address = -1;
    isReserved = false;
    return;
};

void rs::ReservationStationEntry::populateSources(std::string tag, int value)
{
    if (tag_pair.first.compare(tag) == 0)
    {
        valid_pair.first = 1;
        value_pair.first = value;
    }
    if (tag_pair.second.compare(tag) == 0)
    {
        
        valid_pair.second = 1;
        value_pair.second = value;
    }
    return;
};

// #################################################################################################
// ReservationStation
// #################################################################################################

rs::ReservationStation::ReservationStation(Scoreboard* sb, bool force_null): ProcHelper(force_null)
{
    scoreboard = sb;
    size = 128;
    entries = new LinkedList<ReservationStationEntry>();
};

rs::ReservationStationEntry* rs::ReservationStation::getEntry(std::string tag_name)
{
    // if (tag_name.at(0) > 112 || tag_name.at(0) < 97) return NULL;
    LLNode<ReservationStationEntry> *curr = entries->head;
    while(curr != NULL)
    {
        if (curr->payload->getTag().compare(tag_name) == 0) return curr->payload;
        curr = curr->next;
    };
    return NULL;
};

rs::ReservationStationEntry* rs::ReservationStation::hasEmptyEntries()
{
    if (entries->size < 64)
    {
        ReservationStationEntry *entry = new ReservationStationEntry(randomId(5));
        entries->add(entry);
        return entry;
    };;
    return NULL;
};

std::string rs::ReservationStation::reserve(Instructions::Instruction *instrPtr)
{
    rs::ReservationStationEntry* entry = hasEmptyEntries();
    if (entry == NULL && !processor->getPipeline()->stalled())
    {
        processor->getPipeline()->stallPipeline(RS);
        return "Stall";
    }
    entry->setInstruction(instrPtr);
    entry->instrStr = instrPtr->instrString;
    instrPtr->tag = entry->getTag();
    entry->isReserved = true;
    entry->instr_type = instrPtr->type;
    switch (instrPtr->type)
    {
    case RType:
        reserveRType(entry, instrPtr);
        break;
    case IType:
        reserveIType(entry, instrPtr);
        break;
    case JType:
        reserveJType(entry, instrPtr);
        break;
    }
    return entry->getTag();
};
    
ScoreboardEntry* rs::ReservationStation::getScoreboardEntry(Register r)
{
    return scoreboard->getEntry(r);
};

void rs::ReservationStation::reserveRType(ReservationStationEntry *entry, Instructions::Instruction *instrPtr)
{

    ScoreboardEntry* sb_entry_one = getScoreboardEntry(instrPtr->rs);
    ScoreboardEntry* sb_entry_two = getScoreboardEntry(instrPtr->rt);

    entry->value_pair = std::pair<int, int>(sb_entry_one->value, sb_entry_two->value);
    entry->valid_pair = std::pair<int, int>(sb_entry_one->valid, sb_entry_two->valid);
    entry->tag_pair = std::pair<std::string, std::string>(sb_entry_one->tag, sb_entry_two->tag);
    entry->isReserved = true;
    entry->opcode = instrPtr->opcode;
    entry->destination = instrPtr->rd;

    scoreboard->inValidate(instrPtr->rd, entry->getTag());
    return;
};

void rs::ReservationStation::reserveNoDest(ReservationStationEntry *entry, Instructions::Instruction *instrPtr)
{
    ScoreboardEntry* sb_entry_one = getScoreboardEntry(instrPtr->rs);
    ScoreboardEntry* sb_entry_two = getScoreboardEntry(instrPtr->rt);

    entry->opcode = instrPtr->opcode;
    entry->value_pair = std::pair<int, int>(sb_entry_one->value, sb_entry_two->value);
    entry->valid_pair = std::pair<int, int>(sb_entry_one->valid, sb_entry_two->valid);
    entry->tag_pair = std::pair<std::string, std::string>(sb_entry_one->tag, sb_entry_two->tag);
    entry->address = instrPtr->immediateOrAddress;
    return;
};

void rs::ReservationStation::reserveIType(ReservationStationEntry *entry, Instructions::Instruction *instrPtr)
{
    Opcodes opcode = instrPtr->opcode;
    if (isOpBranch(opcode))
    {
        reserveNoDest(entry, instrPtr);
        return;
    }
    ScoreboardEntry* sb_entry_one = getScoreboardEntry(instrPtr->rs);

    entry->value_pair = std::pair<int, int>(sb_entry_one->value, instrPtr->immediateOrAddress);
    entry->valid_pair = std::pair<int, int>(sb_entry_one->valid, 1);
    entry->tag_pair = std::pair<std::string, std::string>(sb_entry_one->tag, "~");
    entry->destination = instrPtr->rt;
    entry->isReserved = true;
    entry->opcode = instrPtr->opcode;

    scoreboard->inValidate(instrPtr->rt, entry->getTag());
    return;
};

void rs::ReservationStation::reserveJType(ReservationStationEntry *entry, Instructions::Instruction *instrPtr)
{

    entry->value_pair = std::pair<int, int>(0,0);
    entry->valid_pair = std::pair<int, int>(1,1);
    entry->tag_pair = std::pair<std::string, std::string>("~","~");
    entry->isReserved = true;
    entry->opcode = instrPtr->opcode;
    entry->address = instrPtr->immediateOrAddress;

    return;
};

void rs::ReservationStation::populateInstruction(Instructions::Instruction *instrPtr) {};

void rs::ReservationStation::populateTags(std::string tag, int value)
{
    LLNode<ReservationStationEntry> *curr = entries->head;
    while(curr != NULL)
    {
        curr->payload->populateSources(tag, value);
        curr = curr->next;
    }
    return;
};

void rs::ReservationStation::validate(Instructions::Instruction *instrPtr) {};

void rs::ReservationStation::flush()
{
    LLNode<ReservationStationEntry> *curr = entries->head;
    while(curr != NULL)
    {
        ReservationStationEntry* node = curr->payload;
        LLNode<ReservationStationEntry>* next = curr->next;
        entries->removeAndDestroy(curr);
        curr = next;
    }
};

rs::ReservationStationEntry* rs::ReservationStation::getValidInstruction()
{
    LLNode<ReservationStationEntry> *curr = entries->head;
    while(curr != NULL)
    {
        ReservationStationEntry* entry = curr->payload;
        if (!entry->busy && entry->valid_pair.first && entry->valid_pair.second) return entry;
        curr = curr->next;
    }
    return NULL;
};

void rs::ReservationStation::remove(std::string tag)
{
    LLNode<ReservationStationEntry> *curr = entries->head;
    while(curr != NULL)
    {
        ReservationStationEntry* entry = curr->payload;
        if (entry->getTag().compare(tag) == 0)
        {
            entries->remove(curr);
            return;
        }
        curr = curr->next;
    }
    return;
};

Opcodes rs::ReservationStation::getOpcode(std::string tag)
{
    LLNode<ReservationStationEntry> *curr = entries->head;
    while(curr != NULL)
    {
        ReservationStationEntry* entry = curr->payload;
        if (entry->getTag().compare(tag) == 0)
        {
            return entry->opcode;
        }
        curr = curr->next;
    }
    return NOP;
};

bool rs::ReservationStation::areAllEntriesFree()
{
    LLNode<ReservationStationEntry> *curr = entries->head;
    while(curr != NULL)
    {
        ReservationStationEntry* entry = curr->payload;
        if (entry->isReserved) return false;
        curr = curr->next;
    }
    return true;
};

int rs::ReservationStation::getSize()
{
    return entries->size;
};

void rs::ReservationStation::print()
{
    std::cout
    << "\n"
    << termcolor::bright_green
    << "|Tag \t|"
    << "Src One\tValid\tValue\t|"
    << "Src Two\tValid\tValue\t|"
    << "Busy\t|"
    << "Instr\t\t\t|"
    << termcolor::reset
    << std::endl;

    LLNode<ReservationStationEntry> *curr = entries->head;
    while(curr != NULL)
    {
        ReservationStationEntry* entry = curr->payload;
        if (!entry->isReserved) 
        {
            curr = curr->next;
            continue;
        }
        std::cout
        << termcolor::bright_blue
        << "|"
        << termcolor::bold << termcolor::bright_cyan
        << entry->getTag()
        << termcolor::reset << termcolor::bright_blue
        << "\t|"
        << entry->tag_pair.first
        << "\t\t"
        << entry->valid_pair.first
        << "\t"
        << entry->value_pair.first
        << "\t|"
        << entry->tag_pair.second
        << "\t\t"
        << entry->valid_pair.second
        << "\t"
        << entry->value_pair.second
        << "\t|"
        << entry->busy
        << "\t|"
        << entry->instrStr 
        << "\t\t|"
        << termcolor::reset
        << std::endl;
        curr = curr->next;
    }
    std::cout << std::endl;
};
