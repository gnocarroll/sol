#include <cstdlib>
#include <fstream>
#include <iostream>

#include "ast/ast_builder.h"
#include "char_stream.h"
#include "parse.h"
#include "ast/value.h"

std::string_view usage("sol [optional: filepath]");

int main(int argc, char *argv[]) {
    std::optional<std::ifstream> in_stream;

    if (argc == 2) {
        auto stream = std::ifstream(argv[1]);

        if (!stream.good()) {
            std::cerr << "failed to open input file, ensure path is valid\n";
            std::exit(1);
        }

        in_stream = std::move(stream);
    }
    else if (argc > 2) {
        std::cerr << usage << '\n';
        std::exit(1);
    }

    auto ast_builder = ast::AST(
        in_stream ? CharStream(*in_stream) : CharStream(std::cin)
    );

    auto program = parse::parse_program(ast_builder);

    if (ast_builder.n_errs() > 0) {
        for (const auto& err : ast_builder.get_errs()) {
            std::cerr << "line " << err.get_line_no() << ", ";
            std::cerr << "pos " << err.get_line_offset() << ": ";

            std::cerr << err.get_err_msg();
            std::cerr << '\n';
        }

        return 1;
    }

    program.execute();

    if (program.ctx.n_errs() > 0) {
        for (const auto& err : program.ctx.get_errs()) {
            std::cerr << err.get_err_msg();
            std::cerr << '\n';
        }

        return 1;
    }

    return 0;
}