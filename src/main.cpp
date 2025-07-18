#include <iostream>

#include "char_stream.h"
#include "parse/parse_program.h"

int main(void) {
    CharStream cstream(std::cin);

    auto program = parse::parse_program(cstream);

    auto ret_code = program.execute();

    if (ret_code.is_err()) {
        std::cout << *ret_code.get_err_msg() << '\n';
    }

    return 0;
}