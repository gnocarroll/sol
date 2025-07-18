#include <iostream>

#include "char_stream.h"
#include "parse_program.h"

int main(void) {
    CharStream cstream(std::cin);

    auto program = parse_program(cstream);

    program.execute();

    return 0;
}