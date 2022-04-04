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
            std::cout << "comes here" << std::endl;
            eventType type = base.get();
            if (listeners.find(type) == listeners.end()) return;
            auto&& funcs = listeners.at(type);
            std::cout << "comes here2" << std::endl;
            for (auto && func: funcs)
            {
                func(base);
            }
            std::cout << "comes here3" << std::endl;
        };

        void addEventListerner(const eventType type, function_ptr&& func_ptr)
        {
            listeners[type].push_back(func_ptr);
            return;
        };

        void removeEventListener(const eventType type)
        {
            auto itr = listeners.find(type);
            if (itr != listeners.end()) listeners.erase(itr);
            return;
        };
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