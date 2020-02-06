//
// Created by mojiajun on 2020/2/6.
//

#include "../src/util/time_stamp.h"
#include <iostream>

using tinyreactor::Timestamp;

int main() {
    std::cout << Timestamp::now().toString() << '\n';
}