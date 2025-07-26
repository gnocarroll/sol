#include "treewalk/execution_context.h"

namespace treewalk {

LiveInstance& ExecutionContext::live_instance_factory() {
	live_instances.push_back(std::make_unique<LiveInstance>());

	return *live_instances.back().get();
}

LiveInstance& ExecutionContext::make_live_instance(ast::Instance& instance, LiveValuePtr&& value) {
	LiveInstance& live_instance = live_instance_factory();

	live_instance._instance = &instance;
	live_instance.value = std::move(value);

	// maintain mapping from ast::Instance -> LiveInstance
	instance_map[&instance] = &live_instance;

	return live_instance;
}

std::optional<LiveInstance*> ExecutionContext::get_live_instance(ast::Instance& instance) {
	if (instance_map.count(&instance) == 0) return {};

	return instance_map[&instance];
}

}