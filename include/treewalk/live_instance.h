#pragma once

#include "ast/instance.h"
#include "mixins.h"
#include "treewalk/live_value.h"

namespace treewalk {

struct LiveInstance : public HasErrFlag {
    LiveValuePtr value = LiveErrValue::create();
    
    const ast::Instance* _instance = nullptr;

    const ast::Instance& instance() {
        return *_instance;
    }

    const LiveValuePtr& get_value() const {
        return value;
    }
    void set_value(LiveValuePtr&& new_value) {
        value = std::move(new_value);
    }
};

DEF_DERIVED_TYPES(LiveInstance)

}