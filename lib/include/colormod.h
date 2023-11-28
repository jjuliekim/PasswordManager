//
// Created by julie on 11/27/2023.
//

#ifndef PASSWORDMANAGER_COLORMOD_H
#define PASSWORDMANAGER_COLORMOD_H

#include <ostream>

namespace Color {
    enum Code {
        RED = 31,
        GREEN = 32,
        BLUE = 34,
        CYAN = 96,
        COLOR_DEFAULT = 39,
        BOLD = 1,
        UNDERLINE = 4,
        FORMAT_RESET = 0
    };

    class Modifier {
        Code code;
    public:
        explicit Modifier(Code pCode) : code(pCode) {}

        friend std::ostream &
        operator<<(std::ostream &os, const Modifier &mod) {
            return os << "\033[" << mod.code << "m";
        }
    };
}

#endif //PASSWORDMANAGER_COLORMOD_H
