#pragma once

#include <functional>
#include <map>
#include <random>

template <typename ...argument_types>
class delegate {
private:
    using return_type = void;
    using invoker_t = std::function<return_type(argument_types...)>;
    using key_t = std::random_device::result_type;
    std::map<key_t, invoker_t> invokers;
public:
    return_type invoke(argument_types ...args) {
        for (auto key_invoker : invokers) {
            key_invoker.second(args...);
        }
    }

    return_type operator() (argument_types ...args) {
        this->invoke(args...);
    }

    key_t add(invoker_t invoker) {
        auto key = find_unoccupied_key();
        invokers.insert(std::make_pair(key, invoker));
        return key;
    }

    template <class class_t, typename method_return_t>
    key_t add(class_t& obj, method_return_t (class_t::*method)(argument_types...)) {
        return this->add([&obj, method](argument_types... args) {
            (obj.*method)(args...);
        });
    }

    void remove(key_t invoker_key) {
        invokers.erase(invoker_key);
    }

    invoker_t function() {
         return [this](argument_types ...args){
             this->invoke();
         };
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
