cmake_minimum_required(VERSION 3.15)
project(event_loop)
project(event_system)

set(CMAKE_CXX_STANDARD 14)

add_library(
        event_system
        delegat.h
        event.h
        event_define.h
        event_loop.h
        interval.h
        timer.h
        event_loop.cpp
)