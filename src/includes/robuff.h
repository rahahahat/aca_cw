#include "linkedlist.h"
#include <string>

class ROBEntry
{
    private:
        std::string tag;
        std::string instrStr;
        bool valid;
    public:
        ROBEntry(std::string tag_name, std::string instr_str);
        void setValid();
};

class ReorderBuffer
{
    private:
        int max_size;
        LinkedList<ROBEntry> *buffer;
    public:
        ReorderBuffer(int size);
        void addEntry(std::string tag_name, std::string instr_str);
        void pop();
        void nextTick();
};