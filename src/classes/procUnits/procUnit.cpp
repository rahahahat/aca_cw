#include "procUnit.h"
#include "constants.h"
#include "termcolor.h"
#include "processor.h"
#include "pipeline.h"
#include "util.h"

ProcUnit::ProcUnit()
{
    processor = Processor::getProcessorInstance();
    busy = false;
    return;
}