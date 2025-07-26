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
#include "treewalk/live_scope.h"

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
    LiveScopePtrVec live_scopes;

    LiveScope& scope_factory() {
        live_scopes.emplace_back(std::make_unique<LiveScope>());

        return *live_scopes.back().get();
    }

public:


};

}