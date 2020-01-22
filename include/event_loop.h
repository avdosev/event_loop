#pragma once

#include <list>
#include <random>
#include <algorithm>
#include <utility>

#include "event.h"

class EventLoop {
private:
    std::list<event_pair> events;
    std::list<event_pair> ready_events;
    volatile bool _running = true;

public:
    EventLoop() = default;
    EventLoop(const EventLoop&) = delete;
    ~EventLoop();

    void exec();
    void runNext();
    event_id_t addEvent(event_t event);
    void removeEvent(event_id_t id);
    event_t getEvent(event_id_t id);
    void stop();
    bool isRunning() const;
    bool isFinished() const;
    void sleep(uint32_t time_to_sleep, tiker_t time_cheker);

private:
    bool checkReadyEvents();
    event_pair getReadyEvent();
    event_id_t find_unoccupied_key();
};