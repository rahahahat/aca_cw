#include "procUnit.h"
#include "constants.h"
#include "termcolor.h"

ProcUnit::ProcUnit(int units): total_units(units)
{
    processor = NULL;
    pipeline = NULL;
    return;
}

void ProcUnit::attachToProcessor(Processor *proc)
{
    processor = proc;
}