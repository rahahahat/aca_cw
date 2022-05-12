#include "procUnit.h"
#include "termcolor.h"
#include "processor.h"
#include "pipeline.h"
#include "util.h"
#include "config.h"

ProcUnit::ProcUnit()
{
    processor = Processor::getProcessorInstance();
    busy = false;
    config = getConfig();
    return;
}