#pragma once

#include "mixins.h"

namespace ast {

struct ASTObject : public HasFilePos, public HasErrFlag {
	ASTObject() {}
	ASTObject(ASTObject& other) = delete;
};

}