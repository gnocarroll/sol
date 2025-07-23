#pragma once

#include <string>
#include <vector>

#include "ast/expr.h"
#include "ast/instance.h"
#include "ast/statement.h"
#include "char_stream.h"
#include "file_pos.h"
#include "lang_err.h"
#include "mixins.h"

namespace ast {

struct ASTErr : public HasFilePos, public LangErr {
    ASTErr() {}
    ASTErr(FilePos file_pos, std::string &&err_msg) :
        HasFilePos(file_pos), LangErr(std::move(err_msg)) {}
};

class AST : public ErrorRegistry<ASTErr> {
public:
    friend struct Expr;

private:
    size_t cstream_lookback = 0;

    // centralize ownership of AST objects in this AST class
    ExprPtrVec exprs;
    InstancePtrVec instances;
    StatementPtrVec statements;
    ScopePtrVec scopes;

public:
    class CStreamLookback {
        AST &ast_builder;
        size_t old;

        CStreamLookback(AST& ast_builder, size_t new_lookback) :
            ast_builder(ast_builder),
            old(ast_builder.cstream_lookback) {
            ast_builder.cstream_lookback = new_lookback;
        }
    public:
        ~CStreamLookback() {
            ast_builder.cstream_lookback = old;
        }

        friend class AST;
    };

    CharStream cstream;

    AST(CharStream &&cstream) :
        cstream(std::move(cstream)) {}

    void register_error(std::string&& err_msg) {
        ErrorRegistry<ASTErr>::register_error(
            ASTErr(cstream.get_file_pos(cstream_lookback), std::move(err_msg))
        );
    }

    auto create_char_lookback(size_t lookback) {
        return CStreamLookback(*this, lookback);
    }

private:
    Expr& expr_factory();
public:

    Expr& make_binary_expr(Expr& lhs, Operator op, Expr& rhs);
    Expr& make_unary_expr(Operator op, Expr& rhs);
    Expr& make_literal_expr(const LangType& lang_type, int64_t value);
};

}