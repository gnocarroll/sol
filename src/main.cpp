#include <iostream>

#include "ast/ast_builder.h"
#include "char_stream.h"
#include "parse.h"
#include "ast/value.h"

int main(void) {
    for (auto op : ast::Value::get_supported_ops()) {
        std::cout << op() << "\n";
    }

    auto ast_builder = ast::ASTBuilder(CharStream(std::cin));

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
    }

    return 0;
}