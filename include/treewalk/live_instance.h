#pragma once

#include "ast/instance.h"
#include "mixins.h"
#include "treewalk/live_value.h"

namespace treewalk {

class LiveInstance : public HasErrFlag {
    LiveValuePtr value = LiveErrValue::create();
public:
    const ast::Instance& instance;

    LiveInstance(const ast::Instance& instance) :
        instance(instance) {
    }
    LiveInstance(const ast::Instance& instance, LiveValuePtr&& value) :
        value(std::move(value)), instance(instance) {
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