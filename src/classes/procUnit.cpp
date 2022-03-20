#include "procUnit.h"
#include "constants.h"
#include "termcolor.h"
#include "processor.h"
#include "pipeline.h"

ProcUnit::ProcUnit()
{
    processor = NULL;
    pipeline = NULL;
    return;
}

void ProcUnit::attachToProcessor(Processor *proc)
{
    processor = proc;
    return;
}