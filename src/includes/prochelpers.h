#include <iostream>
#include <map>
#include <string>
#include "isa.h"
#include <tuple>
#include "events.h"
#include <array>
#include "instruction.h"

#ifndef _SCOREBOARD_DEFINED_
#define _SCOREBOARD_DEFINED_

#include "processor.h"


class ProcHelper
{
    public:
        ProcHelper(bool force_null);
        Processor *processor;
};


class ScoreboardEntry {
    public:
        int valid;
        std::string tag;
        Register reg;
        int value;
        ScoreboardEntry(Register r);
};

class Scoreboard: public ProcHelper
{
    private:
        std::map<Register, ScoreboardEntry*> board;
        std::map<Register, ScoreboardEntry*> savedState;
    public:
        Scoreboard(bool force_null);
        ScoreboardEntry* getEntry(Register r);
        void saveState();
        void restoreState();
        void equaliseSavedState();
        bool validate(Register r, int value, std::string tag);
        void inValidate(Register r, std::string tag_name);
        std::pair<int, int> isValid(Register r);
        int getSize();
        void memDump();
        void invalidatePC();
        bool isPCValid();
};

class ResultForwarder: ProcHelper
{
    private:
        std::map<Register, int> valueMap;
        std::map<Register, int> savedState;
    public:
        ResultForwarder(): ProcHelper(true) {};
        void saveState();
        void restoreState();
        void equaliseSavedState();
        void addValue(Register r, int value);
        void removeValue(Register r);
        std::pair<int, int> getValue(Register r);
        int getSize();
        void memDump();
};

// TODO: Reserve value_two with immediate
class ReserveEntry
{
    private:
        std::string tag;
    public:
        ReserveEntry(std::string tag_name);
        std::string instrStr;
        Opcodes opcode;
        Register destination;
       
        bool busy;
        bool isReady;
        int address;

        std::pair<int, int> value_pair;
        std::pair<bool, bool> valid_pair;
        std::pair<std::string, std::string> tag_pair;

        std::pair<int, int> updateValues(std::pair<int, int> pair); 
        std::pair<bool, bool> updateValids(std::pair<bool, bool> pair);
        std::pair<std::string, std::string> updateTags(std::pair<std::string, std::string> pair);

        std::pair<int, int> getValues();
        std::pair<bool, bool> getValids();
        std::pair<std::string, std::string> getTags();
        std::string getTag();
};


namespace rs
{
   
    class ReservationStationEntry : public ReserveEntry
    {
        public:
            ReservationStationEntry(std::string tag_name);

            InstructionType instr_type;
            bool isReserved;

            void validateSourcesOnEvent(const EventBase& base);
            void populateSources(std::string tag, int value);
    };

    class ReservationStation: public ProcHelper
    { 
        private:
            int size;
            Scoreboard* scoreboard;
            std::map<std::string, ReservationStationEntry*> entries;
            void reserveRType(ReservationStationEntry *entry, Instructions::Instruction *instrPtr);
            void reserveIType(ReservationStationEntry *entry, Instructions::Instruction *instrPtr);
            void reserveJType(ReservationStationEntry *entry, Instructions::Instruction *instrPtr);
            void reserveNoDest(ReservationStationEntry *entry, Instructions::Instruction *instrPtr);
        public:
            ReservationStation(Scoreboard* sb, bool force_null);
            ReservationStationEntry* getEntry(std::string tag_name);
            ReservationStationEntry* hasEmptyEntries();
            ScoreboardEntry* getScoreboardEntry(Register r);
            void print();
            void populateInstruction(Instructions::Instruction *instrPtr);
            void validate(Instructions::Instruction *instrPtr);
            void remove(std::string tag);
            void reserve(Instructions::Instruction *instrPtr);
            void populateTags(std::string tag, int value);
            bool areAllEntriesFree();
            rs::ReservationStationEntry* getValidInstruction();
    };
};

#endif