#pragma once

#include <functional>
#include <memory>

class Event;

using event_id_t = uint32_t;
using event_t = std::shared_ptr<Event>;
using callback_t = std::function<void(void)>;
using tiker_t = std::function<uint32_t(void)>;

using event_pair = std::pair<event_id_t, event_t>;