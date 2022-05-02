#include "prochelpers.h"
#include "constants.h"
#include "termcolor.h"
#include "processor.h"
#include "termcolor.h"
#include <sstream>
#include "util.h"

ProcHelper::ProcHelper(bool force_null)
{
    if (force_null) 
    {
        processor = NULL;
        return;
    }
    processor = Processor::getProcessorInstance();
    return;
}

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
};

bool Scoreboard::validate(Register r, int value, std::string tag)
{
    auto itr = board.find(r);
    if (itr != board.end()) {
        if (tag.compare(itr->second->tag) != 0) return false;
        itr->second->valid = 1;
        itr->second->value = value;
        return true;
    }
    return false;
};

void Scoreboard::invalidatePC()
{
    auto itr = board.find($pc);
    if (itr != board.end()) itr->second->valid = 0;
    return;
}

bool Scoreboard::isPCValid()
{
    return board.find($pc)->second->valid == 1;
}

void Scoreboard::inValidate(Register r, std::string new_tag) 
{
    auto itr = board.find(r);
    if (itr != board.end()) {
        itr->second->valid = 0;
        itr->second->tag = new_tag;
        return;
    }
    return;
}

std::pair<int, int> Scoreboard::isValid(Register r) 
{
    auto itr = board.find(r);
    if (itr != board.end()) {
        return std::pair<int, int>(itr->second->valid, itr->second->value);
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
    auto itr = board.find(r);
    if (itr != board.end()) {
        return itr->second;
    }
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
    return;
}

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

// TODO: validate all rs entries waiting for a tag on validate.
rs::ReservationStationEntry::ReservationStationEntry(std::string tag_name): ReserveEntry(tag_name)
{
    address = -1;
    isReserved = false;
    return;
}

void rs::ReservationStationEntry::validateSourcesOnEvent(const EventBase& base)
{
    // const Event<rsEventData>& event = static_cast<const Event<rsEventData>&>(base);
    // std::string tag = event.payload.tag_name;
    // int value = event.payload.value;
    // populateSources(tag, value);
    // EventWrapper::getEventWrapperInstance()->removeEventListener(tag);
    // return;
}

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
}

rs::ReservationStation::ReservationStation(Scoreboard* sb, bool force_null): ProcHelper(force_null)
{
    scoreboard = sb;
    size = 64;
    entries = new LinkedList<ReservationStationEntry>();
    // for (int x = 0; x < size; x++)
    // {
    //     int tagIndex = x+1;
    //     std::stringstream ss;
    //     tagIndex < 10 ? ss << "tag0"<< tagIndex : ss << "tag"<< tagIndex;
    //     std::string tag = ss.str();
    //     entries.insert(std::pair<std::string, ReservationStationEntry*>(tag, new ReservationStationEntry(tag)));
    // }
};

rs::ReservationStationEntry* rs::ReservationStation::getEntry(std::string tag_name)
{
    // if (tag_name.at(0) > 112 || tag_name.at(0) < 97) return NULL;
    LLNode<ReservationStationEntry> *curr = entries->head;
    while(curr != NULL)
    {
        if (curr->payload->getTag().compare(tag_name) == 0) return curr->payload;
        curr = curr->next;
    }
    return NULL;
}

rs::ReservationStationEntry* rs::ReservationStation::hasEmptyEntries()
{
    if (entries->size < 64)
    {
        ReservationStationEntry *entry = new ReservationStationEntry(randomId(5));
        entries->add(entry);
        return entry;
    }
    return NULL;
}

std::string rs::ReservationStation::reserve(Instructions::Instruction *instrPtr)
{
    rs::ReservationStationEntry* entry = hasEmptyEntries();
    if (entry == NULL && !processor->getPipeline()->stalled())
    {
        processor->getPipeline()->stallPipeline(RS);
        return;
    }
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
}
    
ScoreboardEntry* rs::ReservationStation::getScoreboardEntry(Register r)
{
    return scoreboard->getEntry(r);
}

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
}
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
    if (opcode == BEQ || opcode == BGTE || opcode == BL || opcode == BNE)
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
}

