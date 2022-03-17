#include <iostream>
#include <map>
#include <string>
#include "isa.h"
#include <tuple>
#include "events.h"
#include <array>
#ifndef _SCOREBOARD_DEFINED_
#define _SCOREBOARD_DEFINED_

class ProcHelper: public EventDispatcher {};


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
        Scoreboard();
        void saveState();
        void restoreState();
        void equaliseSavedState();
        void validate(Register r, int value);
        void inValidate(Register r);
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
    typedef std::tuple<std::string, int, int> rsv_entry;
    
    class ReservationStationEntry
    {
        public:
            ReservationStationEntry(std::string tag_name);
            const std::string tag;
            int isReserved;
            rsv_entry src_one;
            rsv_entry src_two;
            InstructionType instr_type;
    };

    class ReservationStation: public ProcHelper
    { 
        private:
            std::map<std::string, ReservationStationEntry*> entries;
        public:
            ReservationStation();
            bool hasEmptyEntries();
            ReservationStationEntry* getEntry(std::string tag_name);
    };
}


#endif;