#include "TuplePrinter.h"
#include <iostream>

int main(void) {
    std::tuple<bool, char, int> t{false, 'A', 100};
    std::cout << t;
    return 0;
}