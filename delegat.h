#pragma once

#include <functional>
#include <map>
#include <random>

template <typename UnusedType>
class delegate;

template <typename return_type, typename ...argument_types>
class delegate <return_type(argument_types...)> {
private:
    using invoker_t = std::function<return_type(argument_types...)>;
    using key_t = std::random_device::result_type;
    std::map<key_t, invoker_t> invokers;
public:
    return_type invoke(argument_types ...args) {
        for (auto key_invoker : invokers) {
            key_invoker.second(args...);
        }
    }

    key_t add(invoker_t invoker) {
        auto key = find_unoccupied_key();
        invokers.insert(std::make_pair(key, invoker));
        return key;
    }

    void remove(key_t invoker_key) {
        invokers.erase(invoker_key);
    }

    void operator += (invoker_t invoker) {
        add(invoker);
    }

    void operator -= (invoker_t invoker) {
        remove(invoker);
    }

private:
    key_t find_unoccupied_key() {
        std::random_device rand;
        key_t new_key;
        do {
            new_key = rand();
        } while (invokers.find(new_key) != invokers.end());
        return new_key;
    }
};
