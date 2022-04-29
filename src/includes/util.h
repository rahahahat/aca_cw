#include <iostream>
#include <ctime>
#include <unistd.h>
#include <vector> 
#include <string> 
#include <sstream> 
#include <fstream>

#ifndef _UTILS_INCLUDED_
#define _UTILS_INCLUDED_

std::vector<std::string> splitString(const std::string& str);
void printCycleStart(int cycle, int num_instr);
void printCycleEnd(int cycle);
void printProgramEnd(int cycle);
void stepMode();
std::string randomId(const int len);

#endif