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

    template <typename T, typename... Args>
    std::unique_ptr<T> make_w_pos(Args&&... args) {
        auto ret = std::make_unique<T>(std::forward<Args>(args)...);

        ret->set_file_pos(
            cstream.get_file_pos(cstream_lookback)
        );

        return ret;
    }

    Expr& add_expr(ExprPtr&& expr) {
        exprs.emplace_back(expr);

        return *exprs.back().get();
    }
    Statement& add_statement(StatementPtr&& statement) {
        statements.emplace_back(statement);

        return *statements.back().get();
    }
    Instance& add_instance(InstancePtr&& instance) {
        instances.emplace_back(instance);

        return *instances.back().get();
    }
    Scope& add_scope(ScopePtr&& scope) {
        scopes.emplace_back(scope);

        return *scopes.back().get();
    }

    // make unique_ptr and add to internal vec in one step
    template <typename T, typename... Args>
    Expr& make_expr(Args&&... args) {
        return add_expr(std::make_unique<T>(std::forward<Args>(args)...));
    }
    template <typename T, typename... Args>
    Statement& make_statement(Args&&... args) {
        return add_statement(std::make_unique<T>(std::forward<Args>(args)...));
    }
    template <typename... Args>
    Instance& make_instance(Args&&... args) {
        return add_instance(std::make_unique<Instance>(std::forward<Args>(args)...));
    }
    template <typename... Args>
    Scope& make_scope(Args&&... args) {
        return add_scope(std::make_unique<Scope>(std::forward<Args>(args)...));
    }
};

}