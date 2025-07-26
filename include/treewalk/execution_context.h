#pragma once

#include <cstdint>
#include <expected>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

#include "ast/ast_object.h"
#include "ast/instance.h"
#include "ast/type.h"
#include "lang_err.h"
#include "macros.h"
#include "mixins.h"
#include "treewalk/live_instance.h"

namespace treewalk {

class ExecutionErr : public LangErr {
public:
    const ast::ASTObject& source;

    ExecutionErr(const ast::ASTObject& source) :
        source(source) {}
    ExecutionErr(const ast::ASTObject& source, std::string&& err_msg) :
        LangErr(std::move(err_msg)), source(source) {}
};

class ExecutionContext : public ErrorRegistry<ExecutionErr> {
    LiveInstancePtrVec live_instances;

    LiveInstance& live_instance_factory();

    std::unordered_map<ast::Instance*, LiveInstance*> instance_map;

public:
    LiveInstance& make_live_instance(ast::Instance& instance, LiveValuePtr&& value);

    std::optional<LiveInstance*> get_live_instance(ast::Instance& instance);
};

}