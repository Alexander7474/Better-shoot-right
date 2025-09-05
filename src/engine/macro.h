//
// Created by artyom on 7/7/25.
//

#ifndef MACRO_H
#define MACRO_H

#include <iostream>

#define DEBUG_VALUE(x) std::cout << "[DEBUG] " << #x << " = " << x << '\n';
#define DEBUG_MESSAGE(x) std::cout << "[DEBUG] " << x << '\n';
#define ERROR_MESSAGE(x)                                                       \
        std::cerr << "\x1b[30;41m" << "[ERROR] " << x << "\x1b[0m" << '\n';

#endif // MACRO_H
