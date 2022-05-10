#include <iostream>
#include <map>
#include <string>
#include "isa.h"
#include <tuple>
#include "events.h"
#include <array>
#include "instruction.h"
#include "linkedlist.h"  

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
        Processor *processor;
    public:
        Scoreboard(bool force_null);
        ScoreboardEntry* getEntry(Register r);
        bool validate(Register r, int value, std::string tag);
        void inValidate(Register r, std::string tag_name);
        std::pair<int, int> isValid(Register r);
        int getSize();
        void memDump();
        void invalidatePC();
        bool isPCValid();
        void flush();
        void validateAll();
        void writeToARF();
};

// TODO: Reserve value_two with immediate
class ReserveEntry
{
    private:
        std::string tag;
        Instructions::Instruction *instr;
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
        void setInstruction(Instructions::Instruction *instrPtr);
        Instructions::Instruction* getInstruction();
};


namespace rs
{
   
    class ReservationStationEntry : public ReserveEntry
    {
        public:
            ReservationStationEntry(std::string tag_name);
            InstructionType instr_type;
            bool isReserved;
            void populateSources(std::string tag, int value);
    };

    class ReservationStation: public ProcHelper
    { 
        private:
            int size;
            Scoreboard* scoreboard;
            LinkedList<ReservationStationEntry> *entries;

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
            Opcodes getOpcode(std::string tag);
            void remove(std::string tag);
            std::string reserve(Instructions::Instruction *instrPtr);
            void populateTags(std::string tag, int value);
            bool areAllEntriesFree();
            void flush();
            int getSize();
            rs::ReservationStationEntry* getValidInstruction();
    };
};

#endif