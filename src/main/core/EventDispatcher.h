#pragma once
#include <queue>

#include "Event.h"

class EventDispatcher {

public:
    void pushEvent(const Event& event) {
        m_events.push(event);
    }

    bool pollEvent(Event& event) {
        if (m_events.empty()) return false;
        event = m_events.front();
        m_events.pop();
        return true;
    }

private:
    std::queue<Event> m_events;
};
