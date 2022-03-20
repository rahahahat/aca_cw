#include <string>
#include <map>
#include "termcolor.h"
#include <memory>
#include "constants.h"
#include <functional>
#include <vector>

#ifndef _EVENTS_DEFINED_
#define _EVENTS_DEFINED_

typedef std::string eventType;

class EventBase {
    protected:
        eventType type;
    public:
        int cast;
        EventBase() {
            type = "";
        };
        virtual eventType get() const {
            return type;
        }
        virtual void set(eventType _type)
        {
            if (this->type.compare("") != 0) return;
            type = _type;
            return;
        }

};

template <typename T>
class Event: public EventBase
{
    public:
        Event() {};
        T payload;
};

class EventWrapper
{
    using function_ptr = std::function<void(const EventBase&)>;

    private:
        std::map<eventType, std::vector<function_ptr>> listeners;
        EventWrapper() {};
        
    public:
        static EventWrapper* getEventWrapperInstance()
        {
            static EventWrapper* instance;
            if (instance == NULL) 
            {
                instance = new EventWrapper();
            }
            return instance;
        };
        void disptachEventToListerner(const EventBase& base) const
        {

            eventType type = base.get();
            if (listeners.find(type) == listeners.end()) return;
            auto&& funcs = listeners.at(type);
            for (auto && func: funcs)
            {
                func(base);
            }
        };

        void addEventListerner(const eventType type, function_ptr&& func_ptr);
};

class EventDispatcher
{
    private:
        EventWrapper *eventWrapper;
    public:
        EventDispatcher() {
            this->eventWrapper = EventWrapper::getEventWrapperInstance();
        };
        void dispatch(const EventBase& base)
        {
            eventWrapper->disptachEventToListerner(base);
            return;
        };
};

#endif