#include "parser.h"
#include "constants.h"
#include "processor.h"
#include "pipeline.h"
#include "isa.h"
#include "util.h"
#include <iostream>
#include <string>
#include <map>

int main(int argc, char* argv[]) {
    std::string hex_string = "-0xa";
    int res = std::stoi(hex_string, 0, 16);
    std::cout << res << std::endl;
    return 0;
}