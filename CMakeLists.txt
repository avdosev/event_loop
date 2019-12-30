cmake_minimum_required(VERSION 3.15)
project(event_loop)

set(CMAKE_CXX_STANDARD 14)

add_executable(
        event_loop
        delegat.h
        event.h
        event_define.h
        event_loop.h
        interval.h
        timer.h
        main.cpp
)

add_library(
        delegat.h
        event.h
        event_define.h
        event_loop.h
        interval.h
        timer.h
)