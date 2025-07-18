#include <iostream>

#include "char_stream.h"
#include "parse.h"

int main(void) {
    CharStream cstream(std::cin);

    auto program = parse::parse_program(cstream);

    if (program.has_err()) {
        std::cerr << "Err occurred while building AST, exiting.\n";
        return 1;
    }

    auto ret_code = program.execute();

    if (ret_code.is_err()) {
        std::cout << *ret_code.get_err_msg() << '\n';
    }

    return 0;
}