void rs::ReservationStation::reserveJType(ReservationStationEntry *entry, Instructions::Instruction *instrPtr)
{

    entry->value_pair = std::pair<int, int>(0,0);
    entry->valid_pair = std::pair<int, int>(1,1);
    entry->tag_pair = std::pair<std::string, std::string>("~","~");
    entry->isReserved = true;
    entry->opcode = instrPtr->opcode;
    entry->address = instrPtr->immediateOrAddress;

    return;
}

void rs::ReservationStation::populateInstruction(Instructions::Instruction *instrPtr)
{
    std::string rs_tag = instrPtr->tag;
    rs::ReservationStationEntry* rs_entry = getEntry(rs_tag);
    if (!rs_entry->isReserved) return;
    int isValid = rs_entry->valid_pair.first && rs_entry->valid_pair.second;
    if (!isValid) return;
    switch(instrPtr->type)
    {
        case RType:
            instrPtr->src2 = rs_entry->value_pair.second;
            instrPtr->src1 = rs_entry->value_pair.first;
            break;
        case IType:
            instrPtr->src1 = rs_entry->value_pair.first;
            break;
    }
    instrPtr->isReadyToExecute = true;
    return;
}

void rs::ReservationStation::populateTags(std::string tag, int value)
{
    LLNode<ReservationStationEntry> *curr = entries->head;
    while(curr != NULL)
    {
        curr->payload->populateSources(tag, value);
        curr = curr->next;
    }
    return;
    // for (auto const& entry : entries)
    // {
    //     rs::ReservationStationEntry *rse = entry.second;
    //     rse->populateSources(tag, value);
    // }
}

void rs::ReservationStation::validate(Instructions::Instruction *instrPtr)
{
    // auto itr = entries.find(instrPtr->tag);
    // if (itr == entries.end()) return;
    // itr->second->result = instrPtr->temp;
    // return;
}

rs::ReservationStationEntry* rs::ReservationStation::getValidInstruction()
{
    std::cout << termcolor::on_bright_red << entries->size << termcolor::reset << std::endl;
    LLNode<ReservationStationEntry> *curr = entries->head;
    while(curr != NULL)
    {
        ReservationStationEntry* entry = curr->payload;
        if (!entry->busy && entry->valid_pair.first && entry->valid_pair.second) return entry;
        curr = curr->next;
    }
    // for (auto const& entry : entries)
    // {
    //     if (!entry.second->busy && entry.second->valid_pair.first && entry.second->valid_pair.second)
    //     {
    //         entry.second->busy = true;
    //         return entry.second;
    //     }
    // }
    return NULL;
}

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
    // auto entry = entries.find(tag);
    // if (entry == entries.end()) return;
    // rs::ReservationStationEntry* rsv_entry = entry->second;
    // rsv_entry->updateTags(std::pair<std::string, std::string>("~", "~"));
    // rsv_entry->updateValids(std::pair<int, int>(0, 0));
    // rsv_entry->updateValues(std::pair<int, int>(0, 0));
    // rsv_entry->busy = false;
    // rsv_entry->isReserved = false;
    return;
}

bool rs::ReservationStation::areAllEntriesFree()
{
    LLNode<ReservationStationEntry> *curr = entries->head;
    while(curr != NULL)
    {
        ReservationStationEntry* entry = curr->payload;
        if (entry->isReserved) return false;
        curr = curr->next;
    }
    // for (auto const& entry : entries)
    // {
    //     if (entry.second->isReserved) return false;
    // }
    return true;
}

int rs::ReservationStation::getSize()
{
    return entries->size;
}

void rs::ReservationStation::print()
{
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
        << termcolor::red << termcolor:: bold
        << "Tag: "
        << entry->getTag()
        << termcolor::reset
        << termcolor:: green << termcolor:: bold
        << "     | "
        << "tag: "
        << entry->tag_pair.first
        << " | "
        << "valid: "
        << entry->valid_pair.first
        << " | "
        << "value: "
        << entry->value_pair.first
        << " |"
        << termcolor::reset
        << termcolor::blue << termcolor:: bold
        << "     | "
        << "tag: "
        << entry->tag_pair.second
        << " | "
        << "valid: "
        << entry->valid_pair.second
        << " | "
        << "value: "
        << entry->value_pair.second
        << " | busy: " << entry->busy
        << " | instr: " << entry->instrStr 
        << termcolor::reset
        << std::endl;
        curr = curr->next;
    }
    // for (auto const& entry : entries)
    // {

    // }
}
