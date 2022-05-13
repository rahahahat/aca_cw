#include "json.hpp"
#include <fstream>
#include <iostream>
#include <unistd.h>


#ifndef _CONFIG_DEFINED_
#define _CONFIG_DEFINED_


enum BranchPredictorType {
    SPECULATE, ONEBIT, TWOBIT
};

struct conf
{
    std::string program;
    int stop_time;
    BranchPredictorType predictor;

    struct debug_conf
    {
        bool enabled;
        int till;
        bool print;

    } *debug;
    struct unit_conf
    {
        int commit;
        int decode;
        int execute;
        int memory;
    } *units;
    struct speculate_conf
    {
        bool take_branch;
    } *speculate;
    struct output_conf
    {
        bool all;
        int num_bytes;
        std::string filename;

    } *output;
    struct capacity_conf {
        int rsv;
        int lsq;
        int rob;
        int instrQ;
    } *capacity;
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
        config->output = new conf::output_conf();
        config->capacity = new conf::capacity_conf();

        config->program = jconf["program"].get<std::string>();
        config->stop_time = jconf["stop_time"].get<int>();

        config->debug->enabled = jconf["debug"]["enabled"].get<bool>();
        config->debug->till = jconf["debug"]["till"].get<int>();
        config->debug->print = jconf["debug"]["print"].get<bool>();

        config->units->decode = jconf["units"]["decode"].get<int>();
        config->units->execute = jconf["units"]["execute"].get<int>();
        config->units->memory = jconf["units"]["memory"].get<int>();
        config->units->commit = jconf["units"]["commit"].get<int>();

        config->speculate->take_branch = jconf["speculate"]["take_branch"].get<bool>();

        config->output->all = jconf["output"]["all"].get<bool>();
        config->output->num_bytes = jconf["output"]["num_bytes"].get<int>();
        config->output->filename = jconf["output"]["filename"].get<std::string>();

        config->capacity->lsq = jconf["capacity"]["lsq"].get<int>();
        config->capacity->rob = jconf["capacity"]["rob"].get<int>();
        config->capacity->rsv = jconf["capacity"]["rsv"].get<int>();
        config->capacity->instrQ = jconf["capacity"]["instrQ"].get<int>();

        int pred = jconf["predictor"].get<int>();
        if (pred > 2)
        {
            pred = 0;
        }
        config->predictor = (BranchPredictorType) pred;
    }
    return config;
}

#endif