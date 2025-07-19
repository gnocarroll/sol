#pragma once

#include <string>

#include "ast/ast_object.h"
#include "lang_err.h"
#include "mixins.h"

namespace treewalk {

class ExecutionErr : public LangErr {
    ast::ASTObject& source;

public:
    ExecutionErr(ast::ASTObject& source) :
        source(source) {}
    ExecutionErr(ast::ASTObject& source, std::string&& err_msg) :
        source(source), LangErr(std::move(err_msg)) {}
};

class ExecutionContext : public ErrorRegistry<ExecutionErr> {

};

}