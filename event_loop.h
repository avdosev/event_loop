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

void EventLoop::exec() { 
    _running = true;
    while (_running) {
        this->runNext();
    }
}

void EventLoop::runNext() {
    if (this->checkReadyEvents()) {
        auto event_pair = this->getReadyEvent();
        auto event = event_pair.second;
        event->stopTracking();
        event->run();
        if (!event->needRemove()) {
            // так как элемента уже нет в списке его нужно добавить
            event->startTracking();
            events.push_back(event_pair);
        }
    }
}

event_id_t EventLoop::addEvent(event_t event) {
    auto id = find_unoccupied_key();
    event->startTracking();
    events.push_back(event_pair{id, std::move(event)});
    return id;
}

void EventLoop::removeEvent(event_id_t id) {
    auto remover = [id](const event_pair& ePair){
        return ePair.first == id;
    };
    events.remove_if(remover);
    ready_events.remove_if(remover);
}

event_t EventLoop::getEvent(event_id_t id) {
    auto searcher = [id](const event_pair& ePair){
        return ePair.first == id;
    };
    auto iter = std::find_if(events.begin(), events.end(), searcher);
    if (iter != events.end())
        return iter->second;
    iter = std::find_if(ready_events.begin(), ready_events.end(), searcher);
    if (iter != ready_events.end())
        return iter->second;
    return nullptr;
}

bool EventLoop::isFinished() const {
    return !isRunning();
}

void EventLoop::stop() {
    _running = false;
}

bool EventLoop::isRunning() const {
    return _running;
}

void EventLoop::sleep(uint32_t time_to_sleep, tiker_t time_cheker) {
    uint32_t start = time_cheker();
    uint32_t current;
    bool started_with_false = !isRunning();
    do {
        runNext();
        current = time_cheker();
    } while((isRunning() || started_with_false) && ((current-start) <= time_to_sleep));
}

bool EventLoop::checkReadyEvents() {
    if (!ready_events.empty())
        return true;

    for (auto iter = events.begin(); iter != events.end(); ++iter) {
        auto event_p = *iter;
        if (event_p.second->isReady()) {
            iter = events.erase(iter);
            if (iter != events.end()) iter = std::prev(iter);
            ready_events.push_back(event_p);
        }
    }

    return !ready_events.empty();
}

event_pair EventLoop::getReadyEvent() {
    if (ready_events.empty())
        return event_pair{0, nullptr};

    auto e_p = ready_events.back();
    ready_events.pop_back();
    return e_p;
}

event_id_t EventLoop::find_unoccupied_key() {
    std::random_device rnd;
    event_id_t id;
    bool found;
    do {
        id = rnd();
        auto searcher = [id](const event_pair& ePair){
            return ePair.first == id;
        };
        auto iter = std::find_if(events.begin(), events.end(), searcher);
        found = iter != events.end();
        if (found) continue;
        iter = std::find_if(ready_events.begin(), ready_events.end(), searcher);
        found = iter != ready_events.end();
    } while (found);
    return id;
}

EventLoop::~EventLoop() {
    for (auto& event : events) {
        event.second->stopTracking();
    }
    events.clear();
    for (auto& event : ready_events) {
        event.second->stopTracking();
    }
    events.clear();
}
