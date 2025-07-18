#include <iostream>

#include "char_stream.h"
#include "parse_expr.h"

int main(void) {
    std::string line;

    while (std::getline(std::cin, line)) {
        std::istringstream isstream(line);
        CharStream cstream(isstream);

        auto expr = parse_expr(cstream);
        auto value = (*expr)->eval();

        if (value) std::cout << *value;
        else std::cout << "ERR";

        std::cout << '\n';
    }

    return 0;
}