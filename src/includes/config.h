#include "json.hpp"
#include <fstream>
#include <iostream>
#include <unistd.h>


#ifndef _CONFIG_DEFINED_
#define _CONFIG_DEFINED_


struct conf
{
    std::string program;
    int stop_time;
    struct debug_conf
    {
        bool enabled;
        int till;
        bool printSb;
    } *debug;
    struct unit_conf
    {
        int decode;
        int execute;
        int memory;
    } *units;
    struct speculate_conf
    {
        bool take_branch;
    } *speculate;
    struct print_conf
    {
        bool all;
        int num_bytes;
    } *print;
    // struct 
    // {
    //     bool enabled;
    //     std::string predictor;
    // } branch;
};

inline conf* getConfig()
{

    static conf* config;
    if (config == NULL)
    {
        nlohmann::json jconf;
        std::ifstream i("config.json");
        i >> jconf;
        i.close();
        config = new conf();
        config->debug = new conf::debug_conf();
        config->units = new conf::unit_conf();
        config->speculate = new conf::speculate_conf();
        config->print = new conf::print_conf();

        config->program = jconf["program"].get<std::string>();
        config->stop_time = jconf["stop_time"].get<int>();
        config->debug->enabled = jconf["debug"]["enabled"].get<bool>();
        config->debug->till = jconf["debug"]["till"].get<int>();
        config->debug->printSb = jconf["debug"]["printSb"].get<bool>();

        config->units->decode = jconf["units"]["decode"].get<int>();
        config->units->execute = jconf["units"]["execute"].get<int>();
        config->units->memory = jconf["units"]["memory"].get<int>();

        config->speculate->take_branch = jconf["speculate"]["take_branch"].get<bool>();

        config->print->all = jconf["print"]["all"].get<bool>();
        config->print->num_bytes = jconf["print"]["num_bytes"].get<int>();

    }
    return config;
}

#endif