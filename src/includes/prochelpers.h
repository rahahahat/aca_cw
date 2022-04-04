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

struct rsEventData
{
    std::string tag_name;
    int value;
};

class ProcHelper: public EventDispatcher {
    public:
        ProcHelper(bool force_null);
        Processor *processor;
};


class ScoreboardEntry {
    private:
        std::string tag;
        Register reg;
        int valid;
        int value;
    public:
        ScoreboardEntry(Register r);
        int isValid()
        {
            return valid;
        };
        int getValue()
        {
            return value;
        };
        void updateValue(int val)
        {
            value = val;
            return;
        };
        void updateValidity(int validity)
        {
            if (validity != valid)
            {
                valid = validity;
            }
            return;
        };
        void updateTag(std::string tag_name)
        {
            tag = tag_name;
            return;
        };
        std::string getTag()
        {
            return tag;
        };
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
        void validate(Register r, int value, std::string tag);
        void inValidate(Register r, std::string tag_name);
        std::pair<int, int> isValid(Register r);
        int getSize();
        void memDump();
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

namespace rs
{
   
    class ReservationStationEntry
    {
        public:
            ReservationStationEntry(std::string tag_name);

            InstructionType instr_type;
            const std::string tag;
            int result;
            bool isReserved;
            bool valid_result;

            std::string tag_one;
            int valid_one;
            int val_one;

            std::string tag_two;
            int valid_two;
            int val_two;

            void validateSourcesOnEvent(const EventBase& base);
            void validateSources(std::string tag, int value);

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
            void reserveEntryOnEvent(const EventBase& base);
            void populateInstruction(Instructions::Instruction *instrPtr);
            void validate(Instructions::Instruction *instrPtr);
            void remove(Instructions::Instruction* instrPtr);
            void reserve(Instructions::Instruction *instrPtr);
    };
}


#endif