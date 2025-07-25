#pragma once

#include <string>
#include <vector>

#include "ast/expr.h"
#include "ast/instance.h"
#include "ast/program.h"
#include "ast/scope.h"
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
private:
    size_t cstream_lookback = 0;

    // centralize ownership of AST objects in this AST class

    ProgramPtrVec programs;
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
    Instance& instance_factory();
    Scope& scope_factory();
    Expr& expr_factory();

    template <typename T>
    T& statement_factory() {
        statements.emplace_back(std::make_unique<T>());

        return *dynamic_cast<T*>(&statements.back());
    }

    Program& program_factory();
public:
    Instance& make_instance(std::string&& name, const ast::LangType& lang_type = ast::lang_err_type);

    Expr& make_binary_expr(Expr& lhs, Operator op, Expr& rhs);
    Expr& make_unary_expr(Operator op, Expr& rhs);
    Expr& make_literal_expr(const LangType& lang_type, int64_t value);
    Expr& make_instance_expr(Instance& instance);
    Expr& make_err_expr();
    
    Statement& make_compound_statement(std::vector<Statement*>&& statements);
    Statement& make_create_statement(Instance& instance, Expr& expr);
    Statement& make_modify_statement(Instance& instance, Expr& expr);
    Statement& make_print_statement(std::optional<Expr*> expr = {});
    Statement& make_err_statement();

    Scope& make_scope();

    Program& make_program(Scope& global_scope, Statement& entry_point);
};

}