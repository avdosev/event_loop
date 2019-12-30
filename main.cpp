//
// Created by avdosev on 29.12.2019.
//
#include <iostream>
#include <ctime>
#include "event_loop.h"
#include "interval.h"
using namespace std;

uint32_t seconds() {
    return time(nullptr);
}

int main() {
    EventLoop loop;
    loop.addEvent(makeInterval([](){
        cout << "puk_1" << endl;
    }, 1, seconds));

    loop.addEvent(makeInterval([](){
        cout << "puk_2" << endl;
    }, 2, seconds));

    loop.addEvent(makeInterval([](){
        cout << "puk_3" << endl;
    }, 3, seconds));

    loop.addEvent(makeInterval([](){
        cout << "puk_4" << endl;
    }, 4, seconds));

    loop.addEvent(makeInterval([](){
        cout << "puk_5" << endl;
    }, 5, seconds));

    loop.addEvent(makeInterval([](){
        cout << "puk_6" << endl;
    }, 6, seconds));

    loop.sleep(10, seconds);
}
