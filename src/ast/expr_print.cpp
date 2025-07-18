#include "ast/expr.h"

#include <cmath>

namespace ast {

void BinaryExpr::print(std::ostream &ostream) {
    ostream << '(';
    auto ttype = op.get_token_type();

    if (!ttype) ostream << "ERR";
    else {
        auto ttext = ttype->get_text();

        if (!ttext) ostream << "ERR";
        else ostream << *ttext;
    }

    ostream << ' ';

    lhs->print(ostream);

    ostream << ' ';

    rhs->print(ostream);

    ostream << ')';
}

void UnaryExpr::print(std::ostream &ostream) {
    ostream << '(';
    auto ttype = op.get_token_type();

    if (!ttype) ostream << "ERR";
    else {
        auto text = ttype->get_text();

        if (!text) ostream << "ERR";
        else ostream << *text;
    }

    ostream << ' ';

    sub_expr->print(ostream);

    ostream << ')';
}

void InstanceExpr::print(std::ostream &ostream) {
    ostream << instance.name;
}

}