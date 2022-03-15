#include <iostream>
#include "events.h"
#include "termcolor.h"

void EventWrapper::addEventListerner(const eventType type, function_ptr&& func_ptr)
{
    listeners[type].push_back(func_ptr);
    return;
};