#include <iostream>

#include "char_stream.h"
#include "parse_program.h"

int main(void) {
    std::string line;

    while (std::getline(std::cin, line)) {
        std::istringstream isstream(line);
        CharStream cstream(isstream);

        auto program = 
    }

    return 0;
}