#pragma once

#include <string>

#include "ast/ast_object.h"
#include "lang_err.h"
#include "mixins.h"

namespace treewalk {

class ExecutionErr : public LangErr {
public:
    ast::ASTObject& source;

    ExecutionErr(ast::ASTObject& source) :
        source(source) {}
    ExecutionErr(ast::ASTObject& source, std::string&& err_msg) :
        LangErr(std::move(err_msg)), source(source) {}
};

class ExecutionContext : public ErrorRegistry<ExecutionErr> {

};

}