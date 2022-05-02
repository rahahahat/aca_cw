#include "util.h"
#include "termcolor.h"

std::vector<std::string> splitString(const std::string& str) {
	std::vector<std::string> result;
	std::istringstream iss(str);
	for (std::string s; iss >> s; )
		result.push_back(s);
	return result;
}

void printCycleStart(int cycle, int num_instr)
{
	std::cout << termcolor::bold
	<< "\n\nClock cycle: "
	<< termcolor::green
	<< cycle
	<< termcolor::white << " (Number of Instructions in Pipeline: " 
	<< termcolor::blue 
	<< num_instr << termcolor::white << ") "
	<< " ----------------------------------"
	<< termcolor::reset << std::endl << std::endl;
}

void printCycleEnd(int cycle)
{
	std::cout << termcolor::bold 
	<< "\nClock cycle: "
	<< termcolor::green
	<< cycle
	<< termcolor::white
	<< " has Ended"
	<< termcolor::reset << std::endl << std::endl;
}

void printProgramEnd(int cycle)
{
	std::cout << termcolor::bold << termcolor::green << "Program has ended!" << termcolor::reset << std::endl;
    std::cout << termcolor::bold << termcolor::blue << "Program took: " << cycle << " cycles to finish" <<termcolor::reset << std::endl;
}

std::string randomId(const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    
    return tmp_s;
}
