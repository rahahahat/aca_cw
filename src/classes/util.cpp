#include "util.h"

std::vector<std::string> splitString(const std::string& str) {
	std::vector<std::string> result;
	std::istringstream iss(str);
	for (std::string s; iss >> s; )
		result.push_back(s);
	return result;
}