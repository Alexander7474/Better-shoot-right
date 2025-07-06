//
// Created by artyom on 7/7/25.
//

#ifndef MACRO_H
#define MACRO_H

#include <iostream>

#define DEBUG_VALUE(x) std::cout << "[DEBUG] " << #x << " = " << x << '\n';
#define DEBUG_MESSAGE(x) std::cout << "[DEBUG] " << x << '\n';
#define ERROR_MESSAGE(x) std::cerr << "[ERROR] " << x << '\n';

#endif //MACRO_H
