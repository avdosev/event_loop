//
// Created by avdosev on 29.12.2019.
//
#include <iostream>
#include <ctime>
#include "event_loop.h"
#include "interval.h"
#include "delegat.h"
using namespace std;

uint32_t seconds() {
    return time(nullptr);
}


int main() {
    EventLoop loop;
    delegate<> puk;

    puk.add([](){
        cout << "puk_1" << endl;
    });
    puk.add([](){
        cout << "puk_2" << endl;
    });
    puk.add([](){
        cout << "puk_3" << endl;
    });


    loop.addEvent(makeInterval(puk.function(), 1, seconds));

    loop.sleep(10, seconds);
}
