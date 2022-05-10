#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

#ifndef _PARSER_INCLUDED_
#define _PARSER_INCLUDED_

class Processor;

class Parser
{
    private:
        std::vector<std::string> programList; 
        Processor *processor;
    public:
        int totalPrograms;
        Parser(Processor *processor);
        std::vector<std::string> parseProgram(std::string filename);
};

#endif