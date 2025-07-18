#include <iostream>

#include "char_stream.h"
#include "parse_expr.h"

int main(void) {
    std::string line;

    while (std::getline(std::cin, line)) {
        std::istringstream isstream(line);
        CharStream cstream(isstream);

        auto expr = parse_expr(cstream);

        if (!expr) {
            std::cout << "ERR\n";
            continue;
        }

        (*expr)->print();
        std::cout << '\n';
    }

    return 0;
}